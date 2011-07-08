#include "error.h"
#include "int64.h"
#include <assert.h>
#include <ctype.h>
#include <string>

using namespace std;

//////////////////////////////////////////////////////////////////////////////
//return the index of the square referred to by a string in human-readable
//format.
//////////////////////////////////////////////////////////////////////////////
unsigned char squareFromString(string squareString)
{
    int x = squareString[0] - 'a';
    int y = 8 - (squareString[1] - '0');
   
    return x + y * 8;
}

//////////////////////////////////////////////////////////////////////////////
//Returns the string representation of a square
//////////////////////////////////////////////////////////////////////////////
string stringFromSquare(unsigned char index)
{
    string squareString = "00"; //init some two character string
    
    int row = index / 8;
    int col = index % 8;
    
    squareString[0] = col + 'a';
    squareString[1] = (8 - row) + '0';
    
    return squareString;
}

