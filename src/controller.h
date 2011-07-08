#ifndef __JR__CONTROLLER_H__
#define __JR__CONTROLLER_H__

#include <iostream>
#include "bot.h"

using namespace std;

class Bot;

/*************************************************************************
 * This class defines the controller thread, which handles all IO between the
 * AEI gameroom controller and the bot controller. This is to abstract the
 * IO thread from the bot thread and avoid rewriting the IO handling for
 * every version of a bot.
 */
class Controller
{
    public:
    /*************************************************************************
     * Construct a controller interfacing some bot and some output stream to
     * write log messages to. The bot passed in should already be ready to
     * accept any commands (e.g it should allocate all its memory, be running
     * its work threads if necessary, etc.)
     ************************************************************************/
    Controller(Bot * bot, ostream& log): log(log)
    {
        this->bot = bot;
        bot->controller = this;
    }

    ~Controller() {}

    void mainLoop();

    void sendMove(const string& moveString);
    void sendInfo(const string& infoType, const string& value);
    void sendLog(const string& logString);
 
    private:
    Bot * bot;
    ostream& log;
};
#endif
