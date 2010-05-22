#include "StdAfx.h"
#include "Utils.h"
#include <limits.h>

using namespace boost;
Utils::~Utils(void)
{
}

bool Utils::gr(double a,double b)
{
	return (a-b > SMALL);
}

bool Utils::sm(double a,double b)
{
	return (b-a > SMALL);
}

bool Utils::smOrEq(double a,double b)
{
	return (a-b < SMALL);
}

bool Utils::eq(double a, double b)
{
	return (a - b < SMALL) && (b - a < SMALL); 
}

bool Utils::grOrEq(double a,double b)
{

	return (b-a < SMALL);
}

/**
* Rounds a double to the given number of decimal places.
*/
double Utils::roundDouble(double value,int afterDecimalPoint)
{
	double mask = pow(10.0, (double)afterDecimalPoint);
	unsigned long long rnd_val = (unsigned long long)value * mask;
	return (double)(rnd_val) / mask;
}

std::string Utils::toStringVal( int val )
{
	// This method is said to be inefficient
	return boost::lexical_cast<string>(val);
}

string Utils::toStringVal(double val)
{
	// This method is said to be inefficient
	return boost::lexical_cast<string>(val);
}

string Utils::doubleToString(double value, int afterDecimalPoint)
{
	string stringBuffer = "";
	double temp = 0;
	int dotPosition = 0;
	long precisionValue = 0;

	temp = value * pow(10.0, afterDecimalPoint);
	if (abs(temp) < LONG_MAX) 
	{
		precisionValue = 	(temp > 0) ? (long)(temp + 0.5) : -(long)(abs(temp) + 0.5);
		if (precisionValue == 0)
		{
			stringBuffer  = "0";
		} else {
			stringBuffer = lexical_cast<string>(precisionValue);
		}
		if (afterDecimalPoint == 0) {
			return stringBuffer;
		}
		dotPosition = stringBuffer.length() - afterDecimalPoint;
		while (((precisionValue < 0) && (dotPosition < 1)) ||
			(dotPosition < 0)) {
				if (precisionValue < 0) {
					stringBuffer.insert(1, "0");
				} else {
					stringBuffer.insert(0, "0");
				}
				dotPosition++;
		}
		stringBuffer.insert(dotPosition, ".");
		if ((precisionValue < 0) && (stringBuffer[1] == '.'))
		{
			stringBuffer.insert(1, "0");
		} else if (stringBuffer[0] == '.') {
			stringBuffer.insert(0, "0");
		}
		int currentPos = stringBuffer.length() - 1;
		while ((currentPos > dotPosition) && (stringBuffer[currentPos] == '0')) 
		{
				stringBuffer[currentPos--] = ' ';
		}
		if (stringBuffer[currentPos] == '.') {
			stringBuffer[currentPos] = ' ';
		}

		return stringBuffer;
	}
	return stringBuffer;
}

string Utils::doubleToString(double value, int width,int afterDecimalPoint)
{

	string tempString = doubleToString(value, afterDecimalPoint);
	size_t result_length = width;
	char * result =  new char[width];;
	int dotPosition = 0;

	if ((afterDecimalPoint >= width) 
		|| ((int)tempString.find('e') != -1)||((int)tempString.find('E') != -1)) 
	{ // Protects sci notation
			return tempString;
	}

	// Initialize result
	
	for (int i = 0; i < result_length ; i++) {
		result[i] = ' ';
	}

	if (afterDecimalPoint > 0) {
		// Get position of decimal point and insert decimal point
		dotPosition = (int)tempString.find('.');
		if (dotPosition == -1) {
			dotPosition = tempString.length();
		} else {
			result[width - afterDecimalPoint - 1] = '.';
		}
	} else {
		dotPosition = tempString.length();
	}


	int offset = width - afterDecimalPoint - dotPosition;
	if (afterDecimalPoint > 0)
	{
		offset--;
	}

	// Not enough room to decimal align within the supplied width
	if (offset < 0) {
		return tempString;
	}

	// Copy characters before decimal point
	for (int i = 0; i < dotPosition; i++) {
		result[offset + i] = tempString[i];
	}

	// Copy characters after decimal point
	for (int i = dotPosition + 1; i < tempString.length(); i++) {
		result[offset + i] = tempString[i];
	}
	string res(result,result+result_length);
	string new_res = res;
	delete result;
	return new_res;
	}

double Utils::sum(double* doubles,size_t length)
{

	double sum = 0;

	for (int i = 0; i < length; i++)
	{
		sum += doubles[i];
	}
	return sum;
}

string Utils::toStringVal(double val, int precission)
{
	// This method is said to be inefficient
	return toStringVal(roundDouble(val,precission));
}


void Utils::Normalize(double * doubles,size_t _length)
{
	double sum = 0;
	for (int i = 0; i < _length; i++) {
		sum += doubles[i];
	}
	Normalize(doubles, sum,_length);
}

int Utils::MaxIndex(double * _doubles,size_t _length)
{

	double maximum = 0;
	int maxIndex = 0;

	for (int i = 0; i < _length; i++) {
		if ((i == 0) || (_doubles[i] > maximum)) {
			maxIndex = i;
			maximum = _doubles[i];
		}
	}

	return maxIndex;
}

void Utils::Normalize(double * doubles, double sum,size_t _length)
{

	if (sum == 0)
	{
		return;
	}
	for (int i = 0; i < _length; i++) {
		doubles[i] /= sum;
	}
}