#pragma once
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
#include <set>

#include "predicate.h"
#include "rules.h"

using namespace std;

class DatalogProgram {
public:
    vector<Predicate> schemes;
    vector<Predicate> facts;
    vector<Rule> rules;
    vector<Predicate> queries;
    set<string> domain;

    string toString() const {
        stringstream ss;
        // schemes
        ss << "Schemes(" << schemes.size() << "):\n";
        for (auto& s : schemes) ss << "  " << s.toString() << "\n";

        // facts
        ss << "Facts(" << facts.size() << "):\n";
        for (auto& f : facts) ss << "  " << f.toString() << ".\n";

        // rules
        ss << "Rules(" << rules.size() << "):\n";
        for (auto& r : rules) ss << "  " << r.toString() << "\n";

        // queries
        ss << "Queries(" << queries.size() << "):\n";
        for (auto& q : queries) ss << "  " << q.toString() << "?\n";

        // domain
        ss << "Domain(" << domain.size() << "):\n";
        for (auto& d : domain) ss << "  " << d << "\n";

        return ss.str();
    }
};
