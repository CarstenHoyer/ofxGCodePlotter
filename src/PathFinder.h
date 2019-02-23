//
//  PathFinder.hpp
//  ofxGCodePlotter
//
//  Created by Carsten Høyer on 16/02/2019.
//

#pragma once

#include <stdio.h>
#include "ofPoint.h"
#include "GCodePath.h"
#include "Node.h"
#include <vector>

using namespace std;

class PathFinder
{
public:
    PathFinder();
    vector<GCodePath> findPath(vector<GCodePath> paths);
private:
    float heuristicCostEstimate(GCodePath* start, GCodePath* end);
    float distBetween(Node* current, Node* neighbor);
    Node* getOpenPathWithLowestFScore(set<Node*> openSet, map<Node*, float> fScore);
    vector<GCodePath> reconstructPath(map<Node*, Node*> cameFrom, Node* current);
    float getEuclidianDistance(ofPoint p1, ofPoint p2);
};
