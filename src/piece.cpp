#include "piece.h"
#include <assert.h>
#include <ctype.h>

//////////////////////////////////////////////////////////////////////////////
//return a encoded piece (read step.h) from a readable character
//////////////////////////////////////////////////////////////////////////////
unsigned char pieceFromChar(char pieceChar)
{
    unsigned char type = 0;
    switch (tolower(pieceChar))
	{
        case 'e':
            type = ELEPHANT;
            break;
        case 'm':
            type = CAMEL;
            break;
        case 'h':
            type = HORSE;
            break;
        case 'd':
            type = DOG;
            break;
        case 'c':
            type = CAT;
            break;
        case 'r':
            type = RABBIT;
            break;
    }

    unsigned char color;

    if (islower(pieceChar))
        color = SILVER;
    else
        color = GOLD;

    return genPiece(color,type);
}

//////////////////////////////////////////////////////////////////////////////
//return a human readable character referring to the piece that was given
//////////////////////////////////////////////////////////////////////////////
char charFromPiece(unsigned char piece)
{
    unsigned char color = colorOfPiece(piece);
    unsigned char type  = typeOfPiece(piece);

    char pieceChar = 0;
    
    switch (type)
    {
        case ELEPHANT:
            pieceChar = 'e';
            break;
        case CAMEL:
            pieceChar = 'm';
            break;
        case HORSE:
            pieceChar = 'h';
            break;
        case DOG:
            pieceChar = 'd';
            break;
        case CAT:
            pieceChar = 'c';
            break;
        case RABBIT:
            pieceChar = 'r';
            break;
    }
    
    if (color == GOLD)
        pieceChar = toupper(pieceChar);
    
    return pieceChar;
}

//////////////////////////////////////////////////////////////////////////////
//returns the string representation of a color
//////////////////////////////////////////////////////////////////////////////
string stringFromColor(unsigned char color)
{
    if (color == GOLD)
        return "GOLD";
    if (color == SILVER)
        return "SILVER";
    
    return "NONE"; //??? should never get here
}

//////////////////////////////////////////////////////////////////////////////
//returns the one character representation of a color
//////////////////////////////////////////////////////////////////////////////
char charFromColor(unsigned char color)
{
    if (color == GOLD)
        return 'g';
    
    if (color == SILVER)
        return 's';
    
    return '?';
}

