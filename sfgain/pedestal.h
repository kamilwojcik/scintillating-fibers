#ifndef PEDESTAL_H
#define PEDESTAL_H


//sfbeam includes
#include "interfaceStructs.h"

using namespace std;

class pedestal : public TObject
{
public:
	pedestVals values;
	argSet arguments;
	
	pedestal();
	pedestal(argSet&);
	~pedestal();
	
	bool setup(argSet&);
	bool getStatus();
	void printValues();

protected:
	bool status;
	bool load_file();
};

#endif // PEDESTAL_H
