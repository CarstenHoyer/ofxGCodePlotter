//
//  GCodePath.h
//  ofGcode
//
//  Created by Carsten Høyer on 02/02/2019.
//

#pragma once

#include "ofMain.h"

class GCodePath
{
public:
    GCodePath();
    string arcTo(ofPoint start, ofPoint end, ofPoint center, bool clockwise);
    string print();
    ofPoint mapPoint(ofPoint point);
    float mapFloat(float number);
    
    shared_ptr<ofPoint> start;
    void setStart(ofPoint point);
    
    shared_ptr<ofPoint> end;
    void setEnd(ofPoint point);
    
    shared_ptr<ofPoint> center;
    void setCenter(ofPoint point);
    
    shared_ptr<float> radiusX;
    void setRadiusX(float radius);
    
    shared_ptr<float> radiusY;
    void setRadiusY(float radius);
    
    float z;
    float angleBegin;
    float angleEnd;

    int type;
    int position;
    int unit;
    enum Type {
        Move,
        MoveZ,
        Line,
        Arc,
        ArcNegative,
        Unit,
        Position,
        End
    };
    enum Position {
        Absolute,
        Relative
    };
    enum UnitType {
        Mm
    };
};
