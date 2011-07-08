#ifndef __JR__BOT_H__
#define __JR__BOT_H__

#include "controller.h"
#include <iostream>

using namespace std;

class Controller;

/***************************************************************************
 * This defines the interface for a bot to implement in order to work with
 * the controller class. To do this, bot classes should derive this class and
 * override all necessary functions.
 **************************************************************************/
class Bot
{
    public:
    // Set the input variables with the name, author, and version of the bot
    virtual void getInfo(string& name, string& author, string& version) = 0;

    // First stop any searches currently going. Then reset the board position
    // for a new game.  
    //
    // MUST return only if these tasks are completed.
    virtual void startNewGame() = 0;

        
    // Stop all searches and set the board state to another position.
    // Arguments:
    //    color - the side to move. Either the constant GOLD or SILVER defined
    //            in piece.h
    //    position - a string describing the board position. The format is
    //               the AEI short board format, using only [], piece chars
    //               and spaces.
    // 
    // MUST return only if these tasks are completed.
    virtual void setPosition(unsigned char color, string position) = 0;


    // Stop all searches and play a move on the board.
    // Arguments:
    //      moveStr - a string describing the move. This string is the
    //                same format as given by the AEI protocol, perhaps with
    //                some leading whitespace.
    // MUST return only if these tasks are completed.
    virtual void doMoveString(string moveStr) = 0;


    // If a search is not already going, start a search for the purpose of
    // pondering (i.e. thinking during an opponent's turn). Search can be
    // done asynchronously, but should be set-up to be interruptible.
    virtual void startPonder() = 0;

     
    // If a search is not already going, start a search for the purpose of
    // finding a move to play in the current position. Search can be
    // done asynchronously, but should be set-up to be interruptible. The
    // bot can send a move to play at any time by calling the 
    // controller's sendMove function.
    virtual void startSearch() = 0;

    // Stop all searches and then calls the controller's sendMove function
    // with the best move to play from the current position. 
    //
    // MUST return only if these tasks are completed.
    virtual void forceStop() = 0;
    
    // Stop all searches and clean-up, as the program will now quit. MUST
    // return only if all these tasks are completed.
    virtual void forceQuit() = 0;

    Controller * controller;
 
}; 


#endif
