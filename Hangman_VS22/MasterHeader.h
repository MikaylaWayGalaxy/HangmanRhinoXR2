#ifndef MASTERHEADER_H
#define MASTERHEADER_H
#include <cmath>

//global constant for PI
const double PI = 3.141592653589793;



//Default ComPort
//char DefaultCPort[5] = "COM1";



//2d coordinate point
//can also act as a vector in R^2
struct Coord
{
	//member data
	double x;										//x component
	double y;										//y component

	//overloaded operators
	Coord operator=(const Coord &Right);
	bool operator==(const Coord &Right);
	Coord operator+(const Coord &Right);
	Coord operator-(const Coord &Right);
	Coord operator+=(const Coord &Right);
	Coord operator-=(const Coord &Right);
	Coord operator*=(const double &Scalar);

	//member functions
	double Magnitude() { return sqrt(pow(this->x, 2.0) + pow(this->y, 2.0)); }
	double AngleDeg() { return atan2(this->y, this->x) * 180 / PI; }
	double AngleRad() { return atan2(this->y, this->x); }
};
//multiplication operators, must be outside structure definition to allow commutativity
Coord operator*(const double& Scalar, Coord Vector);
Coord operator*(Coord Vector, const double& Scalar);



//Dot product
double Dot(Coord Left, Coord Right);



//Angle pair
struct AnglePair
{
	double theta1;
	double theta2;
};

#endif