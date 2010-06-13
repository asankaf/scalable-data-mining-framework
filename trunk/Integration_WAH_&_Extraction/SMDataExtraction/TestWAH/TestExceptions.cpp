#include "TestExceptions.h"


using namespace std;
TestCompressionExceptions::TestCompressionExceptions(void)
{
}

TestCompressionExceptions::~TestCompressionExceptions(void)
{
}

void TestCompressionExceptions::TestSuite()
{
	//TestCovertingExceptions();	
	TestNullExceptions();
}

void TestCompressionExceptions::TestCovertingExceptions()
{
	WrapDataSource * source = CreateDataSource();
	try{
		CompressionHandler::ConvertTo(source,BitStreamInfo::NULL_COMP);
	}
	catch(invalid_compression_type_exception & ex)
	{
		cout <<"To string : " << ex.toString() << endl;
	}
	catch(...)
	{
		cout <<"All exceptions caught" << endl;
	}
}

void TestCompressionExceptions::TestNullExceptions()
{
	try{
		CompressionHandler::ConvertTo(NULL,BitStreamInfo::VERTICAL_STREAM_FORMAT);
	}
	catch(null_operand_exception & ex)
	{
		cout <<"To string : " << ex.toString() << endl;
	}
	catch(...)
	{
		cout <<"All exceptions caught" << endl;
	}
}

WrapDataSource * TestCompressionExceptions::CreateDataSource()
{
	/*
	LoadSavedDataSources *lsd = new LoadSavedDataSources(ConfigurationReader::ReadConfiguration(ConfigurationReader::METAFILE_NAME),
			ConfigurationReader::ReadConfiguration(ConfigurationReader::DATAFILE_NAME));	
		DataSources *dsLoaded = lsd->loadSavedEncodedData();
		return (*dsLoaded)(ConfigurationReader::ReadConfiguration(ConfigurationReader::FILE_NAME));*/
	
	return Utils::CreateDataSource(ConfigurationReader::ReadConfiguration(ConfigurationReader::DATAFILE_NAME),
		ConfigurationReader::ReadConfiguration(ConfigurationReader::METAFILE_NAME),
		ConfigurationReader::ReadConfiguration(ConfigurationReader::FILE_NAME));
}
