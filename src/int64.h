#ifndef __JRARIMAABOT_INT64__
#define __JRARIMAABOT_INT64__

//////////////////////////////////////////////////////////////////////////////
//This class contains classes and functions to generate and manipulate 64 bit 
//integers, wheter they are bitboard representations, hashes, etc..
//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>
#include <assert.h>

// Bitscan return code
#define NO_BIT_FOUND -1

using namespace std;

// Create a typedef for 64 bit integers, as the actual implementation might
// differ in different systems
typedef unsigned long long Int64;

// Some predefined constants 
const Int64 COL_A = (((Int64) 1)) |  
                    (((Int64) 1) << 8)  |
                    (((Int64) 1) << 16) |
                    (((Int64) 1) << 24) |
                    (((Int64) 1) << 32) |
                    (((Int64) 1) << 40) |
                    (((Int64) 1) << 48) |
                    (((Int64) 1) << 56);

const Int64 COL_H = (((Int64) 1) << 7)  |  
                    (((Int64) 1) << 15) |
                    (((Int64) 1) << 23) |
                    (((Int64) 1) << 31) |
                    (((Int64) 1) << 39) |
                    (((Int64) 1) << 47) |
                    (((Int64) 1) << 55) |
                    (((Int64) 1) << 63);

const Int64 ROW_1 = (((Int64) 255) << 56);
const Int64 ROW_8 = (((Int64) 255));

const Int64 traps = (((Int64) 1) << 18) |
                    (((Int64) 1) << 21) |
                    (((Int64) 1) << 42) |    
                    (((Int64) 1) << 45);


//some functions implemented in int64.cpp
string Int64ToString(Int64 i);
string BitboardToString(Int64 i);
Int64  randInt64();

//////////////////////////////////////////////////////////////////////////////
//Returns the 64 bit integer with all bits set to 0 except for the bit
//at the specified index, which is set to 1
//////////////////////////////////////////////////////////////////////////////
inline Int64 Int64FromIndex(unsigned int index)
{
    return ((Int64) 1) << index;
}

//////////////////////////////////////////////////////////////////////////////
// Returns the lowest index of the lowest 1 bit in a 64 bit integer. If there
// are none found, -1 is returned
//////////////////////////////////////////////////////////////////////////////
inline int bitScanForward(Int64 i)
{
    return __builtin_ffsll(i) - 1;
}

//////////////////////////////////////////////////////////////////////////////
// Returns the number of bits set to 1 in a 64 bit integer.
//////////////////////////////////////////////////////////////////////////////
inline int numBits(Int64 i)
{
    return __builtin_popcountll(i);
}

//////////////////////////////////////////////////////////////////////////////
// Returns the 64 bit integer that has 1's for the lower section of bits
// specified by numbits and 0's in all other bits
//////////////////////////////////////////////////////////////////////////////
inline Int64 Int64LowerBitsFilled(unsigned int numBits)
{
    Int64 i = 0;
    for (unsigned int bit = 0; bit < numBits; bit++)
    {
        i |= ((Int64)1 << bit);
    }

    return i;
}

//////////////////////////////////////////////////////////////////////////////
// returns the bitboard neighboring any of the positions specified on the
// input bitboard
//////////////////////////////////////////////////////////////////////////////
inline Int64 getNeighbors(Int64 i)
{
    return ((i << 1) & ~COL_A) | ((i >> 1) & ~COL_H)
            | (i << 8) | (i >> 8);
}

//////////////////////////////////////////////////////////////////////////////
// returns the neighbors bitboard without the south neighbor
//////////////////////////////////////////////////////////////////////////////
inline Int64 getNeighborsUp(Int64 i)
{
    return ((i << 1) & ~COL_A) | ((i >> 1) & ~COL_H)
          | (i >> 8);
}

//////////////////////////////////////////////////////////////////////////////
// returns the neighbors bitboard without the north neighbor
//////////////////////////////////////////////////////////////////////////////
inline Int64 getNeighborsDown(Int64 i)
{
    return ((i << 1) & ~COL_A) | ((i >> 1) & ~COL_H)
          | (i << 8);
}

#endif
