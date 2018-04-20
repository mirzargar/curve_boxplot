#include "Utility.h"
#include "Point2D.h"

Point2D::Point2D(double x1, double y1){
    set(x1, y1);
}

Point2D::Point2D(const Point2D &p){
    set(p);
}

Point2D::Point2D(const Point2D* p){
    x = p->x;
    y = p->y;
}

double& Point2D::operator[](int i){
    if(i<0 || i>1){
        cerr <<"2D point index bounds error" <<endl;
        exit(1);
    }
    switch(i){
    case 0:
        return x;
    default:
        return y;
    }
}
const double& Point2D::operator[](int i)const{
    if(i < 0 || i > 1){
      cerr << "2D point index bounds error" << endl;
      exit(1);
    }

    switch(i){
    case 0:
      return x;
    default:
      return y;
    }
}

void Point2D::print() const{
    cout << "[" << x << " " << y << "]";
}
void Point2D::set(double x1=0, double y1=0){
    x = x1;
    y = y1;
}
void Point2D::set(const Point2D &p){
    x = p.x;
    y = p.y;
}
