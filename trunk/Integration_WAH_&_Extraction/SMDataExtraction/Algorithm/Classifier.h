#pragma once
#include "WrapDataSource.h"
#include <iostream>
#include <xstring>

using namespace std;
class Classifier
{
public:
	_declspec(dllexport) Classifier(void);
	_declspec(dllexport) virtual ~Classifier(void);

	/**
	* Generates a classifier. Must initialize all fields of the classifier
	* that are not being set via options (ie. multiple calls of buildClassifier
	* must always lead to the same result). Must not change the dataset
	* in any way.	
	*/
	_declspec(dllexport) virtual void buildClassifier(WrapDataSource * _data) = 0;

	_declspec(dllexport) virtual string toString(){return NULL;}
	//_declspec(dllexport) virtual void buildClassifier(WrapDataSource * _data) = 0;

};
