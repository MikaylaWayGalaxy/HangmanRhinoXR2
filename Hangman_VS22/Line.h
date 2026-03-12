#ifndef LINE_H
#define LINE_H
#include "RoboMath.h"
#include <functional>

class Line
{
private:
	Coord Start;
	Coord End;
	Coord Vector;
	double Magnitude;
	double Angle;

public:
	Line();
	Line(Coord Start, Coord End);
	bool Move(bool (*MoveToPoint)(Coord));
	//bool Move(std::function<bool(Coord)>);
	//bool Move(Coord Current, Coord Target, bool (*MoveToPoint)(Coord));
	double GetLength() { return Magnitude; }
	double GetAngle()  { return Angle; }
};

//passing function as parameter
// output_type (*func)(input_type, input_type)
//ex: int (*func)(int, int)
//use & to get the address of the function when passing as parameter

#endif