//
//  ofxGcodePlotter.ccp
//  ofxGcodePlotter
//
//  Created by Carsten Høyer on 01/02/2019.
//

#include "ofxGcodePlotter.h"
#include "ofxCubicBezierToBiarc.h"
#include "GCodePath.h"

ofxGCodePlotter::ofxGCodePlotter()
{
    // Empty.
}

ofxGCodePlotter::ofxGCodePlotter(string configPath, float width, float height)
{
    config = Config(configPath);
    ofxCubicBezierToBiarc bezierToBiarc = ofxCubicBezierToBiarc();
    
    inWidth = width;
    inHeight = height;
    scale = min((config.paperWidth - (config.horizontalMargin * 2)) / width, (config.paperHeight - (config.verticalMargin * 2)) / height);
    float xOffset = config.xOffset + config.horizontalMargin;
    if (config.xIsNegative)
    {
        xOffset *= -1;
    }
    float yOffset = config.yOffset + config.verticalMargin;
    if (config.yIsNegative)
    {
        yOffset *= -1;
    }
    offset = ofPoint(xOffset, yOffset);
}

vector<GCodePath> ofxGCodePlotter::generate(vector<ofPath> paths) {
    vector<GCodePath> gps = addStart();
    for (auto iter = paths.begin(); iter != paths.end(); ++iter) {
        vector<GCodePath> cmdPaths = pathToCommand((*iter));
        gps.insert(gps.end(), cmdPaths.begin(), cmdPaths.end());
    }
    vector<GCodePath> endCommands = addEnd();
    gps.insert(gps.end(), endCommands.begin(), endCommands.end());
    return gps;
}

vector<GCodePath> ofxGCodePlotter::addStart()
{
    vector<GCodePath> gps;
    GCodePath unit;
    unit.type = GCodePath::Type::Unit;
    unit.unit = GCodePath::UnitType::Mm;
    gps.push_back(unit);
    
    GCodePath position;
    position.type = GCodePath::Type::Position;
    position.position = config.absolutePositioning ? GCodePath::Position::Absolute : GCodePath::Position::Relative;
    gps.push_back(position);
    
    vector<GCodePath> homeCmds = home();
    gps.insert(gps.end(), homeCmds.begin(), homeCmds.end());
    return gps;
}

vector<GCodePath> ofxGCodePlotter::addEnd()
{
    vector<GCodePath> gps = home();
    GCodePath end;
    end.type = GCodePath::Type::End;
    gps.push_back(end);
    return gps;
}

vector<GCodePath> ofxGCodePlotter::home()
{
    vector<GCodePath> gps;
    
    GCodePath homeZ;
    homeZ.type = GCodePath::Type::MoveZ;
    homeZ.z = 0;
    gps.push_back(homeZ);
    
    GCodePath homeXY;
    homeXY.type = GCodePath::Type::Move;
    homeXY.setEnd(ofPoint(0, 0));
    gps.push_back(homeXY);
    
    return gps;
}

void ofxGCodePlotter::updateCurrentPos(ofPoint to, ofPoint** currentPos)
{
    *currentPos = new ofPoint(to.x, to.y);
}

GCodePath ofxGCodePlotter::makeMovePath(ofPoint to, ofPoint** startPos, ofPoint** currentPos)
{
    GCodePath gp;
    gp.setEnd(to);
    gp.type = GCodePath::Type::Move;
    if (*startPos == nullptr) {
        *startPos = new ofPoint(to.x, to.y);
    }
    updateCurrentPos(to, currentPos);
    return gp;
}

GCodePath ofxGCodePlotter::makeLinePath(ofPoint to, ofPoint** currentPos)
{
    GCodePath gp;
    gp.setStart(ofPoint((**currentPos).x, (**currentPos).y));
    gp.setEnd(to);
    gp.type = GCodePath::Type::Line;
    updateCurrentPos(to, currentPos);
    return gp;
}

vector<GCodePath> ofxGCodePlotter::makeBezierPaths(ofPath::Command cmd, ofPoint** startPos, ofPoint** currentPos)
{
    vector<GCodePath> gps;
    
    ofPath bezierPath;
    bezierPath.moveTo(ofPoint((**currentPos).x, (**currentPos).y));
    bezierPath.bezierTo(cmd.cp1, cmd.cp2, cmd.to);
    
    vector<tuple<ofPath, ofPath>> biarcs = bezierToBiarc.ApproxCubicBezier(bezierPath, 5, 5);
    for (auto biarc = biarcs.begin(); biarc != biarcs.end(); ++biarc)
    {
        auto cmd1 = get<0>(*biarc).getCommands()[1];
        GCodePath arc1 = makeArcPath(cmd1, currentPos);
        gps.push_back(arc1);
        
        auto cmd2 = get<1>(*biarc).getCommands()[1];
        GCodePath arc2 = makeArcPath(cmd2, currentPos);
        gps.push_back(arc2);
    }
    
    return gps;
}

GCodePath ofxGCodePlotter::makeArcPath(ofPath::Command cmd, ofPoint** currentPos)
{    
    GCodePath gp;
    gp.setCenter(cmd.to);
    
    ofPoint start = pointOnCircle(cmd.to, cmd.angleBegin, cmd.radiusX, cmd.radiusY);
    gp.setStart(start);
    
    ofPoint end = pointOnCircle(cmd.to, cmd.angleEnd, cmd.radiusX, cmd.radiusY);
    gp.setEnd(end);
    
    gp.angleBegin = cmd.angleBegin;
    gp.angleEnd = cmd.angleEnd;
    gp.setRadiusX(cmd.radiusX);
    gp.setRadiusY(cmd.radiusY);
    
    // GCode is flipped compared to ofPath.
    gp.type = cmd.type != ofPath::Command::arc
    ? GCodePath::Type::Arc
    : GCodePath::Type::ArcNegative;
    
    updateCurrentPos(end, currentPos);
    
    return gp;
}


GCodePath ofxGCodePlotter::makeClosePath(ofPoint to, ofPoint** startPos, ofPoint** currentPos)
{
    GCodePath gp;
    gp.setStart(ofPoint((**currentPos).x, (**currentPos).y));
    gp.setEnd(to);
    gp.type = GCodePath::Type::Line;
    currentPos = nullptr;
    startPos = nullptr;
    return gp;
}

vector<GCodePath> ofxGCodePlotter::pathToCommand(ofPath path)
{
    vector<GCodePath> paths;
    auto commands = path.getCommands();
    ofPoint* startPos = nullptr;
    ofPoint* currentPos = nullptr;
    for (auto command = commands.begin(); command != commands.end(); ++command) {
        if ((*command).type == ofPath::Command::moveTo) {
            GCodePath gp = makeMovePath((*command).to, &startPos, &currentPos);
            paths.push_back(gp);
        }
        
        else if ((*command).type == ofPath::Command::lineTo)
        {
            if (!startPos)
            {
                GCodePath gp = makeMovePath((*command).to, &startPos, &currentPos);
                paths.push_back(gp);
            }
            else
            {
                GCodePath gp = makeLinePath((*command).to, &currentPos);
                paths.push_back(gp);
            }
        }
        else if ((*command).type == ofPath::Command::arc || (*command).type == ofPath::Command::arcNegative)
        {
            
            if (!startPos || positionIsOutOfSync(*command, *startPos)) {
                ofPoint pathStart = pointOnCircle(*command);
                GCodePath mt = makeLinePath(pathStart, &currentPos);
                paths.push_back(mt);
            }
            GCodePath gp = makeArcPath(*command, &currentPos);
            paths.push_back(gp);
        }
        
        else if ((*command).type == ofPath::Command::bezierTo)
        {
            vector<GCodePath> gps = makeBezierPaths(*command, &startPos, &currentPos);
            paths.insert(paths.end(), gps.begin(), gps.end());
        }
        
        else if ((*command).type == ofPath::Command::close && startPos)
        {
            GCodePath gp = makeClosePath(*startPos, &startPos, &currentPos);
            paths.push_back(gp);
        }
    }
    
    flipPaths(paths);
    scalePaths(paths);
    transformPaths(paths);
    return paths;
}

void ofxGCodePlotter::transformPaths(vector<GCodePath>& paths)
{
    for (auto iter = paths.begin(); iter != paths.end(); ++iter)
    {
        if((*iter).start != nullptr)
        {
            float xStart = config.xIsNegative ? -1 * (*(*iter).start).x : (*(*iter).start).x;
            float yStart = config.yIsNegative ? -1 * (*(*iter).start).y : (*(*iter).start).y;
            (*iter).setStart(ofPoint(xStart + offset.x, yStart + offset.y));
        }
        if((*iter).end != nullptr)
        {
            float xEnd = config.xIsNegative ? -1 * (*(*iter).end).x : (*(*iter).end).x;
            float yEnd = config.yIsNegative ? -1 * (*(*iter).end).y : (*(*iter).end).y;
            (*iter).setEnd(ofPoint(xEnd + offset.x, yEnd + offset.y));
        }
        if((*iter).center != nullptr)
        {
            float xCenter = config.xIsNegative ? -1 * (*(*iter).center).x : (*(*iter).center).x;
            float yCenter = config.yIsNegative ? -1 * (*(*iter).center).y : (*(*iter).center).y;
            (*iter).setCenter(ofPoint(xCenter + offset.x, yCenter + offset.y));
        }
    }
}

void ofxGCodePlotter::scalePaths(vector<GCodePath>& paths)
{
    for (auto iter = paths.begin(); iter != paths.end(); ++iter)
    {
        if((*iter).start != nullptr) {
            (*iter).setStart((*(*iter).start) * scale);
        }
        if((*iter).end != nullptr) {
            (*iter).setEnd((*(*iter).end) * scale);
        }
        if((*iter).center != nullptr) {
            (*iter).setCenter((*(*iter).center) * scale);
        }
        if((*iter).radiusX != nullptr) {
            (*iter).setRadiusX((*(*iter).radiusX) * scale);
        }
        if((*iter).radiusY != nullptr) {
            (*iter).setRadiusY((*(*iter).radiusY) * scale);
        }
    }
}

void ofxGCodePlotter::flipPaths(vector<GCodePath>& paths)
{
    for (auto iter = paths.begin(); iter != paths.end(); ++iter)
    {
        if (config.flipX)
        {
            if((*iter).start != nullptr) {
                float xStart = inWidth - (*(*iter).start).x;
                (*iter).setStart(ofPoint(xStart, (*(*iter).start).y));
            }
            if((*iter).end != nullptr) {
                float xEnd = inWidth - (*(*iter).end).x;
                (*iter).setEnd(ofPoint(xEnd, (*(*iter).end).y));
            }
            if((*iter).center != nullptr) {
                float xC = inWidth - (*(*iter).center).x;
                (*iter).setCenter(ofPoint(xC, (*(*iter).center).y));
            }
            
            if ((*iter).type == GCodePath::Type::Arc) {
                (*iter).type = GCodePath::Type::ArcNegative;
                int i = 0;
            }
            else if ((*iter).type == GCodePath::Type::ArcNegative)
            {
                (*iter).type = GCodePath::Type::Arc;
                int i = 0;
            }
        }
        
        if (config.flipY)
        {
            if((*iter).start != nullptr) {
                float yStart = inHeight - (*(*iter).start).y;
                (*iter).setStart(ofPoint((*(*iter).start).x, yStart));
            }
            if((*iter).end != nullptr) {
                float yEnd = inHeight - (*(*iter).end).y;
                (*iter).setEnd(ofPoint((*(*iter).end).x, yEnd));
            }
            if((*iter).center != nullptr) {
                float yC = inHeight - (*(*iter).center).y;
                (*iter).setCenter(ofPoint((*(*iter).center).x, yC));
            }
            if((*iter).radiusX != nullptr) {
                float radiusY = inHeight - (*(*iter).radiusY);
                (*iter).setRadiusY(radiusY);
            }
            
            if ((*iter).type == GCodePath::Type::Arc) {
                (*iter).type = GCodePath::Type::ArcNegative;
                int i = 0;
            }
            else if ((*iter).type == GCodePath::Type::ArcNegative)
            {
                (*iter).type = GCodePath::Type::Arc;
                int i = 0;
            }
        }
    }
}

ofPoint ofxGCodePlotter::pointOnCircle(ofPath::Command cmd)
{
    return pointOnCircle(cmd.to, cmd.angleBegin, cmd.radiusX, cmd.radiusY);
}

ofPoint ofxGCodePlotter::pointOnCircle(ofPoint center, float angle, float radiusX, float radiusY)
{
    ofPoint point;
    M_PI;
    point.x = center.x + (cos(angle * (M_PI/180.0)) * radiusX);
    point.y = center.y + (sin(angle * (M_PI/180.0)) * radiusY);
    return point;
}

bool ofxGCodePlotter::positionIsOutOfSync(ofPath::Command cmd, ofPoint startPos)
{
    ofPoint pathStart = pointOnCircle(cmd.to, cmd.angleBegin, cmd.radiusX, cmd.radiusY);
    return (startPos.x != pathStart.x || startPos.y != pathStart.y);
}

void ofxGCodePlotter::print(vector<GCodePath> paths)
{
    for (auto iter = paths.begin(); iter != paths.end(); ++iter)
    {
        cout << (*iter).print() << "\n";
    }
}
