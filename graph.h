#pragma once
#include <iostream>
#include <sstream>
#include <set>
#include <string>
#include <map>
#include "node.h"

using namespace std;

class Graph {

private:
    map<int,Node> nodes;

public:
    Graph(int size) {
        for (int nodeID = 0; nodeID < size; nodeID++)
            nodes[nodeID] = Node();
    }

    void addEdge(int fromNodeID, int toNodeID) {
        nodes[fromNodeID].addEdge(toNodeID);
    }

    Graph reverse() const {
        Graph reversed(nodes.size());

        for (auto& [from, node] : nodes) {
            for (int to : node.getAdjacent()) {
                reversed.addEdge(to, from);
            }
        }

        return reversed;
    }

    void dfsPostOrder(int nodeID, set<int>& visited, vector<int>& stack) {
        visited.insert(nodeID);

        for (int neighbor : nodes[nodeID].getAdjacent()) {
            if (!visited.count(neighbor)) {
                dfsPostOrder(neighbor, visited, stack);
            }
        }

        stack.push_back(nodeID);
    }

    vector<int> getPostOrder() {
        set<int> visited;
        vector<int> stack;

        for (auto& [id, _] : nodes) {
            if (!visited.count(id)) {
                dfsPostOrder(id, visited, stack);
            }
        }

        return stack; // contains postorder
    }

    void dfsCollect(int nodeID, set<int>& visited, set<int>& component) {
        visited.insert(nodeID);
        component.insert(nodeID);

        for (int neighbor : nodes[nodeID].getAdjacent()) {
            if (!visited.count(neighbor)) {
                dfsCollect(neighbor, visited, component);
            }
        }
    }

    vector<set<int>> getSCCs(const vector<int>& postOrder) {
        set<int> visited;
        vector<set<int>> sccs;

        for (auto it = postOrder.rbegin(); it != postOrder.rend(); ++it) {
            int node = *it;

            if (!visited.count(node)) {
                set<int> component;
                dfsCollect(node, visited, component);
                sccs.push_back(component);
            }
        }

        return sccs;
    }

    bool hasSelfDependency(int nodeID) const {
        return nodes.at(nodeID).getAdjacent().count(nodeID);
    }

    string toString() const {
        stringstream out;

        for (auto& pair: nodes) {
            int nodeID = pair.first;
            Node node = pair.second;

            out << "R" << nodeID << ":" << node.toString() << endl;
        }

        return out.str();
    }
};