//**********************************************************************************************//
//											 Hangman											//
// created by Mikayla Fulmer																	//
// last edited 5-14-24																			//
//																								//
// Plays hangman using a robot and a whiteboard. Designed to be used with a Rhino XR-2 robot	//
// arm. Set up robot so that it is facing right and holding a pen that is in contact with a		//
// whiteboard. Robot is very sensitive to starting position, try to make sure the arms are		//
// as straight and orthogonal as possible. Check comport using device manager and modify		//
// ComPort variable in main to match.															//
//																								//
//**********************************************************************************************//
#include <iostream>
#include <string>
#include <conio.h>
#include "Rhino.h"
#include "MasterHeader.h"
#include "Hangman.h"
using namespace std;

int main()
{
char ComPort[5] = "COM1";               //communication part used to connect to Rhino robot, use COM1 for school computer, use device manager to find otherwise
Hangman Game(ComPort);                  //instance of Hangman class, connects to com port
char Letter;                            //holds letter input by user to guess the word

cout << "Please wait while the robot draws the board.\n";

Game.SetDelay(200);                     //sets delay between indiviual moves, 1/5 of a second
Game.SetInchDiv(10);                    //sets resolution of drawing, 10 moves per inch
Game.SetScale(1.5);						//sets scale of each letter to 1.5 inches
Game.ToggleCout();						//disables printing points
Game.Start();                           //starts game using a random word

system("cls");
cout << "Welcome to Hangman!\n"
<< "Please enter letters using the keyboard\n"
<< "and the Rhino robot will do the rest.\n";
while (Game.GetStatus() == 0)                               //guess letters until the game is a win or a loss
{
Letter = _getch();						 //compiler doesn't like getch(), use _getch() instead
cout << Letter << endl;
Game.Guess(Letter);
}
if (Game.GetStatus() > 0)
{
cout << "\nCongratulations!\n"
<< "You correctly guessed the word.\n";
}
else
{
cout << "\nAw, better luck next time.\n"
<< "The word was: " << Game.GetWord() << ".\n";
}

Game.RotateHome();						//moves back to home position for next application
return 0;
}