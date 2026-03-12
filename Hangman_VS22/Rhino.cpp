#include "Rhino.h"

//**********************************************************************************************//
//									  Rhino member functions									//
//**********************************************************************************************//

//constructor, connects COMPORT
Rhino::Rhino(char* CPort)
{
	TserialConnection.connect(CPort, 9600, spEVEN);
	//copy ComPort to be input CPort
	//assumes array size is always 5
	for (int i = 0; i < 5; i++)
	{
		ComPort[i] = CPort[i];
	}
}



//builds cstring and sends to TserialConnention
//takes in Servo as a char and direction as a double
bool Rhino::MoveServo(char Servo, double Direction)
{
	char Output[5] = { 0 };					//Output to Tserial
	int _40Count;							//number of times to send groupings of 40
	int Remainder;							//remainder after groupings of 40
	char StoreRem[5] = { 0 };				//stores remainder in base 10

	Output[0] = Servo;						//set first char = servo letter
	if (Direction > 0)						//set second char = sign of direction
		Output[1] = '+';
	else if (Direction < 0)
		Output[1] = '-';
	else
		return 0;							//if direction == 0, return false

	Direction = abs(Direction);											//sign already accounted for, get absolute value to allow moving in negative direction
	_40Count  = static_cast<int>(round(Direction)) / 40;				//groupings of 40 deciced by integer divide
	Remainder = static_cast<int>(round(Direction)) % 40;				//remainder decided my modulus
	
	Output[2] = '4';						//set last two chars to "40"
	Output[3] = '0';
	//for loop using _40Count
	for (int i = 0; i < _40Count; i++)
	{
		//temp for testing
		//cout << Output << endl;

		TserialConnection << Output;		//send to Tserial
		Sleep(Delay);						//wait for robot to move
	}

	//send remainder
	if (Remainder > 0)
	{
		_itoa_s(Remainder, StoreRem, 10);	//stores remainder as a cstring in base 10
		Output[2] = StoreRem[0];			//set last two chars to remainder
		Output[3] = StoreRem[1];

		//temp for testing
		//cout << Output << endl;

		TserialConnection << Output;		//send to Tserial
		Sleep(Delay);						//wait for robot to move
	}
	return 1;
}



//Move to point by specified mode
//Mode: 0: RotateMove, 1: MoveLine, 2: MoveArc(clockwise), 3: MoveArc(counter-clockwise), 4: MoveToPoint
bool Rhino::Move(int Mode, Coord Target, double height)
{
	switch (Mode)
	{
	case 0:
		return RotateMove(Target);
	case 1:
		return MoveLine(Target);
	case 2:
		return MoveArc(Target, height, false);
	case 3:
		return MoveArc(Target, height, true);
	case 4:
		return MoveToPoint(Target);
	//case 28:						//signals end of file when reading gcode
	//	return false;
	}
	return false;					//if reached, something has gone wrong
}



//Move to point by specified mode
//version without the height parameter, disables arc modes
//Mode: 0: RotateMove, 1: MoveLine, 2: MoveArc(clockwise), 3: MoveArc(counter-clockwise), 4: MoveToPoint
bool Rhino::Move(int Mode, Coord Target)
{
	switch (Mode)
	{
	case 0:
		return RotateMove(Target);
	case 1:
		return MoveLine(Target);
	case 2:
		return false;				//no arcs supported in this function, use version that takes in height
	case 3:
		return false;				//no arcs supported in this function, use version that takes in height
	case 4:
		return MoveToPoint(Target);
	}
	return false;					//if reached, something has gone wrong
}



//Moves to specified point
//calculates angles and sends to MoveServo
bool Rhino::MoveToPoint(Coord Target)
{
	RhinoMath.SetCoord(Target);						//calculates angles
	double ServoEDir = ServoE - 750.0;				//Holds the direction to be sent to MoveServo
	double ServoDDir = ServoD - 750.0;				//	initialize to current - 750 to account for how arm has previously moved in accordance with Rhino servo directions
	double ServoDTrueDir;							//holds true direction for ServoD to accurately track position
	bool Moved = true;								//validates move occured

	//rounds to nearest hundredth, divides into increments of 0.12 degrees, and adds to direction
	//uses complementary angle of inverse kinematics angles
	ServoEDir += round(100.0 * (90.0 - RhinoMath.GetTheta1())) / (100.0 * 0.12);
	ServoDDir += round(100.0 * (90.0 - RhinoMath.GetTheta2())) / (100.0 * 0.12);

	ServoDTrueDir = ServoDDir;						//saves the angular distance that would be moved if not modified
	ServoDDir -= ServoEDir;							//adjusts for the fact that Rhino moves upper arm when moving lower arm

	//Moves servos and test if moved
	if (!(MoveServo('E', ServoEDir)) && ServoEDir != 0.0)
		Moved = false;
	if (!(MoveServo('D', ServoDDir)) && ServoDDir != 0.0)
		Moved = false;
	
	//update current position after move
	if (Moved)
	{
		ServoE -= ServoEDir;
		ServoD -= ServoDTrueDir;
	}

	//print point being moved to, allows plotting points in Desmos
	if (OutputPoints)
		cout << fixed << setprecision(2) << "(" << CurrentPoint().x << ", " << CurrentPoint().y << ")\n";

	return Moved;
}



//Move to a point via a straight line
bool Rhino::MoveLine(Coord Target)
{
	Coord Current = RhinoMath.CurrentPoint();							//current position, initialize to starting point
	Coord Vector;														//vector to move along
	double Magnitude;													//magnitude of total movement
	//double Angle;														//angle from Current to Target
	unsigned int NumMoves = 6;											//number of times the main vector will be divided into smaller vectors, initialized as 6 for safety but overwritten later
	
	if (Current == Target)												//if already at target, return false
		return false;

	Vector = Target - Current;											//subtract vectors componentwise to get movement vector
	Magnitude = sqrt(pow(Vector.x, 2) + pow(Vector.y, 2));				//get magnitude/length of total move
	NumMoves = static_cast<unsigned int>(Magnitude * InchDiv);			//multiply magnitude by a number then round to nearest integer, results in moving approximately that fraction of an inch per move
	//Angle = atan2(Vector.y, Vector.x);								//currently unused in this implementation, careful: angle is in radians if used later
	if (NumMoves == 0)													//if length rounds down to zero, move once
		NumMoves++;

	//divide vector by number of moves
	Vector.x = Vector.x / static_cast<double>(NumMoves);
	Vector.y = Vector.y / static_cast<double>(NumMoves);

	for (unsigned int i = 0; i < NumMoves; i++)							//loop to move along line
	{
		Current += Vector;												//move current to next point
		if (!MoveToPoint(Current))										//move to current
			return false;												//	if movement fails, exit loop
	}

	return true;														//if this point is reached, move has succeeded
}



//Move to a point along an elliptical arc
//takes in a target point, how elliptical you want your arc, and whether to move clockwise or counter-clockwise 
//height = 1 is a circle, height > 1 is a stretched ellipse, 0 < height < 1 is a squished curve, height = 0 is a line
bool Rhino::MoveArc(Coord Target, double height, bool CCW)
{
	Coord Current = RhinoMath.CurrentPoint();			//current/starting point, is updated during move
	Coord Center;										//center of the ellipse, also the mid point between current and target
	Coord SlopeVector;									//vector from current to center
	Coord Apex;											//apex of ellipse
	double a, b;										//scalars of ellipse axes, a goes with x, b goes with y before rotation
	double t = 0.0;										//parameter of ellipse, t goes from 0 to pi to move halfway around
	double RotAngle;									//angle ellipse is rotated
	double CosAng, SinAng;								//holds sin and cos of rotation angle so they only have to be calculated once
	double Arclength;									//approximation of the arclength of half the ellipse
	unsigned int NumMoves = 10;							//number of smaller moves along ellipse, initialized to 10 for safety

	if (Current == Target)								//if already at target, exit
		return false;
	if (height == 0.0)									//if straight line, use line function (should be equivilent but saves some computation time)
		return MoveLine(Target);

	//determines which direction to move along ellipse
	height = abs(height);								//negative height ignored, uses clockwise/counter-clockwise flag instead
	if (CCW)											//if moving counter-clockwise, negate height and start t at PI rather than 0
	{
		height = -height;
		t = PI;
	}

	//calculate values
	Center = (Current + Target) * 0.5;												//calculate center using mid point formula
	SlopeVector = Center - Current;													//calculate slope vector using vector subtraction
	Apex.x = Center.x - height * SlopeVector.y;										//find apex by starting at center and moving along
	Apex.y = Center.y + height * SlopeVector.x;										//	the perpendicular slope scaled by height
	RotAngle = atan2(Center.y - Apex.y, Center.x - Apex.x) + (PI / 2.0);			//angle of rotation is the angle from center to apex plus 90 degrees
	CosAng = cos(RotAngle);															//calculate sin and cos of rotation angle
	SinAng = sin(RotAngle);															//	stores in radians
	b =  sqrt( pow(height*SlopeVector.x, 2.0) + pow(height * SlopeVector.y, 2.0) );	//b is the magnitude of the slope vector scaled by height
	a = -sqrt( pow(SlopeVector.x, 2.0) + pow(SlopeVector.y, 2.0) );					//a is the magnitude of the slope vector, negated so the parameterization goes in the correct direction

	//determine number of times to divide arc
	//need to find arclength
	if (height == 1)									//if semicircle, arclength = pi*radius
		Arclength = PI * b;
	else												//else approximates arclength using midpoint rule with 8 rectangles
	{
		int n = 8;										//number of rectangles
		Arclength = 0.0;
		for (int k = 0; k < n; k++)						//sum the height of the midpoints
		{
			//after taking derivative and getting magnitude
			//f(t) = sqrt( (a^2)sin^2(t) + (b^2)cos^2(t) )
			Arclength += sqrt( pow(a, 2.0) * pow(sin(PI/(2.0*n) + k*PI/n), 2.0) + pow(b, 2.0) * pow(cos(PI/(2.0*n) + k*PI/n), 2.0) );
		}
		Arclength *= PI/n;								//multiply by delta t
	}
	NumMoves = static_cast<unsigned int>(round(Arclength * InchDiv));				//multiply arclength by number of inch divisions, then round to nearest whole number
	if (NumMoves == 0)									//if arclength rounds down to zero, move to target directly
		NumMoves++;

	//loop to move to along arc
	for (unsigned int i = 0; i < NumMoves; i++)
	{
		//increment t to next division of pi depending on if traveling clockwise or counter-clockwise
		if (CCW)
			t -= PI / NumMoves;
		else
			t += PI / NumMoves;
		
		//	[rotation matrix] * [parameterized ellispe] + [center] 
		//	| cos(RotAngle)   -sin(RotAngle) |   | a*cos(t) |   | Center.x |
		//	| sin(RotAngle)    cos(RotAngle) | * | b*sin(t) | + | Center.y |
		Current.x = a * CosAng * cos(t) - b * SinAng * sin(t) + Center.x;			//parameterization of rotated ellipse, from a linear transformation
		Current.y = a * SinAng * cos(t) + b * CosAng * sin(t) + Center.y;			//	see notes for details
		if (!MoveToPoint(Current))													//move to next point
			return false;															//	if a move fails, stop moving and return false
	}
	return true;																	//if this point is reached, move has succeeded
}



//Moves to home position, default is (9,9)
bool Rhino::MoveHome()
{
	return MoveToPoint(Home);
}



//Rotates away from whiteboard
//assumes robot is starting with ServoD to the right of ServoE when facing the whiteboard
bool Rhino::RotateOut()
{
	ServoF -= 40.0;
	return MoveServo('F', -40.0);
}



//Rotates toward the whiteboard
//assumes robot is starting with ServoD to the right of ServoE when facing the whiteboard
bool Rhino::RotateIn()
{
	if (ServoF >= 0.0)
		return false;
	ServoF += 40.0;
	return MoveServo('F', 40.0);
}



//Rotates toward the whiteboard
//assumes robot is starting with ServoD to the right of ServoE when facing the whiteboard
//pass in a bool that allows moving ServoF in the positive direction, only use if you know it's safe to do so
bool Rhino::RotateIn(bool Safe)
{
	if (Safe)
	{
		ServoF += 40.0;
		return MoveServo('F', 40.0);
	}
	else return RotateIn();
}



//Rotates away from whiteboard, moves to point directly, then rotates toward whiteboard
bool Rhino::RotateMove(Coord Target)
{
	bool Success = true;
	if (CurrentPoint() == Target)
		return false;
	RotateOut();
	Success = MoveToPoint(Target);
	RotateIn();
	return Success;
}



//draws circle with given center and radius
bool Rhino::DrawCircle(Coord Center, double radius)
{
	bool Success;
	Coord Start = Center;
	Coord End = Center;
	Start.x -= radius;
	End.x += radius;

	Success = RotateMove(Start);
	if (!Success)
		return false;
	Success = MoveArc(End, 1.0, false);
	if (!Success)
		return false;
	return MoveArc(Start, 1.0, false);
}



//draws the specified character
//pass in the character, the lower left starting point, and how large you want the number to be
//some letters don't take up the full space
//letters are centered if smaller rather than touching the starting point
//currently only accepts capital letters
//to do: add lowercase letters and numbers
bool Rhino::DrawChar(char Character, Coord Start, double Scale)
{
	switch (Character)
	{
	//uppercase letters
	case('A'):
		Move(0, Start);
		Move(1, { Start.x + Scale * 0.5, Start.y + Scale });
		Move(1, { Start.x + Scale, Start.y });
		Move(0, { (Start.x + Start.x + Scale * 0.5) * 0.5, (Start.y + Start.y + Scale) * 0.5 });
		Move(1, { (2.0 * Start.x + 0.5 * Scale) * 0.5 + 0.5 * Scale, (2.0 * Start.y + Scale) * 0.5 });
		break;
	case('B'):
		Move(0, { Start.x + 0.25 * Scale, Start.y });
		Move(1, { Start.x + 0.25 * Scale, Start.y + Scale });
		Move(2, { Start.x + 0.25 * Scale, Start.y + 0.5 * Scale }, 1.8);
		Move(2, { Start.x + 0.25 * Scale, Start.y }, 2.0);
		break;
	case('C'):
		Move(0, { Start.x + 0.7 * Scale, Start.y + Scale });
		Move(3, { Start.x + 0.7 * Scale, Start.y }, 0.8);
		break;
	case('D'):
		Move(0, { Start.x + 0.05 * Scale, Start.y});
		Move(1, { Start.x + 0.05 * Scale, Start.y + Scale });
		Move(2, { Start.x + 0.05 * Scale, Start.y }, 1.8);
		break;
	case('E'):
		Move(0, { Start.x + 0.2 * Scale, Start.y });
		Move(1, { Start.x + 0.2 * Scale, Start.y + Scale });
		Move(1, { Start.x + 0.2 * Scale + 0.6 * Scale, Start.y + Scale });
		Move(0, { Start.x + 0.2 * Scale, Start.y + 0.5 * Scale });
		Move(1, { Start.x + 0.2 * Scale + 0.6 * Scale, Start.y + 0.5 * Scale });
		Move(0, { Start.x + 0.2 * Scale, Start.y });
		Move(1, { Start.x + 0.2 * Scale + 0.6 * Scale, Start.y });
		break;
	case('F'):
		Move(0, { Start.x + 0.2 * Scale, Start.y });
		Move(1, { Start.x + 0.2 * Scale, Start.y + Scale });
		Move(1, { Start.x + 0.2 * Scale + 0.6 * Scale, Start.y + Scale });
		Move(0, { Start.x + 0.2 * Scale, Start.y + 0.5 * Scale });
		Move(1, { Start.x + 0.2 * Scale + 0.6 * Scale, Start.y + 0.5 * Scale });
		break;
	case('G'):				//not perfect but close enough, partial arc function may help here
		Move(0, { Start.x + 0.5 * Scale, Start.y + Scale });
		Move(3, { Start.x + 0.5 * Scale, Start.y }, 1.0);
		Move(3, { Start.x + Scale, Start.y + 0.5 * Scale }, 0.3535);
		Move(1, { Start.x + 0.5 * Scale, Start.y + 0.5 * Scale });
		break;
	case('H'):
		Move(0, { Start.x + 0.2 * Scale, Start.y });
		Move(1, { Start.x + 0.2 * Scale, Start.y + Scale });
		Move(0, { Start.x + 0.2 * Scale, Start.y + 0.5 * Scale });
		Move(1, { Start.x + 0.8 * Scale, Start.y + 0.5 * Scale });
		Move(0, { Start.x + 0.8 * Scale, Start.y + Scale });
		Move(1, { Start.x + 0.8 * Scale, Start.y });
		break;
	case('I'):
		Move(0, { Start.x + 0.2 * Scale, Start.y });
		Move(1, { Start.x + 0.8 * Scale, Start.y });
		Move(0, { Start.x + 0.5 * Scale, Start.y });
		Move(1, { Start.x + 0.5 * Scale, Start.y + Scale });
		Move(0, { Start.x + 0.2 * Scale, Start.y + Scale });
		Move(1, { Start.x + 0.8 * Scale, Start.y + Scale });
		break;
	case('J'):
		Move(0, { Start.x + 0.2 * Scale, Start.y + Scale });
		Move(1, { Start.x + 0.8 * Scale, Start.y + Scale });
		Move(0, { Start.x + 0.5 * Scale, Start.y + Scale });
		Move(1, { Start.x + 0.5 * Scale, Start.y + 0.2 * Scale });
		Move(2, { Start.x + 0.2 * Scale, Start.y + 0.2 * Scale }, 1.3);
		break;
	case('K'):
		Move(0, { Start.x + 0.2 * Scale, Start.y });
		Move(1, { Start.x + 0.2 * Scale, Start.y + Scale });
		Move(0, { Start.x + 0.2 * Scale, Start.y + 0.5 * Scale });
		Move(1, { Start.x + 0.8 * Scale, Start.y + Scale });
		Move(0, { Start.x + 0.2 * Scale, Start.y + 0.5 * Scale });
		Move(1, { Start.x + 0.8 * Scale, Start.y });
		break;
	case('L'):
		Move(0, { Start.x + 0.2 * Scale, Start.y + Scale });
		Move(1, { Start.x + 0.2 * Scale, Start.y });
		Move(1, { Start.x + 0.8 * Scale, Start.y });
		break;
	case('M'):
		Move(0, Start);
		Move(1, { Start.x, Start.y + Scale });
		Move(1, { Start.x + 0.5 * Scale, Start.y + 0.5 * Scale });
		Move(1, { Start.x + Scale, Start.y + Scale });
		Move(1, { Start.x + Scale, Start.y });
		break;
	case('N'):
		Move(0, { Start.x + 0.2 * Scale, Start.y });
		Move(1, { Start.x + 0.2 * Scale, Start.y + Scale });
		Move(1, { Start.x + 0.8 * Scale, Start.y });
		Move(1, { Start.x + 0.8 * Scale, Start.y + Scale });
		break;
	case('O'):
		Move(0, { Start.x + 0.1 * Scale, Start.y + 0.5 * Scale });
		Move(2, { Start.x + 0.9 * Scale, Start.y + 0.5 * Scale }, 1.2);
		Move(2, { Start.x + 0.1 * Scale, Start.y + 0.5 * Scale }, 1.2);
		break;
	case('P'):
		Move(0, { Start.x + 0.25 * Scale, Start.y });
		Move(1, { Start.x + 0.25 * Scale, Start.y + Scale });
		Move(2, { Start.x + 0.25 * Scale, Start.y + 0.5 * Scale }, 2.0);
		break;
	case('Q'):
		Move(0, { Start.x + 0.1 * Scale, Start.y + 0.5 * Scale });
		Move(2, { Start.x + 0.9 * Scale, Start.y + 0.5 * Scale }, 1.2);
		Move(2, { Start.x + 0.1 * Scale, Start.y + 0.5 * Scale }, 1.2);
		Move(0, { Start.x + 0.6 * Scale, Start.y + 0.4 * Scale });
		Move(1, { Start.x + Scale, Start.y });
		break;
	case('R'):
		Move(0, { Start.x + 0.25 * Scale, Start.y });
		Move(1, { Start.x + 0.25 * Scale, Start.y + Scale });
		Move(2, { Start.x + 0.25 * Scale, Start.y + 0.5 * Scale }, 2.0);
		Move(1, { Start.x + 0.75 * Scale, Start.y });
		break;
	case('S'):				//not perfect but close enough, partial arc function may help here
		Move(0, { Start.x + 0.75 * Scale, Start.y + 0.75 * Scale });
		Move(3, { Start.x + 0.25 * Scale, Start.y + 0.75 * Scale }, 1.0);
		Move(3, { Start.x + 0.5 * Scale, Start.y + 0.5 * Scale }, 0.3535);
		Move(2, { Start.x + 0.75 * Scale, Start.y + 0.25 * Scale }, 0.3535);
		Move(2, { Start.x + 0.25 * Scale, Start.y + 0.25 * Scale }, 1.0);
		break;
	case('T'):
		Move(0, { Start.x + 0.2 * Scale, Start.y + Scale });
		Move(1, { Start.x + 0.8 * Scale, Start.y + Scale });
		Move(0, { Start.x + 0.5 * Scale, Start.y + Scale });
		Move(1, { Start.x + 0.5 * Scale, Start.y });
		break;
	case('U'):
		Move(0, { Start.x + 0.2 * Scale, Start.y + Scale });
		Move(1, { Start.x + 0.2 * Scale, Start.y + 0.25 * Scale });
		Move(3, { Start.x + 0.8 * Scale, Start.y + 0.25 * Scale }, 0.8);
		Move(0, { Start.x + 0.8 * Scale, Start.y + Scale });
		Move(1, { Start.x + 0.8 * Scale, Start.y });
		break;
	case('V'):
		Move(0, { Start.x + 0.2 * Scale, Start.y + Scale });
		Move(1, { Start.x + 0.5 * Scale, Start.y });
		Move(1, { Start.x + 0.8 * Scale, Start.y + Scale });
		break;
	case('W'):
		Move(0, { Start.x, Start.y + Scale });
		Move(1, { Start.x + 0.25 * Scale, Start.y });
		Move(1, { Start.x + 0.5 * Scale, Start.y + 0.5 * Scale });
		Move(1, { Start.x + 0.75 * Scale, Start.y });
		Move(1, { Start.x + Scale, Start.y + Scale });
		break;
	case('X'):
		Move(0, { Start.x + 0.2 * Scale, Start.y + Scale });
		Move(1, { Start.x + 0.8 * Scale, Start.y });
		Move(0, { Start.x + 0.2 * Scale, Start.y  });
		Move(1, { Start.x + 0.8 * Scale, Start.y + Scale });
		break;
	case('Y'):
		Move(0, { Start.x + 0.2 * Scale, Start.y + Scale });
		Move(1, { Start.x + 0.5 * Scale, Start.y + 0.6 * Scale });
		Move(1, { Start.x + 0.8 * Scale, Start.y + Scale });
		Move(0, { Start.x + 0.5 * Scale, Start.y + 0.6 * Scale });
		Move(1, { Start.x + 0.5 * Scale, Start.y });
		break;
	case('Z'):
		Move(0, { Start.x + 0.2 * Scale, Start.y + Scale });
		Move(1, { Start.x + 0.8 * Scale, Start.y + Scale });
		Move(1, { Start.x + 0.2 * Scale, Start.y });
		Move(1, { Start.x + 0.8 * Scale, Start.y });
		break;
	default:
		return false;
	}
	return true;
}



//pass in the name of a file containing Gcode
//reads the file and moves accordingly
bool Rhino::MoveGcodeH(string FileName)
{
	Parse File(FileName);								//open file
	int Mode = 0;										//temp variables containing parameters for Move command
	Coord Target = { 9.0, 9.0 };						//	initalize to dummy data, overwrites during GetLine
	double Height = 1.0;

	if (!File.IsOpen())									//check if file is open, return false if not
		return false;

	while (File.GetLine(Mode, Target, Height))			//read file line by line and send to Move command
		Move(Mode, Target, Height);						//to do: validate data
	return true;
}



//manually set start point if not starting at (9,9)
void Rhino::SetStartPoint(Coord Start)
{
	Home = Start;
	RhinoMath.SetCoord(Start);
	ServoE = RhinoMath.GetTheta1() / 0.12;
	ServoD = RhinoMath.GetTheta2() / 0.12;
}