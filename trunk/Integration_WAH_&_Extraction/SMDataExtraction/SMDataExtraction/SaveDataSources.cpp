#include "stdafx.h"
#include "SaveDataSources.h"
#include "tinyxml.h"
#include <time.h>
#include <sstream>

using namespace std;

bool DataSourceSerialization::serializeDataSource(){
	string xmlFile = this->_xmlFileName + ".xml";

	TiXmlDocument doc;
	TiXmlDeclaration *decl = new TiXmlDeclaration("1.0","UTF-8","");
	doc.LinkEndChild(decl);

	TiXmlElement *rootNode = new TiXmlElement("DataSources");
	rootNode->SetAttribute("noOfdataSources",this->_dataSources->noOfdataSources());
	doc.LinkEndChild(rootNode);

	TiXmlComment *comments = new TiXmlComment(" Meta Information in Data Sources ");
	rootNode->LinkEndChild(comments);

	for(int l = 0 ; l < this->_dataSources->noOfdataSources() ; l++)
	{
		TiXmlElement *dsNode = new TiXmlElement("DataSource");
		rootNode->LinkEndChild(dsNode);

		vector<string> dsNames = this->_dataSources->dsNames();
		WrapDataSource *ds = (*this->_dataSources)(dsNames[l]);
		dsNode->SetAttribute("Name",ds->DataSourceName().c_str());

		TiXmlElement *attNo = new TiXmlElement("noOfAttributes");
		attNo->LinkEndChild(new TiXmlText(itoa(ds->noOfAttributes(),new char[1024],10)));
		dsNode->LinkEndChild(attNo);

		TiXmlElement *noRows = new TiXmlElement("noOfRows");
		noRows->LinkEndChild(new TiXmlText(itoa(ds->noOfRows(),new char[1024],10)));
		dsNode->LinkEndChild(noRows);

		TiXmlElement *dsType = new TiXmlElement("DataSourceType");
		dsType->LinkEndChild(new TiXmlText(itoa((int)ds->SourceType(),new char[1024],10)));
		dsNode->LinkEndChild(dsType);
		// 
		// 	TiXmlElement *qryData = new TiXmlElement("QueryDataInfo");
		// 	qryData->LinkEndChild(new TiXmlText(ds->queryExecPointer().queryStmt()));
		// 	dsNode->LinkEndChild(qryData);

		TiXmlElement *codedAtts = new TiXmlElement("CodedAttributes");
		dsNode->LinkEndChild(codedAtts);

		for (int i = 0 ; i < ds->noOfAttributes() ; i++)
		{
			if(ds->codedAttributes()[i]->attributeType() == ATT_TYPE::SIGNEDINT_VAL){
				EncodedIntAttribute *intAtt = static_cast<EncodedIntAttribute*>(ds->codedAttributes()[i]);
				TiXmlElement *att = new TiXmlElement("Attribute");
				att->SetAttribute("ID",intAtt->attributeID()); 
				att->SetAttribute("Name",intAtt->attributeName().c_str());
				att->SetAttribute("Type",(int)intAtt->attributeType());


				TiXmlElement *noVBitStreams = new TiXmlElement("noOfVBitStreams");
				noVBitStreams->LinkEndChild(new TiXmlText(itoa(intAtt->NoOfVBitStreams(),new char[1024],10)));
				att->LinkEndChild(noVBitStreams);

				TiXmlElement *maxVal = new TiXmlElement("maxval");
				maxVal->LinkEndChild(new TiXmlText(ltoa(intAtt->maxAttVal(),new char[1024],10)));
				att->LinkEndChild(maxVal);

				TiXmlElement *minVal = new TiXmlElement("minval");
				minVal->LinkEndChild(new TiXmlText(ltoa(intAtt->minAttVal(),new char[1024],10)));
				att->LinkEndChild(minVal);

// 				TiXmlElement *signMap = new TiXmlElement("SignMapVal");
// 				signMap->LinkEndChild(new TiXmlText(ltoa(signMapAsLong(intAtt->SignBitMap()).to_ulong(),new char[1024],10)));
// 				att->LinkEndChild(signMap);

				TiXmlElement *signBitSet = new TiXmlElement("SignBitSet");
				string s;
				to_string(intAtt->SignBitSet(),s);
				signBitSet->LinkEndChild(new TiXmlText(s.c_str()));
				att->LinkEndChild(signBitSet);

				codedAtts->LinkEndChild(att);

			}
			else if (ds->codedAttributes()[i]->attributeType() == ATT_TYPE::MULTICAT_VAL)
			{
				EncodedMultiCatAttribute *catVal = static_cast<EncodedMultiCatAttribute*>(ds->codedAttributes()[i]);
				TiXmlElement *att = new TiXmlElement("Attribute");
				att->SetAttribute("ID",catVal->attributeID()); 
				att->SetAttribute("Name",catVal->attributeName().c_str());
				att->SetAttribute("Type",(int)catVal->attributeType());

				TiXmlElement *uniVals = new TiXmlElement("UniqueValues");
				uniVals->SetAttribute("noOfUniqueVals",catVal->noOfUniqueValues());
				for (int j = 0 ; j < catVal->noOfUniqueValues() ; j++)
				{
					TiXmlElement *val = new TiXmlElement("Val");
					val->SetAttribute("ID",j);
					val->LinkEndChild(new TiXmlText(catVal->uniqueValList()[j].c_str()));
					uniVals->LinkEndChild(val);
				}
				att->LinkEndChild(uniVals);

				codedAtts->LinkEndChild(att);
			}
			else if (ds->codedAttributes()[i]->attributeType() == ATT_TYPE::DOUBLE_VAL)
			{
				EncodedDoubleAttribute* doubleAtt = static_cast<EncodedDoubleAttribute*>(ds->codedAttributes()[i]);
				TiXmlElement *att=new TiXmlElement("Attribute");
				att->SetAttribute("ID",doubleAtt->attributeID());
				att->SetAttribute("Name",doubleAtt->attributeName().c_str());
				att->SetAttribute("Type",(int)doubleAtt->attributeType());

				TiXmlElement *noVBitStreams = new TiXmlElement("noOfVBitStreams");
				noVBitStreams->LinkEndChild(new TiXmlText(itoa(doubleAtt->NoOfVBitStreams(),new char[1024],10)));
				att->LinkEndChild(noVBitStreams);

				std::ostringstream oss;
				oss<<doubleAtt->maxAttVal();
				string maxv = oss.str();
				TiXmlElement *maxVal = new TiXmlElement("maxval");
				maxVal->LinkEndChild(new TiXmlText(maxv.c_str()));
				att->LinkEndChild(maxVal);

				std::ostringstream oss_1;
				oss_1<<doubleAtt->minAttVal();
				string minv = oss_1.str();
				TiXmlElement *minVal = new TiXmlElement("minval");
				minVal->LinkEndChild(new TiXmlText(minv.c_str()));
				att->LinkEndChild(minVal);

// 				TiXmlElement *signMap = new TiXmlElement("SignMapVal");
// 				signMap->LinkEndChild(new TiXmlText(ltoa(signMapAsLong(doubleAtt->SignBitMap()).to_ulong(),new char[1024],10)));
// 				att->LinkEndChild(signMap);

				TiXmlElement *signBitSet = new TiXmlElement("SignBitSet");
				string s;
				to_string(doubleAtt->signBitSet(),s);
				signBitSet->LinkEndChild(new TiXmlText(s.c_str()));
				att->LinkEndChild(signBitSet);
				
				TiXmlElement *precision = new TiXmlElement("PrecisionVal");
				precision->LinkEndChild(new TiXmlText(ltoa(doubleAtt->Precision(),new char[1024],10)));
				att->LinkEndChild(precision);

				codedAtts->LinkEndChild(att);
			}
		}
	}

	doc.SaveFile(("../Reports/" + xmlFile).c_str());
	saveCodedData();
	return true;
}

void DataSourceSerialization::saveCodedData(){
	string fileName = "../Reports/" + this->_dataFileName + ".xml";
	TiXmlDocument doc;
	TiXmlDeclaration *decl = new TiXmlDeclaration("1.0","UTF-8","");
	doc.LinkEndChild(decl);

	TiXmlElement *rootNode = new TiXmlElement("DataSources");
	rootNode->SetAttribute("noOfDataSources",this->_dataSources->noOfdataSources());
	doc.LinkEndChild(rootNode);

	TiXmlComment *comments = new TiXmlComment(" Actual Encoded data in Data Sources ");
	rootNode->LinkEndChild(comments);

	TiXmlElement *dsNode = new TiXmlElement("DataSource");
	rootNode->LinkEndChild(dsNode);

	for(int l = 0 ; l < this->_dataSources->noOfdataSources() ; l++)
	{
		vector<string> dsNames = this->_dataSources->dsNames();
		WrapDataSource *ds = (*this->_dataSources)(dsNames[l]);
		dsNode->SetAttribute("Name",ds->DataSourceName().c_str());

		TiXmlElement *codeddataNode = new TiXmlElement("CodedAttributes");
		dsNode->LinkEndChild(codeddataNode);

		for (int i = 0 ; i < ds->noOfAttributes() ; i++)
		{
			EncodedAttributeInfo *attInfo = ds->codedAttributes()[i];
			TiXmlElement *att = new TiXmlElement("Attribute");
			att->SetAttribute("ID",attInfo->attributeID());
			att->SetAttribute("Type",(int)attInfo->attributeType());
			att->SetAttribute("Name",attInfo->attributeName().c_str());
			att->SetAttribute("noOfVBitStreams",attInfo->NoOfVBitStreams());

			TiXmlElement *VBitStreams=new TiXmlElement("VBitStreams");
			for (int j = 0 ; j < attInfo->NoOfVBitStreams() ; j++)
			{
				TiXmlElement *vbs = new TiXmlElement("vbitstream");
				vbs->SetAttribute("ID",j);
				string s;
				to_string(attInfo->vBitStreams()[j]->getProcessedBitStream(),s);
				vbs->LinkEndChild(new TiXmlText(s.c_str()));
				VBitStreams->LinkEndChild(vbs);
			}
			att->LinkEndChild(VBitStreams);

			codeddataNode->LinkEndChild(att);
		}
	}

	doc.SaveFile(fileName.c_str());
}


dynamic_bitset<> DataSourceSerialization::signMapAsLong(vector<bool> signMap){

	dynamic_bitset<> temp(signMap.size());
	try
	{
		for (int i = 0 ; i < signMap.size() ; i++)
		{
			temp[i] = signMap[i];
		}
	}catch(std::exception &ex){

	}
	return temp;
}
