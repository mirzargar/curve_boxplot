#ifndef UTILITY_H
#define UTILITY_H


// *********
// This class includes various combinatorial and geometrical routines that are required
// (uses CGAL and VTK for some operations)
// *********

#include <sstream>
#include "Matrix.h"
#include "Point2D.h"
#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkLine.h>
#include <vtkPolygon.h>
#include <vtkPolyLine.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/convex_hull_2.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Boolean_set_operations_2.h>

#define _USE_MATH_DEFINES
#define DEBUG

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point_2;
typedef std::vector<Point_2> Points;
typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef Kernel::Point_2                                   PolyPoint_2;
typedef CGAL::Polygon_2<Kernel>                           Polygon_2;
typedef CGAL::Polygon_with_holes_2<Kernel>                Polygon_with_holes_2;
typedef std::list<Polygon_with_holes_2>                   Pwh_list_2;
//typename CGAL::Polygon_2<Kernel, Container>::Vertex_const_iterator  Vertex_iterator;

typedef pair<int, int> IntPairVector;
typedef pair<double, int> my_pair;

/////////////////
// Combinatorics
/////////////////
unsigned long choose(unsigned long n, unsigned long k);
template <typename Iterator>
bool next_combination(const Iterator first, Iterator k, const Iterator last);
Matrix<int> getCombinationMatrix(unsigned int n, unsigned int k);

// maybe have an image class and put these for image class
Matrix<int> zeroCrossing(Matrix<double> M);

//template <typename T>
Matrix<int> getLevelSetMask(Matrix<double> m, double levelSet);
Matrix<int> getBinaryUnion(Matrix<int> m, Matrix<int> n);
Matrix<int> getBinaryIntersect(Matrix<int> m, Matrix<int> n);
double getPercentBinarySubset(Matrix<int> m, Matrix<int> n);

string StringCat(string s1, string s2);
bool IsElement(int p, Matrix<int> M);
vector<int> getSortedIndex(const vector<double>& v);
string get_hex_color();

//////////////////
// Geomertries
/////////////////

double dist2D(Point2D p1, Point2D p2);
int minUtil(double x1, double x2, double x3);

//vector<Point2D*> getConvexHull(vector<Point2D*> in);
Points getConvexHull(Points in);

template<class Kernel, class Container>
void print_polygon(const CGAL::Polygon_2<Kernel, Container>& P){
  typename CGAL::Polygon_2<Kernel, Container>::Vertex_const_iterator  vit;

  //cout << "[ " << P.size() << " vertices:";
  for (vit = P.vertices_begin(); vit != P.vertices_end(); ++vit){
  //for(vit = this->polygon.vectices_begin(); vit != this->polygon.vertices_end(); ++vit)
      //cout << *vit << endl;
      double dx = CGAL::to_double((*vit).x());
      double dy = CGAL::to_double((*vit).y());
      cout<< dx << " " << dy<< endl;
  }
  cout << endl;

  return;
}

template<class Kernel, class Container>
void print_polygon_with_holes(const CGAL::Polygon_with_holes_2<Kernel, Container>& pwh){

  if (! pwh.is_unbounded())
      print_polygon(pwh.outer_boundary());
  else
        cout << "{ Unbounded polygon." << endl;

  typename CGAL::Polygon_with_holes_2<Kernel,Container>::Hole_const_iterator  hit;
  unsigned int k = 1;

  cout << "  " << pwh.number_of_holes() << " holes:" <<endl;
}

Polygon_2 get_polygon(Points p);

void print_polygon_union(Points PSet1, Points PSet2);

Points get_polygon_union(Points PSet1, Points PSet2);

// *** projection code
Point2D LambertConformalConicProj(double lon, double lat);


#endif // UTILITY_H
