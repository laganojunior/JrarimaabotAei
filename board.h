#ifndef __JR__BOARD_H__
#define __JR__BOARD_H__
//The class that is the board representation

#include "int64.h"
#include "piece.h"
#include "step.h"
#include <string>
#include <vector>

using namespace std;

class Board
{
    public:
    Board();
    ~Board();
    
    //functions///////////////////////////////////////////////////////////////

    void newGame();

    void doPlacementString(const string& pStr);
    void doMoveString(const string& mStr);
    
    void doCombo(const StepCombo& combo);
    void undoCombo(const StepCombo& combo);

    void doStep(const Step& step)
    {
        // Get the aggregrate bitboard. Note that for captures, this will
        // contain only 1 non-zero bit.
        Int64 agg = Int64FromIndex(step.getFrom()) |
                    Int64FromIndex(step.getTo());

        // xor out the aggregrate from the piece's bitboard. If this is a move,
        // then this will zero out the previous square and 1 the new square. If
        // this is a capture, then the previous square is just zeroed out.
        const unsigned char piece = step.getPiece();
        pieces[colorOfPiece(piece)][typeOfPiece(piece)] ^= agg; 
    }

    void undoStep(const Step& step)
    {
        // The same procedure doing the step again actually undoes the step
        doStep(step); 
    }

    void changeTurn()
    {
        sideToMove = oppColorOf(sideToMove);
        stepsLeft = 4;
    }

    //variables///////////////////////////////////////////////////////////////

    //bitboards to store positions of the pieces. first index is color, second 
    //index is piece type
    Int64 pieces[MAX_COLORS][MAX_TYPES];

    unsigned char sideToMove; //current player to move
    unsigned int turnNumber;
    unsigned int stepsLeft;   //number of steps left in this move 
                              //available

    bool isSetup;
};

//display operator
ostream& operator<<(ostream& out, Board b);
#endif
