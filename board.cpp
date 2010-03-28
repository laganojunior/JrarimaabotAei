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
// Generates the list of all legal 1-step moves. The input pointer must
// point to something already allocated as this will write all the moves
// generated directly from the address given onward. Returns the number of
// moves generated.
//////////////////////////////////////////////////////////////////////////////
int Board :: gen1Steps(StepCombo * combos)
{
    // Get all of the pieces on the board to check for obstacles and captures
    Int64 allColorPieces[MAX_COLORS] = {0, 0};
    for (int i = 0; i < MAX_TYPES; i++)
    {
        allColorPieces[GOLD] |= pieces[GOLD][i];
        allColorPieces[SILVER] |= pieces[SILVER][i];
    }

    Int64 allPieces = allColorPieces[GOLD] | allColorPieces[SILVER];

    // Bitboard to keep track of all the stronger pieces on the opposing
    // side for freezing constraints. 
    Int64 oppStrongerPieces = 0; 

    int numGen = 0;

    // Iterate through each type of piece starting from the elephant down,
    // up to the rabbit which must be treated specially
    for (unsigned char type = 0; type < MAX_TYPES - 1; type++)
    {
        unsigned char piece = genPiece(sideToMove, type);

        // Get all the squares containing this piece which is not
        // frozen
        Int64 pieceBoard = pieces[sideToMove][type] &
                           ~getNeighbors(oppStrongerPieces);

        // Go through square of which this piece is on
        int from;
        while ((from = bitScanForward(pieceBoard)) != NO_BIT_FOUND)
        {
            Int64 pos =  Int64FromIndex(from);
            pieceBoard ^= pos;
  
            // Make a move for every neighboring square that is free 
            Int64 freeNeighbors =  getNeighbors(pos) & ~allPieces;

            int to;
            while ((to = bitScanForward(freeNeighbors)) != NO_BIT_FOUND)
            {     
                Int64 toPos = Int64FromIndex(to);
                freeNeighbors ^= toPos;
 
                combos[numGen].reset();
                combos[numGen].appendMove(piece, from, to);
                // Look for resulting captures if any when performing the
                // move. 
                unsigned char capPiece, capSquare;
                if (checkMoveLeadsToCapture(from, to, piece,
                                            allColorPieces[sideToMove],
                                            capPiece, capSquare))
                {
                    combos[numGen].appendCapture(capPiece, capSquare);
                }

                numGen++;
            }
        }

        // Changing piece type in next iteration, so the stronger opposing
        // bitboard should be updated with the next type of piece that
        // will now be stronger in the next iteration. Note that this is just
        // the piece type that was considered during this iteration.
        oppStrongerPieces |= pieces[oppColorOf(sideToMove)][type];
    }

    // Take care of the rabbit, rabbits cannot move backward.

    // Get non-frozen rabbits
    Int64 pieceBoard = pieces[sideToMove][RABBIT]
                      & ~getNeighbors(oppStrongerPieces);
    unsigned char piece = genPiece(sideToMove, RABBIT);

    // Go through square of which this piece is on
    int from;
    while ((from = bitScanForward(pieceBoard)) != NO_BIT_FOUND)
    {
        Int64 pos =  Int64FromIndex(from);
        pieceBoard ^= pos;

        // Make a move for every neighboring square that is free. Note
        // for rabbits, rabbits cannot move backward, so this is dependent
        // on which side is moving.
        Int64 freeNeighbors;
       
        if (sideToMove == GOLD)
            freeNeighbors = getNeighborsUp(pos) & ~allPieces;
        else
            freeNeighbors = getNeighborsDown(pos) & ~allPieces;
            
        int to;
        while ((to = bitScanForward(freeNeighbors)) != NO_BIT_FOUND)
        {     
            Int64 toPos = Int64FromIndex(to);
            freeNeighbors ^= toPos;

            combos[numGen].reset();
            combos[numGen].appendMove(piece, from, to);
            // Look for resulting captures if any when performing the
            // move. 
            unsigned char capPiece, capSquare;
            if (checkMoveLeadsToCapture(from, to, piece,
                                        allColorPieces[sideToMove],
                                        capPiece, capSquare))
            {
                combos[numGen].appendCapture(capPiece, capSquare);
            }
            numGen++;
        }
    } 

    return numGen;
}

//////////////////////////////////////////////////////////////////////////////
// Generates the list of all allowed pushes and pulls. As in gen1Steps, the
// input pointer must point to something already allocated as this will write 
// all the moves generated directly from the address given onward. Returns
// the number of moves generated.
//////////////////////////////////////////////////////////////////////////////
int Board :: genPushPull(StepCombo * combos)
{
    // Get all of the pieces on the board to check for obstacles and captures
    Int64 allColorPieces[MAX_COLORS] = {0, 0};
    for (int i = 0; i < MAX_TYPES; i++)
    {
        allColorPieces[GOLD] |= pieces[GOLD][i];
        allColorPieces[SILVER] |= pieces[SILVER][i];
    }

    Int64 allPieces = allColorPieces[GOLD] | allColorPieces[SILVER];

    // Bitboard to keep track of all the stronger pieces on the opposing
    // side for freezing constraints. 
    Int64 oppStrongerPieces = 0; 

    // Keep track of weaker pieces on the opposing side for pushing and pulling
    Int64 oppWeakerPieces = allColorPieces[oppColorOf(sideToMove)]
                            ^ pieces[oppColorOf(sideToMove)][ELEPHANT];

    int numGen = 0;

    // Iterate through each type of piece starting from the elephant down
    // but not including the rabbit (which can't push or pull
    for (unsigned char type = 0; type < MAX_TYPES - 1; type++)
    {
        unsigned char piece = genPiece(sideToMove, type);

        // Get all the squares containing this piece which is not
        // frozen and near some opposing weaker piece
        Int64 pieceBoard = pieces[sideToMove][type] &
                           ~getNeighbors(oppStrongerPieces) &
                           getNeighbors(oppWeakerPieces);

        // Go through square of which this piece is on
        int from;
        while ((from = bitScanForward(pieceBoard)) != NO_BIT_FOUND)
        {
            Int64 pos =  Int64FromIndex(from);
            pieceBoard ^= pos;
  
            // First consider pulls. First make a move for every
            // neighboring square that is free 
            Int64 freeNeighbors =  getNeighbors(pos) & ~allPieces;

            int to;
            while ((to = bitScanForward(freeNeighbors)) != NO_BIT_FOUND)
            {     
                Int64 toPos = Int64FromIndex(to);
                freeNeighbors ^= toPos;

                StepCombo prefix; 
                prefix.appendMove(piece, from, to); 
                // Look for resulting captures if any when performing the
                // move.
                unsigned char capPiece, capSquare;
                if (checkMoveLeadsToCapture(from, to, piece,
                                            allColorPieces[sideToMove],
                                            capPiece, capSquare))
                {
                    prefix.appendCapture(capPiece, capSquare);
                }

                // Now for each weaker piece, pull the piece to old position
                for (int pullType = type + 1; pullType < MAX_TYPES; pullType++)
                {
                    Int64 pulls = pieces[oppColorOf(sideToMove)][pullType]
                                 & getNeighbors(pos);
                    unsigned char pullPiece = genPiece(oppColorOf(sideToMove),
                                                       pullType);
                    
                    int pullFrom;
                    while ((pullFrom = bitScanForward(pulls)) != NO_BIT_FOUND)
                    {
                        pulls ^= Int64FromIndex(pullFrom);

                        combos[numGen] = prefix;
                        combos[numGen].appendMove(pullPiece, pullFrom, from);
                        
                        // Check if pulling piece leads to capture
                        unsigned char capPiece, capSquare;
                        if (checkMoveLeadsToCapture(pullFrom, from, pullPiece,
                                     allColorPieces[oppColorOf(sideToMove)],
                                                    capPiece, capSquare))
                        {
                            combos[numGen].appendCapture(capPiece, capSquare);
                        }                                                 

                        numGen++;
                    } 
                } 
            }

            // Now consider pushes. For every weaker piece, move it to
            // some free neighbor and move this piece in its former position
            for (int pushType = type + 1; pushType < MAX_TYPES; pushType++)
            {
                unsigned char pushPiece = genPiece(oppColorOf(sideToMove),
                                                   pushType);

                Int64 push =  pieces[oppColorOf(sideToMove)][pushType]
                            & getNeighbors(pos);

                int pushFrom;
                while ((pushFrom = bitScanForward(push)) != NO_BIT_FOUND)
                {
                    Int64 pushedPos = Int64FromIndex(pushFrom);
                    push ^= pushedPos;
                    
                    // Find all free squares to push to
                    Int64 pushToPos = getNeighbors(pushedPos) & ~allPieces;
                    
                    int pushTo;
                    while ((pushTo = bitScanForward(pushToPos)) 
                            != NO_BIT_FOUND)
                    {
                        pushToPos ^= Int64FromIndex(pushTo);

                        combos[numGen].reset();

                        // Add step for pushing
                        combos[numGen].appendMove(pushPiece, pushFrom,
                                                             pushTo);
                        
                        // Check if pushing piece leads to capture
                        unsigned char capPiece, capSquare;
                        if (checkMoveLeadsToCapture(pushFrom, pushTo, pushPiece,
                                     allColorPieces[oppColorOf(sideToMove)],
                                                    capPiece, capSquare))
                        {
                            combos[numGen].appendCapture(capPiece, capSquare);
                        }                                                 

                        // Add step for moving the pushing piece
                        combos[numGen].appendMove(piece, from, pushFrom);

                        // Check if moving the pushing piece leads to capture
                        if (checkMoveLeadsToCapture(from, pushFrom, piece,
                                     allColorPieces[sideToMove],
                                                    capPiece, capSquare))
                        {
                            combos[numGen].appendCapture(capPiece, capSquare);
                        }

                        numGen++;
                    }
                }
            } 
        }

        // Changing piece type in next iteration, so the stronger opposing
        // bitboard should be updated with the next type of piece that
        // will now be stronger in the next iteration. Note that this is just
        // the piece type that was considered during this iteration.
        oppStrongerPieces |= pieces[oppColorOf(sideToMove)][type];
        
        // Also update weaker pieces. This is just removing the set of pieces
        // that will be coming up in the next iteration
        oppWeakerPieces ^= pieces[oppColorOf(sideToMove)][type + 1];
    }
    
    return numGen;
}

//////////////////////////////////////////////////////////////////////////////
//Prints the board onto the stream designated by out
//////////////////////////////////////////////////////////////////////////////
ostream& operator<<(ostream& out, Board b)
{
    //print out the side to move
    out << charFromColor(b.sideToMove) << endl;

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
