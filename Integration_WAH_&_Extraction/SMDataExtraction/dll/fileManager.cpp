// $Id$
// Author: John Wu <John.Wu at ACM.org>
//         Lawrence Berkeley National Laboratory
// Copyright 2000-2009 Univeristy of California
//
// Purpose:
// This file contains an implementation of the fileManager used by IBIS.
//
// Note:
// use malloc and realloc to manage memory when the file content is
// actually in memory.  The main reason for doing so is to avoid malloc for
// resizing.  This may potentially cause problems with memory allocation
// through the new operator provided by the C++ compiler.
//
#if defined(_WIN32) && defined(_MSC_VER)
#pragma warning(disable:4786)	// some identifier longer than 256 characters
#endif
#include "fileManager.h"
#include "resource.h"
#include "array_t.h"

#include <typeinfo>	// typeid
#include <string>	// std::string
#include <stdio.h>	// fopen, fread, remove
#include <stdlib.h>	// malloc, realloc, free
#include <sys/stat.h>	// stat, open
#include <time.h>
//#include <limits>	// std::numeric_limits

#if defined(HAVE_SYS_SYSCTL) || defined(__APPLE__) || defined(__FreeBSD__)
#include <sys/sysctl.h> // sysctl
#endif

#if defined(_WIN32) && defined(_MSC_VER)
#  include <windows.h>
#  include <psapi.h>	// GetPerformanceInfo, struct PERFORMANCE_INFORMATION
#elif HAVE_MMAP
#  include <sys/mman.h>	// mmap
#endif

// initialize static varialbes (class members) of fileManager
time_t ibis::fileManager::hbeat = 0;
uint64_t ibis::fileManager::maxBytes = 0;
uint32_t ibis::fileManager::pagesize = 8192;
unsigned int ibis::fileManager::maxOpenFiles = 0;
ibis::util::sharedInt64 ibis::fileManager::totalBytes;

// default to about 256 MB
#define _FASTBIT_DEFAULT_MEMORY_SIZE 256*1024*1024

// explicit instantiation required
template int ibis::fileManager::getFile<uint64_t>
(char const*, array_t<uint64_t>&, ACCESS_PREFERENCE);
template int ibis::fileManager::getFile<int64_t>
(char const*, array_t<int64_t>&, ACCESS_PREFERENCE);
template int ibis::fileManager::getFile<uint32_t>
(char const*, array_t<uint32_t>&, ACCESS_PREFERENCE);
template int ibis::fileManager::getFile<int32_t>
(char const*, array_t<int32_t>&, ACCESS_PREFERENCE);
template int ibis::fileManager::getFile<uint16_t>
(char const*, array_t<uint16_t>&, ACCESS_PREFERENCE);
template int ibis::fileManager::getFile<int16_t>
(char const*, array_t<int16_t>&, ACCESS_PREFERENCE);
template int ibis::fileManager::getFile<char>
(char const*, array_t<char>&, ACCESS_PREFERENCE);
template int ibis::fileManager::getFile<signed char>
(char const*, array_t<signed char>&, ACCESS_PREFERENCE);
template int ibis::fileManager::getFile<unsigned char>
(char const*, array_t<unsigned char>&, ACCESS_PREFERENCE);
template int ibis::fileManager::getFile<ibis::rid_t>
(char const*, array_t<ibis::rid_t>&, ACCESS_PREFERENCE);
template int ibis::fileManager::getFile<float>
(char const*, array_t<float>&, ACCESS_PREFERENCE);
template int ibis::fileManager::getFile<double>
(char const*, array_t<double>&, ACCESS_PREFERENCE);

// time to wait for other threads to unload files in use
#ifndef FASTBIT_MAX_WAIT_TIME
#if defined(DEBUG) || defined(_DEBUG)
#define FASTBIT_MAX_WAIT_TIME 5
#else
#define FASTBIT_MAX_WAIT_TIME 60
#endif
#endif

/// Given a file name, place the content in an array_t<T>.  This function
/// waits for memory to become available if there is enough memory to read
/// the file content into memory.  The compiler macro FASTBIT_MAX_WAIT_TIME
/// defines the maximum amount of time (in seconds) it may wait.
///
/// The return value is zero (0) if the function is successful, otherwise
/// returns a non-zero value.
template <typename T>
int ibis::fileManager::getFile(const char* name, array_t<T>& arr,
			       ACCESS_PREFERENCE pref) {
    int ierr;
    try {
	storage* st = 0;
	ierr = getFile(name, &st, pref);
	if (ierr == 0) {
	    if (st) {
		array_t<T> tmp(st);
		arr.swap(tmp);
	    }
	    else {
		arr.clear();
	    }
	}

	LOGGER(ibis::gVerbose > 12)
	    << "ibis::fileManager::getFile -- got " << arr.size()
	    << " ints from " << name;
    }
    catch (...) {
	ierr = -1;
    }
    return ierr;
} // ibis::fileManager::getFile

/// Given a file name, place the content in an array_t<T>.  This function
/// will fail if there isn't enough memory to read the content of the file
/// immediately.
///
/// The return value is zero (0) if the function is successful, otherwise
/// returns a non-zero value.
template <typename T>
int ibis::fileManager::tryGetFile(const char* name, array_t<T>& arr,
				  ACCESS_PREFERENCE pref) {
    int ierr;
    try {
	storage* st = 0;
	ierr = tryGetFile(name, &st, pref);
	if (ierr == 0) {
	    if (st) {
		array_t<T> tmp(st);
		arr.swap(tmp);
	    }
	    else {
		arr.clear();
	    }
	}

	LOGGER(ibis::gVerbose > 12)
	    << "ibis::fileManager::getFile -- got " << arr.size()
	    << " ints from " << name;
    }
    catch (...) {
	ierr = 0;
    }
    return ierr;
} // ibis::fileManager::tryGetFile

// print the current status of the file manager
void ibis::fileManager::printStatus(std::ostream& out) const {
    size_t mtot=0, itot=0;
    char tstr[28];
    ibis::util::getLocalTime(tstr);

    //readLock lck("printStatus"); // acquiring lock here may cause dead lock
    out << "\n--- " << tstr << "\nThe number of memory mapped files is "
	<< mapped.size() << ". (max = " << maxOpenFiles
	<< ")\n";
    for (fileList::const_iterator it0 = mapped.begin();
	 it0 != mapped.end(); ++it0) {
	mtot += (*it0).second->size();
	(*it0).second->printStatus(out);
    }
    out << "\nSize of all mapped files is " << mtot << std::endl;
    out << "\n\nThe number of files read into memory is " << incore.size()
	<< ".\n";
    for (fileList::const_iterator it1 = incore.begin();
	 it1 != incore.end(); ++it1) {
	itot += (*it1).second->size();
	(*it1).second->printStatus(out);
    }
    out << "\nThe total size of all files read into memory is " << itot
	<< std::endl;
    out << "\nSize of all named storages is " << itot + mtot
	<< "\nSize of all unnamed storages is "
	<< ibis::fileManager::totalBytes() - (itot + mtot)
	<< "\nThe total size of all named and unnamed storages is "
	<< ibis::fileManager::totalBytes()
	<< "\nThe prescribed maximum size is " << maxBytes
	<< "\nNumber of pages accessed (recorded so far) is "
	<< page_count << " (page size = " << pagesize << ")\n"
	<< std::endl;
} // ibis::fileManager::printStatus

// remove a file from cache
void ibis::fileManager::flushFile(const char* name) {
    if (name == 0 || *name == 0) return;
    ibis::util::mutexLock lck(&mutex, name);
    fileList::iterator it = mapped.find(name);
    if (it != mapped.end()) {
	if ((*it).second->inUse() == 0) {
	    LOGGER(ibis::gVerbose > 7)
		<< "fileManager::flushFile -- removing \"" << (*it).first
		<< "\" from the list of mapped files";
	    delete (*it).second;
	    mapped.erase(it);
	}
	else {
	    LOGGER(ibis::gVerbose > 2)
		<< "fileManager::flushFile -- can not remove \"" << (*it).first
		<< "\" because it is in use (" << (*it).second->inUse() << ')';
	}
    }
    else if (incore.end() != (it = incore.find(name))) {
	if ((*it).second->inUse() == 0) {
	    LOGGER(ibis::gVerbose > 7)
		<< "fileManager::flushFile -- removing \"" << (*it).first
		<< "\" from the list of incore files";
	    delete (*it).second;
	    incore.erase(it);
	}
	else {
	    LOGGER(ibis::gVerbose > 2)
		<< "fileManager::flushFile -- can not remove \"" << (*it).first
		<< "\" because it is in use (" << (*it).second->inUse() << ')';
	}
    }
    else {
	LOGGER(ibis::gVerbose > 5)
	    << "fileManager::flushFile will do nothing because \"" << name
	    << "\" is not tracked by the file manager";
    }
} // ibis::fileManager::flushFile

// remove all files from the specified directory (include all sub
// directories)
void ibis::fileManager::flushDir(const char* name) {
    if (name == 0 || *name == 0) return;
    ibis::util::mutexLock lck(&mutex, name);
    LOGGER(ibis::gVerbose > 5)
	<< "fileManager::flushDir -- removing records of all files in " << name;

    uint32_t deleted = 0;
    const uint32_t len = strlen(name);
    const uint32_t offset = len + (FASTBIT_DIRSEP != name[len-1]);

    while (1) { // loop forever
	// is there any files within the directory
	uint32_t cnt = 0;
	fileList::iterator it = mapped.begin();
	while (it != mapped.end()) {
	    fileList::iterator next = it; ++next;
	    if (strncmp((*it).first, name, len)==0) {
		if (strchr((*it).first+offset, FASTBIT_DIRSEP) == 0) {
		    if ((*it).second->inUse() > 0) {
			++ cnt;
			ibis::util::logger lg;
			lg.buffer() << "Warning -- fileManager::flushDir "
				    << "can not remove mapped file ("
				    << (*it).first << ").  It is in use";
			if (ibis::gVerbose > 3) {
			    lg.buffer() << "\n";
			    (*it).second->printStatus(lg.buffer());
			}
		    }
		    else {
			//writeLock wlck(this, "flushDir");
			LOGGER(ibis::gVerbose > 7)
			    << "fileManager::flushDir -- removing \""
			    << (*it).first
			    << "\" from the list of mapped files";
			delete (*it).second;
			mapped.erase(it);
			++ deleted;
		    }
		}
	    }
	    it = next;
	}

	it = incore.begin();
	while (it != incore.end()) {
	    fileList::iterator next = it; ++next;
	    if (strncmp((*it).first, name, len)==0) {
		if (strchr((*it).first+offset, FASTBIT_DIRSEP) == 0) {
		    if ((*it).second->inUse()) {
			++ cnt;
			ibis::util::logger lg;
			lg.buffer() << "Warning -- fileManager::flushDir "
				    << "can not remove in-memory file ("
				    << (*it).first << ").  It is in use";
			if (ibis::gVerbose > 3) {
			    lg.buffer() << "\n";
			    (*it).second->printStatus(lg.buffer());
			}
		    }
		    else {
			//writeLock wlck(this, "flushDir");
			LOGGER(ibis::gVerbose > 7)
			    << "fileManager::flushDir -- removing \""
			    << (*it).first
			    << "\" from the list of incore files";
			delete (*it).second;
			incore.erase(it);
			++ deleted;
		    }
		}
	    }
	    it = next;
	}

	if (cnt) {// there are files in use
	    LOGGER(ibis::gVerbose >= 0)
		<< "Warning -- fileManager::flushDir(" << name
		<< ") finished with " << cnt << " file"
		<< (cnt>1?"s":"") << " still in memory";
	    return;
	    //	    pthread_cond_wait(&cond, &mutex);
	}
	else {
	    LOGGER(ibis::gVerbose > 5)
		<< "fileManager::flushDir -- removed " << deleted
		<< " file" << (deleted>1?"s":"") << " from " << name;
	    return;
	}
    }
} // ibis::fileManager::flushDir

/// Change the class variable maxBytes to the newsize.  Return 0 if
/// successful, a negative number otherwise.
///
/// This function simply changes the maximum bytes allowed, without
/// enforcing this limit.  Future operations that require more memory will
/// be subject to the new cache size limit.
///
/// Reducing the cache size while there are on-going operations can have
/// very undesirable effect, therefore this function will not accept a new
/// size if it is less than the current number of bytes in memory.  It
/// might be helpful to call ibis::fileManager::clear to reduce the memory
/// usage before changing the cache size.
int ibis::fileManager::adjustCacheSize(uint64_t newsize) {
    ibis::util::mutexLock lock(&(ibis::util::envLock),
			       "fileManager::adjustCacheSize");
    if (newsize > ibis::fileManager::totalBytes()) {
	LOGGER(ibis::gVerbose > 0)
	    << "fileManager::adjustCacheSize(" << newsize
	    << ") changes cache size from " << maxBytes << " to " << newsize;
	maxBytes = newsize;
	return 0;
    }
    else {
	LOGGER(ibis::gVerbose >= 0)
	    << "Warning -- fileManager::adjustCacheSize(" << newsize
	    << ") can not proceed because the new size is not larger than "
	    "the current memory used (" << ibis::fileManager::totalBytes()
	    << ")";
	return -1;
    }
} // ibis::fileManager::adjustCacheSize

/// The function cleans the memory cache used by FastBit.  It destroys the
/// two lists of files it holds and therefore make the file not accessible
/// any object new objects.  However, the actual underlying memory may
/// still be present if they are being actively used.  This function is
/// effective on if all other operations have ceased.  To force an
/// individual file to be unloaded use ibis::fileManager::flushFile.  To
/// force all files in a directory to be unloaded used
/// ibis::fileManager::flushDir.
void ibis::fileManager::clear() {
    if (ibis::gVerbose > 12 ||
	(ibis::fileManager::totalBytes() > 0 && ibis::gVerbose > 6)) {
	ibis::util::logger lg;
	lg.buffer() << "ibis::fileManager::clear -- starting ...";
	printStatus(lg.buffer());
    }

    ibis::util::mutexLock mlck(&mutex, "fileManager::clear");
    if (! mapped.empty() || ! incore.empty()) {
	std::vector<roFile*> tmp; // temporarily holds the read-only files
	writeLock wlck(this, "clear");
	tmp.reserve(mapped.size()+incore.size());
	for (fileList::const_iterator it=mapped.begin();
	     it != mapped.end(); ++it) {
	    tmp.push_back((*it).second);
	}
	mapped.clear();
	for (fileList::const_iterator it=incore.begin();
	     it != incore.end(); ++it) {
	    tmp.push_back((*it).second);
	}
	incore.clear();
	// delete the read-only files stored in the std::vector because the
	// FileList uses the name of the file (part of the object to be
	// deleted) as the key (of a std::map).
	for (size_t j = 0; j < tmp.size(); ++ j)
	    delete tmp[j];
    }
    LOGGER((ibis::fileManager::totalBytes() != 0 && ibis::gVerbose > 0) ||
	   ibis::gVerbose > 8)
	<< "fileManager::clear -- completed with "
	<< ibis::fileManager::totalBytes()
	<< " byte" << (ibis::fileManager::totalBytes()>1 ? "s" : "")
	<< " of storage remain in memory after removing all managed objects";
} // ibis::fileManager::clear

void ibis::fileManager::addCleaner(const ibis::fileManager::cleaner* cl) {
    ibis::util::mutexLock lck(&mutex, "fileManager::addCleaner");
    cleanerList::const_iterator it = cleaners.find(cl);
    if (it == cleaners.end())
	cleaners.insert(cl);
} // ibis::fileManager::addCleaner

void ibis::fileManager::removeCleaner(const ibis::fileManager::cleaner* cl) {
    ibis::util::mutexLock lck(&mutex, "fileManager::removeCleaner");
    cleanerList::iterator it = cleaners.find(cl);
    if (it != cleaners.end())
	cleaners.erase(it);
} // ibis::fileManager::removeCleaner

/// The instance function of the fileManager singleton.
ibis::fileManager& ibis::fileManager::instance() {
    static ibis::fileManager theManager;
//	ibis::fileManager theManager;
    return theManager;
} // ibis::fileManager::instance

/// The protected constructor of the ibis::fileManager class.  There are
/// three parameters that can be specified in a configuration file to
/// control this object, fileManager.maxBytes, fileManager.maxOpenFiles,
/// and fileManager.minMapSize.  If you are unsure of what to do, then
/// don't specify anything -- the default values are typically acceptable.
///
/// \arg filemanager.maxBytes The maximum number of bytes of all objects
/// under control of the file manager, e.g.,
/// \verbatim
/// fileManager.maxBytes = 500MB
/// \endverbatim
/// One may specify a number followed by KB, MB, or GB (without space in
/// between).  If not specified, this constructor attempts to determine the
/// size of the physical memory available and will use half of the memory
/// for caching FastBit objects.
///
/// \arg fileManager.maxOpenFiles This file manager will keep the number of
/// open files below this specified maximum.  Note that FastBit usually
/// invokes the lower level function open, which typical can use more file
/// handles than the higher level ones such as fopen.  If not specified, it
/// will use three quarters of maximum file halder defined by _SC_OPEN_MAX.
///
/// \arg fileManager.minMapSize The minimal size of a file before FastBit
/// will attempt to use memory map on it.  For smaller files, it is more
/// efficient to read the whole content into memory rather than keeping a
/// file open.  The default value is defined by the macro FASTBIT_MIN_MAP_SIZE.
ibis::fileManager::fileManager()
    : page_count(0), minMapSize(FASTBIT_MIN_MAP_SIZE), nwaiting(0) {
    {
	unsigned long sz = static_cast<unsigned long>
	    (ibis::gParameters().getNumber("fileManager.maxBytes"));
	if (sz > 0)
	    maxBytes = sz;
	sz = static_cast<unsigned long>
	    (ibis::gParameters().getNumber("fileManager.maxOpenFiles"));
	if (sz > 10)
	    maxOpenFiles = sz;
	sz = static_cast<unsigned long>
	    (ibis::gParameters().getNumber("fileManager.minMapSize"));
	if (sz != 0)
	    minMapSize = sz;
    }
    if (maxBytes < FASTBIT_MIN_MAP_SIZE) {
	LOGGER(ibis::gVerbose > 3 && maxBytes > 0)
	    << "user input parameter fileManager.maxBytes (" << maxBytes
	    << ") is too small, trying to determine the physical memory "
	    "size and use half of it";
#ifdef _SC_PHYS_PAGES
	// most *nix flavors defines this for physical number of pages
	uint64_t mem = 0;
#ifdef _SC_PAGESIZE
	pagesize = sysconf(_SC_PAGESIZE);
	mem = static_cast<uint64_t>(sysconf(_SC_PHYS_PAGES)) * pagesize;
#elif defined(_SC_PAGE_SIZE)
	pagesize = sysconf(_SC_PAGE_SIZE);
	mem = static_cast<uint64_t>(sysconf(_SC_PHYS_PAGES)) * pagesize;
#endif
	LOGGER(ibis::gVerbose > 4 && mem > 0)
	    << "fileManager::ctor found the physical memory size to be "
	    << mem << " bytes";
	mem /= 2; // allow half to be used by fileManager
	if (mem > ULONG_MAX)
	    maxBytes = (ULONG_MAX - (ULONG_MAX>>2));
	else if (mem > 0)
	    maxBytes = mem;
#elif defined(CTL_HW) && (defined(HW_MEMSIZE) || defined(HW_PHYSMEM))
	// BSD flavored systems provides sysctl for finding out the
	// physical memory size
	uint64_t mem = 0;
	size_t len = sizeof(mem);
	int mib[2] = {CTL_HW, 0};
#ifdef HW_MEMSIZE
	mib[1] = HW_MEMSIZE;
#else
	mib[1] = HW_PHYSMEM;
#endif
	if (sysctl(mib, 2, &mem, &len, NULL, 0) == 0 && len <= sizeof(mem)) {
	    LOGGER(ibis::gVerbose > 4 && mem > 0)
		<< "fileManager::ctor found the physical memory size to be "
		<< mem << " bytes";
	    mem >>= 1;
	    if (mem > ULONG_MAX)
		maxBytes = (ULONG_MAX - (ULONG_MAX>>2));
	    else if (mem > 0)
		maxBytes = mem;
	}
	else {
	    LOGGER(ibis::gVerbose > 1)
		<< "fileManager failed to determine the physical memory size "
		<< "with sysctl(CTL_HW=" << CTL_HW << ", HW_PHYSMEM="
		<< HW_PHYSMEM << ") -- "
		<< (errno != 0 ? strerror(errno) :
		    (len != sizeof(mem)) ? "return value of incorrect size" :
		    "unknwon error")
		<< ", mem=" << mem;
	    maxBytes = _FASTBIT_DEFAULT_MEMORY_SIZE;
	}
#elif defined(_PSAPI_H_)
	// MS Windows uses psapi to physical memory size
	PERFORMANCE_INFORMATION pi;
	if (GetPerformanceInfo(&pi, sizeof(pi))) {
	    size_t avail = pi.PhysicalAvailable;
	    size_t mem = (pi.PhysicalTotal >> 1);
	    LOGGER(ibis::gVerbose > 4 && mem > 0)
		<< "fileManager::ctor found the physical memory size to be "
		<< pi.PhysicalTotal * pi.PageSize << " bytes";
	    if (avail > mem) mem = avail; // take it if available
	    if (mem < (ULONG_MAX / pi.PageSize))
		maxBytes = mem * pi.PageSize;
	    else
		maxBytes = (ULONG_MAX - (ULONG_MAX >> 2));
	}
	else {
	    char *lpMsgBuf;
	    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			  FORMAT_MESSAGE_FROM_SYSTEM | 
			  FORMAT_MESSAGE_IGNORE_INSERTS,
			  NULL, GetLastError(),
			  MAKELANGID(LANG_NEUTRAL,
				     SUBLANG_DEFAULT),
			  (LPTSTR) &lpMsgBuf, 0, NULL);
	    LOGGER(ibis::gVerbose >= 0)
		<< "fileManager::ctor -- failed to determine the physical "
		<< "memory size -- " << lpMsgBuf;
	    LocalFree(lpMsgBuf);	// Free the buffer.
	}
	pagesize = pi.PageSize;
// 	SYSTEM_INFO sysinfo;
// 	GetSystemInfo(&sysinfo);
// 	pagesize = sysinfo.dwPageSize;
#else
	maxBytes = _FASTBIT_DEFAULT_MEMORY_SIZE;
	LOGGER(ibis::gVerbose > 2)
	    << "fileManager::ctor -- using a default value of " << maxBytes
	    << " bytes";
#endif
    }

    if (maxOpenFiles < 8) { // maxOpenFiles is too small
#if defined(_SC_OPEN_MAX)
	// maximum number of open files is defined in sysconf
	uint32_t sz = sysconf(_SC_OPEN_MAX);
	maxOpenFiles = static_cast<uint32_t>(0.75*sz);
#else
	maxOpenFiles = 60;
#endif
    }
    // final adjustment based on stdio limitation
#if defined(OPEN_MAX)
    maxOpenFiles = static_cast<int>
	(maxOpenFiles<=0.8*OPEN_MAX?maxOpenFiles:0.8*OPEN_MAX);
#elif defined(STREAM_MAX)
    maxOpenFiles = static_cast<int>
	(maxOpenFiles<=0.9*STREAM_MAX?maxOpenFiles:0.9*STREAM_MAX);
#endif
#if defined(FOPEN_MAX)
    if (maxOpenFiles < FOPEN_MAX)
	maxOpenFiles = maxOpenFiles;
#endif
    if (pthread_rwlock_init(&lock, 0) != 0)
	throw ibis::bad_alloc("pthread_rwlock_init failed in fileManager "
			      "ctor");
    if (pthread_mutex_init(&mutex, 0) != 0)
	throw ibis::bad_alloc("pthread_mutex_init failed in "
			      "fileManager ctor");
    if (pthread_cond_init(&cond, 0) != 0)
	throw ibis::bad_alloc("pthread_cond_init(cond) failed in "
			      "fileManager ctor");
    if (pthread_cond_init(&readCond, 0) != 0)
	throw ibis::bad_alloc("pthread_cond_init(readCond) failed in "
			      "fileManager ctor");
    LOGGER(ibis::gVerbose > 1)
	<< "fileManager initialization complete\n\t-- maxBytes="
	<< maxBytes << ", maxOpenFiles=" << maxOpenFiles;
} // ibis::fileManager::fileManager

/// Destructor.
ibis::fileManager::~fileManager() {
    clear();
    (void)pthread_rwlock_destroy(&lock);
    (void)pthread_mutex_destroy(&mutex);
    (void)pthread_cond_destroy(&cond);
} // ibis::fileManager::~fileManager

/// Record a newly allocated storage in the two lists.
/// The caller needs to hold a mutex lock on the file manager to ensure
/// correct operations.
void ibis::fileManager::recordFile(ibis::fileManager::roFile* st) {
    if (st == 0) return;
    if (st->begin() == st->end()) return;
    std::string evt = "fileManager::recordFile";
    if (ibis::gVerbose > 8) {
	std::ostringstream oss;
	oss << "(" << static_cast<void*>(st) << ", "
	    << static_cast<void*>(st->begin()) << ", " << st->size();
	if (st->filename() != 0)
	    oss << ", " << st->filename();
	oss << ")";
	evt += oss.str();
    }

    LOGGER(ibis::gVerbose > 12)
	<< evt << " -- record storage object " << (void*)st;
    increaseUse(st->bytes(), evt.c_str());
    if (st->filename() == 0)
	return;

    readLock rock(evt.c_str());
    if (st->mapped) {
	fileList::const_iterator it = mapped.find(st->filename());
	if (it == mapped.end()) {
	    if (incore.find(st->filename()) != incore.end()) {
		LOGGER(ibis::gVerbose >= 0)
		    << "Warning -- " << evt
		    << " trying to register a memory mapped storage object ("
		    << st->filename()
		    << ") while one with the same name is already in "
		    << "the incore list";
		throw "ibis::fileManager::recordFile trying to register two "
		    "storages with the same file name (old incore, "
		    "new mapped)";
	    }
	    mapped[st->filename()] = st;
	}
	else if (st != (*it).second) {
	    LOGGER(ibis::gVerbose >= 0)
		<< "Warning -- " << evt
		<< " trying to register a memory mapped storage object ("
		<< st->filename()
		<< ") while one with the same name is already in "
		<< "the mapped list";
	    throw "ibis::fileManager::recordFile trying to register two "
		"storage related the same file (both mapped)";
	}
    }
    else {
	fileList::const_iterator it = incore.find(st->filename());
	if (it == incore.end()) {
	    if (mapped.find(st->filename()) != mapped.end()) {
		LOGGER(ibis::gVerbose >= 0)
		    << "Warning -- " << evt
		    << " trying to register an incore storage object ("
		    << st->filename()
		    << ") while one with the same name is already in "
		    << "the mapped list";
		throw "ibis::fileManager::recordFile trying to register two "
		    "storage related the same file (old mapped, "
		    "new incore)";
	    }
	    incore[st->filename()] = st;
	}
	else if (st != (*it).second) {
	    LOGGER(ibis::gVerbose >= 0)
		<< "Warning -- " << evt
		<< " trying to register an incore storage object ("
		<< st->filename()
		<< ") while one with the same name is already in "
		<< "the mapped list";
	    throw "ibis::fileManager::recordFile trying to register two "
		"storage related the same file (both incore)";
	}
    }
} // ibis::fileManager::recordFile

/// Retrieve the file content as a storage object.  The object *st returned
/// from this function is owned by the fileManager.  The caller should NOT
/// delete *st!  This function will wait for the fileManager to unload some
/// in-memory objects if there isn't enough memory for the file.
///
/// Upon successful completion of the task, it returns zero; otherwise, it
/// returns a non-zero value to indicate an error and it does not modify the
/// content of storage object.
int ibis::fileManager::getFile(const char* name, storage** st,
			       ACCESS_PREFERENCE pref) {
    if (name == 0 || *name == 0) return -100;
    int ierr = 0;
    long unsigned bytes = 0; // the file size in bytes
    std::string evt = "fileManager::getFile";
    if (ibis::gVerbose >= 0) {
	evt += '(';
	evt += name;
	evt += ')';
    }
    {   // determine the file size, whether the file exist or not
	Stat_T tmp;
	if (0 == UnixStat(name, &tmp)) {
	    bytes = tmp.st_size;
	    if (bytes == 0) {
		LOGGER(ibis::gVerbose >= 0)
		    << evt << ": the named file is empty";
		ierr = -106;
		return ierr;
	    }
	}
	else {
	    if (ibis::gVerbose > 11 || errno != ENOENT) {
		LOGGER(ibis::gVerbose >= 0)
		    << "ibis::fileManager::getFile(" << name
		    << ") -- command stat failed: " << strerror(errno);
	    }
	    ierr = -101;
	    return ierr;
	}
    }

    ibis::util::mutexLock lck(&mutex, evt.c_str()); // only one instance can run
    readLock rock(evt.c_str());
    // is the named file among those mapped ?
    fileList::iterator it = mapped.find(name);
    if (it != mapped.end()) { // found it
	*st = (*it).second;
	return ierr;
    }

    // is the named file among those incore
    it = incore.find(name);
    if (it != incore.end()) { // found it
	*st = (*it).second;
	return ierr;
    }

    // is the file being read by another thread?
    if (reading.find(name) != reading.end()) {
	do {
	    LOGGER(ibis::gVerbose > 5)
		<< evt << " -- waiting for another thread to read \""
		<< name << "\"";
	    ierr = pthread_cond_wait(&readCond, &mutex);
	    if (ierr != 0) {
		ierr = -112;
		return ierr;
	    }
	} while (reading.find(name) != reading.end());

	it = mapped.find(name);
	if (it != mapped.end()) {
	    *st = (*it).second;
	    return ierr;
	}
	it = incore.find(name);
	if (it != incore.end()) {
	    *st = (*it).second;
	    return ierr;
	}
	ierr = -110; // the pending read did not succeed. retry?
	return ierr;
    }
    reading.insert(name); // add to the reading list
    LOGGER(ibis::gVerbose > 5)
	<< evt << " -- attempting to read " << name << " ("
	<< bytes << " bytes)";

    //////////////////////////////////////////////////////////////////////
    // need to actually open it up -- need to modify the two lists
    // unload enough files to free up space
    if (bytes + ibis::fileManager::totalBytes() > maxBytes) {
	LOGGER(ibis::gVerbose > 5)
	    << evt << " -- need to unload " << bytes
	    << " bytes for \"" << name << "\", maxBytes=" << maxBytes
	    << ", totalBytes=" << ibis::fileManager::totalBytes();
	ierr = unload(bytes);
    }
    else if (mapped.size() >= maxOpenFiles && bytes >= minMapSize) {
	LOGGER(ibis::gVerbose > 7)
	    << evt << " -- need to unload some files before reading \""
	    << name << "\", maxBytes=" << maxBytes
	    << ", totalBytes=" << ibis::fileManager::totalBytes();
	ierr = unload(0); // unload whatever can be freed
    }
    if (ierr < 0) {
	LOGGER(ibis::gVerbose >= 0)
	    << evt << " -- unable to free up " << bytes
	    << " bytes to read the file " << name << ", ierr = -102";
	reading.erase(name);
	ierr = -102;
	return ierr;
    }

    ibis::fileManager::roFile* tmp = new ibis::fileManager::roFile();
    if (tmp == 0) {
	LOGGER(ibis::gVerbose >= 0)
	    << evt << " -- unable to allocate a new roFile object for \""
	    << name << "\"";
	reading.erase(name);
	ierr = -103;
	return ierr;
    }
    ibis::horometer timer;
    if (ibis::gVerbose > 7)
	timer.start();
#if defined(HAVE_FILE_MAP)
    // now we can ask the question: "to map or not to map?"
    size_t sz = minMapSize;
    if (mapped.size() > (maxOpenFiles >> 1)) {
	// compute the maximum size of the first ten files
	it = mapped.begin();
	for (int cnt = 0; cnt < 10 && it != mapped.end(); ++ cnt, ++ it)
	    if (sz < (*it).second->size())
		sz = (*it).second->size();
	if (sz < FASTBIT_MIN_MAP_SIZE)
	    sz = FASTBIT_MIN_MAP_SIZE;
    }
    if (mapped.size() < maxOpenFiles && 
	(pref == PREFER_MMAP || (pref == MMAP_LARGE_FILES && bytes >= sz))) {
	// map the file read-only
	tmp->mapFile(name);
	if (tmp->begin() == 0) {
	    // read the file into memory
	    tmp->doRead(name);
	}
    }
    else {
	// read the file into memory
	tmp->doRead(name);
    }
#else
    tmp->doRead(name);
#endif
    if (tmp->size() == bytes) {
	recordFile(tmp);
	LOGGER(ibis::gVerbose > 5)
	    << evt << " -- completed "
	    << (tmp->isFileMap()?"mmapping":"retrieving") << " "
	    << tmp->size() << " bytes from " << name;

	if (ibis::gVerbose > 7) {
	    timer.stop();
	    double tcpu = timer.CPUTime();
	    double treal = timer.realTime();
	    double rt1 = tcpu > 0 ? (1e-6*tmp->size()/tcpu) : 0.0;
	    double rt2 = treal > 0 ? (1e-6*tmp->size()/treal) : 0.0;
	    ibis::util::logger lg;
	    lg.buffer() << evt << " took " << treal << " sec(elapsed) ["
			<< tcpu << " sec(CPU)] to "
			<< (tmp->isFileMap()?"mmap ":"read ") << tmp->size()
			<< " bytes at a speed of " << rt2
			<< " MB/s [" << rt1 << "]";
	    if (ibis::gVerbose > 11) {
		lg.buffer() << "\n";
		(void) tmp->printStatus(lg.buffer());
	    }
	}

	*st = tmp; // pass tmp to the caller
	ierr = 0;
    }
    else {
	LOGGER(ibis::gVerbose >= 0)
	    << "Warning -- ibis::fileManager::getFile(" << name
	    << ") failed retrieving " << bytes << " bytes (actually retrieved "
	    << tmp->size() << ")";
	delete tmp;
	ierr = -104;
    }

    reading.erase(name); // no longer on the list reading
    (void) pthread_cond_broadcast(&readCond); // tell all others
    return ierr;
} // int ibis::fileManager::getFile

/// Try to retrieve the content of the named file.  The storage object *st
/// returned from this function is owned by the fileManager.  The caller is
/// NOT to delete *st.  This function will not wait for the fileManager to
/// free any memory if there isn't enough free space available.
///
/// It returns 0 to indicate success and a negative value to indicate
/// error.  In particular, it returns -102 if there is not enough space to
/// read the whole file into memory.
int ibis::fileManager::tryGetFile(const char* name, storage** st,
				  ACCESS_PREFERENCE pref) {
#if DEBUG+0 > 1 || _DEBUG+0 > 1
    LOGGER(ibis::gVerbose > 5)
	<< "fileManager::tryGetFile -- attempt to retrieve \"" << name
	<< "\", currently there are " << mapped.size() << " mapped files and "
	<< incore.size() << " incore files";
#endif
    int ierr = 0;
    unsigned long bytes = 0; // the file size in bytes
    ibis::util::mutexLock lck(&mutex, "fileManager::tryGetFile");
    readLock rock("tryGetFile");

    // is the named file among those mapped ?
    fileList::iterator it = mapped.find(name);
    if (it != mapped.end()) { // found it
	*st = (*it).second;
	return ierr;
    }

    // is the named file among those incore
    it = incore.find(name);
    if (it != incore.end()) { // found it
	*st = (*it).second;
	return ierr;
    }

    {   // first determine the file size, whether the file exist or not
	Stat_T tmp;
	if (0 == UnixStat(name, &tmp)) {
	    bytes = tmp.st_size;
	    if (bytes == 0) {
		LOGGER(ibis::gVerbose >= 0)
		    << "ibis::fileManager::tryGetFile(" << name
		    << ") file is empty.";
		ierr = -106;
		return ierr;
	    }
	}
	else {
	    if (ibis::gVerbose > 11 || errno != ENOENT) {
		LOGGER(ibis::gVerbose >= 0)
		    << "ibis::fileManager::tryGetFile(" << name
		    << ") -- command stat failed: " << strerror(errno);
	    }
	    ierr = -101;
	    return ierr;
	}
    }

    // not enough space to get the file
    if (bytes + ibis::fileManager::totalBytes() > maxBytes) {
	return -102; // not enough space
    }
    if (reading.find(name) != reading.end()) {
	ierr = -111; // another thread is reading the same file
	return ierr;
    }
    reading.insert(name); // record the name
    LOGGER(ibis::gVerbose > 5)
	<< "fileManager::tryGetFile -- attempting to read " << name
	<< "(" << bytes << " bytes)";

    //////////////////////////////////////////////////////////////////////
    // need to actually open it up -- need to modify the two lists
    ibis::fileManager::roFile* tmp = new ibis::fileManager::roFile();
    ibis::horometer timer;
    if (ibis::gVerbose > 7)
	timer.start();
    // "to map or not to map", that is the question
#if defined(HAVE_FILE_MAP)
    size_t sz = minMapSize;
    if (mapped.size() > (maxOpenFiles >> 1)) {
	fileList::const_iterator mit = mapped.begin();
	for (int cnt = 0; cnt < 10 && mit != mapped.end(); ++ cnt, ++ mit)
	    if (sz < (*it).second->size())
		sz = (*it).second->size();
	if (sz < FASTBIT_MIN_MAP_SIZE)
	    sz = FASTBIT_MIN_MAP_SIZE;
    }
    if (mapped.size() < maxOpenFiles &&
	(pref == PREFER_MMAP || (pref == MMAP_LARGE_FILES && bytes >= sz))) {
	// map the file read-only
	tmp->mapFile(name);
	if (tmp->begin() == 0) {
	    // read the file into memory
	    tmp->doRead(name);
	}
    }
    else {
	// read the file into memory
	tmp->doRead(name);
    }
#else
    tmp->doRead(name);
#endif
    if (tmp->size() == bytes) {
	recordFile(tmp);
	LOGGER(ibis::gVerbose > 5)
	    << "fileManager::tryGetFile(" << name << ") completed "
	    << (tmp->isFileMap()?"mmapping":"retrieving") << " "
	    << tmp->size() << " bytes";

	if (ibis::gVerbose > 7) {
	    timer.stop();
	    double tcpu = timer.CPUTime();
	    double treal = timer.realTime();
	    double rt1 = tcpu > 0 ? (1e-6*tmp->size()/tcpu) : 0.0;
	    double rt2 = treal > 0 ? (1e-6*tmp->size()/treal) : 0.0;
	    ibis::util::logger lg;
	    lg.buffer() << "ibis::fileManager -- tryGetFile(" << name
			<< ") took " << treal << " sec(elapsed) ["
			<< tcpu  << " sec(CPU)] to "
			<< (tmp->isFileMap()?"mmap ":"read ") << tmp->size()
			<< " bytes at a speed of " << rt2
			<< " MB/s [" << rt1 << "]";
	    if (ibis::gVerbose > 11) {
		lg.buffer() << "\n";
		(void) tmp->printStatus(lg.buffer());
	    }
	}

	*st = tmp; // pass tmp to the caller
	ierr = 0;
    }
    else {
	LOGGER(ibis::gVerbose >= 0)
	    << "Warning -- fileManager::tryGetFile(" << name
	    << ") failed retrieving " << bytes << " bytes (actually retrieved "
	    << tmp->size() << ")";
	delete tmp;
	ierr = -107;
    }

    reading.erase(name);    // no longer on the list reading
    (void) pthread_cond_broadcast(&readCond); // tell all others
    return ierr;
} // ibis::fileManager::tryGetFile

/// Retrieve a portion of a file content.  Both the file name and the file
/// descriptor are passed in to this function so that it can make a
/// decision on whether to use a file map or directly read the content into
/// memory.  It prefers the read option more because the caller is more
/// like to touch every bytes in an explicitly specified portion of a
/// file.  More specifically, it uses file map only if the file of the file
/// segement is 4 * FASTBIT_MIN_MAP_SIZE and the number of mapped files is
/// less than half of the maxOpenFiles.
ibis::fileManager::storage*
ibis::fileManager::getFileSegment(const char* name, const int fdes,
				  const off_t b, const off_t e) {
#if DEBUG+0 > 1 || _DEBUG+0 > 1
    LOGGER(ibis::gVerbose > 5)
	<< "fileManager::getFileSegment -- attempt to retrieve \"" << name
	<< "\", currently there are " << mapped.size() << " mapped files and "
	<< incore.size() << " incore files";
#endif
    ibis::fileManager::storage *st = 0;
    if (((name == 0 || *name == 0) && fdes < 0) || b >= e)
	return st;

    int ierr = 0;
    unsigned long bytes = e - b; // the size (in bytes) of the file segment
    std::string evt = "fileManager::getFileSegment";
    if (ibis::gVerbose > 5) {
	std::ostringstream oss;
	oss << "(" << (name != 0 && *name != 0 ? name : "?") << ", "
	    << fdes << ", " << b << ", " << e << ")";
	evt += oss.str();
    }
    LOGGER(ibis::gVerbose > 5) << evt << " ...";

    //////////////////////////////////////////////////////////////////////
    // need to actually open it up -- need to modify the two lists
    // unload enough files to free up space
    if (bytes + ibis::fileManager::totalBytes() > maxBytes) {
	LOGGER(ibis::gVerbose > 5)
	    << evt << " -- need to unload " << bytes << " bytes for \""
	    << name << "\", maxBytes=" << static_cast<double>(maxBytes)
	    << ", totalBytes=" << ibis::fileManager::totalBytes();
	ibis::util::mutexLock lck(&ibis::fileManager::instance().mutex,
				  evt.c_str());
	ierr = ibis::fileManager::instance().unload(bytes);
    }
    if (ierr < 0) {
	LOGGER(ibis::gVerbose >= 0)
	    << evt << " -- unable to free up " << bytes
	    << "bytes to read the file " << name;
	ierr = -108;
	return st;
    }

    ibis::horometer timer;
    if (ibis::gVerbose > 7)
	timer.start();
    bool ismapped = false;
    // "to map or not to map", that is the question
#if defined(HAVE_FILE_MAP)
    if (name != 0 && *name != 0) {
	size_t sz = (FASTBIT_MIN_MAP_SIZE << 2); // more than 4 pages
	const size_t nmapped = ibis::fileManager::instance().mapped.size();
	if (nmapped+nmapped < maxOpenFiles && bytes >= sz) {
	    // map the file read-only
	    try {
		st = new ibis::fileManager::rofSegment(name, b, e);
		ismapped = true;
	    }
	    catch (...) {
		if (fdes >= 0) {
		    st = new ibis::fileManager::storage(fdes, b, e);
		}
		else {
		    st = new ibis::fileManager::storage(name, b, e);
		}
		ismapped = false;
	    }
	}
	else if (fdes >= 0) {
	    st = new ibis::fileManager::storage(fdes, b, e);
	}
	else {
	    st = new ibis::fileManager::storage(name, b, e);
	}
    }
    else if (fdes >= 0) {
	st = new ibis::fileManager::storage(fdes, b, e);
    }
#else
    if (fdes >= 0) {
	st = new ibis::fileManager::storage(fdes, b, e);
    }
    else {
	st = new ibis::fileManager::storage(name, b, e);
    }
#endif
    if (st->size() == bytes) {
	LOGGER(ibis::gVerbose > 5)
	    << evt <<" completed " << (ismapped?"mmapping":"reading")
	    << " " << st->size() << " bytes";

	if (ibis::gVerbose > 7) {
	    timer.stop();
	    double tcpu = timer.CPUTime();
	    double treal = timer.realTime();
	    double rt1 = tcpu > 0 ? (1e-6*st->size()/tcpu) : 0.0;
	    double rt2 = treal > 0 ? (1e-6*st->size()/treal) : 0.0;
	    ibis::util::logger lg;
	    lg.buffer() << evt << " took " << treal <<  " sec(elapsed) ["
			<< tcpu << " sec(CPU)] to "
			<< (st->isFileMap()?"mmap ":"read ") << st->size()
			<< " bytes at a speed of " << rt2 << " MB/s ["
			<< rt1 << "]";
	    if (ibis::gVerbose > 11) {
		lg.buffer() << "\n";
		(void) st->printStatus(lg.buffer());
	    }
	}
    }
    else {
	LOGGER(ibis::gVerbose >= 0)
	    << "Warning -- " << evt << " failed retrieving " << bytes
	    << " bytes (actually retrieved " << (st ? st->size() : 0U) << ")";
    }
    return st;
} // ibis::fileManager::getFileSegment

/// Unload enough space so that a file of size bytes can be loaded.  Caller
/// must hold a mutex lock to prevent simutaneous invocation of this
/// function.  It will wait a maximum of FASTBIT_MAX_WAIT_TIME seconds if
/// not enough memory can be freed immediately.
int ibis::fileManager::unload(size_t size) {
    if (size > 0 && maxBytes > ibis::fileManager::totalBytes() &&
	size+ibis::fileManager::totalBytes() <= maxBytes) {
	// there is enough space
	return 0;
    }
    if (size > maxBytes) {
	LOGGER(ibis::gVerbose >= 0)
	    << "Warning -- request fileManager::unload(" << size
	    << ") can not be honored, maxBytes (" << std::setprecision(3)
	    << static_cast<double>(maxBytes) << ") too small";
	return -113;
    }
    if (ibis::gVerbose > 4) {
	ibis::util::logger lg;
	if (ibis::gVerbose > 8) {
	    printStatus(lg.buffer());
	}
	if (size > 0)
	    lg.buffer()
		<< "\nibis::fileManager::unload(" << size
		<< ") to free up " << size << " bytes of space (totalBytes="
		<< ibis::fileManager::totalBytes() << ", maxBytes="
		<< std::setprecision(3) << static_cast<double>(maxBytes)
		<< ")";
	else
	    lg.buffer()
		<< "\nibis::fileManager::unload to free up all unused "
		<< "space (totalBytes=" << ibis::fileManager::totalBytes()
		<< ", maxBytes=" << std::setprecision(3)
		<< static_cast<double>(maxBytes) << ")";
    }

    // collect the list of files that can be unloaded
    std::vector<fileList::iterator> candidates;
    fileList::iterator it;
    time_t startTime = time(0);
    time_t current = startTime;

    while (current < startTime+FASTBIT_MAX_WAIT_TIME) { // will wait
	size_t sum = 0; // sum of the total bytes that can can be unloaded
	for (it=mapped.begin(); it!=mapped.end(); ++it) {
	    if ((*it).second->inUse() == 0 &&
		(*it).second->pastUse() > 0) {
		sum += (*it).second->size();
	    }
	}
	for (it=incore.begin(); it!=incore.end(); ++it) {
	    if ((*it).second->inUse() == 0 &&
		(*it).second->pastUse() > 0) {
		sum += (*it).second->size();
	    }
	}
	if (maxBytes <= ibis::fileManager::totalBytes() ||
	    ibis::fileManager::totalBytes()-sum > maxBytes-size)
	    // invoke the external cleaners and recompute the total
	    invokeCleaners();

	// collects the candidates to be removed
	for (it=mapped.begin(); it!=mapped.end(); ++it) {
	    if ((*it).second->inUse() == 0 &&
		(*it).second->pastUse() > 0) {
		candidates.push_back(it);
	    }
	}
	for (it=incore.begin(); it!=incore.end(); ++it) {
	    if ((*it).second->inUse() == 0 &&
		(*it).second->pastUse() > 0) {
		candidates.push_back(it);
	    }
	}

	if (candidates.size() > 1) {
	    const size_t ncand = candidates.size();
	    // sort the candidates in descending order of scores
	    std::vector<float> scores(candidates.size());
	    for (unsigned i = 0; i < ncand; ++ i) {
		scores[i] = (*(candidates[i])).second->score();
	    }
	    for (unsigned stride = ncand/2; stride > 0; stride /= 2) {
		for (unsigned i = 0; i < ncand - stride; ++ i) {
		    if (scores[i] < scores[i+stride]) {
			float tmp = scores[i];
			scores[i] = scores[i+stride];
			scores[i+stride] = tmp;
			it = candidates[i];
			candidates[i] = candidates[i+stride];
			candidates[i+stride] = it;
		    }
		}
	    }
	}
	if (size == 0) {
	    if (ibis::gVerbose > 4 && candidates.size() > 0) {
		ibis::util::logger lg;
		lg.buffer() << "ibis::fileManager::unload -- unloading all ("
			    << candidates.size() << ") inactive files";
		if (ibis::gVerbose > 6) {
		    lg.buffer() << "\n";
		    (*it).second->printStatus(lg.buffer());
		}
	    }
	    for (size_t i = 0; i < candidates.size(); ++ i) {
		it = candidates[i];
		roFile *tmp = (*it).second; // fileManager::roFile to delete
		if (tmp->mapped) {
		    mapped.erase(it);
		}
		else {
		    incore.erase(it);
		}
		delete tmp;
	    }
	    return 0;
	}
	else if (candidates.size() > 0) {
	    // note: totalBytes is updated when an object is deleted
	    while (candidates.size() > 0 &&
		   maxBytes-size < ibis::fileManager::totalBytes())  {
		it = candidates.back();
		roFile *tmp = (*it).second;
		if (ibis::gVerbose > 4) {
		    ibis::util::logger lg;
		    lg.buffer() << "ibis::fileManager::unload -- "
			"unloading file \"" << (*it).first << "\"";
		    if (ibis::gVerbose > 7) {
			lg.buffer() << "\n";
			(*it).second->printStatus(lg.buffer());
		    }
		}

		if (tmp->mapped) {
		    mapped.erase(it);
		}
		else {
		    incore.erase(it);
		}
		delete tmp; // remove the target selected
		candidates.resize(candidates.size()-1);
	    }
	    if (maxBytes-size >= ibis::fileManager::totalBytes())
		return 0;
	}

	candidates.clear(); // remove space taken up by candiates
// 	if (mapped.size()+incore.size() < 3) { // allow at least three files
// 	    return 1;
// 	}

	if (nwaiting > 0) {
	    // a primitive strategy: only one thread can wait for any
	    // positive amount of space
	    LOGGER(ibis::gVerbose >= 0)
		<< "Warning -- fileManager::unload yields to another thread "
		<< "already waiting for memory ...";
	    return -108;
	}

	++ nwaiting;
	if (ibis::gVerbose > 3) {
	    ibis::util::logger lg;
	    lg.buffer() << "ibis::fileManager::unload unable to find " << size
			<< " bytes of free space (totalBytes="
			<< ibis::fileManager::totalBytes() << ", maxBytes="
			<< maxBytes << "), will wait...";
	    if (ibis::gVerbose > 6) {
		lg.buffer() << "\n";
		printStatus(lg.buffer());
	    }
	}
	int ierr = 0;
	// has to wait for condition change
#if defined(CLOCK_REALTIME) && (defined(HAVE_STRUCT_TIMESPEC) || defined(__USE_POSIX) || _POSIX_VERSION+0 > 199900)
	// has clock_gettime to get the current time
	struct timespec tsp;
	ierr = clock_gettime(CLOCK_REALTIME, &tsp);
	if (ierr == 0) {
	    tsp.tv_sec += (FASTBIT_MAX_WAIT_TIME > 4 ?
			   (FASTBIT_MAX_WAIT_TIME  >> 2) : 1);
	    ierr = pthread_cond_timedwait(&cond, &mutex, &tsp);
	}
	else {
	    tsp.tv_sec = current + (FASTBIT_MAX_WAIT_TIME > 4 ?
				    (FASTBIT_MAX_WAIT_TIME  >> 2) : 1) + 1;
	    tsp.tv_nsec = 0;
	    ierr = pthread_cond_timedwait(&cond, &mutex, &tsp);
	}
#elif (defined(_WIN32) && defined(_MSC_VER)) || defined(HAVE_STRUCT_TIMESPEC) || defined(__USE_POSIX) || _POSIX_VERSION+0 > 199900
	// assume pthread implementation has pthread_cond_timedwait
	struct timespec tsp;
	tsp.tv_sec = current + (FASTBIT_MAX_WAIT_TIME > 4 ?
				(FASTBIT_MAX_WAIT_TIME  >> 2) : 1) + 1;
	tsp.tv_nsec = 0;
	ierr = pthread_cond_timedwait(&cond, &mutex, &tsp);
#else
	ierr = pthread_cond_wait(&cond, &mutex);
#endif
	-- nwaiting;
	if (ierr != 0 && ierr != ETIMEDOUT) {
	    LOGGER(ibis::gVerbose >= 0)
		<< "Warning -- fileManager::unload unable to wait for release "
		"of memory ... " << strerror(ierr);
	    break; // get out of the while loop
	}
	current = time(0);
    } // while (...)

    // time-out
    LOGGER(ibis::gVerbose >= 0)
	<< "Warning -- fileManager::unload unable to free enough space for "
	<< size << " byte" <<  (size>1 ? "s" : "") << " (totalBytes="
	<< ibis::fileManager::totalBytes() << ", maxBytes=" << maxBytes << ")";
    return -109;
} // ibis::fileManager::unload

void ibis::fileManager::invokeCleaners() const {
    LOGGER(ibis::gVerbose > 5)
	<< "fileManager invoking registered external cleaners ...";
    const uint64_t before = ibis::fileManager::totalBytes();
    for (cleanerList::const_iterator it = cleaners.begin();
	 it != cleaners.end();
	 ++it)
	(*it)->operator()();

    if (ibis::fileManager::totalBytes() < before) {
	LOGGER(ibis::gVerbose > 7)
	    << "ibis::fileManager -- external cleaners reduce totalBytes from "
	    << before << " to " << ibis::fileManager::totalBytes();
    }
    else if (ibis::gVerbose > 5) {
	ibis::util::logger lg;
	lg.buffer() << "ibis::fileManager -- external cleaners "
		    << "did not reduce the total bytes ("
		    << ibis::fileManager::totalBytes() << ")";
	if (ibis::gVerbose > 10) {
	    lg.buffer() << "\n";
	    printStatus(lg.buffer());
	}
    }
} // ibis::fileManager::invokeCleaners

/// To be used by clients that are aware of the memory usages of in-memory
/// objects since the in-memory objects based on ibis::fileManager::storage
/// does not produce signals when they are freed.
void ibis::fileManager::signalMemoryAvailable() const {
    ibis::util::mutexLock lock(&mutex, "fileManager::signalMemoryAvailable");
    if (nwaiting > 0) {
	int ierr =
	    pthread_cond_signal(&(ibis::fileManager::instance().cond));
	LOGGER(ierr != 0 && ibis::gVerbose >= 0)
	    << "Warning -- fileManager::signalMemoryAvailable received code "
	    << ierr << " from pthread_cond_signal";
    }
} // ibis::fileManager::signalMemoryAvailable

//////////////////////////////////////////////////////////////////////
// functions for the buffer template
//
/// Constructor.  The incoming argument is the number of elements to be
/// allocated.  If it is zero, the default is to use 16 MB of space, and
/// the number of elements is 16 million divided by the size of the
/// element.  If it fails to allocate the requested memory, it will reduce
/// the number of elements by a half and then by a quarter for a total of
/// seven times.  If it failed all eight tries, it will set the buffer
/// address to nil and the number of elements to zero.  It also check to
/// make sure it does not use more than 1/4th of free memory.  The buffer
/// may contain no elements at all if there is insufficient amount of
/// memory to use.  The caller should always check that buffer.size() > 0
/// and buffer.address() != 0.
template <typename T>
ibis::fileManager::buffer<T>::buffer(size_t sz) : buf(0), nbuf(sz) {
    long unsigned nfree = ibis::fileManager::bytesFree();
    if (nfree > 0x80000000) // will not use more than 2GB for a buffer
	nfree = 0x80000000;
    if (nfree == 0) {
	nbuf = 0;
	return;
    }
    if (nbuf == 0)
	nbuf = 16777216/sizeof(T); // preferred buffer size is 16 MB
    if (nbuf*sizeof(T) > (nfree >> 2)) {
	nbuf = (nfree >> 2) / sizeof(T);
    }
    if (nbuf == 0) return;
    try {buf = new T[nbuf];}
    catch (const std::bad_alloc&) { // 1
	nbuf >>= 1; // reduce the size by half and try again
	if (nbuf == 0) return;
	try {buf = new T[nbuf];}
	catch (const std::bad_alloc&) { // 2
	    nbuf >>= 2; // reduce the size by a quarter and try again
	    if (nbuf == 0) return;
	    try {buf = new T[nbuf];}
	    catch (const std::bad_alloc&) { // 3
		nbuf >>= 2; // reduce the size by a quarter and try again
		if (nbuf == 0) return;
		try {buf = new T[nbuf];}
		catch (const std::bad_alloc&) { // 4
		    nbuf >>= 2;
		    if (nbuf == 0) return;
		    try {buf = new T[nbuf];}
		    catch (const std::bad_alloc&) { // 5
			nbuf >>= 2;
			if (nbuf == 0) return;
			try {buf = new T[nbuf];}
			catch (const std::bad_alloc&) { // 6
			    nbuf >>= 2;
			    if (nbuf == 0) return;
			    try {buf = new T[nbuf];}
			    catch (const std::bad_alloc&) { // 7
				nbuf >>= 2;
				if (nbuf == 0) return;
				try {buf = new T[nbuf];}
				catch (const std::bad_alloc&) { // 8
				    nbuf = 0;
				    buf = 0;
				}
			    }
			}
		    }
		}
	    }
	}
    }
    if (nbuf > 0) {
	std::string evt = "fileManager::buffer";
	if (ibis::gVerbose > 8) {
	    evt += '<';
	    evt += typeid(T).name();
	    evt += '>';
	    std::ostringstream oss;
	    oss << "(" << static_cast<void*>(buf) << ", " << nbuf << ")";
	    evt += oss.str();
	}
	ibis::fileManager::increaseUse(nbuf*sizeof(T), evt.c_str());
    }
} // ibis::fileManager::buffer::ctor

template<typename T>
ibis::fileManager::buffer<T>::~buffer() {
    if (buf != 0) {
	delete [] buf;

	std::string evt = "fileManager::buffer";
	if (ibis::gVerbose > 8) {
	    evt += '<';
	    evt += typeid(T).name();
	    evt += '>';
	    std::ostringstream oss;
	    oss << "(" << static_cast<void*>(buf) << ", " << nbuf << ")";
	    evt += oss.str();
	}
	ibis::fileManager::decreaseUse(nbuf*sizeof(T), evt.c_str());
    }
} // ibis::fileManager::buffer::dtor

/// Increase the number of elements can be stored in the buffer to sz.  If
/// the input size is 0, it doubles the current size.  If the input value
/// is not 0 but less than the current size, nothing is done.  It return
/// the number of elements can be stored.  Since a buffer is intended to be
/// for temporary storage, the existing content is not copied when
/// reallocating new memory.  It will not allocate more than 2GB of memory.
template<typename T>
size_t ibis::fileManager::buffer<T>::resize(size_t sz) {
    long unsigned nfree = ibis::fileManager::bytesFree();
    if (nfree > 0x80000000) // will not use more than 2GB for a buffer
	nfree = 0x80000000;
    if (sz == 0) sz = nbuf + nbuf;
    if (sz == 0) sz = 2048;
    if (sz > nbuf && nfree/sizeof(T) >= sz) {
	std::string evt = "fileManager::buffer";
	if (ibis::gVerbose > 8) {
	    evt += '<';
	    evt += typeid(T).name();
	    evt += '>';
	    std::ostringstream oss;
	    oss << "::resize(" << sz << ")";
	    evt += oss.str();
	}
	T* tmp = new T[sz];
	if (tmp != 0) {
	    delete [] buf;
	    buf = tmp;

	    ibis::fileManager::increaseUse((sz-nbuf)*sizeof(T), evt.c_str());
	    nbuf = sz;
	}
	else {
	    LOGGER(ibis::gVerbose > 1)
		<< evt << " failed to allocate a new array with " << sz
		<< " elements, keeping existing content";
	}
    }
    return nbuf;
} // ibis::fileManager::buffer<T>::resize

// explicit template instantiation required
template class ibis::fileManager::buffer<char>;
template class ibis::fileManager::buffer<signed char>;
template class ibis::fileManager::buffer<unsigned char>;
template class ibis::fileManager::buffer<float>;
template class ibis::fileManager::buffer<double>;
template class ibis::fileManager::buffer<int16_t>;
template class ibis::fileManager::buffer<uint16_t>;
template class ibis::fileManager::buffer<int32_t>;
template class ibis::fileManager::buffer<uint32_t>;
template class ibis::fileManager::buffer<int64_t>;
template class ibis::fileManager::buffer<uint64_t>;

//////////////////////////////////////////////////////////////////////
// functions for the storage class
//
/// Constructor.  Allocate storage for an array of the specified size (in
/// bytes).
ibis::fileManager::storage::storage(size_t n)
    : name(0), m_begin(0), m_end(0), nacc(0), nref() {
    LOGGER(ibis::gVerbose > 15)
	<< "fileManager::storage::storage(" << n << ") ...";
    if (n < 8) n = 8; // at least 8 bytes
    if (n+ibis::fileManager::totalBytes() > ibis::fileManager::maxBytes) {
	/*
				  ibis::util::mutexLock lck(&ibis::fileManager::instance().mutex,
				  				  "fileManager::storage::ctor");*/
				  
	//int ierr = ibis::fileManager::instance().unload(n);
		int ierr  = 0;
	if (ierr < 0) {
	    LOGGER(ibis::gVerbose >= 0)
		<< "Warning -- fileManager::storage::ctor unable to find "
		<< n << " bytes of space in memory";
	    throw ibis::bad_alloc("storage::ctor(memory) failed");
	}
    }
    m_begin = static_cast<char*>(malloc(n));
    if (m_begin != 0) { // malloc was a success
	m_end = m_begin + n;
	std::string evt = "fileManager::storage";
	if (ibis::gVerbose > 8) {
	    std::ostringstream oss;
	    oss << "(" << static_cast<void*>(this) << ": " << n << " --> "
		<< static_cast<void*>(m_begin) << ")";
	    evt += oss.str();
	    LOGGER(ibis::gVerbose > 10)
		<< evt << " initialization completed";
	}
	ibis::fileManager::increaseUse(n, evt.c_str());
    }
    else {
	if (ibis::gVerbose >= 0) {
	    ibis::util::logger lg;
	    lg.buffer() << "Warning -- storage: unable to malloc(" << n
			<< ") bytes of storage";
	    if (ibis::gVerbose > 1) {
		lg.buffer() << "\n";
		printStatus(lg.buffer()); // dump the current list of files
	    }
	}
	throw ibis::bad_alloc("unable to allocate new storage object");
    }
} // ibis::fileManager::storage::storage

/// Read part of a open file, from [begin, end).
ibis::fileManager::storage::storage(const char* fname,
				    const off_t begin,
				    const off_t end)
    : name(0), m_begin(0), m_end(0), nacc(0), nref() {
    if (fname == 0 || *fname == 0 || end <= begin) return;

    long nbytes = end - begin;
    off_t ierr = read(fname, begin, end);
    if (ierr != nbytes) {
	LOGGER(ibis::gVerbose >= 0)
	    << "Warning -- expected to read " << nbytes << " byte"
	    << (nbytes > 1 ? "s" : "") << " from " << fname
	    << ", but only read " << ierr;
	throw ibis::bad_alloc("storage::ctor failed to read file segement");
    }
} // ibis::fileManager::storage::storage

/// Read part of a open file, from [begin, end).
ibis::fileManager::storage::storage(const int fdes,
				    const off_t begin,
				    const off_t end)
    : name(0), m_begin(0), m_end(0), nacc(0), nref() {
    if (fdes < 0 || end <= begin) return;

    long nbytes = end - begin;
    off_t ierr = read(fdes, begin, end);
    if (ierr != nbytes) {
	LOGGER(ibis::gVerbose >= 0)
	    << "Warning -- expected to read " << nbytes << " byte"
	    << (nbytes > 1 ? "s" : "") << " from file descriptor " << fdes
	    << ", but only read " << ierr;
	throw ibis::bad_alloc("storage::ctor failed to read file segement");
    }
} // ibis::fileManager::storage::storage

/// Copy constructor.  Copy only the part between begin and end [begin, end).
ibis::fileManager::storage::storage(const char* begin, const char* end)
    : name(0), m_begin(0), m_end(0), nacc(0), nref() {
    if (end <= begin) return;
    LOGGER(ibis::gVerbose > 15)
	<< "fileManager::storage::storage(" << static_cast<const void*>(begin)
	<< ", " << static_cast<const void*>(end) << ") ...";
    long nbytes = end - begin;
    if (nbytes+ibis::fileManager::totalBytes() > ibis::fileManager::maxBytes) {
	ibis::util::mutexLock lck(&ibis::fileManager::instance().mutex,
				  "fileManager::storage::ctor");
	int ierr = ibis::fileManager::instance().unload(nbytes);
	if (ierr < 0) {
	    LOGGER(ibis::gVerbose >= 0)
		<< "Warning -- fileManager::storage is unable to find "
		<< nbytes << " bytes of space to copy from "
		<< static_cast<const void*>(begin);
	    throw ibis::bad_alloc("storage::ctor(copy memory) failed");
	}
    }
    m_begin = static_cast<char*>(malloc(nbytes));
    if (m_begin != 0) { // malloc was a success
	(void)memcpy(m_begin, begin, nbytes);
	m_end = m_begin + nbytes;
	std::string evt = "fileManager::storage";
	if (ibis::gVerbose > 8) {
	    std::ostringstream oss;
	    oss << "(" << static_cast<void*>(this) << ": "
		<< static_cast<const void*>(begin) << " -- "
		<< static_cast<const void*>(end) << " to "
		<< static_cast<const void*>(m_begin) << ")";
	    evt += oss.str();
	    LOGGER(ibis::gVerbose > 10)
		<< evt << " initialization completed";
	}
	ibis::fileManager::increaseUse(nbytes, evt.c_str());
    }
    else {
	if (ibis::gVerbose >= 0) {
	    ibis::util::logger lg;
	    lg.buffer() << "Warning -- fileManager copy constructor "
		"is unable to allocate " << nbytes << "bytes\n";
	    printStatus(lg.buffer()); // dump the current list of files
	}
	throw ibis::bad_alloc("unable to copy of in-memory object");
    }
} // ibis::fileManager::storage::storage

/// Copy constructor.  Make an in-memory copy.
ibis::fileManager::storage::storage(const ibis::fileManager::storage& rhs)
    : name(0), m_begin(0), m_end(0), nacc(0), nref() {
    LOGGER(ibis::gVerbose > 15)
	<< "fileManager::storage::storage(" << static_cast<const void*>(&rhs)
	<< ") ... start copying";
    unsigned long nbytes = rhs.size();
    if (nbytes == 0) return;

    if (nbytes+ibis::fileManager::totalBytes() > ibis::fileManager::maxBytes) {
	ibis::util::mutexLock lck(&ibis::fileManager::instance().mutex,
				  "fileManager::storage::ctor");
	int ierr = ibis::fileManager::instance().unload(nbytes);
	if (ierr < 0) {
	    LOGGER(ibis::gVerbose >= 0)
		<< "Warning -- fileManager::storage is unable to find "
		<< nbytes << " bytes of space to make an in-memory copy";
	    throw ibis::bad_alloc("storage::ctor(copy) failed");
	}
    }
    m_begin = static_cast<char*>(malloc(nbytes));
    if (m_begin != 0) { // malloc was a success
	(void)memcpy(static_cast<void*>(m_begin),
		     static_cast<void*>(rhs.m_begin), nbytes);
	m_end = m_begin + nbytes;
	std::string evt = "fileManager::storage";
	if (ibis::gVerbose > 8) {
	    std::ostringstream oss;
	    oss << "(" << static_cast<void*>(this) << ": "
		<< static_cast<void*>(rhs.m_begin) << ", "
		<< rhs.size() << " --> " << static_cast<void*>(m_begin)
		<< ")";
	    evt += oss.str();
	    LOGGER(ibis::gVerbose > 10)
		<< evt << " initialization completed";
	}
	ibis::fileManager::increaseUse(nbytes, evt.c_str());
    }
    else {
	if (ibis::gVerbose >= 0) {
	    ibis::util::logger lg;
	    lg.buffer() << "Warning -- fileManager copy constructor "
		"is unable to allocate " << nbytes << " bytes\n";
	    printStatus(lg.buffer()); // dump the current list of files
	}
	throw ibis::bad_alloc("unable to copy a storage object");
    }
} // ibis::fileManager::storage::storage

/// Assignment operator.  Make an in-memory copy through the copy
/// constructor.
ibis::fileManager::storage&
ibis::fileManager::storage::operator=
(const ibis::fileManager::storage& rhs) {
    storage tmp(rhs);
    swap(tmp);
    return *this;
} // ibis::fileManager::storage::operator=

/// Copy function.  Make an in-meory copy following the copy-and-swap idiom.
void ibis::fileManager::storage::copy
(const ibis::fileManager::storage& rhs) {
    try {
	ibis::fileManager::storage cp(rhs);
	swap(cp);
    }
    catch (...) {
	LOGGER(ibis::gVerbose >= 0)
	    << "Warning -- fileManager::storage::copy("
	    << static_cast<const void*>(&rhs)
	    << ") failed to allocate a copy in memory";
    }
} // ibis::fileManager::storage::copy

/// Enlarge the memory allocated for the storage object.  It increases the
/// memory reserved to the specified size (in bytes).  It increases the
/// current array by 61.8% if nelm is smaller than the current size.  The
/// caller needs to check the actual size of the storage object to make
/// sure a new larger size is actually available.
void ibis::fileManager::storage::enlarge(size_t nelm) {
    std::string evt = "fileManager::storage::enlarge";
    const size_t oldsize = size();
    if (nelm < oldsize)
	nelm = (oldsize<1024 ? oldsize+oldsize :
		static_cast<uint32_t>(1.6180339887498948482*oldsize));
    if (ibis::gVerbose > 8) {
	std::ostringstream oss;
	oss << "(" << static_cast<void*>(this) << ": size " << oldsize
	    << " --> " << nelm << ")";
	evt += oss.str();
    }
    if (oldsize == 0) { // empty storage
	if (nelm+ibis::fileManager::totalBytes() >
	    ibis::fileManager::maxBytes) {
	    
				     /*
				      				       ibis::util::mutexLock lck(&ibis::fileManager::instance().mutex,
				      				      				      				      evt.c_str());*/
				      				      
				      
	   // int ierr = ibis::fileManager::instance().unload(nelm);
					  int ierr = 0;
	    if (ierr < 0) {
		LOGGER(ibis::gVerbose >= 0)
		    << "Warning -- " << evt << " is unable to find "
		    << nelm << " bytes of space in memory";
		throw ibis::bad_alloc("storage::enlarge failed");
	    }
	}
	m_begin = static_cast<char*>(malloc(nelm));
	if (m_begin != 0) {
	    m_end = m_begin + nelm;
	}
	else { // try malloc again
	    int ierr = 0;
	    {
		ibis::util::mutexLock
		    lck(&ibis::fileManager::instance().mutex, evt.c_str());
		ierr = ibis::fileManager::instance().unload(nelm);
	    }
	    if (ierr < 0) {
		m_end = m_begin;
		LOGGER(ibis::gVerbose >= 0)
		    << "Warning -- " << evt << " is unable to find "
		    << nelm << " bytes of space in memory";
		throw ibis::bad_alloc("storage::enlarge (retry) failed");
	    }
	    m_begin = static_cast<char*>(malloc(nelm));
	    if (m_begin != 0) {
		m_end = m_begin + nelm;
	    }
	    else {
		m_begin = 0; m_end = 0;
		if (ibis::gVerbose >= 0) {
		    ibis::util::logger lg;
		    lg.buffer()
			<< "Warning -- " << evt << " unable to allocate "
			<< nelm << " bytes\n";
		    printStatus(lg.buffer()); // dump the current list of files
		}
		throw ibis::bad_alloc("failed allocation in enlarge");
	    }
	}
	LOGGER(ibis::gVerbose > 10)
	    << evt << " -- allocated " << nelm << " bytes at "
	    << static_cast<void*>(m_begin);
	ibis::fileManager::increaseUse(nelm, evt.c_str());
    }
    else { // resize the current storage object with copy-and-swap
	try {
	    ibis::fileManager::storage cp(nelm);
	    memcpy(cp.m_begin, m_begin, oldsize);
	    swap(cp);
	}
	catch (...) {
	    LOGGER(ibis::gVerbose >= 0)
		<< "Warning -- " << evt << " failed to allocate new storage, "
		"current storage unchanged";
	}
    }
} // ibis::fileManager::storage::enlarge

/// Actually freeing the storage allocated.
void ibis::fileManager::storage::clear() {
    std::string evt = "fileManager::storage::clear";
    if (nref() > 0) {
	LOGGER(ibis::gVerbose > 3)
	    << "Warning -- " << evt << " -- storage object at 0x "
	    << static_cast<void*>(m_begin) << " busy (nref=" << nref() << ")";
	return;
    }
    if (ibis::gVerbose > 8) {
	std::ostringstream oss;
	oss << "(" << static_cast<void*>(this) << ", "
	    << static_cast<void*>(m_begin);
	if (name)
	    oss << ", " << name;
	oss << ")"; 
	evt += oss.str();
	LOGGER(ibis::gVerbose > 10)
	    << evt << " ...";
    }

    ibis::fileManager::decreaseUse(size(), evt.c_str());
    free(m_begin);
    m_begin = 0;
    m_end = 0;
    nacc = 0;
    if (name) {
	delete [] name;
	name = 0;
    }
} // ibis::fileManager::storage::clear

/// Print information about the storage object to the specified output
/// stream.
void ibis::fileManager::storage::printStatus(std::ostream& out) const {
    if (name)
	out << "file name       " << name << "\n";
    out << "storage @ " << static_cast<const void*>(this) << ", "
	<< static_cast<const void*>(m_begin);
    if (m_begin != 0 && m_end > m_begin) {
	out << ", 1st 32 bits = " << std::hex
	    << *reinterpret_cast<uint32_t*>(m_begin) << std::dec;
	if (m_end >= m_begin+8)
	    out << ", 1st 64 bits = " << std::hex
		<< *reinterpret_cast<uint64_t*>(m_begin) << std::dec;
    }
    out << "\n# of bytes      " << size()
	<< "\t# of past acc   " << nacc
	<< "\t# of active acc " << nref() << std::endl;
} // ibis::fileManager::storage::printStatus

/// Read part of a open file [begin, end).  Return the number of bytes read.
off_t ibis::fileManager::storage::read(const char* fname,
				       const off_t begin,
				       const off_t end) {
    off_t nread = 0;
    if (fname == 0 || *fname == 0) return -1;
    if (end <= begin) return nread;

    std::string evt = "fileManager::storage::read";
    if (ibis::gVerbose >= 0) {
	std::ostringstream oss;
	oss << "(" << "fname=" << fname << ", begin=" << begin << ", end="
	    << end << ")";
	evt += oss.str();
    }
    int fdes = UnixOpen(fname, OPEN_READONLY);
    if (fdes < 0) {
	LOGGER(ibis::gVerbose > 0)
	    << "Warning -- " << evt << " failed to open the named file";
	return -2;
    }
    ibis::util::guard gfdes = ibis::util::makeGuard(UnixClose, fdes);
#if defined(_WIN32) && defined(_MSC_VER)
    (void)_setmode(fdes, _O_BINARY);
#endif

    off_t nbytes = end - begin;
    if (m_begin == 0U) {
	if (nbytes+ibis::fileManager::totalBytes() >
	    ibis::fileManager::maxBytes) {
	    ibis::util::mutexLock lck(&ibis::fileManager::instance().mutex,
				      evt.c_str());
	    int ierr = ibis::fileManager::instance().unload(nbytes);
	    if (ierr < 0) {
		LOGGER(ibis::gVerbose >= 0)
		    << "Warning -- " << evt << " is unable to find "
		    << nbytes << " bytes of space in memory";
		throw ibis::bad_alloc("storage::read failed");
	    }
	}
	m_begin = static_cast<char*>(malloc(nbytes));
	if (m_begin == 0){
	    m_end = m_begin;
	    {
		ibis::util::logger lg;
		lg.buffer() << "Warning -- " << evt
			    << " is unable to allocate " << nbytes
			    << " bytes\n";
		printStatus(lg.buffer()); // dump the current list of files
	    }
	    throw ibis::bad_alloc("failed to allocate space for reading");
	}
	else {
	    LOGGER(ibis::gVerbose > 10)
		<< evt << " -- allocated " << nbytes << " bytes at "
		<< (void*) m_begin;
	}
	ibis::fileManager::increaseUse(nbytes, evt.c_str());
	m_end = m_begin + nbytes;
    }
    else if (m_end < nbytes+m_begin) { // not enough space
	enlarge(nbytes);
    }

    nread = UnixSeek(fdes, begin, SEEK_SET);
    if (nread != begin) {
	LOGGER(ibis::gVerbose >= 0)
	    << "Warning -- " << evt << " failed to seek to " << begin << " ... "
	    << (errno!=0 ? strerror(errno) : "???");
	return 0;
    }

    if (ibis::gVerbose < 8) {
	nread = UnixRead(fdes, m_begin, nbytes);

	ibis::fileManager::instance().recordPages(begin, end);
	LOGGER(nread != nbytes && ibis::gVerbose >= 0)
	    << "Warning -- " << evt << " allocated " << nbytes << " bytes at "
	    << static_cast<const void*>(m_begin) << ", but only read " << nread;
    }
    else {
	ibis::horometer timer;
	timer.start();
	nread = UnixRead(fdes, m_begin, nbytes);
	timer.stop();

	ibis::fileManager::instance().recordPages(begin, end);
	if (nread == nbytes) {
	    double tcpu = timer.CPUTime();
	    double treal = timer.realTime();
	    double rt1 = tcpu > 0 ? (1e-6*nbytes/tcpu) : 0.0;
	    double rt2 = treal > 0 ? (1e-6*nbytes/treal) : 0.0;
	    LOGGER(ibis::gVerbose > 7)
		<< evt << " -- read " << nbytes << " bytes in "
		<< treal << " sec(elapsed) [" << tcpu
		<< " sec(CPU)] at a speed of "
		<< std::setprecision(3) << rt2 << " MB/s ["
		<< std::setprecision(3) << rt1 << "]";
	}
	else {
	    LOGGER(ibis::gVerbose >= 0)
		<< "Warning -- " << evt << " allocated " << nbytes
		<< " bytes at "	<< static_cast<const void*>(m_begin)
		<< ", but only read " << nread;
	}
    }
    return nread;
} // ibis::fileManager::storage::read

/// Read part of a open file [begin, end).  Return the number of bytes read.
off_t ibis::fileManager::storage::read(const int fdes,
				       const off_t begin,
				       const off_t end) {
    off_t nread = 0;
    if (fdes < 0 || end <= begin) return nread;

    std::string evt = "fileManager::storage::read";
    if (ibis::gVerbose >= 0) {
	std::ostringstream oss;
	oss << "(" << "fdes=" << fdes << ", begin=" << begin << ", end="
	    << end << ")";
	evt += oss.str();
    }
    off_t nbytes = end - begin;
    if (m_begin == 0U) {
	if (nbytes+ibis::fileManager::totalBytes() >
	    ibis::fileManager::maxBytes) {
	    ibis::util::mutexLock lck(&ibis::fileManager::instance().mutex,
				      evt.c_str());
	    int ierr = ibis::fileManager::instance().unload(nbytes);
	    if (ierr < 0) {
		LOGGER(ibis::gVerbose >= 0)
		    << "Warning -- " << evt << " is unable to find "
		    << nbytes << " bytes of space in memory";
		throw ibis::bad_alloc("storage::read failed");
	    }
	}
	m_begin = static_cast<char*>(malloc(nbytes));
	if (m_begin == 0){
	    m_end = m_begin;
	    {
		ibis::util::logger lg;
		lg.buffer() << "Warning -- " << evt
			    << " is unable to allocate " << nbytes
			    << " bytes\n";
		printStatus(lg.buffer()); // dump the current list of files
	    }
	    throw ibis::bad_alloc("failed to allocate space for reading");
	}
	else {
	    LOGGER(ibis::gVerbose > 10)
		<< evt.c_str() << " -- allocated " << nbytes << " bytes at "
		<< (void*) m_begin;
	}
	ibis::fileManager::increaseUse(nbytes, evt.c_str());
	m_end = m_begin + nbytes;
    }
    else if (m_end < nbytes+m_begin) { // not enough space
	enlarge(nbytes);
    }

    nread = UnixSeek(fdes, begin, SEEK_SET);
    if (nread != begin) {
	LOGGER(ibis::gVerbose >= 0)
	    << "Warning -- " << evt << " failed to seek to " << begin << " ... "
	    << (errno!=0 ? strerror(errno) : "???");
	return 0;
    }

    if (ibis::gVerbose < 8) {
	nread = UnixRead(fdes, m_begin, nbytes);

	ibis::fileManager::instance().recordPages(begin, end);
	LOGGER(nread != nbytes && ibis::gVerbose >= 0)
	    << "Warning -- " << evt << " allocated " << nbytes << " bytes at "
	    << static_cast<const void*>(m_begin) << ", but only read " << nread;
    }
    else {
	ibis::horometer timer;
	timer.start();
	nread = UnixRead(fdes, m_begin, nbytes);
	timer.stop();

	ibis::fileManager::instance().recordPages(begin, end);
	if (nread == nbytes) {
	    double tcpu = timer.CPUTime();
	    double treal = timer.realTime();
	    double rt1 = tcpu > 0 ? (1e-6*nbytes/tcpu) : 0.0;
	    double rt2 = treal > 0 ? (1e-6*nbytes/treal) : 0.0;
	    LOGGER(ibis::gVerbose > 7)
		<< evt << " -- read " << nbytes << " bytes in "
		<< treal << " sec(elapsed) [" << tcpu
		<< " sec(CPU)] at a speed of "
		<< std::setprecision(3) << rt2 << " MB/s ["
		<< std::setprecision(3) << rt1 << "]";
	}
	else {
	    LOGGER(ibis::gVerbose >= 0)
		<< "Warning -- " << evt << " allocated " << nbytes
		<< " bytes at " << static_cast<const void*>(m_begin)
		<< ", but only read " << nread;
	}
    }
    return nread;
} // ibis::fileManager::storage::read

void ibis::fileManager::storage::write(const char* file) const {
    size_t n, i;
    FILE *in = fopen(file, "wb");
    if (in == 0) {
	LOGGER(ibis::gVerbose >= 0)
	    << "Warning -- storage::write is unable open file \""
	    << file << "\" ... "
	    << (errno!=0 ? strerror(errno) : "no free stdio stream");
	return;
    }

    n = m_end - m_begin;
    i = fwrite(static_cast<void*>(m_begin), 1, n, in);
    fclose(in); // close the file
    if (i != n) {
	LOGGER(ibis::gVerbose >= 0)
	    << "Warning -- storage::write expects to write "
	    << n << " bytes to \"" << file << "\", but only wrote " << i;
	remove(file); // remove the file
    }
} // ibis::fileManager::storage::write

////////////////////////////////////////////////////////////////////////
// member functions of fileManager::roFile (the read-only file)
//
// the counters nref and nacc should be modified under the control of some
// mutex lock -- however doing so will significantly increate the execution
// time of these simple functions.  Currently we do not use this approach
// but rely on the fact that we should get the access counts correctly.
//
/// Start using a file.  Increments the active reference.
void ibis::fileManager::roFile::beginUse() {
    // acquire a read lock
    if (name != 0) {
	ibis::fileManager::instance().gainReadAccess(name);
    }
    lastUse = time(0);
    ++ nref;
} // ibis::fileManager::roFile::beginUse

/// Stop using a file.  Decrement the active reference count.
void ibis::fileManager::roFile::endUse() {
    const uint32_t nr0 = -- nref; // number of current references
    ++ nacc; // number of past accesses

    // relinquish the read lock
    if (name != 0) {
	ibis::fileManager::instance().releaseAccess(name);
	// signal to ibis::fileManager that this file is ready for deletion
	if (nr0 == 0)
	    pthread_cond_signal(&(ibis::fileManager::instance().cond));
    }
} // ibis::fileManager::roFile::endUse

/// Freeing the storage allocated.  It will only proceed if there is no
/// active reference to it.
void ibis::fileManager::roFile::clear() {
    std::string evt = "fileManager::roFile::clear";
    if (nref() > 0) {
	LOGGER(ibis::gVerbose > 3)
	    << evt << " -- storage " << m_begin << " is busy (nref="
	    << nref() << ") and can't be cleared";
	return;
    }
    if (ibis::gVerbose > 8) {
	std::ostringstream oss;
	oss << "(" << static_cast<void*>(this) << ", "
	    << static_cast<void*>(m_begin);
	if (name)
	    oss << ", " << name;
	oss << ")";
	evt += oss.str();
	LOGGER(ibis::gVerbose > 10)
	    << evt << " ...";
    }

    size_t sz = size();
    ibis::fileManager::decreaseUse(sz, evt.c_str());
    if (mapped == 0) {
	free(m_begin);
    }
    else {
#if defined(_WIN32) && defined(_MSC_VER)
	UnmapViewOfFile(map_begin);
	CloseHandle(fmap);
	CloseHandle(fdescriptor);
#elif (HAVE_MMAP+0 > 0)
	munmap((caddr_t)map_begin, fsize);
	UnixClose(fdescriptor);
#endif
    }

    if (name) {
	delete [] name;
	name = 0;
    }

    m_end = 0;
    m_begin = 0;
} // ibis::fileManager::roFile::clear

void ibis::fileManager::roFile::printStatus(std::ostream& out) const {
    if (name) 
	out << "file name: " << name << "\n";
    printBody(out);
} // ibis::fileManager::roFile::printStatus

void ibis::fileManager::roFile::printBody(std::ostream& out) const {
    char tstr0[28], tstr1[28];
    ibis::util::secondsToString(opened, tstr0);
    ibis::util::secondsToString(lastUse, tstr1);
    out << "storage @ " << static_cast<const void*>(this) << ", "
	<< static_cast<const void*>(m_begin);
    if (m_begin != 0 && m_end > m_begin) {
	out << ", 1st 32 bits = " << std::hex
	    << *reinterpret_cast<uint32_t*>(m_begin) << std::dec;
	if (m_end >= m_begin+8)
	    out << ", 1st 64 bits = " << std::hex
		<< *reinterpret_cast<uint64_t*>(m_begin) << std::dec;
    }
#if (HAVE_MMAP+0 > 0)
    if (fdescriptor >= 0) {
	out << "\nfile descriptor " << fdescriptor
	    << "\nfmap size       " << fsize
	    << "\nbase address    " << map_begin;
    }
#elif defined(_WIN32) && defined(_MSC_VER)
    if (fdescriptor != INVALID_HANDLE_VALUE) {
	out << "\nfile descriptor " << fdescriptor
	    << "\nfmap handle     " << fmap
	    << "\nbase address    " << map_begin;
    }
#endif
    out << "\nmapped          " << (mapped?"y":"n")
	<< "\topened at       " << tstr0
	<< "\tlast used at    " << tstr1
	<< "\n# of bytes      " << size()
	<< "\t# of past acc   " << nacc
	<< "\t# of active acc " << nref() << std::endl;
} // ibis::fileManager::roFile::printBody

void ibis::fileManager::roFile::read(const char* file) {
    if (file == 0 || *file == 0) return;
    if (nref() == 0) {
	if (name) {
	    ibis::fileManager::instance().flushFile(name);
	}
	else {
	    clear();
	}
    }
    else {
	LOGGER(ibis::gVerbose >= 0)
	    << "Warning -- fileManager::roFile "
	    << static_cast<const void*>(this)
	    << " is busy and cann't read new content";
	return;
    }
    doRead(file);
    if (m_begin != 0 && m_end > m_begin) {
	ibis::util::mutexLock lck(&ibis::fileManager::instance().mutex, file);
	ibis::fileManager::instance().recordFile(this);
    }
} // ibis::fileManager::roFile::read

/// Read the content of a file into memory.
void ibis::fileManager::roFile::doRead(const char* file) {
    size_t n, i;
    std::string evt = "fileManager::roFile";
    // first find out the size of the file
    Stat_T tmp;
    if (0 == UnixStat(file, &tmp)) { // get stat correctly
	n = tmp.st_size;
    }
    else {
	LOGGER(ibis::gVerbose >= 0)
	    << "Warning -- " << evt << " is unable find out the size of \""
	    << file << "\"";
	return;
    }

    int in = UnixOpen(file, OPEN_READONLY);
    if (in < 0) {
	LOGGER(ibis::gVerbose >= 0)
	    << "Warning -- " << evt << " is unable open file \"" << file
	    << "\" ... " << (errno ? strerror(errno) : "no free stdio stream");
	return;
    }
    if (ibis::gVerbose > 5) {
	std::ostringstream oss;
	oss << "(" << static_cast<const void*>(this) << ", doRead " << file
	    << ")";
	evt += oss.str();
    }

    clear(); // clear the current content
    m_begin = static_cast<char *>(malloc(n));
    if (m_begin == 0) {
	LOGGER(ibis::gVerbose > 5)
	    << "Warning -- " << evt << " failed to allocate " << n
	    << " bytes of memory, attempt to free all that can be freed";
	ibis::fileManager::instance().unload(0);

	m_begin = static_cast<char *>(malloc(n));
	if (m_begin == 0) {
	    LOGGER(ibis::gVerbose >= 0)
		<< "Warning -- " << evt << " failed to allocate "
		<< n << " bytes of memory after two tries";
	    UnixClose(in);
	    m_end = 0;
	    return;
	}
    }

#if defined(_WIN32) && defined(_MSC_VER)
    (void)_setmode(in, _O_BINARY);
#endif
    i = UnixRead(in, static_cast<void*>(m_begin), n);
    ibis::fileManager::instance().recordPages(0, n);
    UnixClose(in); // close the file
    if (i == static_cast<size_t>(-1)) {
	LOGGER(ibis::gVerbose >= 0)
	    << "Warning -- " << evt << " encountered an error (errno=" << errno
	    << ") while calling function read ... " << strerror(errno);
	free(m_begin);
	m_begin = 0;
	m_end = 0;
	return;
    }
    else if (i != n) {
	LOGGER(ibis::gVerbose >= 0)
	    << "Warning -- roFile::read(" << file << ") expects to read "
	    << n << " bytes from, but only got " << i;
    }
    else {
	LOGGER(ibis::gVerbose > 6)
	    <<"roFile::doRead -- read " << n << " bytes from file \""
	    << file << "\" to " << static_cast<const void*>(m_begin);
    }
    name = ibis::util::strnewdup(file);
    m_end = m_begin + i;
    opened = time(0);
} // ibis::fileManager::roFile::doRead

/// Read a portion of a file into memory.
/// Do NOT record the name of the file.  This is different from the one that
/// read the whole file which automatically records the name of the file.
void ibis::fileManager::roFile::doRead(const char* file, off_t b, off_t e) {
    if (file == 0 || *file == 0 || b >= e)
	return;

    long i;
    const long n = e - b;
    int in = UnixOpen(file, OPEN_READONLY);
    if (in < 0) {
	LOGGER(ibis::gVerbose >= 0)
	    << "Warning -- roFile::read is unable open file \""
	    << file << "\" ... "
	    << (errno ? strerror(errno) : "no free stdio stream");
	return;
    }

    clear(); // clear the current content
    m_begin = static_cast<char *>(malloc(n));
    if (m_begin == 0) {
	LOGGER(ibis::gVerbose > 5)
	    << "roFile::read(" << file << ") -- failed to allocate " << n
	    << " bytes of memory, attempt to free all that can be freed";
	ibis::fileManager::instance().unload(0);

	m_begin = static_cast<char *>(malloc(n));
	if (m_begin == 0) {
	    LOGGER(ibis::gVerbose >= 0)
		<< "Warning -- roFile::read(" << file << ", " << b << ", "
		<< e << ") failed to allocate " << n
		<< " bytes of memory afte rtwo tries";
	    UnixClose(in);
	    m_end = 0;
	    return;
	}
    }

#if defined(_WIN32) && defined(_MSC_VER)
    (void)_setmode(in, _O_BINARY);
#endif
    i = UnixRead(in, static_cast<void*>(m_begin), n);
    ibis::fileManager::instance().recordPages(b, e);
    UnixClose(in); // close the file
    if (i == -1L) {
	LOGGER(ibis::gVerbose >= 0)
	    << "Warning -- roFile::read(" << file << ", " << b
	    << ", %" << e << ") encountered an error (errno=" << errno 
	    << ") calling function read ... " << strerror(errno);
	free(m_begin);
	m_begin = 0;
	m_end = 0;
	return;
    }
    else if (i != n) {
	LOGGER(ibis::gVerbose >= 0)
	    << "Warning -- roFile::read(" << file << ", " << b << ", " << e
	    << ") expects to read " << n << " bytes, but only got " << i;
    }
    else {
	LOGGER(ibis::gVerbose > 6)
	    << "roFile::doRead -- read " << n << " bytes from file \""
	    << file << "\"[" << b << ", " << e << ") to "
	    << static_cast<const void*>(m_begin);
    }
    m_end = m_begin + i;
    opened = time(0);
} // ibis::fileManager::roFile::doRead

#if defined(HAVE_FILE_MAP)
void ibis::fileManager::roFile::mapFile(const char* file) {
    if (file == 0 || *file == 0) return;
    if (nref() == 0) {
	if (name) {
	    ibis::fileManager::instance().flushFile(name);
	}
	clear();
    }
    else {
	LOGGER(ibis::gVerbose >= 0)
	    << "Warning -- ibis::fileManager::roFile is busy and cann't "
	    "read new content";
	return;
    }
    Stat_T tmp;
    if (0 != UnixStat(file, &tmp)) { // get stat correctly
	LOGGER(ibis::gVerbose >= 0)
	    << "Warning -- roFile::mapFile is unable find out the size of \""
	    << file << "\"";
	return;
    }
    if (tmp.st_size > 0) {
	doMap(file, 0, tmp.st_size, 0);
    }
    else {
	LOGGER (ibis::gVerbose > 3)
	    << "roFile::mapFile -- file " << file << " exists but is empty";
	return;
    }
    if (m_end >= m_begin + tmp.st_size) {
	// doMap function has finished correctly
	name = ibis::util::strnewdup(file);
    }
    else {
	LOGGER(ibis::gVerbose > 5)
	    << "roFile::mapFile(" << file << ") failed on the 1st try, "
	    "see if anything can be freed before try again";
	clear();
	ibis::fileManager::instance().unload(0); // free whatever can be freed
	doMap(file, 0, tmp.st_size, 0);

	if (m_end >= m_begin + tmp.st_size) {
	    // doMap function has finished correctly on the second try
	    name = ibis::util::strnewdup(file);
	}
	else {
	    LOGGER(ibis::gVerbose >= 0)
		<< "Warning -- roFile::mapFile failed twice to map file \""
		<< file << "\", will the plain read fair better?";
	    clear();
	    doRead(file);
	    if (m_end >= m_begin + tmp.st_size) {
		name = ibis::util::strnewdup(file);
	    }
	    else {
		LOGGER(ibis::gVerbose >= 0)
		    << "Warning -- roFile::mapFile(" << file
		    << ") did NOT read anything from file either";
		clear();
	    }
	}
    }
} // ibis::fileManager::roFile::mapFile

/// Constructor.
ibis::fileManager::rofSegment::rofSegment(const char *fn, off_t b, off_t e)
    : ibis::fileManager::roFile(), filename_(fn), begin_(b), end_(e) {
    if (fn == 0 || *fn == 0 || b >= e)
	return;

    doMap(fn, b, e, 0);
    if (m_begin == 0 || m_begin + (e-b) != m_end) {
	// clear the partially filled object and try again
	clear();
	throw ibis::bad_alloc("fileManager::rofSegment failed to map file");
    }

    if (m_begin != 0) {
	std::string evt = "fileManager::rofSegment";
	if (ibis::gVerbose > 8) {
	    std::ostringstream oss;
	    oss << "("  << static_cast<void*>(this) << ": " << fn << ", " << b
		<< ", " << e << " --> " << static_cast<void*>(m_begin) << ", "
		<< static_cast<void*>(m_end) << ")";
	    evt += oss.str();
	}
	ibis::fileManager::increaseUse(size(), evt.c_str());
    }
    else {
	throw ibis::bad_alloc("fileManager::rofSegment failed to map file");
    }
} // ibis::fileManager::rofSegment::rofSegment

void ibis::fileManager::rofSegment::printStatus(std::ostream& out) const {
    if (! filename_.empty()) 
	out << "file name: " << filename_ << "[" << begin_ << ", "
	    << end_ << ")\n";
    printBody(out);
} // ibis::fileManager::rofSegment::printStatus
#endif

/// This function maps the specified portion of the file in either
/// read_only (@c opt = 0) mode or read_write (@c opt != 0) mode.
/// @note It assumes the current object contain no valid information.  The
/// caller is responsible to calling function @c clear if necessary.
#if defined(_WIN32) && defined(_MSC_VER)
void ibis::fileManager::roFile::doMap(const char *file, off_t b, off_t e,
				      int opt) {
    if (file == 0 || *file == 0 || b >= e)
	return; // nothing to do

    if (opt == 0) {
	fdescriptor = CreateFile
	    (file, GENERIC_READ, FILE_SHARE_READ,
	     NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
    }
    else {
	fdescriptor = CreateFile
	    (file, GENERIC_WRITE, FILE_SHARE_READ,
	     NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    }	
    if (fdescriptor != INVALID_HANDLE_VALUE) {
	DWORD dhi, dlo;
	if (sizeof(off_t) > sizeof(DWORD)) {
	    dlo = (e & 0xFFFFFFFF);
	    dhi = (e >> 32);
	}
	else {
	    dlo = e;
	    dhi = 0;
	}
	if (opt == 0) { // create an unnamed file mapping object
	    fmap = CreateFileMapping(fdescriptor, NULL, PAGE_READONLY,
				     dhi, dlo, NULL);
	}
	else {
	    fmap = CreateFileMapping(fdescriptor, NULL, PAGE_READWRITE,
				     dhi, dlo, NULL);
	}
	if (fmap != INVALID_HANDLE_VALUE) {
	    DWORD offset = 0;
	    if (sizeof(off_t) > sizeof(DWORD)) {
		dlo = (b & 0xFFFFFFFF);
		dhi = (b >> 32);
	    }
	    else {
		dlo = b;
		dhi = 0;
	    }
	    if (dlo != 0) { // make sure it matches allocation granularity
		SYSTEM_INFO sysinfo;
		GetSystemInfo(&sysinfo);
		offset = dlo;
		dlo = sysinfo.dwAllocationGranularity *
		    (dlo / sysinfo.dwAllocationGranularity);
		offset -= dlo;
	    }
	    if (opt == 0) {
		map_begin = MapViewOfFile(fmap, FILE_MAP_READ, dhi, dlo,
					  e-b+offset);
	    }
	    else {
		map_begin = MapViewOfFile(fmap, FILE_MAP_WRITE, dhi, dlo,
					  e-b+offset);
	    }
	    if (map_begin) {
		mapped  = 1;
		opened  = time(0);
		m_begin = reinterpret_cast<char*>(map_begin) + offset;
		m_end   = m_begin + (e - b);
		LOGGER(ibis::gVerbose > 6)
		    << "roFile::doMap map " << (e-b) << " bytes of file \""
		    << file << "\" to " << static_cast<const void*>(m_begin);
	    }
	    else {
		if (ibis::gVerbose >= 0) {
		    char *lpMsgBuf;
		    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				  FORMAT_MESSAGE_FROM_SYSTEM | 
				  FORMAT_MESSAGE_IGNORE_INSERTS,
				  NULL, GetLastError(),
				  MAKELANGID(LANG_NEUTRAL,
					     SUBLANG_DEFAULT),
				  (LPTSTR) &lpMsgBuf, 0, NULL);
		    LOGGER(ibis::gVerbose >= 0)
			<< "Warning -- " << lpMsgBuf << " " << file;
		    LocalFree(lpMsgBuf);	// Free the buffer.
		}
		m_begin = 0; m_end = 0; mapped = 0;
	    }
	}
	else {
	    if (ibis::gVerbose >= 0) {
		char *lpMsgBuf;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			      FORMAT_MESSAGE_FROM_SYSTEM | 
			      FORMAT_MESSAGE_IGNORE_INSERTS,
			      NULL, GetLastError(),
			      MAKELANGID(LANG_NEUTRAL,
					 SUBLANG_DEFAULT),
			      (LPTSTR) &lpMsgBuf, 0, NULL);
		LOGGER(ibis::gVerbose >= 0)
		    << "Warning -- " << lpMsgBuf << " " << file;
		LocalFree(lpMsgBuf);	// Free the buffer.
	    }
	    m_begin = 0; m_end = 0; mapped = 0;
	}
    }
    else {
	if (ibis::gVerbose >= 0) {
	    char *lpMsgBuf;
	    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			  FORMAT_MESSAGE_FROM_SYSTEM | 
			  FORMAT_MESSAGE_IGNORE_INSERTS,
			  NULL, GetLastError(),
			  MAKELANGID(LANG_NEUTRAL,
				     SUBLANG_DEFAULT),
			  (LPTSTR) &lpMsgBuf, 0, NULL);
	    LOGGER(ibis::gVerbose >= 0)
		<< "Warning -- " << lpMsgBuf << " " << file;
	    LocalFree(lpMsgBuf);	// Free the buffer.
	}
	m_begin = 0; m_end = 0; mapped = 0;
    }
} // doMap on WIN32 under MSVC
#elif (HAVE_MMAP+0 > 0)
void ibis::fileManager::roFile::doMap(const char* file, off_t b, off_t e,
				      int opt) {
    if (file == 0 || *file == 0 || b >= e)
	return;

    if (opt == 0) {
	fdescriptor = open(file, O_RDONLY);
    }
    else {
	fdescriptor = open(file, O_RDWR);
    }
    if (fdescriptor < 0) {
	LOGGER(ibis::gVerbose >= 0)
	    << "Warning -- roFile::doMap is unable open file \""
	    << file << "\" ... "
	    << (errno ? strerror(errno) : "no free stdio stream");
	m_begin = 0; m_end = 0; mapped = 0;
	return;
    }

    off_t offset = b;
    // the start of the memory map must be on page boundary
    b = ibis::fileManager::instance().pagesize *
	(b / ibis::fileManager::instance().pagesize);
    offset -= b;
    fsize = e - b;
    if (opt == 0) {
	map_begin = mmap(0, fsize, PROT_READ, MAP_PRIVATE, fdescriptor, b);
    }
    else {
	map_begin = mmap(0, fsize, PROT_READ|PROT_WRITE, MAP_SHARED,
			 fdescriptor, b);
    }
    if (map_begin != MAP_FAILED) {
	mapped  = 1;
	opened  = time(0);
	m_begin = reinterpret_cast<char*>(map_begin) + offset;
	m_end   = reinterpret_cast<char*>(map_begin) + fsize;
	LOGGER(ibis::gVerbose > 6)
	    << "roFile::doMap -- map " << fsize << " bytes of file \""
	    << file << "\" to " << static_cast<const void*>(map_begin);
    }
    else {
	close(fdescriptor);
	LOGGER(ibis::gVerbose >= 0)
	    << "Warning -- roFile::doMap failed to map file \""
	    << file << "\" on file descriptor " << fdescriptor << " ... "
	    << strerror(errno);
	m_begin = 0; m_end = 0; mapped = 0;
	fdescriptor = -1;
    }
} // ibis::fileManager::roFile::doMap using mmap
#endif

