#include "board.h"
#include "piece.h"
#include "int64.h"
#include "stringsplit.h"
#include "square.h"

#include <vector>
#include <string>
#include <sstream>

using namespace std;

//////////////////////////////////////////////////////////////////////////////
//Basic constructor, basically initializes an empty board
//////////////////////////////////////////////////////////////////////////////
Board :: Board()
{
    newGame();
}

//////////////////////////////////////////////////////////////////////////////
//Deconstructor, for now it doesn't need to do anything
//////////////////////////////////////////////////////////////////////////////
Board :: ~Board()
{

}

//////////////////////////////////////////////////////////////////////////////
// Resets the board to a new game position
//////////////////////////////////////////////////////////////////////////////
void Board :: newGame()
{
    //zero out the bitboards, could use memset for this, but for now I'll
    //be more explicit
    for (int color = 0; color < MAX_COLORS; color++)
    {
        for (int piece = 0; piece < MAX_TYPES; piece++)
        {
            pieces[color][piece] = 0;
        }
    }

    sideToMove = GOLD;
    stepsLeft = 4;
    turnNumber = 1;
    isSetup = true;
}

//////////////////////////////////////////////////////////////////////////////
// Places pieces on the board according the string received, which is in the
// format of a move taken during the initialization phase of the game
//////////////////////////////////////////////////////////////////////////////
void Board :: doPlacementString(const string& pStr)
{
    vector<string> parts = splitByWhitespace(pStr);    

    for (unsigned int i = 0; i < parts.size(); i++)
    {
        string str = parts[i];

        //read the piece type and color
        unsigned char piece = pieceFromChar(str[0]);
    
        //read the position to place on
        stringstream squareStringStream;
        squareStringStream << str[1] << str[2];
        unsigned int on = squareFromString(squareStringStream.str());

        // Place the piece on the respective bitboard
        pieces[colorOfPiece(piece)][typeOfPiece(piece)] |= Int64FromIndex(on); 
    }
}

//////////////////////////////////////////////////////////////////////////////
// Takes an arbitrary move string and plays the results on the board as the
// current player's turn.
//////////////////////////////////////////////////////////////////////////////
void Board :: doMoveString(const string& mStr)
{
    // Check if it is the setup phase. If so, then a placement string
    // will have been placed    
    if (isSetup)
    {
        doPlacementString(mStr);
        changeTurn();

        if (sideToMove == GOLD) // Back to gold after init phase
        {
            isSetup = false; 
        } 
    }
    else
    {
        StepCombo combo;
        combo.fromString(mStr);
        doCombo(combo);
        changeTurn(); 
    }
} 

/////////////////////////////////////////////////////////////////////////////
// Performs a combo of steps. DOES increase the steps taken.
/////////////////////////////////////////////////////////////////////////////
void Board :: doCombo(const StepCombo& combo)
{
    for (unsigned int i = 0; i < combo.numSteps; i++)
    {
        doStep(combo.steps[i]);
    } 

    stepsLeft -= combo.stepCost;
}

//////////////////////////////////////////////////////////////////////////////
// Undoes a combo
//////////////////////////////////////////////////////////////////////////////
void Board :: undoCombo(const StepCombo& combo)
{
    for (int i = combo.numSteps - 1; i >= 0; i--)
    {
        undoStep(combo.steps[i]);
    } 

    stepsLeft += combo.stepCost;
}
//////////////////////////////////////////////////////////////////////////////
//Prints the board onto the stream designated by out
//////////////////////////////////////////////////////////////////////////////
ostream& operator<<(ostream& out, Board b)
{
    //print out the turn number and side to move
    out << b.turnNumber << charFromColor(b.sideToMove) << endl;

    char pieceArray[64]; //displayable array that the pieces will eventually
                         //be placed onto
    
    //clear out the array, using ' ' as empty square
    for (int i = 0; i < 64; i ++)
        pieceArray[i] = ' ';

    //now scan the bitboards, placing the appropriate piece on the array
    //if bits are found
    for (int color = 0; color < MAX_COLORS; color ++)
    {
        for (int type = 0; type < MAX_TYPES; type ++)
        {
            Int64 bitboard = b.pieces[color][type];
            int index;
            while ((index = bitScanForward(bitboard)) != -1)
            {
                //write the character onto the array
                pieceArray[index] = charFromPiece(genPiece(color,type));
                
                //delete the bit found from the bitboard
                bitboard ^= Int64FromIndex(index);
            }
        }
    }
    
    out <<  " +-----------------+\n";

    //display the array obtained
    for (int row = 0; row < 8; row++)
    {
        out<< 8 - row << "| ";
        for (int column = 0; column < 8; column ++)
        {
            out << pieceArray[column + row * 8] << ' ';
        }
        
        out << '|' << endl;
    }

    out << " +-----------------+\n";
    out << "   a b c d e f g h\n";

    return out;
}                    
