#include "Hangman.h"

//**********************************************************************************************//
//									  Hangman member functions									//
//**********************************************************************************************//

//constructor, connects ComPort of Rhino class
Hangman::Hangman(char* CPort)
{
	TserialConnection.connect(CPort, 9600, spEVEN);					//connects to serial port
}



//destructor
Hangman::~Hangman()
{
	TserialConnection.disconnect();									//disconnects from serial port
	if (AnswerPtr != 0)												//free memory used by bool array
	{
		delete [] AnswerPtr;
		AnswerPtr = 0;
	}
}




//draws the gallows at the start of the game
void Hangman::DrawGallows()
{
	Move(0, { RefPoint.x, RefPoint.y + 1.5 * Scale });
	Move(1, { RefPoint.x, RefPoint.y + 1.5 * Scale + 0.25 * Scale });
	Move(1, { RefPoint.x + Scale, RefPoint.y + 1.5 * Scale + 0.25 * Scale });
	Move(1, { RefPoint.x + Scale, RefPoint.y + 1.5 * Scale });
	Move(0, { RefPoint.x + 0.5 * Scale, RefPoint.y + 1.5 * Scale + 0.25 * Scale });
	Move(1, { RefPoint.x + 0.5 * Scale, RefPoint.y + 1.5 * Scale + 2.0 * Scale });
	Move(1, { RefPoint.x + 1.5 * Scale, RefPoint.y + 1.5 * Scale + 2.0 * Scale });
	Move(1, { RefPoint.x + 1.5 * Scale, RefPoint.y + 1.5 * Scale + 1.75 * Scale });
	GallowsDrawn = true;
}



//increments the error counter and draws the next body part
void Hangman::IncError()
{
	switch (++Error)
	{
	case(1):			//head
		Move(0, { RefPoint.x + 1.5 * Scale, RefPoint.y + 1.5 * Scale + 1.75 * Scale });
		Move(2, { RefPoint.x + 1.5 * Scale, RefPoint.y + (1.5 + 1.5) * Scale }, 1.0);
		Move(2, { RefPoint.x + 1.5 * Scale, RefPoint.y + 1.5 * Scale + 1.75 * Scale }, 1.0);
		break;
	case(2):			//body
		Move(0, { RefPoint.x + 1.5 * Scale, RefPoint.y + (1.5 + 1.5) * Scale });
		Move(1, {RefPoint.x + 1.5 * Scale, RefPoint.y + (1.5 + 1.0) * Scale });
		break;
	case(3):			//left arm
		Move(0, { RefPoint.x + 1.5 * Scale, RefPoint.y + (1.5 + 1.375) * Scale });
		Move(1, { RefPoint.x + (1.5 - 0.25) * Scale, RefPoint.y + (1.5 + 1.125) * Scale });
		break;
	case(4):			//right arm
		Move(0, { RefPoint.x + 1.5 * Scale, RefPoint.y + (1.5 + 1.375) * Scale });
		Move(1, { RefPoint.x + (1.5 + 0.25) * Scale, RefPoint.y + (1.5 + 1.125) * Scale });
		break;
	case(5):			//left leg
		Move(0, {RefPoint.x + 1.5 * Scale, RefPoint.y + (1.5 + 1.0) * Scale });
		Move(1, {RefPoint.x + (1.5 - 0.25) * Scale, RefPoint.y + (1.5 + 0.5) * Scale});
		break;
	case(6):			//right leg, game loss
		Move(0, {RefPoint.x + 1.5 * Scale, RefPoint.y + (1.5 + 1.0) * Scale });
		Move(1, {RefPoint.x + (1.5 + 0.25) * Scale, RefPoint.y + (1.5 + 0.5) * Scale});
		Status = -1;
		break;
	}
	//to do: add easy mode?
}



//draws an incorrect error to the right of the gallows
void Hangman::DrawBadLetter(char Letter)
{
	DrawChar(Letter, { RefPoint.x + 2.5 * Scale + (Error - 1) * 0.9 * Scale, RefPoint.y + 2.5 * Scale }, 1.0);
}



//run to start the game
//chooses a random word from "WordList2.txt"
//draws the starting parts of the game
void Hangman::Start()
{
	ifstream InputFile;								//instance of ifstream object
	int RandNum;									//random number for determining word

	//reset data between games
	if (Status == 0)								//if already in game, exit function
		return;
	Status = 0;										//reset Status
	Error = 0;										//reset Error counter
	if (AnswerPtr != 0)								//free memory and reset bool array
	{
		delete [] AnswerPtr;
		AnswerPtr = 0;
	}

	//open file and set Word
	InputFile.open(WordFileName);					//open file containing list of words
	srand(time(0));									//randomizes based on current time
	RandNum = rand() % MaxWords;					//random number between 0 and MaxWords - 1
	for (int i = 0; i < RandNum; i++)				//move to line number, a faster method may exist
	{
		while (InputFile.get() != '\n');			//move to next line
	}
	Word.clear(); 									//clear Word
	while (InputFile.peek() != '\n' && InputFile.peek() != EOF)					//gets the next line and assigns to Word
	{
		Word.push_back(InputFile.get());
	}
	InputFile.close();								//close file

	//create bool array to check answers
	AnswerPtr = new bool[Word.length()];			//create new bool array and save location to AnswerPtr
	for (int i = 0; i < Word.length(); i++)			//initalize to false
		AnswerPtr[i] = false;

	//draw gallows
	if (!GallowsDrawn)
		DrawGallows();

	//draw lines under each letter
	Move(0, { RefPoint.x, RefPoint.y - 0.2 * Scale });							//move to the first point
	for (int i = 0; i < Word.length(); i++)
	{
		Move(1, { CurrentPoint().x + Scale, CurrentPoint().y});					//draw a line
		if (i < Word.length() - 1)
			Move(0, { CurrentPoint().x + 0.5 * Scale, CurrentPoint().y});		//move to the next point
	}

	RotateHome();									//moves arm out of the way
}



//run to start the game
//uses the input string as the word to be guessed
//draws the starting parts of the game
void Hangman::Start(string WordIn)
{
	if (Status == 0)								//if already in game, exit function
		return;
	Status = 0;										//reset Status
	Error = 0;										//reset Error counter
	if (AnswerPtr != 0)								//free memory and reset bool array
	{
		delete [] AnswerPtr;
		AnswerPtr = 0;
	}

	//validates input string
	if (WordIn.length() > 6 || WordIn.length() == 0)	//6 characters or less and not empty
		return;
	for (int i = 0; i < WordIn.length(); i++)		//letters only
		if (!isalpha(WordIn[i]))
			return;
	Word = WordIn;									//assign Word

	//create bool array to check answers
	AnswerPtr = new bool[Word.length()];			//create new bool array and save location to AnswerPtr
	for (int i = 0; i < Word.length(); i++)			//initalize to false
		AnswerPtr[i] = false;

	//draw gallows
	if (!GallowsDrawn)
		DrawGallows();

	//draw lines under each letter
	Move(0, { RefPoint.x, RefPoint.y - 0.2 * Scale });							//move to the first point
	for (int i = 0; i < Word.length(); i++)
	{
		Move(1, { CurrentPoint().x + Scale, CurrentPoint().y});					//draw a line
		if (i < Word.length() - 1)
			Move(0, { CurrentPoint().x + 0.5 * Scale, CurrentPoint().y});		//move to the next point
	}

	RotateHome();									//moves arm out of the way
}



//Guess a letter in the word
//takes in a character and compares against word
//updates data and draws letters/body parts
//can update status if a win-condition or lose-condition is reached
//returns true if found, false if not
bool Hangman::Guess(char Letter)
{
	bool Found = false;								//holds whether letter was found or not
	Letter = toupper(Letter);

	if (Status != 0 || !isalpha(Letter))			//if not in active game or if input isn't a letter, exit function
		return false;

	//loop through Word, checking each character
	for (int i = 0; i < Word.length(); i++)
	{
		if (toupper(Word[i]) == Letter)				//make sure case is same before comparing
		{
			AnswerPtr[i] = true;					//update bool array
			Found = true;							//at least one entry found
			DrawChar(Letter, {RefPoint.x + i * Scale * 1.5, RefPoint.y}, Scale);		//draws letter
		}
	}

	if (!Found)										//if not found
	{
		IncError();									//increment error counter
		DrawBadLetter(Letter);
	}
	else											//else check if word is complete
	{
		for (int i = 0; i < Word.length(); i++)		//search answer array for false
		{
			if (!AnswerPtr[i])						//if false found, exit function
			{
				RotateHome();
				return true;
			}
		}
		Status = 1;									//if false not found, word must be complete, set Status = 1
	}
	RotateHome();									//moves arm out of the way

	//to do: handle guessing the same latter multiple times

	return Found;
}



//gets the word if not in a game
string Hangman::GetWord()
{
	if (Status != 0)
		return Word;
	else
		return "";
}