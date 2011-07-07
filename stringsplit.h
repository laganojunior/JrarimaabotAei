#ifndef __JR__STRINGSPLIT_H__
#define __JR__STRINGSPLIT_H__

#include <string>
#include <vector>
#include <sstream>

using namespace std;

static vector<string> splitByWhitespace(const string& s)
{
    string buf;            // Have a buffer string
    stringstream ss(s);    // Insert the string into a stream

    vector<string> tokens; // Create vector to hold our words

    while (ss >> buf)
    {
        tokens.push_back(buf);
    }

    return tokens;
}

static vector<string> splitByToken(const string& str, string delimiter)
{
    vector<string> tokens;

    string::size_type lastPos = 0;

    // Find first "non-delimiter".
    string::size_type pos     = str.find(delimiter, lastPos);

    while (string::npos != pos && string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));

        lastPos = pos + delimiter.size();

        // Find next "non-delimiter"
        pos = str.find(delimiter, lastPos);
    }
 
    return tokens;
}

#endif

