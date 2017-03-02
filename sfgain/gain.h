#ifndef GAIN_H
#define GAIN_H

#include "interfaceStructs.h"

using namespace std;

class gain
{
public:
	double Array[2][16];
	double Ribbon[2][4][16];
	
	gain();
	void setOnes();
	void print();
	void multiply(double);

	bool load(string);
	bool saveAs(string);
	void invert();
};

#endif // GAIN_H
