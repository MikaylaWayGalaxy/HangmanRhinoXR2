//Mikayla Fulmer 2-16-24
//Ported to VS22 3-14
//last edited 4-19-24
#include <iostream>
#include <string>
#include <conio.h>                          //for _getch()
#include "Rhino.h"
#include "MasterHeader.h"
#include "Hangman.h"
using namespace std;

int main()
{
    char ComPort[5] = "COM1";               //communication part used to connect to Rhino robot, use COM1 for school computer, use device manager to find otherwise
    //Rhino Robot(ComPort);                   //instance of Rhino class
    //Coord TestPoint;                        //2d coordinate point to move to during program execution
    //Robot.SetDelay(5);                    //delay between servo movements, currently set to 1/5 of a second
    //Robot.SetInchDiv(10);                   //number of moves per inch, sets resolution

    //cout << "X     Y\n";

    char Letter;
    Hangman Game(ComPort);
    Game.SetDelay(125);
    Game.SetInchDiv(10);
    Game.Start();

    //system("cls");
    cout << "Welcome to Hangman!\n" 
         << "Please enter letters using the keyboard\n"
         << "and the Rhino robot will do the rest.\n";
    while (Game.GetStatus() == 0)                               //guess letters until the game is a win or a loss
    {
        Letter = _getch();                  //compiler doesn't like getch(), use _getch() instead
        cout << Letter << endl;
        //Game.Guess(_getch());
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

    //uncomment the object you wish to draw

    //testing MovePartialArc
    //Robot.MovePartialArc({ 10.0, 9.0 }, 3.0*PI/4.0, 1.0, false);
    //Robot.MovePartialArc({ 10.0, 9.0 }, 3.0*PI/4.0, 2.0, true);
    //Robot.MovePartialArc({ 10.0, 9.0 }, 5*PI/4.0, 2.0, false);
    //Robot.MovePartialArc({ 10.0, 9.0 }, PI, 2.0, true);
    //Robot.RotateMove({ -1.0,0.0 });
    //Robot.MovePartialArc({ 0.0, 0.0 }, 5.0*PI/4.0, 2.0, true);
    //Robot.MoveArc({ 8.0, 10.0 }, { 8.0, 9.0 }, true);

    //test reading Gcode from file
    //string File = "TestGcode.txt";
    //Robot.MoveGcodeH(File);

    //cat face using GCode
    //string File = "CatFace.txt";
    //Robot.MoveGCodeH(File);

    //draws a cat face
    /*
    Robot.Move(0, { 3.0, 5.0 });                 //head and ears
    Robot.Move(3, { 7.0, 5.0 }, 1.0);
    Robot.Move(3, { 6.5, 6.0 }, 0.3);
    Robot.Move(0, { 6.5, 6.0 });
    Robot.Move(3, { 7.0, 7.0 }, 0.3);
    Robot.Move(3, { 6.0, 6.5 }, 0.3);
    Robot.Move(3, { 4.0, 6.5 }, 0.5);
    Robot.Move(3, { 3.0, 7.0 }, 0.3);
    Robot.Move(3, { 3.5, 6.0 }, 0.3);
    Robot.Move(3, { 3.0, 5.0 }, 0.3);
    Robot.Move(0, { 4.25, 5.5 });                //face
    Robot.Move(0, { 5.75, 5.5 });
    Robot.DrawCircle({ 5.0, 5.0 }, 0.1);
    Robot.Move(0, { 5.0, 4.9 });
    Robot.Move(1, { 5.0, 4.5 });
    Robot.Move(2, { 4.4, 4.5 }, 1.0);
    Robot.Move(0, { 5.0, 4.5 });
    Robot.Move(3, { 5.6, 4.5 }, 1.0);
    Robot.Move(0, { 3.5, 5.25 });                //whiskers
    Robot.Move(1, { 2.5, 5.5 });
    Robot.Move(0, { 3.5, 5.0 });
    Robot.Move(1, { 2.25, 5.0 });
    Robot.Move(0, { 3.5, 4.75 });
    Robot.Move(1, { 2.5, 4.5 });
    Robot.Move(0, { 6.5, 5.25 });
    Robot.Move(1, { 7.5, 5.5 });
    Robot.Move(0, { 6.5, 5.0 });
    Robot.Move(1, { 7.75, 5.0 });
    Robot.Move(0, { 6.5, 4.75 });
    Robot.Move(1, { 7.5, 4.5 });
    */

    //writes MIKAYLA with DrawChar command
    /*
    Robot.DrawChar('M', { 3.0, 5.0 }, 0.8);
    Robot.DrawChar('I', { 4.0 + 0.2, 5.0 }, 0.8);
    Robot.DrawChar('K', { 5.0 + 0.2 * 2, 5.0 }, 0.8);
    Robot.DrawChar('A', { 6.0 + 0.2 * 3, 5.0 }, 0.8);
    Robot.DrawChar('Y', { 7.0 + 0.2 * 4, 5.0 }, 0.8);
    Robot.DrawChar('L', { 8.0 + 0.2 * 5, 5.0 }, 0.8);
    Robot.DrawChar('A', { 9.0 + 0.2 * 6, 5.0 }, 0.8);
    */

    //writes Mikayla
    /*
    //max left distance: x=3
    //max right distance x=12
    //min height: y=5
    //mid height: y=5.5
    //max height: y=6
    //seven letters
    //six spaces between letters
    //letter dimesions: 1x1
    //spacing distance: 0.33
    Robot.Move(0, { 3.0, 5.0 });                                   //M
    Robot.Move(1, { 3.0, 6.0 });
    Robot.Move(1, { 3.0 + 0.5, 5.5 });
    Robot.Move(1, { 3.0 + 1.0, 6.0 });
    Robot.Move(1, { 3.0 + 1.0, 5.0 });
    Robot.Move(0, { 4.0 + 0.33 + 0.5, 5.0 });                      //i
    Robot.Move(1, { 4.0 + 0.33 + 0.5, 5.5 });
    Robot.Move(0, { 4.0 + 0.33 + 0.5, 6.0 });
    Robot.Move(0, { 5.0 + 2 * 0.33, 6.0 });                        //k
    Robot.Move(1, { 5.0 + 2 * 0.33, 5.0 });
    Robot.Move(0, { 5.0 + 2 * 0.33 + 1.0, 5.0 });
    Robot.Move(1, { 5.0 + 2 * 0.33, 5.33 });
    Robot.Move(1, { 5.0 + 2 * 0.33 + 1.0, 5.67 });
    Robot.Move(0, { 6.0 + 3 * 0.33 + 0.5, 5.5 });                  //a
    Robot.Move(2, { 6.0 + 3 * 0.33 + 0.5, 5.0 }, 0.8);
    Robot.Move(2, { 6.0 + 3 * 0.33 + 0.5, 5.5 }, 0.8);
    Robot.Move(0, { 6.0 + 3 * 0.33 + 0.7, 5.5 });
    Robot.Move(1, { 6.0 + 3 * 0.33 + 0.7, 5.0 });
    Robot.Move(0, { 7.0 + 4 * 0.33, 5.5 });                        //y
    Robot.Move(1, { 7.0 + 4 * 0.33 + 0.5, 5.0 });
    Robot.Move(0, { 7.0 + 4 * 0.33 + 1.0, 5.5 });
    Robot.Move(1, { 7.0 + 4 * 0.33, 4.5 });
    Robot.Move(0, { 8.0 + 5 * 0.33 + 0.5, 5.0 });                  //l
    Robot.Move(1, { 8.0 + 5 * 0.33 + 0.5, 6.0 });
    Robot.Move(0, { 9.0 + 6 * 0.33 + 0.5, 5.5 });                  //a
    Robot.Move(2, { 9.0 + 6 * 0.33 + 0.5, 5.0 }, 0.8);
    Robot.Move(2, { 9.0 + 6 * 0.33 + 0.5, 5.5 }, 0.8);
    Robot.Move(0, { 9.0 + 6 * 0.33 + 0.7, 5.5 });
    Robot.Move(1, { 9.0 + 6 * 0.33 + 0.7, 5.0 });
    */

    //draws a heart (cardioid)
    /*
    TestPoint = { 3.5, 7.0 };
    Robot.RotateMove(TestPoint);
    TestPoint = { 6.0, 6.5 };
    Robot.MoveArc(TestPoint, 1.2, false);
    TestPoint = { 8.5, 7.0 };
    Robot.MoveArc(TestPoint, 1.2, false);
    TestPoint = { 3.5, 7.0 };
    Robot.MoveArc(TestPoint, 1.5, false);
    */

    //draws a smiley face
    /*
    TestPoint = { 6.0, 6.0 };
    Robot.DrawCircle(TestPoint, 3.0);         //start = (3,6), end = (9,6)
    TestPoint = { 4.0, 5.5 };                 //start of mouth
    Robot.RotateMove(TestPoint);
    TestPoint = { 8.0, 5.5 };                 //end of mouth
    Robot.MoveArc(TestPoint, 0.8, true);
    TestPoint = { 4.75, 7.0 };                //eyes
    Robot.RotateMove(TestPoint);
    TestPoint = { 7.25, 7.0 };
    Robot.RotateMove(TestPoint);
    */

    //draws HI
    /*
    TestPoint = { 6.0, 6.0 };               //disconnent pen and move to (6,6)
    Robot.RotateMove(TestPoint);
    TestPoint = { 6.0, 8.0 };               //draw H
    Robot.MoveLine(TestPoint);
    TestPoint = { 6.0, 7.0 };
    Robot.MoveLine(TestPoint);
    TestPoint = { 7.0, 7.0 };
    Robot.MoveLine(TestPoint);
    TestPoint = { 7.0, 6.0 };
    Robot.MoveLine(TestPoint);
    TestPoint = { 7.0, 8.0 };
    Robot.MoveLine(TestPoint);
    TestPoint = { 8.0, 8.0 };               //disconnect pen and draw I
    Robot.RotateMove(TestPoint);
    TestPoint = { 8.0, 6.0 };
    Robot.MoveLine(TestPoint);
    */


    //Robot.RotateHome();                   //moves back to home
    return 0;
}