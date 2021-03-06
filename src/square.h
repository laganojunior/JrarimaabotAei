#ifndef __JR_SQUARE_H__
#define __JR_SQUARE_H__

//some useful functions that are common in many places in the code
#include <string>

//square constants
#define C6 18
#define F6 21
#define C3 42
#define F3 45
#define ILLEGAL_SQUARE 64

#define NUM_SQUARES 64

using namespace std;

//functions to convert to/from ascii representations to internal
//representations
unsigned char squareFromString(string squareString);
string        stringFromSquare(unsigned char index);
#endif
