#pragma once
#include <iostream>
#include <sstream>
#include <set>
#include <string>

using namespace std;

class Node {

private:
    set<int> adjacentNodeIDs;

public:
    void addEdge(int adjacentNodeID) {
        adjacentNodeIDs.insert(adjacentNodeID);
    }

    const set<int>& getAdjacent() const {
        return adjacentNodeIDs;
    }

    string toString() const {
        stringstream out;
        bool first = true;

        for (int id : adjacentNodeIDs) {
            if (!first) out << ",";
            out << "R" << id;
            first = false;
        }

        return out.str();
    }
};