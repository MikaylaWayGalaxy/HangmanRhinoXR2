#ifndef PARSE_H
#define PARSE_H
#include <fstream>
#include <string>
#include <cctype>
#include "MasterHeader.h"
using namespace std;

class Parse
{
private:
	//private member variables
	ifstream InputFile;							//instance of ifstream object
	string FileName;							//name of file
	int Mode;									//holds mode for the Move function in Rhino
	Coord Target;								//holds target for the Move function in Rhino
	double Height;								//holds height for the Move function in Rhino
	Coord Center;								//holds center for the Move function in Rhino (WIP)

public:
	//constructor(s)/destructor
	Parse(string FileName);
	~Parse();

	//public member functions
	bool GetLine(int &Mode, Coord &Target, double &Height);
	bool GetLine(int &Mode, Coord &Target, Coord &Center);
	/*
	bool GetLine();
	int GetMode() { return Mode; }
	Coord GetTarget() { return Target; }
	double GetHeight() { return Height; }
	Coord GetCenter() { return Center; }
	*/
	bool IsOpen() { return !InputFile.fail(); }
	bool IsEOF();
	bool NotEOF();
};

#endif