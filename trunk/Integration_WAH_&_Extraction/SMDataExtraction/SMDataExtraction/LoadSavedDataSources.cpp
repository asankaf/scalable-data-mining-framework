#include "StdAfx.h"
#include "LoadSavedDataSources.h"
#include "tinyxml.h"
#include "EncodedDoubleAttribute.h"

LoadSavedDataSources::LoadSavedDataSources(string metaDataFile,string dataFile)
{
	this->_fileName = dataFile;
	this->_metaFile = metaDataFile;
	this->_rowLimit = 0;
}

LoadSavedDataSources::LoadSavedDataSources(string metaDataFile,string dataFile,long limit)
{
	this->_fileName = dataFile;
	this->_metaFile = metaDataFile;
	this->_rowLimit = limit;
}

LoadSavedDataSources::~LoadSavedDataSources(void)
{	
	
}

DataSources* LoadSavedDataSources::loadSavedEncodedData(bool limit /* = false */){
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
			//Following two lines will load the existance bitmap from the saved file.
			//For older versions of saved data files, this won't be working.
			//So for old data file loading, just comment the following two lines.
			//And also make sure to comment the place where the existance_map is added to wrapdatasource object.
			string existanceMap = dsElement->NextSiblingElement("existanceBitMap")->GetText();
			dynamic_bitset<> existance_map(existanceMap);

			vector<EncodedAttributeInfo*> codedAtts = loadCodedAttributes(dsName,noRows,limit);
			if (limit)
			{
				if (this->_rowLimit > noRows)
				{
					this->_rowLimit = noRows;
				}
				noRows = this->_rowLimit;
			}
			
			dsElement = dsElement->NextSiblingElement("DataSourceType");
			WrapDataSource::DATASOURCE sourceType = getDataSourceType(atoi(dsElement->GetText()));
			WrapDataSource *ds = new WrapDataSource();
			ds->setDSName(dsName);
			ds->noOfAttributes(noAtts);
			ds->noOfRows(noRows);
			ds->setSourceType(sourceType);
			//comment this for loading old saved files. 
			ds->ExistanceDatabitMap(existance_map);
			dsElement = dsElement->NextSiblingElement("CodedAttributes");
			TiXmlElement *attElement = dsElement->FirstChildElement("Attribute");
		//	vector<EncodedAttributeInfo*> codedAtts = loadCodedAttributes(dsName,noRows);
			int counter = 0;
			while (attElement && (counter <= noAtts))
			{
				int attType = attElement->LastAttribute()->IntValue();
				int attID = attElement->FirstAttribute()->IntValue();
				switch(attType)
				{
				case 0:
					{
						EncodedIntAttribute* intAtt = static_cast<EncodedIntAttribute*>(codedAtts[counter]);
						TiXmlElement *attEl = attElement->FirstChildElement("maxval");
						intAtt->setMaxVal(atol(attEl->GetText()));
						attEl = attElement->FirstChildElement("minval");
						intAtt->setMinVal(atol(attEl->GetText()));
						attEl = attElement->FirstChildElement("SignBitSet");
						//attEl = attElement->FirstChildElement("SignMapVal");
// 						vector<bool> signMap;
// 						if (atol(attEl->GetText()) == 0)
// 						{						
// 							signMap.resize(noRows);
// 						}
// 						//Set sign Bit Map for negative vals.
// 						else
// 						{
// 							signMap.resize(noRows);
// 						}
// 						intAtt->setSignBitMap(signMap);

						dynamic_bitset<> signSet((string)attEl->GetText());
						intAtt->setSignBitSet(signSet);
						EncodedAttributeInfo* atts = intAtt;
						codedAtts[counter] = atts;
						break;
					}
				case 1:
					{
						EncodedDoubleAttribute* doubleAtt = static_cast<EncodedDoubleAttribute*>(codedAtts[counter]);
						TiXmlElement *attEl = attElement->FirstChildElement("maxval");
						doubleAtt->setMaxVal(atol(attEl->GetText()));
						attEl = attElement->FirstChildElement("minval");
						doubleAtt->setMinVal(atol(attEl->GetText()));
						attEl = attElement->FirstChildElement("SignBitSet");
						//attEl = attElement->FirstChildElement("SignMapVal");
// 						vector<bool> signMap;
// 						if (atol(attEl->GetText()) == 0)
// 						{						
// 							signMap.resize(noRows);
// 						}
// 						//Set sign Bit Map for negative vals.
// 						else
// 						{
// 							signMap.resize(noRows);
// 						}
// 						doubleAtt->SignBitMap(signMap);

						dynamic_bitset<> signSet((string)attEl->GetText());
						doubleAtt->setSignBitSet(signSet);
						attEl = attElement->FirstChildElement("PrecisionVal");
						doubleAtt->Precision(atol(attEl->GetText()));
						EncodedAttributeInfo* atts = doubleAtt;
						codedAtts[counter] = atts;
						break;
					}
				case 3:
					{
						EncodedMultiCatAttribute* catAtt = static_cast<EncodedMultiCatAttribute*>(codedAtts[counter]);
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
						codedAtts[counter] = atts;
						break;
					}
				}
				attElement = attElement->NextSiblingElement("Attribute");
				counter++;
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
			break;
		case 1:
			return ATT_TYPE::DOUBLE_VAL;
			break;
		case 2:
			return ATT_TYPE::DATE_VAL;
			break;
		case 3:
			return ATT_TYPE::MULTICAT_VAL;
			break;
		case 4:
			return ATT_TYPE::SKIP_VAL;
			break;
	}
}

vector<EncodedAttributeInfo*> LoadSavedDataSources::loadCodedAttributes(string dsName,int rowCount,bool limit){
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
							intAtt->setAttType(getAttType(attType));

							BitStreamInfo** bitStreams = new BitStreamInfo*[noVStreams];
							TiXmlElement *vbs = dsElement->FirstChildElement("VBitStreams")->FirstChildElement("vbitstream");
							for (int k = 0 ; k < noVStreams ; k++)
							{
								BitStreamInfo* bitStr = new VBitStream();
								bitStr->setBitCount(rowCount);
								bitStr->setBitStreamAllocAttID(attID);
								bitStr->setBitStreamAllocAttName(attName);
								string bitStream;
								if (limit)
								{
									bitStream = vbs->GetText();
									long offset = rowCount - this->_rowLimit;
									bitStream = bitStream.substr(offset,this->_rowLimit);
								}
								else bitStream = vbs->GetText();
								dynamic_bitset<> temp(bitStream);
								bitStr->convert(temp);
								bitStreams[k] = bitStr;
								vbs = vbs->NextSiblingElement("vbitstream");
							}
							vector<BitStreamInfo*> tempVB(bitStreams , bitStreams + noVStreams);
							intAtt->setVBitStreams(tempVB);
							attr = intAtt;
							codedAtts.push_back(attr);
							break;
						}
					case 1:
						{
							EncodedDoubleAttribute *doubleAtt = new EncodedDoubleAttribute();
							doubleAtt->setAttID(attID);
							doubleAtt->setAttName(attName);
							doubleAtt->setNoOfVBitStreams(noVStreams,rowCount);
							doubleAtt->setAttType(getAttType(attType));

							BitStreamInfo** bitStreams = new BitStreamInfo*[noVStreams];
							TiXmlElement *vbs = dsElement->FirstChildElement("VBitStreams")->FirstChildElement("vbitstream");
							for (int k = 0 ; k < noVStreams ; k++)
							{
								BitStreamInfo* bitStr = new VBitStream();
								bitStr->setBitCount(rowCount);
								bitStr->setBitStreamAllocAttID(attID);
								bitStr->setBitStreamAllocAttName(attName);
								string bitStream;
								if (limit)
								{
									bitStream = vbs->GetText();
									long offset = rowCount - this->_rowLimit;
									bitStream = bitStream.substr(offset,this->_rowLimit);
								}
								else bitStream = vbs->GetText();
								dynamic_bitset<> temp(bitStream);
								bitStr->convert(temp);
								bitStreams[k] = bitStr;
								vbs = vbs->NextSiblingElement("vbitstream");
							}
							vector<BitStreamInfo*> tempVB(bitStreams , bitStreams + noVStreams);
							doubleAtt->setVBitStreams(tempVB);
							attr = doubleAtt;
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
								string bitStream;
								if (limit)
								{
									bitStream = vbs->GetText();
									long offset = rowCount - this->_rowLimit;
									bitStream = bitStream.substr(offset,this->_rowLimit);
								}
								else bitStream = vbs->GetText();
								dynamic_bitset<> temp(bitStream);
								bitStr->convert(temp);
								bitStreams[k] = bitStr;
								vbs = vbs->NextSiblingElement("vbitstream");
							}
							vector<BitStreamInfo*> tempVB(bitStreams , bitStreams + noVStreams);
							catAtt->setVBitStreams(tempVB);
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