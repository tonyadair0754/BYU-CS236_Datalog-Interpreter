#pragma once
#include "parameter.h"
#include <vector>
#include <string>
#include <sstream>

class Predicate {
public:
    string name;
    vector<Parameter> parameters;

    Predicate() {}

    Predicate(string predName) : name(predName) {}

    string toString() const {
        stringstream ss;
        ss << name << "(";
        for (size_t i = 0; i < parameters.size(); i++) {
            if (i > 0) ss << ",";
            ss << parameters[i].toString();
        }
        ss << ")";
        return ss.str();
    }

    void addParameter(const Parameter& p) {
        parameters.push_back(p);
    }
};

