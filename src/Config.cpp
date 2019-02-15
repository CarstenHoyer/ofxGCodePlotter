//
//  Config.cpp
//  ofGcode
//
//  Created by Carsten Høyer on 01/02/2019.
//

#include "Config.h"

//--------------------------
using namespace std;

Config::Config()
{
    
}

Config::Config(string configPath)
{
    ifstream infile(configPath);
    
    string line;
    while( getline(infile, line) )
    {
        istringstream iss(line);
        string key;
        if( getline(iss, key, '=') )
        {
            string value;
            if( getline(iss, value) ) {
                set(key, value);
            }
        }
    }
}

void Config::set(string key, string value)
{
    float num = atof(value.c_str());
    if (key == "absolutePositioning")
    {
        absolutePositioning = (bool) num > 0;
    }
    else if (key == "canvasWidth")
    {
        canvasWidth = (float) num;
    }
    else if (key == "canvasHeight")
    {
        canvasHeight = (float) num;
    }
    else if (key == "paperWidth")
    {
        paperWidth = (float) num;
    }
    else if (key == "paperHeight")
    {
        paperHeight = (float) num;
    }
    else if (key == "horizontalMargin")
    {
        horizontalMargin = (float) num;
    }
    else if (key == "verticalMargin")
    {
        verticalMargin = (float) num;
    }
    else if (key == "xOffset")
    {
        xOffset = (float) num;
    }
    else if (key == "yOffset")
    {
        yOffset = (float) num;
    }
    else if (key == "toolSizeMinimum")
    {
        toolSizeMinimum = (float) num;
    }
    else if (key == "toolSizeMaximum")
    {
        toolSizeMaximum = (float) num;
    }
    else if (key == "toolZMinimum")
    {
        toolZMinimum = (float) num;
    }
    else if (key == "toolZMaximum")
    {
        toolZMaximum = (float) num;
    }
    else if (key == "flipX")
    {
        flipX = (bool) num;
    }
    else if (key == "flipY")
    {
        flipY = (bool) num;
    }
    else if (key == "xIsNegative")
    {
        xIsNegative = (bool) num;
    }
    else if (key == "yIsNegative")
    {
        yIsNegative = (bool) num;
    }
    else if (key == "zIsNegative")
    {
        zIsNegative = (bool) num;
    }
}
