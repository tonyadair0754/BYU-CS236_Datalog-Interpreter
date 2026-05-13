#pragma once
#include <vector>
#include <sstream>
#include <string>
#include <set>
#include "scheme.h"
#include "tuple.h"

using namespace std;

class Relation {
private:

    string name;
    Scheme scheme;
    set<Tuple> tuples;

public:

    // Default Relation
    Relation() {}

    Relation(const string& name, const Scheme& scheme)
      : name(name), scheme(scheme) { }

    void addTuple(const Tuple& tuple) {
        tuples.insert(tuple);
    }

    string toString() const {
        stringstream out;
        for (const Tuple& tuple : tuples) {
            out << tuple.toString(scheme) << "\n";
        }
        return out.str();
    }

    // Select1 (for X,value)
    Relation select(int index, const string& value) const {
        Relation result(name, scheme);
        for (const Tuple& tuple : tuples)
        {
            if (tuple.at(index) == value)
            {
                result.addTuple(tuple);
            }
        }
        return result;
    }

    // Select2 (for X,X)
    Relation select(int index1, int index2) const {
        Relation result(name, scheme);

        for (const Tuple& tuple : tuples) {
            if (tuple.at(index1) == tuple.at(index2)) {
                result.addTuple(tuple);
            }
        }

        return result;
    }

    // Project
    Relation project(const vector<int>& indices) const {

        vector<string> newNames;
        for (int i : indices) {
            newNames.push_back(scheme.at(i));
        }

        Scheme newScheme(newNames);
        Relation result(name, newScheme);

        for (const Tuple& tuple : tuples) {

            vector<string> newValues;
            for (int i : indices) {
                newValues.push_back(tuple.at(i));
            }

            result.addTuple(Tuple(newValues));
        }

        return result;
    }

    // Rename
    Relation rename(const vector<string>& newNames) const {

        Scheme newScheme(newNames);
        Relation result(name, newScheme);

        for (const Tuple& tuple : tuples) {
            result.addTuple(tuple);
        }

        return result;
    }

    // Needed for counting results
    const set<Tuple>& getTuples() const {
        return tuples;
    }

    // Joinable helper
    vector<pair<int,int>> matchingColumns(const Scheme& left,
                                       const Scheme& right)
    {
        vector<pair<int,int>> matches;

        for (size_t i = 0; i < left.size(); i++) {
            for (size_t j = 0; j < right.size(); j++) {
                if (left[i] == right[j]) {
                    matches.push_back({i,j});
                }
            }
        }
        return matches;
    }

    // Check if two schemes can join
    /*static bool joinable(const Scheme& leftScheme, const Scheme& rightScheme,
        const Tuple& leftTuple, const Tuple& rightTuple) {

        // For every attribute name that appears in both schemes, the values in the tuples must match to be joinable
        for (unsigned leftIndex = 0; leftIndex < leftScheme.size(); leftIndex++) {

            const string& leftName = leftScheme.at(leftIndex);
            const string& leftValue = leftTuple.at(leftIndex);

            cout << "left name: " << leftName << " value: " << leftValue << endl;

            for (unsigned rightIndex = 0; rightIndex < rightScheme.size(); rightIndex++) {

                const string& rightName = rightScheme.at(rightIndex);
                const string& rightValue = rightTuple.at(rightIndex);

                cout << "right name: " << rightName << " value: " << rightValue << endl;

                if (leftName == rightName && leftValue != rightValue) {
                        return false;
                }
            }
        }

    return true;
    }*/

    static bool joinable(const Tuple& leftTuple,
                     const Tuple& rightTuple,
                     const vector<pair<int,int>>& matches)
    {
        for (const auto& m : matches) {
            if (leftTuple.at(m.first) != rightTuple.at(m.second)) {
                return false;
            }
        }
        return true;
    }

    // Join
    /*Relation join(const Relation& right) {
        const Relation& left = *this;

        Scheme newScheme = joinSchemes(left.scheme, right.scheme);
        Relation result("join", newScheme);

        for (const Tuple& leftTuple : left.tuples) {
            for (const Tuple& rightTuple : right.tuples) {

                if (joinable(left.scheme, right.scheme, leftTuple, rightTuple))
                {
                    Tuple newTuple = joinTuples(leftTuple, rightTuple, left.scheme, right.scheme);
                    result.addTuple(newTuple);
                }
            }
        }

        return result;
    }*/

    Relation join(const Relation& right) {
        const Relation& left = *this;

        // build scheme
        Scheme newScheme = joinSchemes(left.scheme, right.scheme);
        Relation result("join", newScheme);

        // compute matching columns ONCE
        vector<pair<int,int>> matches =
            matchingColumns(left.scheme, right.scheme);

        for (const Tuple& leftTuple : left.tuples) {
            for (const Tuple& rightTuple : right.tuples) {

                if (joinable(leftTuple, rightTuple, matches)) {

                    Tuple newTuple =
                        joinTuples(leftTuple, rightTuple,
                                   left.scheme, right.scheme);

                    result.addTuple(newTuple);
                }
            }
        }

        return result;
    }

    Tuple joinTuples(const Tuple& leftTuple, const Tuple& rightTuple, const Scheme& leftScheme, const Scheme& rightScheme) {

        Tuple result = leftTuple;

        for (unsigned i = 0; i < rightScheme.size(); i++) {

            const string& rightName = rightScheme.at(i);
            bool found = false;

            for (unsigned j = 0; j < leftScheme.size(); j++) {
                if (leftScheme.at(j) == rightName) {
                    found = true;
                    break;
                }
            }

            if (!found) {
                result.push_back(rightTuple.at(i));
            }
        }

        return result;
    }

    Scheme joinSchemes(const Scheme& left, const Scheme& right) {

        Scheme result = left;

        for (unsigned i = 0; i < right.size(); i++) {

            const string& rightName = right.at(i);
            bool found = false;

            for (unsigned j = 0; j < left.size(); j++) {
                if (left.at(j) == rightName) {
                    found = true;
                    break;
                }
            }

            if (!found) {
                result.push_back(rightName);
            }
        }

        return result;
    }

    // Union
    bool unionWith(const Relation& other) {

        bool added = false;

        for (const Tuple& t : other.tuples) {

            auto result = tuples.insert(t);

            if (result.second) {
                cout << "  " << t.toString(scheme) << "\n";
                added = true;
            }
        }

        return added;
    }

    const Scheme& getScheme() const
    {
        return scheme;
    }
};