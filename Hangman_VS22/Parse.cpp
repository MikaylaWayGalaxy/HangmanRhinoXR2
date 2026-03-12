#include "Parse.h"

//**********************************************************************************************//
//									 Parse member functions										//
//**********************************************************************************************//

//constructor, opens file
//pass in the name of a file as a string
Parse::Parse(string FileName)
{
	InputFile.open(FileName);
	this->FileName = FileName;
}



//destructor, closes file
Parse::~Parse()
{
	InputFile.close();
}



//reads the next line in a file and assigns data to the variables passed by reference
//version with h for height
bool Parse::GetLine(int &Mode, Coord &Target, double &Height)
{
	const int BuffSize = 10;				//max buffer size
	char Buffer[BuffSize] = { 0 };			//holds characters being read from file, initalize to zero (0 not the character '0')
	int index = 0;							//index for buffer
	bool DecimalFlag = false;				//used to make sure each number has only one decimal point

	if (InputFile.peek() == EOF)			//if at end of file, return false
		return false;

	//read data until you hit a new line or the end of file
	while (InputFile.peek() != '\n' && InputFile.peek() != EOF)
	{
		switch (toupper(InputFile.get()))	//read the next character and convert to uppercase
		{
		case('G'):
			//while next character is a number, or a single decimal point
			//	read data into buffer
			while ((isdigit(InputFile.peek()) || (InputFile.peek() == '.' && !DecimalFlag)) && index < BuffSize)
			{
				if (InputFile.peek() == '.')					//if next character is a decimal point, assume remaining characters are zeros
				{
					DecimalFlag = true;
					break;
				}
				Buffer[index++] = InputFile.get();				//read data into buffer
			}
			this->Mode = Mode = atoi(Buffer);					//convert characters in buffer to integer and assign to Mode (member variable and reference parameter)
			break;

		case('X'):
			//while next character is a number, or a single decimal point, or a minus sign at the start
			//	read data into buffer
			while ((isdigit(InputFile.peek()) || (InputFile.peek() == '-' && index == 0) || (InputFile.peek() == '.' && !DecimalFlag)) && index < BuffSize)
			{
				if (InputFile.peek() == '.')
					DecimalFlag = true;
				Buffer[index++] = InputFile.get();				//read data into buffer
			}
			this->Target.x = Target.x = atof(Buffer);			//convert characters in buffer to double and assign to x component of target (member variable and reference parameter)
			break;

		case('Y'):
			//while next character is a number, or a single decimal point, or a minus sign at the start
			//	read data into buffer
			while ((isdigit(InputFile.peek()) || (InputFile.peek() == '-' && index == 0) || (InputFile.peek() == '.' && !DecimalFlag)) && index < BuffSize)
			{
				if (InputFile.peek() == '.')
					DecimalFlag = true;
				Buffer[index++] = InputFile.get();				//read data into buffer
			}
			this->Target.y = Target.y = atof(Buffer);			//convert characters in buffer to double and assign to y component of target (member variable and reference parameter)
			break;

		case('H'):
			//while next character is a number, or a single decimal point, or a minus sign at the start
			//	read data into buffer
			while ((isdigit(InputFile.peek()) || (InputFile.peek() == '-' && index == 0) || (InputFile.peek() == '.' && !DecimalFlag)) && index < BuffSize)
			{
				if (InputFile.peek() == '.')
					DecimalFlag = true;
				Buffer[index++] = InputFile.get();				//read data into buffer
			}
			this->Height = Height = atof(Buffer);				//convert characters in buffer to double and assign to Height (member variable and reference parameter)
			break;
		}

		//between each letter: reset buffer, index, and decimal flag
		for (int j = 0; j < BuffSize; j++)
			Buffer[j] = 0;
		index = 0;
		DecimalFlag = false;
	}

	InputFile.get();			//move cursor past \n character
	return true;				//end of line reached, return true
}



//reads the next line in a file and assigns data to the variables passed by reference
//version with I and J for center
bool Parse::GetLine(int &Mode, Coord &Target, Coord &Center)
{
	const int BuffSize = 10;				//max buffer size
	char Buffer[BuffSize] = { 0 };			//holds characters being read from file, initalize to zero (0 not the character '0')
	int index = 0;							//index for buffer
	bool DecimalFlag = false;				//used to make sure each number has only one decimal point

	if (InputFile.peek() == EOF)			//if at end of file, return false
		return false;

	//read data until you hit a new line or the end of file
	while (InputFile.peek() != '\n' && InputFile.peek() != EOF)
	{
		switch (toupper(InputFile.get()))	//read the next character and convert to uppercase
		{
		case('G'):
			//while next character is a number, or a single decimal point
			//	read data into buffer
			while ((isdigit(InputFile.peek()) || (InputFile.peek() == '.' && !DecimalFlag)) && index < BuffSize)
			{
				if (InputFile.peek() == '.')					//if next character is a decimal point, assume remaining characters are zeros
				{
					DecimalFlag = true;
					break;
				}
				Buffer[index++] = InputFile.get();				//read data into buffer
			}
			this->Mode = Mode = atoi(Buffer);					//convert characters in buffer to integer and assign to Mode (member variable and reference parameter)
			break;

		case('X'):
			//while next character is a number, or a single decimal point, or a minus sign at the start
			//	read data into buffer
			while ((isdigit(InputFile.peek()) || (InputFile.peek() == '-' && index == 0) || (InputFile.peek() == '.' && !DecimalFlag)) && index < BuffSize)
			{
				if (InputFile.peek() == '.')
					DecimalFlag = true;
				Buffer[index++] = InputFile.get();				//read data into buffer
			}
			this->Target.x = Target.x = atof(Buffer);			//convert characters in buffer to double and assign to x component of target (member variable and reference parameter)
			break;

		case('Y'):
			//while next character is a number, or a single decimal point, or a minus sign at the start
			//	read data into buffer
			while ((isdigit(InputFile.peek()) || (InputFile.peek() == '-' && index == 0) || (InputFile.peek() == '.' && !DecimalFlag)) && index < BuffSize)
			{
				if (InputFile.peek() == '.')
					DecimalFlag = true;
				Buffer[index++] = InputFile.get();				//read data into buffer
			}
			this->Target.y = Target.y = atof(Buffer);			//convert characters in buffer to double and assign to y component of target (member variable and reference parameter)
			break;

		case('I'):
			//while next character is a number, or a single decimal point, or a minus sign at the start
			//	read data into buffer
			while ((isdigit(InputFile.peek()) || (InputFile.peek() == '-' && index == 0) || (InputFile.peek() == '.' && !DecimalFlag)) && index < BuffSize)
			{
				if (InputFile.peek() == '.')
					DecimalFlag = true;
				Buffer[index++] = InputFile.get();				//read data into buffer
			}
			this->Center.x = Center.x = atof(Buffer);			//convert characters in buffer to double and assign to x component of target (member variable and reference parameter)
			break;

		case('J'):
			//while next character is a number, or a single decimal point, or a minus sign at the start
			//	read data into buffer
			while ((isdigit(InputFile.peek()) || (InputFile.peek() == '-' && index == 0) || (InputFile.peek() == '.' && !DecimalFlag)) && index < BuffSize)
			{
				if (InputFile.peek() == '.')
					DecimalFlag = true;
				Buffer[index++] = InputFile.get();				//read data into buffer
			}
			this->Center.y = Center.y = atof(Buffer);			//convert characters in buffer to double and assign to y component of target (member variable and reference parameter)
			break;
		}

		//between each letter: reset buffer, index, and decimal flag
		for (int j = 0; j < BuffSize; j++)
			Buffer[j] = 0;
		index = 0;
		DecimalFlag = false;
	}

	InputFile.get();			//move cursor past \n character
	return true;				//end of line reached, return true
}



/*
//reads the next line in a file and assigns data the private member variables 
//use GetMode, GetTarget, GetHeight, GetCenter to retrive data
bool Parse::GetLine()
{
	const int BuffSize = 10;				//max buffer size
	char Buffer[BuffSize] = { 0 };			//holds characters being read from file, initalize to zero (0 not the character '0')
	int index = 0;							//index for buffer
	bool DecimalFlag = false;				//used to make sure each number has only one decimal point

	if (InputFile.peek() == EOF)			//if at end of file, return false
		return false;

	//read data until you hit a new line or the end of file
	while (InputFile.peek() != '\n' && InputFile.peek() != EOF)
	{
		switch (toupper(InputFile.get()))	//read the next character and convert to uppercase
		{
		case('G'):
			//while next character is a number, or a single decimal point
			//	read data into buffer
			while ((isdigit(InputFile.peek()) || (InputFile.peek() == '.' && !DecimalFlag)) && index < BuffSize)
			{
				if (InputFile.peek() == '.')					//if next character is a decimal point, assume remaining characters are zeros
				{
					DecimalFlag = true;
					break;
				}
				Buffer[index++] = InputFile.get();				//read data into buffer
			}
			Mode = atoi(Buffer);					//convert characters in buffer to integer and assign to Mode (member variable and reference parameter)
			break;

		case('X'):
			//while next character is a number, or a single decimal point, or a minus sign at the start
			//	read data into buffer
			while ((isdigit(InputFile.peek()) || (InputFile.peek() == '-' && index == 0) || (InputFile.peek() == '.' && !DecimalFlag)) && index < BuffSize)
			{
				if (InputFile.peek() == '.')
					DecimalFlag = true;
				Buffer[index++] = InputFile.get();				//read data into buffer
			}
			Target.x = atof(Buffer);			//convert characters in buffer to double and assign to x component of target (member variable and reference parameter)
			break;

		case('Y'):
			//while next character is a number, or a single decimal point, or a minus sign at the start
			//	read data into buffer
			while ((isdigit(InputFile.peek()) || (InputFile.peek() == '-' && index == 0) || (InputFile.peek() == '.' && !DecimalFlag)) && index < BuffSize)
			{
				if (InputFile.peek() == '.')
					DecimalFlag = true;
				Buffer[index++] = InputFile.get();				//read data into buffer
			}
			Target.y = atof(Buffer);			//convert characters in buffer to double and assign to y component of target (member variable and reference parameter)
			break;

		case('H'):
			//while next character is a number, or a single decimal point, or a minus sign at the start
			//	read data into buffer
			while ((isdigit(InputFile.peek()) || (InputFile.peek() == '-' && index == 0) || (InputFile.peek() == '.' && !DecimalFlag)) && index < BuffSize)
			{
				if (InputFile.peek() == '.')
					DecimalFlag = true;
				Buffer[index++] = InputFile.get();				//read data into buffer
			}
			Height = atof(Buffer);				//convert characters in buffer to double and assign to Height (member variable and reference parameter)
			break;

		case('I'):
			//while next character is a number, or a single decimal point, or a minus sign at the start
			//	read data into buffer
			while ((isdigit(InputFile.peek()) || (InputFile.peek() == '-' && index == 0) || (InputFile.peek() == '.' && !DecimalFlag)) && index < BuffSize)
			{
				if (InputFile.peek() == '.')
					DecimalFlag = true;
				Buffer[index++] = InputFile.get();				//read data into buffer
			}
			Center.x = atof(Buffer);			//convert characters in buffer to double and assign to x component of target (member variable and reference parameter)
			break;

		case('J'):
			//while next character is a number, or a single decimal point, or a minus sign at the start
			//	read data into buffer
			while ((isdigit(InputFile.peek()) || (InputFile.peek() == '-' && index == 0) || (InputFile.peek() == '.' && !DecimalFlag)) && index < BuffSize)
			{
				if (InputFile.peek() == '.')
					DecimalFlag = true;
				Buffer[index++] = InputFile.get();				//read data into buffer
			}
			Center.y = atof(Buffer);			//convert characters in buffer to double and assign to y component of target (member variable and reference parameter)
			break;
		}

		//between each letter: reset buffer, index, and decimal flag
		for (int j = 0; j < BuffSize; j++)
			Buffer[j] = 0;
		index = 0;
		DecimalFlag = false;
	}

	InputFile.get();			//move cursor past \n character
	return true;				//end of line reached, return true
}
*/



//if at end of file, return true
bool Parse::IsEOF()
{
	if (InputFile.peek() == EOF)
		return true;
	return false;
}



//if not at end of file, return true
bool Parse::NotEOF()
{
	if (InputFile.peek() != EOF)
		return true;
	return false;
}