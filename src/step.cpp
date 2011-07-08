#include "piece.h"
#include "error.h"
#include "square.h"
#include "step.h"
#include <assert.h>
#include <ctype.h>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

//////////////////////////////////////////////////////////////////////////////
//initialize a move given a string that describes the move. Throws an Error
//if the string is an invalid format, or the step is obviously illegal even
//without any knowledge about other pieces (e.g. a piece is moving off the
//board or a piece is being captured on a non-capture square
//////////////////////////////////////////////////////////////////////////////
void Step :: fromString(string str)
{
    //read the piece type and color
    unsigned char piece = pieceFromChar(str[0]);
    
    //read the position moving from
    stringstream squareStringStream;
    squareStringStream << str[1] << str[2];
    unsigned int from = squareFromString(squareStringStream.str());

    //read the last character for move type
    int to = from;
    switch (str[3])
    {
        case 'n':
        {
            to = from - 8;            
        }break;
        case 's':
        {
            to = from + 8;
        }break;
        case 'w':
        {
            to = from - 1;
        }break;
        case 'e':
        {
            to = from + 1;
        }break;
        case 'x':
        {
            to = -1;
        }break;  
    }

    //generate the step
    if (to == -1)
        genCapture(piece,from);
    else
        genMove(piece,from,to);
} 

//////////////////////////////////////////////////////////////////////////////
//returns the string representation of this step
//////////////////////////////////////////////////////////////////////////////
string Step :: toString()
{

    char moveType = '?'; //If this character remains unchanged, something is
                         //wrong

    unsigned char to = getTo();
    unsigned char from = getFrom();

    if (to - from == 1)
        moveType = 'e';

    if (to - from == -1)
        moveType = 'w';

    if (to - from == -8)
        moveType = 'n';

    if (to - from == 8)
        moveType = 's';

    if (isCapture())
        moveType = 'x';

    stringstream stepStream;
    stepStream << charFromPiece(getPiece()) << stringFromSquare(from)
               << moveType;
    
    return stepStream.str();
}

//////////////////////////////////////////////////////////////////////////////
//Constructor. Sets the combo to be the empty combo
//////////////////////////////////////////////////////////////////////////////
StepCombo :: StepCombo()
{
    reset();
}

//////////////////////////////////////////////////////////////////////////////
//Deconstructor. Does nothing.
//////////////////////////////////////////////////////////////////////////////
StepCombo :: ~StepCombo()
{

}

//////////////////////////////////////////////////////////////////////////////
//returns a string representing this combo
//////////////////////////////////////////////////////////////////////////////
string StepCombo :: toString()
{
    if (numSteps == 0)
        return string("");

    stringstream stream;
    for (unsigned int i = 0; i < numSteps - 1; ++i)
    {
        stream << steps[i].toString() << " ";
    }

    stream << steps[numSteps - 1].toString();
    return stream.str();
}

//////////////////////////////////////////////////////////////////////////////
//sets this step combo using the given string, which should be a string
//of whitespace separated steps
//////////////////////////////////////////////////////////////////////////////
void StepCombo :: fromString(string s)
{
    stringstream stepStream;
    stepStream.str(s);

    reset();
    while (stepStream.good())
    {
        string word;
        stepStream >> word;

        if (stepStream.fail())
            break;

        Step step;
        step.fromString(word);
        addStep(step);
    }
}

//////////////////////////////////////////////////////////////////////////////
//adds a step to this combo.
//////////////////////////////////////////////////////////////////////////////
void StepCombo :: addStep(Step step)
{   
    steps[numSteps++] = step;

    if (!step.isCapture())
        ++ stepCost;
}

//////////////////////////////////////////////////////////////////////////////
//adds a combo of steps to this combo
//////////////////////////////////////////////////////////////////////////////
void StepCombo :: addCombo(StepCombo& combo)
{
    for (unsigned int i = 0; i < combo.numSteps; i++)
    {
        steps[numSteps + i] = combo.steps[i];
    }
    numSteps += combo.numSteps;
    stepCost += combo.stepCost;
}

//////////////////////////////////////////////////////////////////////////////
//Return the source square that the first moving piece is moving from. Returns
//ILLEGAL_SQUARE if there is no such square
//////////////////////////////////////////////////////////////////////////////
unsigned char StepCombo :: getFrom1()
{
    if (stepCost < 1)
        return ILLEGAL_SQUARE;

    return steps[0].getFrom();
}

//////////////////////////////////////////////////////////////////////////////
//Return the destination square that the first piece is moving to. Returns
//ILLEGAL_SQUARE if there is no such square
//////////////////////////////////////////////////////////////////////////////
unsigned char StepCombo :: getTo1()
{
    if (stepCost < 1)
        return ILLEGAL_SQUARE;
    
    return steps[0].getTo();
}

//////////////////////////////////////////////////////////////////////////////
//Return the source square that the second piece is moving from. Note that
//there is no getTo2() function as it is assumed that the second piece is
//moving to the square that the first piece moved from
//////////////////////////////////////////////////////////////////////////////
unsigned char StepCombo :: getFrom2()
{
    if (stepCost < 2)
        return ILLEGAL_SQUARE;

    //note that second step can denote a capture, so the actual
    //second moving step can be afterward
    unsigned int secondMoveIndex = 1;

    if (steps[1].isCapture())
        secondMoveIndex = 2;

    return steps[secondMoveIndex].getFrom();
}

//////////////////////////////////////////////////////////////////////////////
//Returns the first moving piece. Returns NO_PIECE if there isn't any
//////////////////////////////////////////////////////////////////////////////
unsigned char StepCombo :: getPiece1()
{
    if (stepCost < 1)
        return NO_PIECE;

    return steps[0].getPiece();
}

//////////////////////////////////////////////////////////////////////////////
//Returns the second moving piece. Returns NO_PIECE if there isn't any
//////////////////////////////////////////////////////////////////////////////
unsigned char StepCombo :: getPiece2()
{
    if (stepCost < 2)
        return NO_PIECE;

    //note that second step can denote a capture, so the actual
    //second moving step can be afterward
    unsigned int secondMoveIndex = 1;

    if (steps[1].isCapture())
        secondMoveIndex = 2;

    return steps[secondMoveIndex].getPiece();
}

//////////////////////////////////////////////////////////////////////////////
//Returns true iff the first moving piece (if any) is captured
//////////////////////////////////////////////////////////////////////////////
bool StepCombo :: piece1IsCaptured()
{
    if (numSteps < 2)
        return false;

    return steps[1].isCapture()  
       && (steps[0].getPiece() == steps[1].getPiece());
}

//////////////////////////////////////////////////////////////////////////////
//Returns true iff the first moving piece (if any) is captured
//////////////////////////////////////////////////////////////////////////////
bool StepCombo :: piece2IsCaptured()
{
    if (stepCost < 2)
        return false;

    //note that second step can denote a capture, so the actual
    //second moving step can be afterward
    unsigned int secondMoveIndex = 1;

    if (steps[1].isCapture())
        secondMoveIndex = 2;

    if (numSteps < secondMoveIndex + 2)
        return false;

    return steps[secondMoveIndex+1].isCapture() && 
           (steps[secondMoveIndex].getPiece() 
            == steps[secondMoveIndex+1].getPiece());
}

//////////////////////////////////////////////////////////////////////////////
//resets this combo to a blank combo
//////////////////////////////////////////////////////////////////////////////
void StepCombo :: reset()
{
    numSteps = 0;
    stepCost = 0;
}
