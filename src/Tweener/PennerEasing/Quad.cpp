#include "Quad.h"

float Quad::easeIn (float t,float b , float c, float d) {
    t = t/d;
	
    return (c * t * t) + b;
}
float Quad::easeOut(float t,float b , float c, float d) {
    t = t/d;
    
	return (-c * t *(t-2)) + b;
}

float Quad::easeInOut(float t,float b , float c, float d) {
    t = (t/d) * 2;
    
	if (t < 1)
    {
        return ((c/2)*(t*t)) + b;
    }
    
    return -c/2 * (((t-2)*(t-1)) - 1) + b;
	/*
	originally return -c/2 * (((t-2)*(--t)) - 1) + b;

	I've had to swap (--t)*(t-2) due to diffence in behaviour in
	pre-increment operators between java and c++, after hours
	of joy
	*/

}