//
//  Edge.h
//  ofxGCodePlotter
//
//  Created by Carsten Høyer on 16/02/2019.
//

#pragma once

#include <stdio.h>
#include <memory>

using namespace std;

class Node;
class Edge
{
public:
    Edge();
    void setNode(Node* _node);
    Node* node;
    float weight;
};
