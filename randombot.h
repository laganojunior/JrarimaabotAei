#include "bot.h"
#include "controller.h"
#include <cstdlib>
#include <string>
#include <fstream>

using namespace std;

/****************************************************************************
 * This class implements the bot interface by returning a random step
 * or push/pull as the move
 ****************************************************************************/
class RandomBot: public Bot
{
    public:
    RandomBot() : out("BotLog.txt")
    {
        
    }
 
    virtual void getInfo(string& name, string& author, string& version)
    {
        name = "Random Bot";
        author = "Someone really random";
        version = "1";
    }

    virtual void startNewGame()
    {
        board.newGame();
    }    
    
    virtual void doMoveString(string moveStr)
    {
        out << "Got move " << moveStr << endl;
        out.flush();
        board.doMoveString(moveStr);

        out << "Board is now " << endl;
        out << board;
        out.flush();
    }

    virtual void startPonder() {}

    virtual void startSearch() 
    {
        out << "Getting command to start search\n";
        out.flush();

        if (board.isSetup)
        {
            if (board.sideToMove == GOLD)
            {
                controller->sendMove(
           "Ra2 Hb2 Cc2 Md2 Ee2 Cf2 Hg2 Rh2 Ra1 Rb1 Rc1 Dd1 De1 Rf1 Rg1 Rh1");
            }
            else
            {
                controller->sendMove(
           "ra7 hb7 cc7 md7 ee7 cf7 hg7 rh7 ra8 rb8 rc8 dd8 de8 rf8 rg8 rh8");
            }
        }
        else 
        {
            StepCombo fullCombo;
            StepCombo combos[200];

            // Only make generate moves of cost 3, as these cannot end up
            // repeating the same position.
            while (fullCombo.stepCost != 3)
            {
                // For fun, try to make a push or pull whenever possible
                if (fullCombo.stepCost < 2)
                {
                    int numGenned = board.genPushPull(combos);

                    if (numGenned != 0)
                    {
                        int choice = rand() % numGenned;
                        fullCombo.addCombo(combos[choice]);
                        board.doCombo(combos[choice]);
                        continue;
                    }

                    numGenned = board.gen1Steps(combos);

                    if (numGenned != 0)
                    {
                        int choice = rand() % numGenned;
                        fullCombo.addCombo(combos[choice]);
                        board.doCombo(combos[choice]);
                    }

                    // If it gets to this point, there are no steps to play
                    break; 
                }
                else 
                {
                    int numGenned = board.gen1Steps(combos);
                
                    if (numGenned != 0)
                    {
                        int choice = rand() % numGenned;
                        fullCombo.addCombo(combos[choice]);
                        board.doCombo(combos[choice]);
                    }
                    
                    // If it gets to this point, there are no steps to play
                    break; 
                }
            }
            
            board.undoCombo(fullCombo);
            controller->sendMove(fullCombo.toString());
        }
    }
 
    virtual void forceStop() {} 
    virtual void forceQuit() {} 

    private:
        
    Board board;
    ofstream out;

};
