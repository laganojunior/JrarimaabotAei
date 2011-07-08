#include "int64.h"
#include <sstream>
#include <string>
#include <cstdlib>

using namespace std;

//some pre-defined arrays

//////////////////////////////////////////////////////////////////////////////
// Returns a 64 bit integer as a string in base-2 format (i.e. 0 and 1's) 
//////////////////////////////////////////////////////////////////////////////
string Int64ToString(Int64 i)
{
    stringstream out;

    for (int index = 63; index >= 0; index--)
    {
        if (i & Int64FromIndex(index))
            out << '1';
        else
            out << '0';
    }

    return out.str();
}

//////////////////////////////////////////////////////////////////////////////
// Returns a 64 bit integer as a string that represents it as a bitboard, i.e.
// a 8x8 array of bits.
//////////////////////////////////////////////////////////////////////////////
string BitboardToString(Int64 i)
{
    stringstream out;

    for (int row = 0; row < 8; row ++)
    {
        for (int col = 0; col < 8; col ++)  
        {
            if (i & Int64FromIndex(col + row * 8))
                out << '1';
            else
                out << '0';
        }
        out << endl;
    }

    return out.str();
}

//////////////////////////////////////////////////////////////////////////////
// returns a random 64 bit integer
//////////////////////////////////////////////////////////////////////////////
Int64 randInt64()
{
    return (Int64)rand() ^ ((Int64)rand() << 16) ^ ((Int64)rand() << 32)
           ^ ((Int64)rand() << 48);
}
