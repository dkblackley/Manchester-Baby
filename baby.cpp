/*
Manchester Baby project for the Dundee University AC12008 Module - Architecture & Unix
baby.cpp Written by Students Daniel Blackley (160007728) & Ryan Crampton (170013546)
*/


#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "connecter.h"

using namespace std;

enum Operands {JMPCODE, JRPCODE, LDNCODE, STOCODE, SUBCODE, SUB2CODE, CMPCODE, STPCODE};

class Baby {
private:
  //vector acts a simulation of the stor
  vector<string> STORE;
  //ACCUMULATOR holds numbers for arithmatic
  string ACCUMULATOR;
  //set CI to point the addresses in the store
  string CI;
  //PI to hold the current instruction being run
  string PI;
  //memory size
  int memSize;
  //size used for operands
  int addressSize;
  //used for error checking
  bool debug, fileLoaded;

  //displays all values, used while the baby is running
  void displayInfo() {

    if (debug) {
      cout << "\nCI: " << CI << '\n';
      cout << "PI: " << PI << '\n';
    }

    cout << "\nAccumulator: " << ACCUMULATOR <<'\n';
    cout << "Accumulator decimal value: " << binaryToDecimal(memSize, ACCUMULATOR) << '\n' << '\n';
  }

public:
  Baby () { //default constructor
    //set ACCUMULATOR to be 0 for default
    ACCUMULATOR = "00000000000000000000000000000000";
    //set CI to point to the default line 0
    CI = "00000000000000000000000000000000";
    //set current line to contain debugger at default
    PI = "00000000000000000000000000000000";
    //turn off debug mode
    debug = false;
    //default memory size is 32
    memSize = 32;
    //default address size is 5
    addressSize = 5;
    //file has not been loaded yet
    fileLoaded = false;
  }
  //Clears out the STORE
  virtual ~Baby () {
    this->STORE.clear();
  }

  //loads in teh store from a file
  void setStore() {
    this->fileLoaded = false;
    this->STORE = loadFile();
  }

  //return whether or not a file has been loaded
  bool getLoadStatus() { return fileLoaded; }

  //Changes debug mode on/off
  void flipDebugSwitch() {

    if (debug) {
      cout << "Debug disabled" << '\n';
      this->debug = false;
    } else {
      cout << "Debug enabled" << '\n';
      this->debug = true;
    }
  }

  /*
  This method will prompt the user for a filename, then attempt to extract the data
  from that file.
  @return Function returns the loaded file, and returns and empty file with appropriate
  error message upon fail
  */
  vector<string> loadFile()
  {

    std::cout << "Please enter a fileName: " << '\n';

    string fileName;

    cin >> fileName;

    //open the file in read mode
  	FILE *pFile;
  	pFile = fopen(fileName.c_str(), "r");

    vector<string> binaries;

      //if file doesn't exist
    	if (pFile == NULL)
    	{
        std::cerr << "EER: File " << fileName << " Not found!" <<'\n';
        //return and empty vector
  		return binaries;
  	}


  	size_t i, c;
  	char data;
  	i = c = 0;

  	//Determine how big the Array is going to be to make sure it doesn't go outwith boundaries
  	while ((data = fgetc(pFile)) != EOF)
  	{
      //compaability for windows
      if (data == '\r') {
        continue;
      }

      //if data does not consists solely of 1's 0's or newlines
      if (data != '0' && data != '1' && data != '\n') {
        cerr << "UNKOWN CHARACTER IN FILE: " << (int) data << '\n';
        return binaries;
      }

      	if (data != '\n') {
        	i++;
  	    }
  	    else
  	    {
  	    	c++;
  	    }
    	}
    //calcuate number of rows
  	int rows = c;
  	//don't need to cast to a float as this will always be a whole number
  	int columns = i/c;

    //if the file is larger than the baby is designed to handle
    if (columns > memSize || rows > memSize) {
      cerr << "ERR: File is bigger than defined memory limit: " << memSize << '\n';
      return binaries;
    }

    	i = c = 0;
    	//rewind the file pointer to the first value
    	rewind(pFile);
      string currentLine;

      //cycle through file again, this time adding it to vector
    	while ((data = fgetc(pFile)) != EOF)
    	{

      	if (data == '\n')
      	{
        		i++;
        		c = 0;
            binaries.push_back(currentLine);
            currentLine.clear();
      	}
      	else
      	{
        	//set the array to the data in the file
  	    	currentLine.push_back(data);
      	}
    }

    //close file
    fclose(pFile);
    //change boolean as there is now data in store
    this->fileLoaded = true;
    //return vector
    return binaries;
  }

  /*
  	This function takes in an array and compares it to the various legal operations
  	then returns a number based on which opCode matches
  	@instruction - the current instruction sent it, to be calculated what number it
  	relates to
  	@return - A number from 0-7 based on the relevant opCodes
  */
  int getOpcode(string instruction)
  {
  	//int compare;
    //enum Operands {JMPCODE, JRPCODE, LDNCODE, STOCODE, SUBCODE, SUB2CODE, CMPCODE, STPCODE};
  	string opCodes[8] =
  	{
  		{"000"},
  		{"100"},
  		{"010"},
  		{"110"},
  		{"001"},
  		{"101"},
  		{"011"},
  		{"111"},
  	};

    if (debug) {
      cout << "The opcode in the present instruction is: " << instruction;
    }

  	for (int i = 0; i < 8; i++) {

  		if (instruction.compare(opCodes[i]) == 0) {
  			return i;
  		}
  	}
  	return -1;
  }

  /*
  	This method increments the global binary array CI by a single digit, It uses a do while
    loop to add a single bit
  */
  void incriment()
  {


    if (debug) {
      cout << "Incrementing the current CI value by one" << '\n';
      cout << "our current CI is " << CI << '\n';
      cout << "Or as a decimal: " << this->binaryToDecimal(memSize, CI) << '\n';
    }

   	int i = 0;

    //if the first bit is a 1
  	if (CI.at(i) == '1') {
  		do {
        //change the bit to 0
  			CI[i] = '0';

        //until we hit another 0
  		} while(CI[++i] != '0');

      //then change that 0 to a 1
  		CI[i] = '1';

  	} else {
      //else if the first bit is a 0, change it to a 1
  		CI[i] = '1';
  	}

    if (debug) {
      cout << "Now the CI has been incremented to: " << CI << '\n';
      cout << "Or as a decimal: " << this->binaryToDecimal(memSize, CI) << '\n';
    }
  }

  /*
  This function simulates the fetch in the fetch execute cycle, by setting PI to
  the index in the STORE.
  */
  void fetch() {

    if (debug) {
      cout << "Fetching the next insctrution, the previous instruction was: " << PI << '\n';
    }

    //calculate where CI is pointing to
    int lineNumber = binaryToDecimal(memSize, CI);
    //And set PI to hold it
    this->PI = this->STORE.at(lineNumber);

    if (debug) {
      cout << "The New instruction is: " << PI << '\n';
    }
  }


  /*
  Returns the operand from the PI by tacking out the first 5 values
  */
  string decodeOperand () {
    return PI.substr(0, addressSize);
  }

  /*
  Returns the opCode from the PI by tacking out values between 13 and 16
  */
  string decodeOpcode() {
    return PI.substr(13, 3);
  }

  /*
  This function takes in a index and finds the number that the index points to,
  which is in the storage for numbers at the end of STORE
  @param index - the index in the store that our number is held
  @return - returns the number in decimal form
  */
  int getNumAtIndex(int index) {

    string line = this->STORE.at(index);

    //take the number from the firts half of the line
    line = line.substr(0, line.length()/2);

    return binaryToDecimal(16, line);
  }

    /*
    This function takes in the adress of an operand, then sets the CI to jusmp to
    that address
    @param operand - address to be jumped to
    */
    void JMP(string operand)
    {

      if (debug) {
        cout << " which will cause us to call the JMP Instruction" << '\n';
        cout << "After calling JMP, we have Jumped the CI from: " << CI << '\n';
      }

      //change the number to a decimal, so we can find the value in the store
      int num = binaryToDecimal(addressSize, operand);
      num = getNumAtIndex(num);
      //set the CI to be the value found in the store
      this->CI = decimalToBinary(memSize, num);

      if (debug) {
        cout << "To: " << CI << '\n';
      }
    }

    /*
    	This function will take in an operand and then find the value that the operand
      points to and will add its value to the CI's value

      @param operand - The address of the number in the store
    */
    void JRP(string operand)
    {
      if (debug) {
        cout << " which will cause us to call the JRP Instruction" << '\n';
        cout << "After calling JRP, we have Jumped the CI from: " << CI << '\n';
      }

      int location1, location2, result;

      //get value of location1 from store
      location1 = binaryToDecimal(addressSize, operand);
      location1 = getNumAtIndex(location1);

      location2 = binaryToDecimal(memSize, this->CI);

      //add store value to CI
      result = location1 + location2;

      this->CI = decimalToBinary(memSize, result);

      if (debug) {
        cout << "To: " << CI << '\n';
        cout << "Which is an increase of " << location1 - location2 <<'\n';
      }
    }
    /*
    	This function takes in an operand and sets the ACCUMULATOR to hold the negative
      of what the operand points to in te store

      @param operand - The address of the number in the store
    */
    void LDN(string operand)
    {

      if (debug) {
        cout << " which will cause us to call the LDN Instruction" << '\n';
        cout << "After calling LDN, we have changed the ACCUMULATOR from: " << binaryToDecimal(memSize, ACCUMULATOR) << '\n';
      }

      //retreive the value from the store
    	int num = binaryToDecimal(addressSize, operand);
      num = getNumAtIndex(num);

      //set the ACCUMULATOR to be the negative value aken from the store
      ACCUMULATOR = decimalToBinary(memSize, num * -1);

      if (debug) {
        cout << "To : " << binaryToDecimal(memSize, ACCUMULATOR) << '\n';
      }
    }
    /*
    	This function takes in an operand which points to where a number can be stored,
      then sets the contents of accumulator to be stored in that location

      @param operand - The address of the number in the store
    */
    void STO(string operand)
    {

      if (debug) {
        cout << " which will cause us to call the STO Instruction" << '\n';
        cout << "After calling STO, we will be storing the value: " << binaryToDecimal(memSize, ACCUMULATOR) << '\n';
      }

      //retreive number from store
      int num = binaryToDecimal(addressSize, operand);
      //set accumulator to be equal to ir
      this->STORE.at(num) = ACCUMULATOR;

      if (debug) {
        cout << "In the index : " << num << '\n';
      }
    }

    /*
    	This function takes in an operand which points to a number on the store, then retreives
      that number and subtracts it from the accumulator

      @param operand - The address of the number in the store
    */
    void SUB(string operand)
    {

      //get numbers to perform calculations
      int num = binaryToDecimal(addressSize, operand);
      int num2 = binaryToDecimal(memSize, ACCUMULATOR);

      num = getNumAtIndex(num);

      if (debug) {
        cout << " which will cause us to call the SUB Instruction" << '\n';
        cout << "After calling SUB, we will be subtracting the value: " << num << '\n';
        cout << "From the value: " << binaryToDecimal(memSize, ACCUMULATOR) << '\n';
      }

      //set accumulator to the result of the subtraction
      ACCUMULATOR = decimalToBinary(memSize, num2 - num);

      if (debug) {
        cout << "The resulting number is: " << binaryToDecimal(memSize, ACCUMULATOR) << '\n';
      }
    }
    /*
    	if the accumulator has negative values then incriment, if it doesnt, then do nothing
      @param operand - The address of the number in the store
    */
    void CMP(string operand)
    {
    	if (debug) {
        cout << " which will cause us to call the CMP" << '\n';
      }
      if (binaryToDecimal(memSize, ACCUMULATOR) < 0) {
        if (debug) {
          cout << "ACCUMULATOR < 0, so we shall increment CI \n" << '\n';
        }
        incriment();
      }
    }
    /*
    	This stops the proccess

      @param operand - The address of the number in the store
    */
    void STP()
    {
      if (debug) {
        std::cout << " which will cause us to call STP" << '\n';
      }
    	cout << "Manchester baby Simulation reached STOP signal" << '\n';
    }


  /*
  	This method takes the input that the system has detected from the user and turns that into a command, this is passed on by the system through the fetch decode execute cycle
  	@param instruction - This the the instruction code the user has gained from the file
  	@param line - This code is the line that the code is to affect, this varies depending, it is converted from binary and passed on.
  */
  int execute(string instruction, string operand)
  {

  	int opCode = getOpcode(instruction);

  	if(opCode == JMPCODE)
  	{
  		JMP(operand);
  	}
  	else if(opCode == JRPCODE)
  	{
  		JRP(operand);
  	}
  	else if(opCode == LDNCODE)
  	{
  		LDN(operand);
  	}
  	else if(opCode == STOCODE)
  	{
  		STO(operand);
  	}
  	else if(opCode == SUBCODE)
  	{
  		SUB(operand);
  	}
  	else if(opCode == SUB2CODE)
  	{
  		SUB(operand);
  	}
  	else if(opCode == CMPCODE)
  	{
  		CMP(operand);
  	}
  	else if(opCode == STPCODE)
  	{
      STP();
      return -1;
  	}
    return 0;
  }

  /* The following function takes in an integer array of 1's and 0's and converts
  the array into its Integer equivalent. It uses the powers of 2 for each 'bit'
  in the array to multiply it out then increments the current power of two.
  @size - The size argument is how many elements are in the array passed in (usually 32)
  @arrayToConvert - The arrayToConvert argument is the array to be converted to a Integer
  @return - returns the respective integer that is the result of the arrayToConvert
  */
  int binaryToDecimal(int size, string arrayToConvert) {

     int i;
     int decimal = 0, bit = 1;
     bool negative = false;

     reverse(arrayToConvert.begin(), arrayToConvert.end());

     if (arrayToConvert.at(0) == '1') {
       negative = true;

      for (size_t i = 0; i < arrayToConvert.length(); i++) {
        if (arrayToConvert.at(i) == '1') {
          arrayToConvert.at(i) = '0';
        } else {
          arrayToConvert.at(i) = '1';
        }
      }
     }

     for (i = size - 1; i >= 0; i--) {

         decimal += (arrayToConvert[i] - '0')*bit;
         bit *= 2;
     }
     if (negative) {
       ++decimal;
       return decimal * -1;
     }

  	 return decimal;
  }

  /*
  	The method below takes the input and changes that to a binary input, this will take an int and convert it
  	This takes a calculation and applies binary logic, checking for each interval of number it fits, getting a remainder then
  	passing that on
  	@size - This is the size of the item itself, this varies with how many are passed to the system
  	@numToConvert - This is the number being passed in that is about to be converted
  	@return - returns the completed binary number in an array
  */
  string decimalToBinary(int size, int numToConvert) {

  	string binary;
  	int k;

  	for (int i = size-1; i >= 0; i--) {

  		k = numToConvert >> i;

  		if (k & 1) {
  			binary.push_back('1');
  		} else {
  			binary.push_back('0');
  		}
  	}

    reverse(binary.begin(), binary.end());

    return binary;
  }

  /*
  The following function will run the Manchester baby simulation, if the debug mode is turned on
  it will stop to display what the simulation is doing at each step of the cycle
  */
  void runBaby () {

    char debugger;
    int result = 0;

    while (result != -1) {

      if (debug) {
        cout << "\n\nPress enter to continue or input 1 to turn off debugging" << '\n';
        cin.get(debugger);

        if (debugger == '1') {
          this->flipDebugSwitch();
        }
      }

      this->incriment();

      if (debug) {
        cout << "\n\nPress enter to continue or input 1 to turn off debugging" << '\n';
        cin.get(debugger);

        if (debugger == '1') {
          this->flipDebugSwitch();
        }
      }

      this->fetch();

      string opCode = this->decodeOpcode();
      string operand = this->decodeOperand();

      if (debug) {
        cout << "\n\nPress enter to continue or input 1 to turn off debugging" << '\n';
        cin.get(debugger);;

        if (debugger == '1') {
          this->flipDebugSwitch();
        }
      }
      result = this->execute(opCode, operand);

    }

    this->displayInfo();

    //reset all the values to default

    //set ACCUMULATOR to be 0 for default
    ACCUMULATOR = "00000000000000000000000000000000";
    //set CI to point to the default line 0
    CI = "00000000000000000000000000000000";
    //set current line to contain debugger at default
    PI = "00000000000000000000000000000000";
    //turn off debug mode
    debug = false;
  }
};

/*
Basic menu, starts the manchester baby and does some setting up preparations
*/
void babyMenu() {

  bool running = true;
  cout << "\n\nWelcome to the Manchester Baby Program!\n" << '\n';

  Baby *currentBaby = new Baby();

  cout << "Please load an Initial file \n" << '\n';

  while (!currentBaby->getLoadStatus()) {
    currentBaby->setStore();
  }

  while (running) {
      cout << "\nPlease Select an option from the list below:\n" << '\n';

    cout << "Enter 1 to load in a new file" << '\n';
    cout << "Enter 2 to enable/disable debug mode" << '\n';
    cout << "Enter 3 to run the Baby" << '\n';
    //cout << "Enter 4 to switch to 64 bit mode" << '\n';
    cout << "Enter 0 to quit \n" << '\n';


    int input;

    cin >> input;

    if(cin.fail()) {
      cin.clear();
      cin.ignore();
      cout << "Please input an Integer.";
    } else {

      switch (input) {
        case 0:
          running = false;
          cout << "Quitting Manchester baby....." << '\n';
          break;
        case 1:
          do {
            currentBaby->setStore();
          } while(!currentBaby->getLoadStatus());
          break;
        case 2:
          currentBaby->flipDebugSwitch();
          break;
        case 3:
        if (currentBaby->getLoadStatus()) {
          currentBaby->runBaby();
        } else {
          std::cout << "No file loaded into store!" << '\n';
        }
          break;
        default:
          cout << "Number not within range" << '\n';
          break;
      }
    }
  }
}
