//
//  Node.h
//  ofxGCodePlotter
//
//  Created by Carsten Høyer on 16/02/2019.
//

#pragma once

#include <stdio.h>
#include "Edge.h"
#include "GCodePath.h"

using namespace std;

class Node
{
public:
    Node();
    GCodePath* path;
    vector<Node*> edges;
};
