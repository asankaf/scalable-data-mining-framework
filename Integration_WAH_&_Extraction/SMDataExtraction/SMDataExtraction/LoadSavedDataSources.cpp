#include "StdAfx.h"
#include "LoadSavedDataSources.h"
#include "tinyxml.h"

LoadSavedDataSources::LoadSavedDataSources(string metaDataFile,string dataFile)
{
	this->_fileName = dataFile;
	this->_metaFile = metaDataFile;
}

LoadSavedDataSources::~LoadSavedDataSources(void)
{
}

DataSources* LoadSavedDataSources::loadSavedEncodedData(){
	DataSources *dss = new DataSources();
	string metaDataFile = "../Reports/" + this->_metaFile + ".xml";
	string encodedDataFile = "../Reports/" + this->_fileName + ".xml";
	this->_fileName=encodedDataFile;

	TiXmlDocument doc(metaDataFile.c_str());
	bool loaded = doc.LoadFile();
	TiXmlHandle handler( &doc );
	if (loaded)
	{
		TiXmlElement *root = handler.FirstChild("DataSources").ToElement();
		int dataSources = root->FirstAttribute()->IntValue();
		for (int i = 0 ; i < dataSources ; i++)
		{
			TiXmlElement *dsElement = root->FirstChildElement("DataSource");
			string dsName = dsElement->FirstAttribute()->Value();
			dsElement = dsElement->FirstChildElement("noOfAttributes");
			int noAtts = atoi(dsElement->GetText());
			dsElement = dsElement->NextSiblingElement("noOfRows");
			int noRows = atoi(dsElement->GetText());
			dsElement = dsElement->NextSiblingElement("DataSourceType");
			WrapDataSource::DATASOURCE sourceType = getDataSourceType(atoi(dsElement->GetText()));
			WrapDataSource *ds = new WrapDataSource();
			ds->setDSName(dsName);
			ds->noOfAttributes(noAtts);
			ds->noOfRows(noRows);
			ds->setSourceType(sourceType);
			dsElement = dsElement->NextSiblingElement("CodedAttributes");
			TiXmlElement *attElement = dsElement->FirstChildElement("Attribute");
			vector<EncodedAttributeInfo*> codedAtts = loadCodedAttributes(dsName,noRows);
			while (attElement)
			{
				int attType = attElement->LastAttribute()->IntValue();
				int attID = attElement->FirstAttribute()->IntValue();
				switch(attType)
				{
				case 0:
					{
						EncodedIntAttribute* intAtt = static_cast<EncodedIntAttribute*>(codedAtts[attID]);
						TiXmlElement *attEl = attElement->FirstChildElement("maxval");
						intAtt->setMaxVal(atol(attEl->GetText()));
						attEl = attElement->FirstChildElement("minval");
						intAtt->setMinVal(atol(attEl->GetText()));
						attEl = attElement->FirstChildElement("SignMapVal");
						vector<bool> signMap;
						if (atol(attEl->GetText()) == 0)
						{						
							signMap.resize(noRows);
						}
						//Set sign Bit Map for negative vals.
						else
						{
							signMap.resize(noRows);
						}
						intAtt->setSignBitMap(signMap);
						EncodedAttributeInfo* atts = intAtt;
						codedAtts[attID] = atts;
						break;
					}
				case 3:
					{
						EncodedMultiCatAttribute* catAtt = static_cast<EncodedMultiCatAttribute*>(codedAtts[attID]);
						TiXmlElement *uniqueElement = attElement->FirstChildElement("UniqueValues");
						int noUniques = uniqueElement->LastAttribute()->IntValue();
						vector<string> uniqueVals;
						uniqueVals.resize(noUniques);
						uniqueElement = uniqueElement->FirstChildElement("Val");
						for (int k = 0 ; k < noUniques ; k++)
						{
							string val = uniqueElement->GetText();
							uniqueVals[k] = val;
							uniqueElement = uniqueElement->NextSiblingElement("Val");
						}
						catAtt->setUniqueValList(uniqueVals);
						EncodedAttributeInfo *atts = catAtt;
						codedAtts[attID] = atts;
						break;
					}
				}
				attElement = attElement->NextSiblingElement("Attribute");
			}
			ds->CodedAtts(codedAtts);
			dss->insertDataSources(ds);
		}
	}

	return dss;
}

WrapDataSource::DATASOURCE LoadSavedDataSources::getDataSourceType(int sourceType){
	switch (sourceType)
	{
	case 0:
		return WrapDataSource::DATASOURCE::DATABASE;
		
	case 1:
		return WrapDataSource::DATASOURCE::CSVFILE;

	case 2:
		return WrapDataSource::DATASOURCE::XMLFILE;
	}
}

ATT_TYPE LoadSavedDataSources::getAttType(int attType){
	switch(attType){
		case 0:
			return ATT_TYPE::SIGNEDINT_VAL;
		case 1:
			return ATT_TYPE::DOUBLE_VAL;
		case 2:
			return ATT_TYPE::DATE_VAL;
		case 3:
			return ATT_TYPE::MULTICAT_VAL;
		case 4:
			return ATT_TYPE::SKIP_VAL;
	}
}

vector<EncodedAttributeInfo*> LoadSavedDataSources::loadCodedAttributes(string dsName,int rowCount){
	TiXmlDocument doc_1(this->_fileName.c_str());
	doc_1.LoadFile();
	TiXmlHandle handler(&doc_1);
	TiXmlElement *dsElement = handler.FirstChild("DataSources").ToElement();
	dsElement = dsElement->FirstChildElement("DataSource");
	vector<EncodedAttributeInfo*> codedAtts;
	while (dsElement)
	{
		if (strcmp(dsElement->Attribute("Name"),dsName.c_str()) == 0)
		{
			dsElement = dsElement->FirstChildElement("CodedAttributes")->FirstChildElement("Attribute");
			while (dsElement)
			{
				int attID = dsElement->FirstAttribute()->IntValue();
				int attType = atoi(dsElement->Attribute("Type"));
				string attName = dsElement->Attribute("Name");
				int noVStreams = dsElement->LastAttribute()->IntValue();
				EncodedAttributeInfo* attr;
				switch(attType){
					case 0:
						{
							EncodedIntAttribute *intAtt = new EncodedIntAttribute();
							intAtt->setAttID(attID);
							intAtt->setAttName(attName);
							intAtt->setNoOfVBitStreams(noVStreams,rowCount);
							intAtt->setAttType(getAttType(attID));

							BitStreamInfo** bitStreams = new BitStreamInfo*[noVStreams];
							TiXmlElement *vbs = dsElement->FirstChildElement("VBitStreams")->FirstChildElement("vbitstream");
							for (int k = 0 ; k < noVStreams ; k++)
							{
								BitStreamInfo* bitStr = new VBitStream();
								bitStr->setBitCount(rowCount);
								bitStr->setBitStreamAllocAttID(attID);
								bitStr->setBitStreamAllocAttName(attName);
								string bitStream = vbs->GetText();
								dynamic_bitset<> temp(bitStream);
								bitStr->convert(temp);
								bitStreams[k] = bitStr;
								vbs = vbs->NextSiblingElement("vbitstream");
							}
							intAtt->setVBitStreams(bitStreams);
							attr = intAtt;
							codedAtts.push_back(attr);
							break;
						}
					case 3:
						{
							EncodedMultiCatAttribute *catAtt = new EncodedMultiCatAttribute();
							catAtt->setAttID(attID);
							catAtt->setAttName(attName);
							catAtt->setAttType(getAttType(attType));
							catAtt->setNoOfVBitStreams(noVStreams,rowCount);

							BitStreamInfo** bitStreams = new BitStreamInfo*[noVStreams];
							TiXmlElement *vbs = dsElement->FirstChildElement("VBitStreams")->FirstChildElement("vbitstream");
							for (int k = 0 ; k < noVStreams ; k++)
							{
								BitStreamInfo* bitStr = new VBitStream();
								bitStr->setBitCount(rowCount);
								bitStr->setBitStreamAllocAttID(attID);
								bitStr->setBitStreamAllocAttName(attName);
								string bitStream = vbs->GetText();
								dynamic_bitset<> temp(bitStream);
								bitStr->convert(temp);
								bitStreams[k] = bitStr;
								vbs = vbs->NextSiblingElement("vbitstream");
							}
							catAtt->setVBitStreams(bitStreams);
							attr = catAtt;
							codedAtts.push_back(attr);
							break;
						}

				}

				dsElement = dsElement->NextSiblingElement("Attribute");
			}
		}
		else{
			dsElement = dsElement->NextSiblingElement("DataSource");
			continue;
		}
	}
	return codedAtts;
}