#pragma once
#include <string>

using namespace std;

class Parameter {
public:
    string value;
    bool isString;

    Parameter(string v, bool isStr) : value(v), isString(isStr) {}

    string toString() const {
        return value;
    }
};
