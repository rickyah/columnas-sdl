//
//  DataTypes.hpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 25/5/16.
//
//

#ifndef DataTypes_hpp
#define DataTypes_hpp

typedef struct Position
{
    union { int x; int w; };
    union { int y; int h; };
    
} Position;

typedef Position Size;

struct Rect
{
    Position position;
    Size size;
};

#endif /* DataTypes_hpp */