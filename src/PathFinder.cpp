//
//  PathFinder.cpp
//  ofxGCodePlotter
//
//  Created by Carsten Høyer on 16/02/2019.
//

#include "PathFinder.h"
#include <math.h>

using namespace std;

PathFinder::PathFinder() {
    
}

float PathFinder::getEuclidianDistance(ofPoint p1, ofPoint p2) {
    return sqrt(pow(p2.y - p1.y, 2) + pow(p2.x - p1.y, 2));
}

vector<GCodePath> PathFinder::findPath(vector<GCodePath> paths)
{
    vector<Node*> nodes;
    for (auto p1 = paths.begin(); p1 != paths.end(); ++p1)
    {
        Node* node = new Node();
        node->path = &(*p1);
        nodes.push_back(node);
    }
    
    for (auto n1 = nodes.begin(); n1 != nodes.end(); ++n1)
    {
        for (auto n2 = nodes.begin(); n2 != nodes.end(); ++n2)
        {
            if (&(*n1) != &(*n2)) {
                (*n1)->edges.push_back(*n2);
            }
        }
    }
    
    Node* start = nodes.front();
    Node* goal = nodes.back();
    
    // The set of nodes already evaluated
    set<Node*> closedSet;
    
    // The set of currently discovered nodes that are not evaluated yet.
    // Initially, only the start node is known.
    set<Node*> openSet;
    openSet.insert(start);
    
    // For each node, which node it can most efficiently be reached from.
    // If a node can be reached from many nodes, cameFrom will eventually contain the
    // most efficient previous step.
    map<Node*, Node*> cameFrom;
    
    // For each node, the cost of getting from the start node to that node.
    // map with default value of Infinity
    map<Node*, float> gScore;
    
    // For each node, the total cost of getting from the start node to the goal
    // by passing by that node. That value is partly known, partly heuristic.
    map<Node*, float> fScore;
    
    for (auto node = nodes.begin(); node != nodes.end(); ++node)
    {
        gScore.insert(make_pair((*node), numeric_limits<float>::infinity()));
        fScore.insert(make_pair((*node), numeric_limits<float>::infinity()));
    }
    
    // The cost of going from start to start is zero.
    gScore[start] = 0.0f;
    
    // For the first node, that value is completely heuristic.
    fScore[start] = heuristicCostEstimate(start->path, goal->path);
    
    while (openSet.size() > 0) {
        Node* current = getOpenPathWithLowestFScore(openSet, fScore);
        
        if (&current == &goal) {
            return reconstructPath(cameFrom, current);
        }
        
        openSet.erase(current);
        closedSet.insert(current);
        
        for (auto neighbor = current->edges.begin(); neighbor != current->edges.end(); ++neighbor)
        {
            if (closedSet.find(*neighbor) != closedSet.end())
            {
                continue; // Ignore the neighbor which is already evaluated.
            }
            
            float tentativeGScore = gScore[current] + distBetween(current, *neighbor);
            
            if (openSet.find(*neighbor) == openSet.end()) {
                // Discover a new node
                openSet.insert(*neighbor);
            }
            else if (tentativeGScore >= gScore[*neighbor])
            {
                continue;
            }
            
            // This path is the best until now. Record it!
            cameFrom[*neighbor] = current;
            gScore[*neighbor] = tentativeGScore;
            fScore[*neighbor] = gScore[*neighbor] + heuristicCostEstimate((*neighbor)->path, goal->path);
        }
    }
    int i = 0;
}

vector<GCodePath> PathFinder::reconstructPath(map<Node*, Node*> cameFrom, Node* current) {
    vector<GCodePath> paths;
    return paths;
}

float PathFinder::distBetween(Node* current, Node* neighbor)
{
    return 0.0f;
}

Node* PathFinder::getOpenPathWithLowestFScore(set<Node*> openSet, map<Node*, float> fScore)
{
    float score;
    Node* node = nullptr;
    for (auto iter = openSet.begin(); iter != openSet.end(); ++iter)
    {
        if (node == nullptr || fScore[(*iter)] < score) {
            score = fScore[*iter];
            node = *iter;
        }
    }
    return node;
}

float PathFinder::heuristicCostEstimate(GCodePath* path1, GCodePath* path2)
{
    ofPoint start = *path1->end;
    ofPoint end = path2->start != nullptr ? *path2->start : *path2->end;
    return sqrt(pow(end.x - start.x, 2) + pow(end.y - start.y, 2));
}
