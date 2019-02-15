//
//  ofxGcodePlotter.h
//  ofxGcodePlotter
//
//  Created by Carsten Høyer on 01/02/2019.
//

#pragma once

#include "ofMain.h"
#include "Config.h"
#include "GCodePath.h"
#include "ofxCubicBezierToBiarc.h"

class ofxGCodePlotter {
public:
    ofxGCodePlotter();
    ofxGCodePlotter(string configPath, float width, float height);
    float inWidth;
    float inHeight;
    ofPoint offset;
    float scale;
    string filePath;
    Config config;
    vector<string> commands;
    
    vector<GCodePath> generate(vector<ofPath> paths);
    vector<GCodePath> addStart();
    vector<GCodePath> addEnd();
    vector<GCodePath> pathToCommand(ofPath path);
    vector<string> addCommand(string command, string comment = "");
    void print(vector<GCodePath> paths);
    void scalePaths(vector<GCodePath>& paths);
    void transformPaths(vector<GCodePath>& paths);
    void flipPaths(vector<GCodePath>& paths);
    
    
    // Commands
    vector<GCodePath> home();
    void moveTo(ofPoint point);
    void lineTo(ofPoint point);
    void arcTo(ofPoint start, ofPoint end, ofPoint center, bool clockwise);
    void down();
    void up();
    
    
private:
    ofxCubicBezierToBiarc bezierToBiarc;
    GCodePath makeArcPath(ofPath::Command arc, ofPoint** currentPos);
    GCodePath makeMovePath(ofPoint to, ofPoint** startPos, ofPoint** currentPos);
    GCodePath makeLinePath(ofPoint to, ofPoint** currentPos);
    vector<GCodePath> makeBezierPaths(ofPath::Command cmd, ofPoint** startPos, ofPoint** currentPos);
    GCodePath makeClosePath(ofPoint to, ofPoint** startPos, ofPoint** currentPos);
    void updateCurrentPos(ofPoint to, ofPoint** currentPos);
    bool positionIsOutOfSync(ofPath::Command cmd, ofPoint startPos);
    ofPoint pointOnCircle(ofPath::Command cmd);
    ofPoint pointOnCircle(ofPoint center, float angle, float radiusX, float radiusY);
};
