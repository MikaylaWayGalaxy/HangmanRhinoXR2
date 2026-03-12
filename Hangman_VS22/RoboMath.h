#ifndef ROBOMATH_H
#define ROBOMATH_H
#include <cmath>
#include "MasterHeader.h"

class RoboMath
{
private:
	//private member variables
	Coord RectPoint = { 9.0, 9.0 };							//defaults to position (9,9)
	AnglePair ThetaPair = { 90.0, 90.0 };					//defaults both to 90 degrees, theta1 is the lower angle, theta2 is upper
	double Length1;											//Arm 1 is the lower/base arm
	double Length2;											//Arm 2 is the upper/extended arm

	//private member functions
	double CalcAngle2();
	double CalcAngle1();
	void SetAnglePair(AnglePair);							//Reset ThetaPair to a particular value

public:
	//constructor(s)/destructor
	RoboMath()												//Default constructor, sets arm length = 9.0
	{
		Length1 = 9.0;
		Length2 = 9.0;
	}
	RoboMath(double Arm1, double Arm2)						//Overloaded constructor, pass in arm lengths
	{
		Length1 = Arm1;
		Length2 = Arm2;
	}

	//public member functions
	void SetCoord(Coord);
	void SetCoord(Coord, bool);
	AnglePair GetAnglePair() { return ThetaPair; }
	double GetTheta1() { return ThetaPair.theta1; }			//returns lower angle in degrees
	double GetTheta2() { return ThetaPair.theta2; }			//returns upper angle in degrees
	Coord CurrentPoint() { return RectPoint; }

	//WIP
	double RoundRM(double, int);
};

#endif