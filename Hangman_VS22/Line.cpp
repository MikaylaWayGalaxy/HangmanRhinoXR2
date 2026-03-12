#include "Line.h"

//assigns to default vaules, line is a single point at (9,9)
Line::Line()
{
	Start = { 9.0, 9.0 };
	End = { 9.0, 9.0 };
	Vector = { 0.0, 0.0 };
	Magnitude = 0.0;
	Angle = 0.0;
}



Line::Line(Coord Start, Coord End)
{
	this->Start = Start;
	this->End = End;

	Vector.x = End.x - Start.x;
	Vector.y = End.y - Start.y;

	Magnitude = sqrt(pow(Vector.x, 2) + pow(Vector.y, 2));
	Angle = atan2(Vector.y, Vector.x) * 180 / PI;				//stores in degrees
}



bool Line::Move(bool (*MoveToPoint)(Coord))
{
	Coord Current = Start;
	Coord MoveVector = Vector;
	unsigned int NumMoves = 6;							//initialize to 6 for satefy

	if (Magnitude == 0.0)
		return false;

	NumMoves = static_cast<unsigned int>(round(Magnitude)) * 5;				//approx 1/5 of an inch

	MoveVector.x = MoveVector.x / static_cast<double>(NumMoves);
	MoveVector.y = MoveVector.y / static_cast<double>(NumMoves);

	for (unsigned int i = 0; i < NumMoves; i++)						//loop to move along line
	{
		Current.x += MoveVector.x;									//move current to next point
		Current.y += MoveVector.y;
		MoveToPoint(Current);										//use &, reintroduce if statement
		//if (!MoveToPoint(Current))								//replace with reference to function paramenter
		//	return false;
	}
	return true;
}
//bool Line::Move(std::function<bool(Coord)> MoveToPoint)



/*
bool Line::Move(Coord Current, Coord Target, bool (*MoveToPoint)(Coord))
{
	Coord MoveVector;
	unsigned int NumMoves = 6;

	//reassigns class member data
	Start = Current;
	End = Target;
	Vector.x = Target.x - Current.x;
	Vector.y = Target.y - Current.y;
	Magnitude = sqrt(pow(Vector.x, 2) + pow(Vector.y, 2));
	Angle = atan2(Vector.y, Vector.x) * 180 / PI;					//stores in degrees

	if (Magnitude == 0.0)
		return false;

	NumMoves = static_cast<unsigned int>(round(Magnitude)) * 5;				//approx 1/5 of an inch
	MoveVector = Vector;
	MoveVector.x = MoveVector.x / static_cast<double>(NumMoves);
	MoveVector.y = MoveVector.y / static_cast<double>(NumMoves);

	for (unsigned int i = 0; i < NumMoves; i++)						//loop to move along line
	{
		Current.x += MoveVector.x;									//move current to next point
		Current.y += MoveVector.y;
		MoveToPoint(Current);										//use &, reintroduce if statement
		//if (!MoveToPoint(Current))								//replace with reference to function paramenter
		//	return false;
	}

	return true;
}
*/