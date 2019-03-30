#include "util_string.h"
#include <algorithm>
#include <functional>
#include <cstdlib>
#include <iostream>
#include <iostream>
#include <functional>
#include <algorithm>
#include <cstdlib>
#include <numeric>
#include <stdarg.h>
using namespace std;

namespace ACE {

string& Trim(string &s) {

    return RTrim(LTrim(s));
}

string& LTrim(string &s) {
    int (*func)(int) = isspace;
    string::iterator iter;
    iter = find_if(s.begin(), s.end(), std::not1(std::ptr_fun(func)));
    s.erase(s.begin(), iter);

    return s;
}

string& RTrim(string &s) {
    int (*func)(int) = isspace;
    string::reverse_iterator iter;
    iter = find_if(s.rbegin(), s.rend(), not1(ptr_fun(func)));
    s.erase(iter.base(), s.end());
    return s;
}

}// namespace ACE
