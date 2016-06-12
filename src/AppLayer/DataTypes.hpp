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
 * Holds data about time: delta time for the current frame, framecount, and total elapsed ms
 */
struct TimeInfo
{
    double dt;
    int frameCount;
    int elapsedMs;
};

#endif /* DataTypes_hpp */