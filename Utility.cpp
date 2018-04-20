#include "Utility.h"


// getting a random hex color
string get_hex_color(){
    int val = 0;
    stringstream ss;
    ss << '#';
    for (int i = 0; i < 6; i++){
        val = rand()%16;
        ss << std::hex << val;
    }
    return ss.str();
}

double dist2D(Point2D p1, Point2D p2){
    return sqrt(pow(p2.y-p1.y, 2.0)+pow(p2.x-p1.x, 2.0));
}

int minUtil(double x1, double x2, double x3){
    if(x1 < x2)
        return (x1 < x3) ? x1 : x3;
    else
        return (x2 < x3) ? x2 : x3;
}


//////////////////
// Combinatorics
//////////////////
// simple n choose k function!
unsigned long choose(unsigned long n, unsigned long k) {
    if (k > n) {
        return 0;
    }
    unsigned long r = 1;
    for (unsigned long d = 1; d <= k; ++d) {
        r *= n--;
        r /= d;
    }
    return r;
}

template <typename Iterator>
bool next_combination(const Iterator first, Iterator k, const Iterator last){
   /* Credits: Thomas Draper */
   if ((first == last) || (first == k) || (last == k))
      return false;
   Iterator itr1 = first;
   Iterator itr2 = last;
   ++itr1;
   if (last == itr1)
      return false;
   itr1 = last;
   --itr1;
   itr1 = k;
   --itr2;
   while (first != itr1){
      if (*--itr1 < *itr2){
         Iterator j = k;
         while (!(*itr1 < *j)) ++j;
         std::iter_swap(itr1,j);
         ++itr1;
         ++j;
         itr2 = k;
         std::rotate(itr1,j,last);
         while (last != j){
            ++j;
            ++itr2;
         }
         std::rotate(k,itr2,last);
         return true;
      }
   }
   std::rotate(first,k,last);
   return false;
}

// constructing the combination matrix required by data depth analysis
Matrix<int> getCombinationMatrix(unsigned int n, unsigned int k){
    std::vector<int> ints;
    for (unsigned int i = 1; i <= n; ints.push_back(i++));
    unsigned long chooseCount = choose(n,k);

    Matrix<int> combinations(chooseCount, k);

    int counter = 0;
    do{
       for (unsigned int i = 0; i < k; ++i)
            combinations(counter,i) = ints[i];
       counter++;
    }while(next_combination(ints.begin(),ints.begin() + k,ints.end()));
    return combinations;
}

// computing binary mask using isovalue zero
Matrix<int> zeroCrossing(Matrix<double> M){
    Matrix<int> toReturn(M.getXdim(), M.getYdim());
    for(unsigned int i=0; i<M.getXdim()-1;++i)
        for(unsigned int j=0; j<M.getYdim()-1;++j){
            double nx = M(i+1, j);
            double ny = M(i, j+1);
            double current = M(i,j);
            if((nx>0 && current<0) || (nx<0 && current>0) || (nx==0 && current>0) || (nx>0 && current==0)){
                if(fabs(nx)<fabs(current))
                    toReturn(i+1,j) = 1;
                else
                    toReturn(i,j) = 1;
            }
            if((ny>0 && current<0) || (ny<0 && current>0) || (ny==0 && current>0) || (ny>0 && current ==0)){
                if(fabs(ny)<fabs(current))
                    toReturn(i, j+1) = 1;
                else
                    toReturn(i, j) = 1;
            }
        }
    return toReturn;
}

// computing the binary mask from the scalar field
Matrix<int> getLevelSetMask(Matrix<double> m, double levelSet){
    Matrix<int> mask = Matrix<int>(m.getXdim(), m.getYdim(), 0);
    for(unsigned int i=0; i<m.getXdim(); ++i)
        for(unsigned int j=0; j<m.getYdim(); ++j){
            mask(i,j) = ((m(i,j) <= levelSet) ? 0 : 1);
        }
        return mask;
}

// A data depth uitlity function (union)
Matrix<int> getBinaryUnion(Matrix<int> m, Matrix<int> n){
    Matrix<int> temp(m.getXdim(), m.getYdim());
    temp = MaxMatrix(m, n);
    return temp;
}

// A data depth uitlity function (intersection)
Matrix<int> getBinaryIntersect(Matrix<int> m, Matrix<int> n){
    if((m.getXdim() != n.getXdim()) | (m.getYdim() != n.getYdim())){
        throw 1;
        return Matrix<int>();
    }

    Matrix<int> temp(m.getXdim(), m.getYdim());
    for(unsigned int i=0; i<temp.getXdim();++i)
        for(unsigned int j=0; j<temp.getYdim();++j){
            if(m(i,j) == 1 && n(i,j) == 1)
                temp(i,j) = 1;
        }
    return temp;
}

// A data depth uitlity function computing the band inclusion percentage
double getPercentBinarySubset(Matrix<int> m, Matrix<int> n){
    // TODO: check the value range
    if((m.getXdim() != n.getXdim()) | (m.getYdim() != n.getYdim())){
        throw 1;
        return -1.0;
    }

    Matrix<int> diffMatrix(m.getXdim(), m.getYdim());
    diffMatrix = getBinaryIntersect(m, n);
    double temp = 0.0;
    temp = double(diffMatrix.getSum())/double(m.getSum());
    return fabs(temp);
}


string StringCat(string s1, string s2){
    stringstream ss;
    string s3;
    ss.str("");
    ss << s1 << s2;
    s3 = ss.str();
    return s3;
}

bool IsElement(int p, Matrix<int> M){
    //check whether p is in the row vector M

    for(int i=0; i<M.getYdim(); i++)
        if(p == M(0,i))
            return true;
    return false;
}

bool sort_predD(const my_pair& left, const my_pair& right){
    return left.first > right.first;
}

vector<int> getSortedIndex(const vector<double>& v){
    vector<my_pair> VIndx;
    for(unsigned int i=0; i<v.size(); i++)
        VIndx.push_back(my_pair(v[i],i));
    sort(VIndx.begin(), VIndx.end(), sort_predD);
    vector<int> indx;
    for(unsigned int i=0; i<VIndx.size(); i++)
        indx.push_back(VIndx[i].second);
    return indx;
}



//////////////////
// Geomertries
/////////////////

// Comptues the convex hull of a group of points
Points getConvexHull(Points in){
    //Generates the counterclockwise sequence of extrem points from a given set of input points
    Points result;

    CGAL::convex_hull_2(in.begin(), in.end(), std::back_inserter(result));
    return result;
}

// changes the data structure
Polygon_2 get_polygon(Points p){
    Polygon_2 toReturn;
    for(int i=0; i<p.size(); i++)
        toReturn.push_back(PolyPoint_2(p[i].x(), p[i].y()));
    return toReturn;
}


//normal CGAL union function, return the union if the 2 polygon overlap
// and just prints if they are disjoint
void print_polygon_union(Points PSet1, Points PSet2){

    //form the polygon from the point sets
    Polygon_2 P1 = get_polygon(PSet1), P2 = get_polygon(PSet2);

    Polygon_with_holes_2 unionP;

    if(P1.orientation() == CGAL::CLOCKWISE)
       P1.reverse_orientation();

    if(P2.orientation() == CGAL::CLOCKWISE)
        P2.reverse_orientation();

    if(CGAL::join(P1, P2, unionP)){
        print_polygon_with_holes(unionP);
    }else{
        cout << "disjoint polygons, union is trivial !!!" <<endl;
        exit(-1);
    }
}

//modified CGAl union, if the 2 polygons are overlapping returns the union
//if the polygons are disjoint will return the convex hull
Points get_polygon_union(Points PSet1, Points PSet2){

    Points toReturn;
    Points UnionSet;

    if(PSet1.size() == 0 && PSet2.size()){
        cerr << "both sets are empty !!!!"<<endl;
        exit(-1);
    }

    //special case: if one is empty the union is trivial
    if(PSet1.size() == 0)
        return PSet2;

    if(PSet2.size() == 0)
        return PSet1;

    //form the polygon from the point sets
    Polygon_2 P1 = get_polygon(PSet1), P2 = get_polygon(PSet2);

    Polygon_with_holes_2 unionP;

    if(P1.orientation() == CGAL::CLOCKWISE)
       P1.reverse_orientation();

    if(P2.orientation() == CGAL::CLOCKWISE)
        P2.reverse_orientation();

    if(CGAL::join(P1, P2, unionP)){
        //checking to make sure the union is well behaved
        if (! unionP.is_unbounded()){
            Polygon_2 P = unionP.outer_boundary();
            typename CGAL::Polygon_2<Kernel>::Vertex_const_iterator  vit;

            for (vit = P.vertices_begin(); vit != P.vertices_end(); ++vit)
                toReturn.push_back(Point_2(CGAL::to_double((*vit).x()), CGAL::to_double((*vit).y())));
        }else{
              cout << "Unbounded polygon." << endl;
              exit(-1);
        }
    }else{
        //the polygons were disjoint, forming the convex hull
        for(int i=0; i<PSet1.size(); i++)
            UnionSet.push_back(PSet1[i]);

        for(int i=0; i<PSet2.size(); i++)
            UnionSet.push_back(PSet2[i]);
        toReturn = getConvexHull(UnionSet);
    }
    return toReturn;
}

// Computing the map proejction adopted by NOAA (called Lambert Conformal Conic)
Point2D LambertConformalConicProj(double lon, double lat){

    double R = 637.120;
    double lon0 = -95.0*M_PI/180.0;
    double lat0 = 35.0*M_PI/180.0;
    double SP1 = 25.0*M_PI/180.0;

    double n = sin(SP1);
    double theta = n*(lon-lon0);
    double F =  (cos(SP1)*pow(tan(M_PI/4+SP1/2), n))/n;
    double rho = (R*F)/(pow(tan(M_PI/4+lat/2), n));
    double rho0 = (R*F)/pow((tan(M_PI/4+lat0/2)), n);

    double x = rho*sin(theta);
    double y = rho0 - rho*cos(theta);

    return new Point2D(x, y);
}




