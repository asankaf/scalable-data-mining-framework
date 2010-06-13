#include "ExceptionReader.h"
#include "configurationreader.h"

static struct static_block {
static_block() {		
ExceptionReader::BuildFile(string("../conf/exception.exp"));		
}
} _STATIC_BLOCK_INSTANCE;

static struct configutation_init {
	configutation_init() {		
		ConfigurationReader::BuildFile(string("../conf/algorithm.cfg"));

	}
} _CONFIGURATION_INIT_BLOCK;
