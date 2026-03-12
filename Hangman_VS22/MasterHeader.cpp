#include "MasterHeader.h"

//**********************************************************************************************//
//								  Coord overloaded operators									//
//**********************************************************************************************//

//assignment operator
Coord Coord::operator=(const Coord &Right)
{
	this->x = Right.x;
	this->y = Right.y;
	return *this;
}

//comparison operator
bool Coord::operator==(const Coord &Right)
{
	if (this->x == Right.x && this->y == Right.y)
		return true;
	return false;
}

//addition operator (vector addition)
Coord Coord::operator+(const Coord &Right)
{
	Coord Left = *this;
	Left.x += Right.x;
	Left.y += Right.y;
	return Left;
}

//subtraction operator (vector subtraction)
Coord Coord::operator-(const Coord &Right)
{
	Coord Left = *this;
	Left.x -= Right.x;
	Left.y -= Right.y;
	return Left;
}

//addition assignment operator (vector addition)
Coord Coord::operator+=(const Coord &Right)
{
	this->x += Right.x;
	this->y += Right.y;
	return *this;
}

//subtraction assignment operator (vector subtraction)
Coord Coord::operator-=(const Coord &Right)
{
	this->x -= Right.x;
	this->y -= Right.y;
	return *this;
}

//multiplication assignment operator (scalar multiplication)
Coord Coord::operator*=(const double &Scalar)
{
	this->x *= Scalar;
	this->y *= Scalar;
	return *this;
}

//multiplication operator (scalar multiplication) (Coord on left)
Coord operator*(const double &Scalar, Coord Vector)
{
	//passing by value copies data, original data is not modified
	Vector.x *= Scalar;
	Vector.y *= Scalar;
	return Vector;
}

//multiplication operator (scalar multiplication) (Coord on right)
Coord operator*(Coord Vector, const double &Scalar)
{
	//passing by value copies data, original data is not modified
	Vector.x *= Scalar;
	Vector.y *= Scalar;
	return Vector;
}



//**********************************************************************************************//
//										  Global functions										//
//**********************************************************************************************//

//Dot product
double Dot(Coord Left, Coord Right)
{
	return Left.x * Right.x + Left.y * Right.y;
}