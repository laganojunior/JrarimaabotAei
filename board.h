#ifndef __JR__BOARD_H__
#define __JR__BOARD_H__
//The class that is the board representation

#include "int64.h"
#include "piece.h"
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
