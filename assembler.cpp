//
//  assembler.cpp
//  ManchesterBaby
//
//  Created by Ross Maider on 15/11/2018.
//  Copyright © 2018 Ross Maider. All rights reserved.
//

#include "assembler.hpp"
#include <iostream>
#include <fstream>
#include "connecter.h"
int assemblerMenu()
{
	//New assembler
	babyAssembler assembler = *new babyAssembler();

	//Get name of the file to translate
	assembler.getFileName();

	//Do the first pass
	assembler.firstPass();

	//Do the second pass
	assembler.secondPass();

	//Write to file
	assembler.writeToFile();

	//Finish
	return 0;
}
