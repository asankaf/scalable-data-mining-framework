#pragma once
#include "csv_parser.hpp"
#include "PureStringAttInfo.h"
#include <string>
#include "PureIntAttInfo.h"
#include "PureDoubleAttInfo.h"
#include "Commons.h"

/************************************************************************/
/*    Class  :extractedCsvDTO.h
/*	  Started:11.03.2010 13:51:02
/*    Updated:18.04.2010 18:48:51
/*    Author :SEEDMiner
/*    Subj   :Simple Data Transfer Object(DTO) class to store the necessary
/*			  details regarding CSV driven data into the internal framework. 
/*    Version: 
/************************************************************************/

namespace CsvDataExtraction{
	class ExtractedCsvDTO{
	public:
		__declspec(dllexport) ExtractedCsvDTO(){Init();this->_attCount = 0 ; this->_rowCount = 0;this->_stringData = NULL;}
		__declspec(dllexport) PureStringAttInfo** StringData() { return _stringData; }
		__declspec(dllexport) void StringData(PureStringAttInfo** val) { _stringData = val; }
		vector<int> Order() const { return order; }
		void Order(vector<int> val) { order = val; }
		__declspec(dllexport) int RowCount() const { return _rowCount; }
		__declspec(dllexport) void RowCount(int val) { _rowCount = val; }
		__declspec(dllexport) int AttributeCount() const { return _attCount; }
		__declspec(dllexport) void AttributeCount(int val) { _attCount = val; }
		__declspec(dllexport) std::vector<PureIntAttInfo*> IntData() const { return _intData; }
		__declspec(dllexport) void IntData(std::vector<PureIntAttInfo*> val) { _intData = val; }
		__declspec(dllexport) std::vector<PureDoubleAttInfo*> DoubleData() const { return _doubleData; }
		__declspec(dllexport) void DoubleData(std::vector<PureDoubleAttInfo*> val) { _doubleData = val; }
		__declspec(dllexport) std::vector<PureStringAttInfo*> MultiCatData() const { return _multiCatData; }
		__declspec(dllexport) void MultiCatData(std::vector<PureStringAttInfo*> val) { _multiCatData = val; }

		__declspec(dllexport) ~ExtractedCsvDTO(){
			Commons::DeleteVector(this->_intData.begin(),this->_intData.end());
			Commons::DeleteVector(this->_doubleData.begin(),this->_doubleData.end());
			Commons::DeleteVector(this->_multiCatData.begin(),this->_multiCatData.end());

			if (this->_stringData != NULL)
			{
				for (int i=0 ; i<this->_attCount ; i++)
				{				
					if (this->_stringData[i] != NULL)
					{
						delete this->_stringData[i];
					}
				}
				delete []_stringData;
			}

		}

		void Init(){
			Commons::InitVector(this->_intData.begin(),this->_intData.end());
			Commons::InitVector(this->_doubleData.begin(),this->_doubleData.end());
			Commons::InitVector(this->_multiCatData.begin(),this->_multiCatData.end());
		}

		
	private:
		PureStringAttInfo** _stringData;		
		int _rowCount;		
		int _attCount;	
		std::vector<PureIntAttInfo*> _intData;		
		std::vector<PureDoubleAttInfo*> _doubleData;		
		std::vector<PureStringAttInfo*> _multiCatData;
		vector<int> order;
	};
}