#include "Polygon.h"

Polygon::Polygon(){
    Polygon_2 p;
    this->polygon = p;
}

Polygon::Polygon(vector<Point_2> pset){
    Polygon_2 p;
    this->polygon = p;
    for(int i=0; i<pset.size(); i++)
        this->polygon.push_back(PolyPoint_2(pset[i].x(), pset[i].y()));
    this->VNum = this->polygon.size();
}

bool Polygon::is_simple(){
    return this->polygon.is_simple();
}

bool Polygon::is_clk_wise(){
    if(this->polygon.orientation() == CGAL::CLOCKWISE)
        return true;
    else
        return false;
}

template<class Kernel, class Container>
void print_polygon(const CGAL::Polygon_2<Kernel, Container>& P){
  typename CGAL::Polygon_2<Kernel, Container>::Vertex_const_iterator  vit;

  //cout << "[ " << P.size() << " vertices:";
  for (vit = P.vertices_begin(); vit != P.vertices_end(); ++vit)
  //for(vit = this->polygon.vectices_begin(); vit != this->polygon.vertices_end(); ++vit)
      cout << *vit << endl;
  cout << endl;

  return;
}

template<class Kernel, class Container>
void print_CGAL_polygon_with_holes(const CGAL::Polygon_with_holes_2<Kernel, Container>& pwh){

  if (! pwh.is_unbounded())
      print_polygon(pwh);// (pwh.outer_boundary());
  else
        cout << "{ Unbounded polygon." << endl;

  typename CGAL::Polygon_with_holes_2<Kernel,Container>::Hole_const_iterator  hit;
  unsigned int k = 1;

  cout << "  " << pwh.number_of_holes() << " holes:" <<endl;
}


template<class Kernel, class Container>
void get_polygon_union(const CGAL::Polygon_2<Kernel, Container>& P1, const CGAL::Polygon_2<Kernel, Container>& P2){//(vector<Path> PSet1, vector<Path> PSet2){
    //TODO: maybe change the input to be CGAL polygons
    //TODO: for now only handles paths of the same size

    //Polygon_2 P1 = get_CGAL_polygon(PSet1),
    //        P2 = get_CGAL_polygon(PSet2);
    Polygon_with_holes_2 unionP;

    if(P1.orientation() == CGAL::CLOCKWISE)
       P1.reverse_orientation();

    if(P2.orientation() == CGAL::CLOCKWISE)
        P2.reverse_orientation();

    if(CGAL::join(P1, P2, unionP)){
        print_CGAL_polygon_with_holes(unionP);
    }else{
        cout << "disjoint polygons, union is trivial !!!" <<endl;
        exit(-1);
    }
}

template<class Kernel, class Container>
void Polygon::write(string filename){
    ofstream data(filename.c_str());
    typename CGAL::Polygon_2<Kernel, Container>::Vertex_const_iterator  vit;

    for(vit = this->polygon.vertices_begin(); vit != this->polygon.vertices_end(); ++vit)
        data << *vit << endl;
    data << endl;

    return;
}

/*
//TODO
vector<Polygon_2> Polygon::get_simple_polygons(){
    vector<Polygon_2> p;
    return p;
}
*/

