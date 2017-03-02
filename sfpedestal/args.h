/*
 * Copyleft (L) 2015 Kamil Wójcik, University of Silesia
 * kwojcik4@us.edu.pl
 * All rights reversed.
 * 
 */

#ifndef ARGS_H
#define ARGS_H

#include <string>
#include <stdexcept>
#include <iostream>
#include "interfaceStructs.h"


class args
{
public:
	args(int, char**);
	bool args_correct(void);
	argSet setup;
	
private:
	bool args_good, printPedestalInfo;
};

#endif // ARGS_H
