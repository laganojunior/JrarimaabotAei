#ifndef __JRARIMAABOT_PIECE_H__
#define __JRARIMAABOT_PIECE_H__

#include <string>

//piece and side colors
#define MAX_COLORS 2
#define GOLD 0
#define SILVER 1

//piece types
#define MAX_TYPES 6
#define ELEPHANT 0
#define CAMEL    1
#define HORSE    2
#define DOG      3
#define CAT      4
#define RABBIT   5
#define NO_PIECE 6

using namespace std;

// conversion from internal to/from ASCII representations
unsigned char pieceFromChar(char pieceChar);
char          charFromPiece(unsigned char piece);
string        stringFromColor(unsigned char color);
char          charFromColor(unsigned char color);

// set/extract attributes
unsigned char colorOfPiece(unsigned char piece);
unsigned char typeOfPiece(unsigned char piece);
unsigned char genPiece(unsigned char color,unsigned char type);

//////////////////////////////////////////////////////////////////////////////
// return the color of a piece
//////////////////////////////////////////////////////////////////////////////
inline unsigned char colorOfPiece(unsigned char piece)
{
    return piece >> 3; //return the 4th bit
}

//////////////////////////////////////////////////////////////////////////////
// return the type of a piece
//////////////////////////////////////////////////////////////////////////////
inline unsigned char typeOfPiece(unsigned char piece)
{
    //return the first 3 bits
    return piece & 0x7;
}

//////////////////////////////////////////////////////////////////////////////
// return a piece with the specified color and type
//////////////////////////////////////////////////////////////////////////////
inline unsigned char genPiece(unsigned char color,unsigned char type)
{
    return type | (color << 3);
}

/////////////////////////////////////////////////////////////////////////////
// returns the opposite color
/////////////////////////////////////////////////////////////////////////////
inline unsigned char oppColorOf(unsigned char color) 
{
    return color ^ 1;
}
#endif
