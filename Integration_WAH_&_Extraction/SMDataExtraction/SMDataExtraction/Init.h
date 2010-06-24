#pragma once
#include "ExceptionReader.h"
#include "configurationreader.h"

/************************************************************************/
/*    Init.h
/*	  Started:11.03.2010 13:51:02
/*    Updated:28.05.2010 21:18:51
/*    Author :SEEDMiner
/*    Subj   :Initializing class to initialize the readers
/*    Version: 
/************************************************************************/

/**	Initialize the exception reader*/
static struct static_block {
	static_block() {		
		ExceptionReader::BuildFile(string("../conf/exception.exp"));		
	}
} _STATIC_BLOCK_INSTANCE;

/** Initialize the configuration reader*/
static struct configutation_init {
	configutation_init() {		
		ConfigurationReader::BuildFile(string("../conf/algorithm.cfg"));
	}
} _CONFIGURATION_INIT_BLOCK;
