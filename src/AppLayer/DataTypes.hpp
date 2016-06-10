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
typedef struct Position
{
    union { int x; int w; };
    union { int y; int h; };
    
    Position():Position(0,0) {}

    Position(int X, int Y):x(X),y(Y) {}

} Position;
typedef Position Size;


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