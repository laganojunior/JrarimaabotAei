#include "controller.h"
#include "piece.h"

#include <iostream>
#include <sstream>

using namespace std;

/*************************************************************************
 * Runs the main loop of reading IO and calling the trigger functions
 * on the bot thread
 *************************************************************************/
void Controller :: mainLoop()
{
    bool quit = false;
    while (!quit)
    {
        // Read the next command line
        string line;
        getline(cin, line);
    
        stringstream lineStream(line);
        log << "Received Message: " << line << endl;        

        // Parse the first word, which should give which command it is
        string command;
        lineStream >> command;

        log << "Parsed Command: " << command << endl;

        if (command == "aei")
        {
            // AEI protocol handshake
            cout << "protocol-version 1\n";
            string name, author, version;
            
            bot->getInfo(name, author, version);

            log << "Sent: aeiok\n";
            log << "Sent: id name " << name << endl;
            log << "Sent: id author " << author << endl;
            log << "Sent: id version " << version << endl;

            cout << "id name " << name << endl;
            cout << "id author " << author << endl;
            cout << "id version " << version << endl;
            cout << "aeiok\n";
        } 
        else if (command == "isready")
        {
            // By the blocking design of the controller/bot interface,
            // the bot should always be ready to receive new commands
            // if the code gets to this point. So there is no need to wait,
            // signal ready
            log << "Sent: readyok\n";

            cout << "readyok\n";

        }
        else if (command == "newgame")
        {
            log << "Called: bot->startNewGame()\n";

            bot -> startNewGame();
        }
        else if (command == "setposition")
        {
            // Currently not Supported

            char colorChar;
            string posString;
            
            lineStream >> colorChar;
            getline(lineStream, posString);
            posString.erase(0, 1); // Remove leading space

            unsigned char color = '?';
            if (colorChar == 'g')
                color = GOLD;
            else if (colorChar == 's')
                color = SILVER;

            log << "Called: bot->startNewGame()\n";
            log << "\tcolor: " << charFromColor(color) << endl;
            log << "\tposition: " << posString << endl; 

            bot -> setPosition(color, posString);
        } 
        else if (command == "setoption")
        {
            // Currently not supported
            
        }
        else if (command == "makemove")
        {
            string moveString;
            getline(lineStream, moveString);

            log << "Called: bot->doMoveString()\n";
            log << "\tmoveStr: " << moveString << endl;

            bot -> doMoveString(moveString);
            
        }
        else if (command == "go")
        {
            string rest;
            lineStream >> rest;

            if (rest == "ponder")
            {
                log << "Called: bot->startPonder()\n";
                bot -> startPonder();
            } 
            else
            {
                log << "Called: bot->startSearch()\n";
                bot -> startSearch();
            }
        }
        else if (command == "stop")
        {
            log << "Called: bot->forceStop()\n";
            bot -> forceStop();
        }
        else if (command == "quit")
        {
            log << "Called: bot->forceQuit()\n";
            bot -> forceQuit();

            quit = true;
        }
        else
        {
            log << "Unrecognized command\n";
        }
    }
}

/**************************************************************************
 * Sends a move string out to the AEI controller
 **************************************************************************/
void Controller :: sendMove(const string& moveString)
{
    cout << "bestmove " << moveString << endl;
    log <<  "Sent: bestmove " << moveString << endl;
}

/*************************************************************************
 * Sends an information string to the AEI controller to display to the
 * user
 *************************************************************************/
void Controller :: sendInfo(const string& infoType, const string& value)
{
    cout << "info " << infoType << " " << value << endl;
    log  << "Sent: info " << infoType << " " << value << endl;
}

/**************************************************************************
 * Sends a string for the AEI controller to log 
 **************************************************************************/
void Controller :: sendLog(const string& logString)
{
    cout << "log " << logString << endl;
    log  << "Sent: log " << logString << endl;
}


