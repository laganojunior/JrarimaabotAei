#include "board.h"
#include "controller.h"
#include "randombot.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

using namespace std;

int main()
{
    RandomBot bot;
    ofstream out("Log.txt");
    Controller controller(&bot, out);

    controller.mainLoop();
   
    
    return 0;
}

