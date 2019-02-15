//
//  Config.h
//  ofGcode
//
//  Created by Carsten Høyer on 01/02/2019.
//

#pragma once

#include <stdio.h>
#include <sstream>
#include <iostream>
#include <fstream>

//--------------------------
using namespace std;

class Config {
public:
    Config();
    Config(string configPath);
    void set(string key, string value);
    bool absolutePositioning;
    float canvasWidth;
    float canvasHeight;
    float paperWidth;
    float paperHeight;
    float verticalMargin;
    float horizontalMargin;
    float xOffset;
    float yOffset;
    float toolSizeMinimum;
    float toolSizeMaximum;
    float toolZMinimum;
    float toolZMaximum;
    bool flipX;
    bool flipY;
    bool xIsNegative;
    bool yIsNegative;
    bool zIsNegative;
};
