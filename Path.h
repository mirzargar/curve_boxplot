#ifndef __PATH_H_INCLUDED__
#define __PATH_H_INCLUDED__

// *********
// The data structure to hold each ensmeble member (in this case it is a multivariate curve or path)
// *********

#include "Utility.h"
#include "Point2D.h"

#include <vtkParametricSpline.h>
#include <vtkCardinalSpline.h>
#include <vtkSplineFilter.h>
#include <vtkCellArray.h>

class Path{
public:
    Path();
    Path(Path &P);
    Path(const Path *P);
    Path(string filename);
    Path(vector<Point2D *> pset_);

    int get_size() const;
    void load_path(string filename);
    void print_path();
    void write_path(string filename);
    void write_path(ofstream& file);
    const Point2D* operator()(int i) const;
    Point2D* get(int i) const;
    vector<Point2D*> get_pset(){return pset;}
    int get_length() {return length;}
    void path_reparam(string param);
    void get_bounds(int &minLA, int &maxLA, int &minLO, int &maxLO);

private:
	int length;
    std::vector<Point2D*> pset;
    void arclen_reparam();
    void eqnum_reparam();
};

#endif
