#include "RoboMath.h"

//**********************************************************************************************//
//									  RoboMath member functions									//
//**********************************************************************************************//

//Calculates and returns angle 2 which is the upper angle using Inverse Kinematics (see page 7 of Forward Kenematics for details)
double RoboMath::CalcAngle2()
{
	double Num;
	double Den;
	Num = pow(RectPoint.x, 2.0) + pow(RectPoint.y, 2.0) -
		  pow(Length1, 2.0) - pow(Length2, 2.0);
	Den = 2.0 * Length1 * Length2;								//make sure lengths are > 0
	ThetaPair.theta2 = acos(Num / Den) * 180 / PI;				//converts to degrees and saves data to theta2
	return ThetaPair.theta2;
}



//Calculates and returns angle 1 which is the lower angle using Inverse Kinematics (see page 7 of Forward Kenematics for details)
//WARNING!! Must call CalcAngle2 before running this function or the data may not be accurate
double RoboMath::CalcAngle1()
{
	ThetaPair.theta1 = asin((Length2 * sin(ThetaPair.theta2 * PI / 180) ) /		//converts theta2 to radians temporarily
		     sqrt(pow(RectPoint.x, 2.0) + pow(RectPoint.y, 2.0)));
	ThetaPair.theta1 += atan2(RectPoint.y, RectPoint.x);						//Overwrites theta1 to be the new angle
	ThetaPair.theta1 *= 180 / PI;												//convert to degrees
	return ThetaPair.theta1;
}



//Takes in a new coordinate point
//Sets RectPoint to new point and calculates angles required to get there
void RoboMath::SetCoord(Coord NewPoint)
{
	RectPoint.x = NewPoint.x;
	RectPoint.y = NewPoint.y;
	CalcAngle2();
	CalcAngle1();
}



//Takes in a new coordinate point
//Sets RectPoint to new point, if skip flag is not set, calculates angles
void RoboMath::SetCoord(Coord NewPoint, bool Skip)
{
	RectPoint.x = NewPoint.x;
	RectPoint.y = NewPoint.y;
	if (Skip == 1)
		return;
	CalcAngle2();
	CalcAngle1();
}



//Private function that can reset ThetaPair
void RoboMath::SetAnglePair(AnglePair Angles)
{
	ThetaPair.theta1 = Angles.theta1;
	ThetaPair.theta2 = Angles.theta2;
}



//WIP
//Takes in a number and a position to round to
//position == 0 is nearest whole number, 1 is nearest tenth, etc.
double RoboMath::RoundRM(double Number, int Position)
{
	int MoveDecimal = pow(10.0, double(Position));
	return round(double(MoveDecimal) * Number) / double(MoveDecimal);
}