#pragma once
#include <map>
#include <string>
#include "relation.h"

using namespace std;

class Database {

private:
    map<string, Relation> relations;

public:

    void addRelation(const string& name, const Relation& relation) {
        relations[name] = relation;
    }

    Relation& getRelation(const string& name) {
        return relations.at(name);
    }
};