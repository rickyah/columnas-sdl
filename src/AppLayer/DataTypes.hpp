//
//  DataTypes.hpp
//
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

    bool operator==(const Position &other) const { return other.x == x && other.y == y; }
    bool operator!=(const Position &other) const { return !this->operator==(other); }
    
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
    Rect():Rect(0,0,0,0){}
    
    Rect(Position position_, Size size_):position(position_), size(size_) {}

    Rect(int x, int y, int w, int h):position(Position(x,y)), size(Size(w,h)) {}

    bool Contains(Position pos) const
    {
        return pos.x >= position.x
            && pos.y >= position.y
            && pos.x <= position.x + size.w
            && pos.y <= position.y + size.h;
    }
    
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
    float dt;
};

struct RenderFrameInfo : public TimeInfo
{
    // Percent of the current game frame we are rendering
    float interpolation;
};



struct Color
{
    unsigned short r, g, b;
};


#endif /* DataTypes_hpp */
