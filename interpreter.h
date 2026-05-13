#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <string>
#include <map>
#include <stack>

#include "scheme.h"
#include "tuple.h"
#include "relation.h"
#include "database.h"
#include "datalog.h"
#include "graph.h"

using namespace std;

class Interpreter {
private:
    Database database;
    DatalogProgram program;

    // Create empty relation for each scheme
    void evaluateSchemes() {

        for (const Predicate& schemePred : program.schemes) {

            vector<string> attributes;

            for (const Parameter& p : schemePred.parameters) {
                attributes.push_back(p.value);
            }

            Scheme scheme(attributes);
            Relation relation(schemePred.name, scheme);

            database.addRelation(schemePred.name, relation);
        }
    }

    // Create tuple for each fact
    void evaluateFacts() {

        for (const Predicate& fact : program.facts) {

            Relation& relation =
                database.getRelation(fact.name);

            vector<string> values;

            for (const Parameter& p : fact.parameters) {
                values.push_back(p.value);
            }

            relation.addTuple(Tuple(values));
        }
    }

    Relation evaluatePredicate(const Predicate& query)
    {
        Relation result = database.getRelation(query.name);

        map<string,int> varPos;
        vector<int> projectIndices;
        vector<string> renameNames;

        for (size_t i = 0; i < query.parameters.size(); i++) {

            Parameter p = query.parameters[i];

            if (p.isString) {
                result = result.select(i, p.value);
            }
            else {
                if (varPos.count(p.value)) {
                    result = result.select(i, varPos[p.value]);
                } else {
                    varPos[p.value] = i;
                    projectIndices.push_back(i);
                    renameNames.push_back(p.value);
                }
            }
        }
    result = result.project(projectIndices);
    result = result.rename(renameNames);

    return result;
    }

    // Evaluate one rule
    bool evaluateRule(const Rule& rule)
    {
        cout << rule.toString() << "\n";

        vector<Relation> relations;

        // STEP 1 — evaluate body predicates
        for (const Predicate& p : rule.body) {
            relations.push_back(evaluatePredicate(p));
        }

        // STEP 2 — join results
        Relation result = relations[0];

        for (size_t i = 1; i < relations.size(); i++) {
            result = result.join(relations[i]);
        }

        // STEP 3 — project to head variables
        vector<int> projectIndices;

        for (const Parameter& headParam : rule.head.parameters) {

            for (size_t i = 0; i < result.getScheme().size(); i++) {

                if (result.getScheme().at(i) == headParam.value) {
                    projectIndices.push_back(i);
                }
            }
        }

        result = result.project(projectIndices);

        // STEP 4 — rename to match database relation
        Relation& dbRelation =
            database.getRelation(rule.head.name);

        result = result.rename(dbRelation.getScheme());

        // STEP 5 — union into database
        bool added = dbRelation.unionWith(result);

        return added;
    }

    void evaluateRules() {
        cout << "Dependency Graph\n";

        Graph graph = makeGraph(program.rules);
        cout << graph.toString() << endl;

        Graph reverse = graph.reverse();
        vector<int> postOrder = reverse.getPostOrder();
        vector<set<int>> sccs = graph.getSCCs(postOrder);

        cout << "Rule Evaluation\n";

        for (const set<int>& scc : sccs) {

            // print SCC
            cout << "SCC: ";
            bool first = true;
            for (int id : scc) {
                if (!first) cout << ",";
                cout << "R" << id;
                first = false;
            }
            cout << endl;

            bool added;
            int passes = 0;

            do {
                added = false;

                for (int ruleID : scc) {
                    if (evaluateRule(program.rules[ruleID])) {
                        added = true;
                    }
                }

                passes++;

            } while (added && (scc.size() > 1 ||
                     graph.hasSelfDependency(*scc.begin())));

            cout << passes << " passes: ";

            first = true;
            for (int id : scc) {
                if (!first) cout << ",";
                cout << "R" << id;
                first = false;
            }
            cout << endl;
        }

        cout << endl;
    }

public:
    Interpreter(const DatalogProgram& program)
        : program(program) {}

    void interpret();

    // Evaluate all queries
    void evaluateQueries() {

        cout << "Query Evaluation\n";

        for (const Predicate& query : program.queries) {

            cout << query.toString() << "? ";

            Relation result = evaluateQuery(query);

            if (result.getTuples().empty()) {
                cout << "No\n";
            } else {
                cout << "Yes(" << result.getTuples().size() << ")\n";
                cout << result.toString();
            }
        }
    }

    // Evaluate one query
    Relation evaluateQuery(const Predicate& query) {

        // Get relation
        Relation result = database.getRelation(query.name);

        map<string,int> varPos;
        vector<int> projectIndices;
        vector<string> renameNames;

        for (size_t i = 0; i < query.parameters.size(); i++) {

            Parameter p = query.parameters[i];

            if (p.isString) {
                result = result.select(i, p.value);
            }
            else {
                if (varPos.count(p.value)) {
                    result = result.select(i, varPos[p.value]);
                } else {
                    varPos[p.value] = i;
                    projectIndices.push_back(i);
                    renameNames.push_back(p.value);
                }
            }
        }

        result = result.project(projectIndices);
        result = result.rename(renameNames);

        return result;
    }

    static Graph makeGraph(const vector<Rule>& rules) {
        Graph graph(rules.size());

        for (int i = 0; i < rules.size(); i++) {
            const Rule& fromRule = rules[i];
            //cout << "from rule R" << i << ": " << fromRule.toString() << endl;

            for (const Predicate& bodyPred : fromRule.body) {
                //cout << "from body predicate: " << bodyPred.toString() << endl;

                for (int j = 0; j < rules.size(); j++) {
                    const Rule& toRule = rules[j];
                    //cout << "to rule R" << j << ": " << toRule.toString() << endl;

                    // compare names here
                    if (bodyPred.name == toRule.head.name) {
                        graph.addEdge(i, j);
                        //cout << "dependency found: (R" << i << ",R" << j << ")" << endl;
                    }
                }

            }
        }

        return graph;
    }
};