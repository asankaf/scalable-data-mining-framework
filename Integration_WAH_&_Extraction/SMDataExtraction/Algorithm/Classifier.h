#pragma once
#include "WrapDataSource.h"
#include <iostream>
#include <xstring>

using namespace std;
/************************************************************************
*    Class  :Classifier	  
*    Author :Amila De Silva
*    Subj   :
* Classifier interface. All schemes for numeric or nominal prediction in
* SEEDMiner implement this interface. 
*    Version: 1
************************************************************************/
class Classifier
{
public:
	/***
	* Constructor
	*/
	_declspec(dllexport) Classifier(void);

	/***
	* Destructor
	*/
	_declspec(dllexport) virtual ~Classifier(void);

	/***
	* Generates a classifier. Must initialize all fields of the classifier
	* that are not being set via options (ie. multiple calls of buildClassifier
	* must always lead to the same result). Must not change the dataset
	* in any way.	
	*/	
	_declspec(dllexport) virtual void buildClassifier(WrapDataSource * _data) = 0;
	
	/***
	* Gives the string representation of the model.
	*/
	_declspec(dllexport) virtual string toString(){return NULL;}	

};
