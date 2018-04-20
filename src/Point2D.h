#ifndef __POINT_H_INCLUDED__
#define __POINT_H_INCLUDED__

// *********
// A simple class to give us easy 2D point accees
// *********

#include <iostream>

class Point2D{
public:
    double x, y;
    Point2D(double x1, double y1);
    Point2D(const Point2D &p);
    Point2D(const Point2D* p);

    double& operator[](int i);
    const double& operator[](int i)const;

    void print() const;
    void set(double x1, double y1);
    void set(const Point2D &p);

};

#endif
