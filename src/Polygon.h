#ifndef __POLYGON_H_INCLUDED__
#define __POLYGON_H_INCLUDED__

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Boolean_set_operations_2.h>
#include <list>
//#include <iostream>
#include "Utility.h"

typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef Kernel::Point_2                                   PolyPoint_2;
typedef CGAL::Polygon_2<Kernel>                           Polygon_2;
typedef CGAL::Polygon_with_holes_2<Kernel>                Polygon_with_holes_2;
typedef std::list<Polygon_with_holes_2>                   Pwh_list_2;

using namespace std;

class Polygon{
public:
    Polygon_2 polygon;

    Polygon();
    Polygon(Points pset);
	bool is_simple();
	bool is_clk_wise();
    vector<Polygon_2> get_simple_polygons();
    template<class Kernel, class Container>
    friend void print_polygon(const CGAL::Polygon_2<Kernel, Container>& P);
    template<class Kernel, class Container>
    friend void print_CGAL_polygon_with_holes(const CGAL::Polygon_with_holes_2<Kernel, Container>& pwh);
    template<class Kernel, class Container>
    friend void get_polygon_union(const CGAL::Polygon_2<Kernel, Container>& P1, const CGAL::Polygon_2<Kernel, Container>& P2);
    template<class Kernel, class Container>
    void write(string filename);
    //Polygon::get_simple_polygons();


private:
//    Polygon_2 polygon;
	int VNum;
};

#endif
