#include "board.h"
#include "piece.h"
#include "int64.h"

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
