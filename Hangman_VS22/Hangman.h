#ifndef HANGMAN_H
#define HANGMAN_H
#include <fstream>							//for reading from file
#include <cstdlib>							//for rng
#include <ctime>							//for rng
#include <cctype>							//for uppercase conversion and type checking
#include "Rhino.h"
#include "MasterHeader.h"

class Hangman : protected Rhino
{
private:
	//private member variables
	string Word = "ABCDEF";					//word or phrase to be guessed by user, max length is 6 if using 1.5 inch scale
	unsigned int Error = 0;					//counter for how many letters guessed wrong
	int Status = 2;							//status of game: 0 = still playing, 1 = won, -1 = lost, 2 = not in game yet
	bool* AnswerPtr = 0;					//pointer to a bool array that tracks letters correctly guessed
	Coord RefPoint = { 4.0, 4.0 };			//reference point used to orient other functions, lower-left of first letter
	double Scale = 1.5;						//scale in inchs of things to be drawn
	bool GallowsDrawn = false;				//if gallows already drawn, skip between games
	string WordFileName = "WordList2.txt";	//name of file containing words to pull from
	const unsigned int MaxWords = 100;		//number of possible words in word list file

	//private member functions
	void DrawGallows();						//draws the gallows at the start of the game, executed only in Start
	void IncError();						//increments Error and draws the next next body part
	void DrawBadLetter(char);				//draws an incorrect error to the right of the gallows

public:
	//constructor(s)/destructor
	Hangman(char* CPort);
	~Hangman();

	//public member functions
	void Start();
	void Start(string);
	bool Guess(char);
	int GetStatus() { return Status; }
	unsigned int GetError() { return Error; }
	string GetWord();
	void SetDelay(unsigned int Delay) { Rhino::SetDelay(Delay); }
	void SetInchDiv(unsigned int Div) { Rhino::SetInchDiv(Div); }
	void SetScale(double Scale) { this->Scale = abs(Scale); }
	void ToggleCout() { Rhino::ToggleCout(); }
	void SetRefPoint(Coord Point) { RefPoint = Point; }
	void RotateHome() { Rhino::RotateHome(); }

};

#endif