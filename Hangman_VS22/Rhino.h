#ifndef RHINO_H
#define RHINO_H
#include <cmath>
#include <string>
#include <windows.h>		//for Sleep function
#include <iostream>			//for cout command in MoveToPoint
#include <iomanip>			//for cout command in MoveToPoint
#include "MasterHeader.h"
#include "RoboMath.h"
#include "tserial.h"
#include "Parse.h"

class Rhino
{
protected:
	//protected member variables
	Tserial TserialConnection;						//instance of Tserial, used to send commands to robot, protected so that derived classes have access

private:
	//private member variables
	RoboMath RhinoMath;								//instance of RoboMath, used for calculating angles and postions
	char ComPort[5];								//comport used to connect to robot, use COM1 for school computer, use device manager to find otherwise
	double ServoE = 750.0;							//current angular position of lower arm divided by 0.12, defaults to 90 degrees / 0.12 = 750
	double ServoD = 750.0;							//current angular position of upper arm divided by 0.12, defaults to 90 degrees / 0.12 = 750
	double ServoF = 0.0;							//position of base servo, rotates entire arm, uses increments of 0.14
	Coord Home = { 9.0, 9.0 };						//home/starting position, defaults to (9,9)
	unsigned int Delay = 500;						//time in milliseconds in between MoveServo commands, defaults to half a second
	unsigned int InchDiv = 5;						//number of moves for each inch of distance when moving along MoveLine or MoveArc, initialized to 5 aka move every fifth of an inch
	bool OutputPoints = true;						//if true, MoveToPoint will outputs points during move

	//private member functions
	bool MoveServo(char, double);

public:
	//constructor(s)/destructor
	Rhino() {/*do nothing*/}						//default constructor, needed for derived classes, DO NOT USE without adding another way to connect Tserial
	Rhino(char* CPort);								//constructor, connects to tserial
	~Rhino() { TserialConnection.disconnect(); }	//destructor, disconnects serial port

	//movement functions
	bool Move(int, Coord, double);
	bool Move(int, Coord);
	bool MoveToPoint(Coord);
	bool MoveLine(Coord);
	bool MoveArc(Coord, double, bool);
	bool MoveHome();
	bool RotateOut();
	bool RotateIn();
	bool RotateIn(bool);
	bool RotateMove(Coord);
	bool RotateHome() { return RotateMove(Home); }
	bool DrawCircle(Coord, double);
	bool DrawChar(char, Coord, double);
	bool MoveGcodeH(string FileName);

	//accessors and mutators
	Coord CurrentPoint() { return RhinoMath.CurrentPoint(); }
	void SetStartPoint(Coord);
	void SetDelay(unsigned int Delay) { this->Delay = Delay; }
	void SetInchDiv(unsigned int Div) { InchDiv = Div; }
	void ToggleCout() { OutputPoints = !OutputPoints; }				//enables or disables cout command in MoveToPoint
};

#endif