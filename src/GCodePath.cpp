//
//  GCodePath.cpp
//  ofGcode
//
//  Created by Carsten Høyer on 02/02/2019.
//

#include "GCodePath.h"

GCodePath::GCodePath()
{
    
}

void GCodePath::setStart(ofPoint point)
{
    this->start = make_shared<ofPoint>(point);
}

void GCodePath::setEnd(ofPoint point)
{
    this->end = make_shared<ofPoint>(point);
}

void GCodePath::setCenter(ofPoint point)
{
    this->center = make_shared<ofPoint>(point);
}

void GCodePath::setRadiusX(float radius)
{
    this->radiusX = make_shared<float>(radius);
}

void GCodePath::setRadiusY(float radius)
{
    this->radiusY = make_shared<float>(radius);
}

string GCodePath::arcTo(ofPoint start, ofPoint end, ofPoint center, bool clockwise)
{
    ofPoint mStart = start;
    ofPoint mEnd = end;
    ofPoint mCenter = center;
    
    float I = abs(mStart.x - mCenter.x);
    if (mStart.x > mCenter.x) {
        I = -1 * I;
    }
    
    float J = abs(mStart.y - mCenter.y);
    if (mStart.y > mCenter.y) {
        J = -1 * J;
    }
    
    string cmd = clockwise ? "G02" : "G03";
    return cmd + " X" + to_string(mEnd.x) + " Y" + to_string(mEnd.y) + " I" + to_string(I) + " J" + to_string(J);
}

string GCodePath::print()
{
    if (type == Type::Move)
    {
        return "G00 X" +to_string(end->x) + " Y" + to_string(end->y);
    }
    else if (type == Type::MoveZ)
    {
        return "G00 Z" + to_string(z);
    }
    else if (type == Type::Line)
    {
        return "G01 X" + to_string(end->x) + " Y" + to_string(end->y) + " F400.000000";
    }
    else if (type == Type::Arc || type == Type::ArcNegative)
    {
        return arcTo(*start, *end, *center, type == Type::Arc);
    }
    else if (type == Type::Unit)
    {
        return "G21 (All units in mm)";
    }
    else if (type == Type::Position)
    {
        if (position == Position::Absolute) {
            return "G90 (Set to Absolute Positioning)";
        } else {
            return "G91 (Set to Relative Positioning)";
        }
    }
    else if (type == Type::End)
    {
        return "M2 (End program)";
    }
    return "";
}
