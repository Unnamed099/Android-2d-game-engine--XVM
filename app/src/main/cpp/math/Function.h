#ifndef CPP_MATH_FUNCTION_H
#define CPP_MATH_FUNCTION_H

#include <cmath>

class Function
{
public:
    Function();

public:
    static double interpolate(double x0, double y0, double x1, double y1, double x2, double y2, double x);
    static float angle_swap(int dx, int dy);
};

#endif // CPP_MATH_FUNCTION_H