#ifndef __COMMON_UTIL_STRING_H__
#define __COMMON_UTIL_STRING_H__
#include <string>
using std::string;

namespace ACE
{
    string& Trim(string &s);
    string& LTrim(string &s);
    string& RTrim(string &s);
}


#endif 