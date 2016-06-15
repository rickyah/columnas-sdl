//
//  DataTypes.hpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 25/5/16.
//
//

#ifndef DataTypes_hpp
#define DataTypes_hpp

#include <initializer_list>

/*
 * represents a Position or a Size
 */
struct Position
{
  
    int x;
    int y;
    
    Position():Position(0,0) {}

    Position(int X, int Y):x(X),y(Y) {}

};

struct Size
{
    int w;
    int h;
    
    Size():Size(0,0) {}
    
    Size(int W, int H):w(W),h(H) {}
};

struct Rect
{
    Rect(Position pos, Size size):position(pos), size(size) {}

    Rect(int x, int y, int w, int h):position(Position(x,y)), size(Size(w,h)) {}

    Position position;
    Size size;
};

/*
 * Holds data about time:  framecount, and total elapsed ms
 */
struct TimeInfo
{
    int frameCount;
    int elapsedMs;
};

struct LogicFrameInfo : public TimeInfo
{
    // delta time for the current game frame
    double dt;
};

struct RenderFrameInfo : public TimeInfo
{
    // Percent of the current game frame we are rendering
    double interpolation;
};

#endif /* DataTypes_hpp */
