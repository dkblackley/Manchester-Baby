//
//  assembler.hpp
//  ManchesterBaby
//
//  Created by Ross Maider on 15/11/2018.
//  Copyright © 2018 Ross Maider. All rights reserved.
//

#ifndef assembler_hpp
#define assembler_hpp

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

class babyAssembler
{
	//Class fields
	vector<vector<string> > lookupTable;
	vector<string> outputBuffer;
	string fileName;
	string symbolTable[8][2] =
	{
		{"JMP","000"},
		{"JRP","100"},
		{"LDN","010"},
		{"STO","110"},
		{"SUB","001"},
		{"SUB","101"},
		{"CMP","011"},
		{"STP","111"},
	};

public:

	babyAssembler();
	~babyAssembler();
	int openAndReadFile();
	string getAddress(string);
	void addToLookUpTable(string);
	void addToLookUpTable(string, string);
	string searchSymbolTable(string);
	string searchLookupTable(string);
	void printOutLookUp();
	void printOutBuffer();
	bool addAddress(string, string);
	string decToBinary(string);
	void firstPass();
	void secondPass();
	void writeToFile();
	void getFileName();
};

//Constructor
babyAssembler::babyAssembler()
{

}

//Destructor
babyAssembler::~babyAssembler()
{

}

//Get the name of the file translate
void babyAssembler::getFileName()
{
	string fileName;

	//Make sure there is something in it
	while(fileName.empty())
	{
		cout << "Please input the name of the assembly file you wish to convert including the .txt" << endl;
		cin >> fileName;
	}

	//Once satisfied read it in
	this->fileName = fileName;
}

//Test method for debug
int babyAssembler::openAndReadFile()
{
	ifstream in (fileName);
	if(!in)
	{
		cout << "Could not open file" << endl;
		return 1;
	}

	string line;

	while(getline(in, line))
	{
		string label = line.substr(0, 5);
		label.erase(remove(label.begin(),label.end(), ' ' ), label.end() );
		label.erase(remove(label.begin(),label.end(), ':' ), label.end() );
		string instruction = line.substr(10, 3);
		instruction.erase(remove(instruction.begin(),instruction.end(), ' ' ), instruction.end() );
		string operand = line.substr(14, 5);
		operand.erase(remove(operand.begin(),operand.end(), ' ' ), operand.end() );
		string comment = ";";

		if (line.find(";") == 0)
		{
			//cout << "Lines contains a comment at the start" << endl;
		}
		else
		{
			//cout << "Label: " << label << " Instruction: " << instruction << " Operand: " << operand << endl;

			if(!label.empty())
			{
				addToLookUpTable(label);
			}
		}


	}

	return 0;


}

/*
 This code snippet is from
 https://github.com/apoclyps/SoC-Manchester-Baby/blob/master/Assembler/Assembler.cpp
 We have adapted it to our needs
 Conversion for decimal string to binary string
 */
string babyAssembler::decToBinary(string decimal)
{
	unsigned int powerOf2 = 2147483648;
	int decimalNumber = stoi(decimal);
	string binaryNumber = "";
	string flipbits= "00000000000000000000000000000000";
	int counter =0;

	do
	{
		if(decimalNumber >= powerOf2)
		{
			binaryNumber = binaryNumber+"1";
			decimalNumber=decimalNumber-powerOf2;
		}
		else
		{
			binaryNumber = binaryNumber+"0";
		}

		if(decimalNumber!=0)
		{
			powerOf2 = powerOf2/2;
		}

		counter++;
	}while(counter!=32);

	for(int i = 0; i < 32; i++)	{
		flipbits.at(i) = binaryNumber.at(31-i);
	}

	return flipbits;
}

//This method finds the address in a table given the mnemoic
string babyAssembler::searchSymbolTable(string name)
{
	bool found = false;
	int counter = 0;

	//While its not found and we are not at the end of the symbol table
	while(found == false && counter < 8)
	{
		//If its the name
		if(symbolTable[counter][0] == name)
		{
			//Return the binary
			return symbolTable[counter][1];
		}
		//Otherwise next row
		counter++;
	}

	//If not found return could not find
	return "CNF";
}

//This method adds a new label to the table
void babyAssembler::addToLookUpTable(string label)
{
	vector<string> tmp;
	tmp.push_back(label);

	//Add to lookup table
	lookupTable.push_back(tmp);
}

//Add to look up table new label and address
void babyAssembler::addToLookUpTable(string label, string address)
{
	vector<string> tmp;
	tmp.push_back(label);
	tmp.push_back(address);

	//Add to lookup
	lookupTable.push_back(tmp);

}

//Method for debug print out look up
void babyAssembler::printOutLookUp()
{
	for (int i = 0; i < lookupTable.size(); i++)
	{
		for (int j = 0; j < lookupTable[i].size(); j++)
		{
			cout << lookupTable[i][j];
			cout << " ";
		}
		cout << endl;
	}
}

//Method for debug print out the buffer
void babyAssembler::printOutBuffer()
{
	for (int i = 0; i < outputBuffer.size(); i++)
	{
		cout << outputBuffer[i] << endl;
	}
}

//Method for maing sure a label is in the address table
string babyAssembler::searchLookupTable(string label)
{
	bool found = false;
	int counter = 0;

	//While not found and we are not at the end of the lookup table
	while(found == false && counter < lookupTable.size())
	{
		//If its the label we are looking for
		if(lookupTable[counter][0] == label)
		{
			//Return the address
			return lookupTable[counter][0];
		}

		//Increment the counter
		counter++;
	}

	//If not found return NULL
	return NULL;
}

//Method for getting the address in lookup given the label
string babyAssembler::getAddress(string label)
{
	bool found = false;
	int counter = 0;

	//While not found and we are not at the end of the table
	while(found == false && counter < lookupTable.size())
	{
		//If its the one we are looking for
		if(lookupTable[counter][0] == label)
		{
			if (lookupTable[counter][1].empty())
			{
				//Return nothing if its blank
				return "";
			}
			//Return address
			return lookupTable[counter][1];
		}

		//Increment counter
		counter++;
	}

	//Could not find it
	return "CNF";
}

//Method for adding the address given the label and address
bool babyAssembler::addAddress(string label, string address)
{
	//If its in the table
	if(searchLookupTable(label) == label)
	{
		int counter = 0;
		bool added = false;

		//While its not added and we are not at the end of the table
		while(added == false && counter < lookupTable.size())
		{
			//If its the correct one
			if(lookupTable[counter][0] == label)
			{
				//Add it
				vector<string> tmp;
				tmp.push_back(label);
				tmp.push_back(address);

				lookupTable.erase(lookupTable.begin() + counter);

				lookupTable.push_back(tmp);
				added = true;

				return true;
			}

			//Increment counter
			counter++;
		}

		//Not added
		return false;
	}
	else
	{
		//Could not find it
		cout << "Could not found label in lookup table" << endl;
		return false;
	}
}
//First pass of the assembler
void babyAssembler::firstPass()
{
	//Open the file
	ifstream in (fileName);
	//Can't open the file
	if(!in)
	{
		cout << "Could not open file" << endl;
	}

	int lineNum = 1;
	string line;

	//While it still has lines in it
	while(getline(in, line))
	{
		//Seperate tokens
		string label = line.substr(0, 5);
		label.erase(remove(label.begin(),label.end(), ' ' ), label.end() );
		label.erase(remove(label.begin(),label.end(), ':' ), label.end() );

		string instruction = line.substr(10, 3);
		instruction.erase(remove(instruction.begin(),instruction.end(), ' ' ), instruction.end() );
		string operand = line.substr(14, 5);
		operand.erase(remove(operand.begin(),operand.end(), ' ' ), operand.end() );
		string comment = ";";

		//Find comments
		if (line.find(";") == 0)
		{
			//cout << "Lines contains a comment at the start" << endl;
		}
		else
		{
			//cout << "Label: " << label << " Instruction: " << instruction << " Operand: " << operand << endl;

			//Add labels
			if(!label.empty())
			{
				addToLookUpTable(label);
			}
			//Places           0123456789*123456789*123456789*1
			string currLine = "00000000000000000000000000000000";

			//Add instructions
			if(instruction == "VAR")
			{

			}
			else
			{
				currLine.replace(13,3, searchSymbolTable(instruction));
			}

			if(!label.empty() && searchLookupTable(label) == label && instruction == "VAR")
			{
				string binNum = decToBinary(operand);
				string address = to_string(lineNum);
				addAddress(label, address);
				currLine = binNum;
			}

			if(!label.empty() && searchLookupTable(label) == label && instruction == "STP")
			{
				string sLineNum = to_string(lineNum);
				string binNum = decToBinary(sLineNum);
				string address = to_string(lineNum);
				addAddress(label, sLineNum);
				currLine = binNum;
			}

			if(!label.empty() && searchLookupTable(label) == label && label == "START")
			{
				string sLineNum = to_string(0);
				string binNum = decToBinary(sLineNum);
				string address = to_string(lineNum);
				addAddress(label, sLineNum);
				currLine = binNum;
			}

			lineNum++;
			outputBuffer.push_back(currLine);
		}
	}
}

//Second pass adds in the adress
void babyAssembler::secondPass()
{
	//Get line in buffer
	string currentLine;
	int counter = 0;
	currentLine = outputBuffer[counter];

	//Open file
	ifstream in (fileName);

	//Cant open file
	if(!in)
	{
		cout << "Could not open file" << endl;
	}

	string line;

	//Still lines in the file
	while(getline(in, line))
	{
		//Find comments
		if (line.find(";") == 0)
		{
			//cout << "Lines contains a comment at the start" << endl;
		}
		else if (line.find("VAR") == 0) //Find variables
		{

		}
		else
		{
			//Get tokens
			string label = line.substr(0, 5);
			label.erase(remove(label.begin(),label.end(), ' ' ), label.end() );
			label.erase(remove(label.begin(),label.end(), ':' ), label.end() );
			string instruction = line.substr(10, 3);
			instruction.erase(remove(instruction.begin(),instruction.end(), ' ' ), instruction.end() );
			string operand = line.substr(14, 5);
			operand.erase(remove(operand.begin(),operand.end(), ' ' ), operand.end() );
			string comment = ";";

			string address;
			address = getAddress(operand);

			//Variable label
			if(label != "START" && label != "END")
			{
				if(address != "CNF")
				{
					//Redo the line add binary and instructions add to buffer
					int sAddress = stoi(address);
					sAddress--;
					address = to_string(sAddress);
					string binNum = decToBinary(address);

					binNum.replace(13, 3, searchSymbolTable(instruction));

					outputBuffer.erase(outputBuffer.begin() + counter);
					outputBuffer.insert(outputBuffer.begin() + counter, binNum);
				}

				counter ++;
			}

			//Start label
			if(label == "START")
			{
				string address;
				address = getAddress("END");

				if(address != "CNF")
				{
					//Redo the line add binary and instructions add to buffer
					int decimalNumber = stoi(address);
					//bdecimalNumber--;
					string sAddress = to_string(decimalNumber);

					string binNum = decToBinary(address);

					binNum.replace(13, 3, searchSymbolTable(instruction));
					outputBuffer.erase(outputBuffer.begin() + counter);
					outputBuffer.insert(outputBuffer.begin() + (counter), binNum);

				}

				counter ++;
			}

			//End label
			if(label == "END")
			{
				string address;
				address = getAddress("START");

				if(address != "CNF")
				{
					//Redo the line add binary and instructions add to buffer
					int decimalNumber = stoi(address);
					//bdecimalNumber--;
					string sAddress = to_string(decimalNumber);

					string binNum = decToBinary(sAddress);

					binNum.replace(13, 3, searchSymbolTable(instruction));
					//outputBuffer.erase(outputBuffer.begin() + (counter-1));
					outputBuffer.erase(outputBuffer.begin() + counter);
					outputBuffer.insert(outputBuffer.begin() + (counter), binNum);

				}

				counter ++;
			}

		}

	}
}

//Write the output buffer to file
void babyAssembler::writeToFile()
{
	//Open and make the file
	ofstream output_file("./babyBinary.txt");

	//All elements in the buffer
	for (const auto &e : outputBuffer)
	{
		//Write to file
		output_file << e << "\n";
	}
}

#endif /* assembler_hpp */
