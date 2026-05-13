#pragma once
#include "predicate.h"
#include <vector>
#include <string>
#include <sstream>

class Rule {
public:
    Predicate head;
    vector<Predicate> body;

    Rule() {}

    Rule(const Predicate& headPredicate) : head(headPredicate) {}

    string toString() const {
        stringstream ss;
        ss << head.toString() << " :- ";
        for (size_t i = 0; i < body.size(); i++) {
            if (i > 0) ss << ",";
            ss << body[i].toString();
        }
        ss << ".";
        return ss.str();
    }

    void addBodyPredicate(const Predicate& p) {
        body.push_back(p);
    }
};
