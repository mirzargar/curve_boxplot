#include "PathCBD.h"

PathCBD::PathCBD(Ensemble e, string date, string param, string svg_name, string coordinate_bound_filename, int palette_number){
    if(e.get_size() <= 3){
        cerr << "the size of the ensemble should be at least 4" << endl;
        exit(-1);
    }

#ifdef DEBUG
    cout << "constructor number 1" << endl;
#endif

    E = Ensemble(e);
    Probs.resize(E.ESize, 0.0);

#ifdef DEBUG
    cout<<"about to run the fast analysis"<<endl;
#endif

    RunAnalysisFastMinLength(date, param);

    vector<int> SortedIndx = getSortedIndex(Probs);

    cout << endl;

    stringstream ss;
    ss.str(string());
    ss.clear();
    ss << svg_name << ".svg";

    genSVG_continous(SortedIndx, ss.str(), coordinate_bound_filename, palette_number-1);

#ifdef DEBUG
    cout<<"generated the svg"<<endl;
#endif

}

void get_window_size(double& window_width, double& window_height){
    // NOTE: DO NOT USE THIS FUNCTION - IT REQUIRES ntohl WHICH DOES NOT WORK ON MANDAM (UCAR)
    ifstream in(BACKGROUND_FILENAME.c_str());
    unsigned int width, height;
    //double window_width, window_height;


    in.seekg(16);
    in.read((char *)&width, 4);
    in.read((char *)&height, 4);

}

void PathCBD::genSVG(vector<int> Indx, string filename){
    //Note that work for path ensemble of same length //TODO

    //find number of zeros in the probability to set as the number of outliers
    //if there is no zero probability, the number of ourliers will be set to 3
    for(int i=Indx.size()-1; i>=0; i--){
        if(Probs[Indx[i]] > 0.01)
            break;
        else
            ;//outlierNum++;
    }

    //double window_width, window_height;
    //get_window_size(window_width, window_height);
    InitializeSVG(filename, BACKGROUND_FILENAME, window_width, window_height);

    ////////////////////
    //// Draw outliers
    ////////////////////
    Points outliers;

    for(int i=0; i<Indx.size(); i++){
        outliers.clear();
        //TODO:make everything use Point_2
        int length = E.path_set[Indx[i]]->get_size();
        for(int j=0; j<length; j++){
            outliers.push_back(Point_2(E.path_set[Indx[i]]->get(j)->x, E.path_set[Indx[i]]->get(j)->y));
        }
        string color = get_hex_color();
        DrawPath(filename, outliers, color.c_str(), 2, "blue", "none", window_width, window_height);
    }

    CloseSVGFile(filename);
}

void PathCBD::get_BG_img_info(double& minlat, double& maxlat, double& minlon, double& maxlon, double& VPX, double& VPY, double& VPW, double& VPH, string coordinate_bound_filename){
    //string file_name = BG_INFO_FILENAME;

    ifstream data(coordinate_bound_filename.c_str());

    if(!data.is_open()){
        cerr<< "Bad file: " << coordinate_bound_filename.c_str() << endl;
        exit(-1);
    }

    while(!data.eof()){
        data >> VPX >> VPY >> VPW >> VPH >> minlat >> maxlat >> minlon >> maxlon;
    }

    //cout << VPX << ", " << VPY << ", " << VPW << ", " << VPH << ", " << minlat << ", " << maxlat << ", " << minlon << ", " << maxlon << endl;
    data.close();
}

// change of coordinate system - purely required for NCAR intergration
Point_2 transform_to_windowspace(Point_2 p, double latwidth, double lonwidth, double window_width, double window_height, double minlat, double minlon, double VPX, double VPY, double VPW, double VPH){
    Point_2 to_return((p.x()-minlat)/latwidth*VPH*window_width + (1-VPY)*window_width, ((p.y() - minlon)/lonwidth*VPW+VPX)*window_height);
    return to_return;
}

void PathCBD::genSVG_paths(vector<int> Indx, string filename, string coordinate_bound_filename){

    int stroke_width = 2;
    double minlat, maxlat, minlon, maxlon;
    double VPX, VPY, VPW, VPH;

    get_BG_img_info(minlat, maxlat, minlon, maxlon, VPX, VPY, VPW, VPH, coordinate_bound_filename);

    double latwidth = maxlat - minlat;

    double lonwidth = maxlon - minlon;


    //Note that work for path ensemble of same length //TODO
    unsigned int BandWidth = floor(E.ESize/2), outlierNum = 0, envelopSize = 0;

    //find number of zeros in the probability to set as the number of outliers
    //if there is no zero probability, the number of ourliers will be set to 3
    for(int i=E.ESize-1; i>=0; i--){
        //cout << Probs[i] << endl;
        if(Probs[Indx[i]] > 0.005)
            break;
        else
            outlierNum++;
    }

    if(outlierNum == 0)
        outlierNum = 3;

    envelopSize = E.ESize- outlierNum;


    InitializeSVG(filename, BACKGROUND_FILENAME, window_width, window_height);

#ifdef DEBUG
    cout << "drawing envelop: " << (E.ESize-outlierNum) << endl;
#endif

    /////////////////
    //// Drawing envelop
    /////////////////

    //TODO: make everything use Point_2
    Points envelop;

    for(int i=0; i<envelopSize; i++){
        envelop.clear();
        //TODO:make everything use Point_2
        int length = E.path_set[Indx[i]]->get_size();
        for(int j=0; j<length; j++){
            //envelop.push_back(Point_2(E.path_set[Indx[i]]->get(j)->x, E.path_set[Indx[i]]->get(j)->y));
            envelop.push_back(transform_to_windowspace(Point_2(E.path_set[Indx[i]]->get(j)->x/10.0, E.path_set[Indx[i]]->get(j)->y/10.0), latwidth, lonwidth, window_width, window_height, minlat, minlon, VPX, VPY, VPW, VPH));
        }
        DrawPath(filename, envelop, envelop_color[0], stroke_width, envelop_color[0], "none", window_width, window_height);
    }

#ifdef DEBUG
    cout<< "drawing band: " << BandWidth << endl;
#endif

    //////////////////
    //// Drawing band
    //////////////////
    //push the paths that will form the 50% percentile to a vector

    Points band;

    for(int i=0; i<BandWidth; i++){
        band.clear();
        //TODO:make everything use Point_2
        int length = E.path_set[Indx[i]]->get_size();
        for(int j=0; j<length; j++){
            band.push_back(transform_to_windowspace(Point_2(E.path_set[Indx[i]]->get(j)->x/10.0, E.path_set[Indx[i]]->get(j)->y/10.0), latwidth, lonwidth, window_width, window_height, minlat, minlon, VPX, VPY, VPW, VPH));
        }
        //DrawPath(filename, band, "#54278f", 2, "#54278f", "none", false);
        DrawPath(filename, band, band_color[0], stroke_width, band_color[0], "none", window_width, window_height);
    }

#ifdef DEBUG
    cout << "drawing median" << endl;
#endif

    ////////////////////
    //// Draw the median
    ////////////////////
    Points median;

    //TODO: make everything use Point_2
    int length = E.path_set[Indx[0]]->get_size();
    for(int i=0; i<length; i++){
        median.push_back(transform_to_windowspace(Point_2(E.path_set[Indx[0]]->get(i)->x/10.0, E.path_set[Indx[0]]->get(i)->y/10.0), latwidth, lonwidth, window_width, window_height, minlat, minlon, VPX, VPY, VPW, VPH));
    }
    DrawPath(filename, median, median_color[0], stroke_width, median_color[0], "none", window_width, window_height);

    // debugging
    //Points debugg;
    //debugg.push_back(transform_to_windowspace(Point_2(maxlat, minlon), latwidth, lonwidth, window_width, window_height, minlat, minlon, VPX, VPY, VPW, VPH));
    //debugg.push_back(transform_to_windowspace(Point_2(minlat, maxlon), latwidth, lonwidth, window_width, window_height, minlat, minlon, VPX, VPY, VPW, VPH));

    ////////////////////
    //// Draw outliers
    ////////////////////
    Points outliers;

    for(int i=E.ESize-outlierNum; i<E.ESize; i++){
        outliers.clear();
        //TODO:make everything use Point_2
        int length = E.path_set[Indx[i]]->get_size();
        for(int j=0; j<length; j++){
            //outliers.push_back(Point_2(E.path_set[Indx[i]]->get(j)->x, E.path_set[Indx[i]]->get(j)->y));
            outliers.push_back(transform_to_windowspace(Point_2(E.path_set[Indx[i]]->get(j)->x/10.0, E.path_set[Indx[i]]->get(j)->y/10.0), latwidth, lonwidth, window_width, window_height, minlat, minlon, VPX, VPY, VPW, VPH));
        }
        DrawPath(filename, outliers, outlier_color[0], stroke_width, outlier_color[0], "none", window_width, window_height);
    }

    CloseSVGFile(filename);
}

void PathCBD::genSVG_continous(vector<int> Indx, string filename, string coordinate_bound_filename, int palette_number){

    //int stroke_width = 4;
    double minlat, maxlat, minlon, maxlon;
    double VPX, VPY, VPW, VPH;

    get_BG_img_info(minlat, maxlat, minlon, maxlon, VPX, VPY, VPW, VPH, coordinate_bound_filename);

    double latwidth = maxlat - minlat;

    double lonwidth = maxlon - minlon;

    //Note that work for path ensemble of same length //TODO
    vector<Path*> PathSet;
    Points pointSet, convexSet1, convexUnion;
    unsigned int BandWidth = floor(E.ESize/2), outlierNum = 0, envelop = 0;

    //find number of zeros in the probability to set as the number of outliers
    //if there is no zero probability, the number of ourliers will be set to 3
    for(int i=E.ESize-1; i>=0; i--){
        //cout << Probs[i] << endl;
        if(Probs[Indx[i]] > 0.005)
            break;
        else
            outlierNum++;
    }

    //cout << "====================== c" << outlierNum << endl;

    envelop = Indx.size()- outlierNum;

    //double window_width, window_height;
    //get_window_size(window_width, window_height);
    InitializeSVG(filename, BACKGROUND_FILENAME, window_width, window_height);

#ifdef DEBUG
    cout << "drawing envelop: " << (Indx.size()-outlierNum) << endl;
#endif

    /////////////////
    //// Drawing envelop
    /////////////////
    //push the paths that will form the band - outliers
    PathSet.clear();
    for(int i=0; i<envelop; i++)
        PathSet.push_back(E.path_set[Indx[i]]);

    //go through the pathset and form the convexhull for each time step
    for(int i=0; i<E.max_path_length-1; i++){
        pointSet.clear(); convexSet1.clear();
        for(int j=0; j<envelop; j++){
            if( i >= PathSet[j]->get_size()){
                pointSet.push_back(transform_to_windowspace(Point_2(PathSet[j]->get(PathSet[j]->get_size()-1)->x/10.0, PathSet[j]->get(PathSet[j]->get_size()-1)->y/10.0), latwidth, lonwidth, window_width, window_height, minlat, minlon, VPX, VPY, VPW, VPH));
            }else{
                pointSet.push_back(transform_to_windowspace(Point_2(PathSet[j]->get(i)->x/10.0, PathSet[j]->get(i)->y/10.0), latwidth, lonwidth, window_width, window_height, minlat, minlon, VPX, VPY, VPW, VPH));
            }
            if( i+1 >= PathSet[j]->get_size()){
                pointSet.push_back(transform_to_windowspace(Point_2(PathSet[j]->get(PathSet[j]->get_size()-1)->x/10.0, PathSet[j]->get(PathSet[j]->get_size()-1)->y/10.0), latwidth, lonwidth, window_width, window_height, minlat, minlon, VPX, VPY, VPW, VPH));
            }else{
                pointSet.push_back(transform_to_windowspace(Point_2(PathSet[j]->get(i+1)->x/10.0, PathSet[j]->get(i+1)->y/10.0), latwidth, lonwidth, window_width, window_height, minlat, minlon, VPX, VPY, VPW, VPH));
            }
        }
        convexSet1 = getConvexHull(pointSet);
        convexUnion = get_polygon_union(convexSet1, convexUnion);
    }
    FillPolygon(filename, convexUnion, envelop_color[palette_number], envelop_color[palette_number], 1, .5, false, window_width, window_height);

    //draws the paths
    Points envelopPath;

    for(int i=0; i<envelop; i++){
        envelopPath.clear();
        //TODO:make everything use Point_2
        int l = E.path_set[Indx[i]]->get_size();
        for(int j=0; j<l; j++){
            envelopPath.push_back(transform_to_windowspace(Point_2(E.path_set[Indx[i]]->get(j)->x/10.0, E.path_set[Indx[i]]->get(j)->y/10.0), latwidth, lonwidth, window_width, window_height, minlat, minlon, VPX, VPY, VPW, VPH));
        }
        DrawPath(filename, envelopPath, envelop_color[palette_number], 1, envelop_color[palette_number], "none", window_width, window_height);
    }

    convexSet1.clear();
    convexUnion.clear();

#ifdef DEBUG
    cout<< "drawing band: " << BandWidth << endl;
#endif

    //////////////////
    //// Drawing band
    //////////////////
    //push the paths that will form the 50% percentile to a vector
    for(int i=0; i<BandWidth; i++)
        PathSet.push_back(E.path_set[Indx[i]]);

    //go through the pathset and form the convexhull for each time step
    for(int i=0; i<E.max_path_length-1; i++){
        pointSet.clear(); convexSet1.clear();
        for(int j=0; j<BandWidth; j++){
            if( i >= PathSet[j]->get_size()){
                //pointSet.push_back(Point_2(PathSet[j]->get(PathSet[j]->get_size()-1)->x, PathSet[j]->get(PathSet[j]->get_size()-1)->y));
                pointSet.push_back(transform_to_windowspace(Point_2(PathSet[j]->get(PathSet[j]->get_size()-1)->x/10.0, PathSet[j]->get(PathSet[j]->get_size()-1)->y/10.0), latwidth, lonwidth, window_width, window_height, minlat, minlon, VPX, VPY, VPW, VPH));
            }else{
                //pointSet.push_back(Point_2(PathSet[j]->get(i)->x, PathSet[j]->get(i)->y));
                pointSet.push_back(transform_to_windowspace(Point_2(PathSet[j]->get(i)->x/10.0, PathSet[j]->get(i)->y/10.0), latwidth, lonwidth, window_width, window_height, minlat, minlon, VPX, VPY, VPW, VPH));
            }
            if( i+1 >= PathSet[j]->get_size()){
                //pointSet.push_back(Point_2(PathSet[j]->get(PathSet[j]->get_size()-1)->x, PathSet[j]->get(PathSet[j]->get_size()-1)->y));
                pointSet.push_back(transform_to_windowspace(Point_2(PathSet[j]->get(PathSet[j]->get_size()-1)->x/10.0, PathSet[j]->get(PathSet[j]->get_size()-1)->y/10.0), latwidth, lonwidth, window_width, window_height, minlat, minlon, VPX, VPY, VPW, VPH));
            }else{
                //pointSet.push_back(Point_2(PathSet[j]->get(i+1)->x, PathSet[j]->get(i+1)->y));
                pointSet.push_back(transform_to_windowspace(Point_2(PathSet[j]->get(i+1)->x/10.0, PathSet[j]->get(i+1)->y/10.0), latwidth, lonwidth, window_width, window_height, minlat, minlon, VPX, VPY, VPW, VPH));
            }
        }
        convexSet1 = getConvexHull(pointSet);
        convexUnion = get_polygon_union(convexSet1, convexUnion);
    }
    FillPolygon(filename, convexUnion, band_color[palette_number], band_color[palette_number], 1, .5, false, window_width, window_height);

    //draws the paths
    //Points envelopPath;
    for(int i=0; i<BandWidth-1; i++){
        envelopPath.clear();
        //TODO:make everything use Point_2
        int l = E.path_set[Indx[i]]->get_size();
        for(int j=0; j<l; j++){
            envelopPath.push_back(transform_to_windowspace(Point_2(E.path_set[Indx[i]]->get(j)->x/10.0, E.path_set[Indx[i]]->get(j)->y/10.0), latwidth, lonwidth, window_width, window_height, minlat, minlon, VPX, VPY, VPW, VPH));
        }
        DrawPath(filename, envelopPath, band_color[palette_number], 0.7, band_color[palette_number], "none", window_width, window_height);
    }

#ifdef DEBUG
    cout << "drawing median" << endl;
#endif

    ////////////////////
    //// Draw the median
    ////////////////////
    Points median;

    //TODO: make everything use Point_2
    int length = E.path_set[Indx[0]]->get_size();
    //for(int i=0; i<E.max_path_length; i++){
    for(int i=0; i<length; i++){
        median.push_back(transform_to_windowspace(Point_2(E.path_set[Indx[0]]->get(i)->x/10.0, E.path_set[Indx[0]]->get(i)->y/10.0), latwidth, lonwidth, window_width, window_height, minlat, minlon, VPX, VPY, VPW, VPH));
    }
    DrawPath(filename, median, median_color[palette_number], 2, median_color[palette_number], "none", window_width, window_height);

    ////////////////////
    //// Draw outliers
    ////////////////////
    Points outliers;

    for(int i=Indx.size()-outlierNum; i<Indx.size(); i++){
        outliers.clear();
        //TODO:make everything use Point_2
        int length = E.path_set[Indx[i]]->get_size();
        for(int j=0; j<length; j++){
            outliers.push_back(transform_to_windowspace(Point_2(E.path_set[Indx[i]]->get(j)->x/10.0, E.path_set[Indx[i]]->get(j)->y/10.0), latwidth, lonwidth, window_width, window_height, minlat, minlon, VPX, VPY, VPW, VPH));
        }
        DrawPath(filename, outliers, outlier_color[palette_number], 1.4, outlier_color[palette_number], "none", window_width, window_height);
    }

    CloseSVGFile(filename);
}


vtkSmartPointer<vtkPolygon> PathCBD::getTriangle(vector<Point2D> v){
    //forms a vtk triangle

    vtkSmartPointer<vtkPolygon> polygon =
      vtkSmartPointer<vtkPolygon>::New();
    for(int i=0; i<v.size(); i++){
        polygon->GetPoints()->InsertNextPoint(v[i].x, v[i].y, 0.0);
    }
    return polygon;
}

int PathCBD::PointInPolygonCheck(vtkSmartPointer<vtkPolygon> polygon, Point2D p){
    //check whether a point is inside a triangle or not
    double n[3], test[3];
    int InVal;
    test[0] = p.x; test[1] = p.y; test[2] = 0.0;

    polygon->ComputeNormal(polygon->GetPoints()->GetNumberOfPoints(),
              static_cast<double*>(polygon->GetPoints()->GetData()->GetVoidPointer(0)), n);

    double bounds[6];
      polygon->GetPoints()->GetBounds(bounds);

    InVal = polygon->PointInPolygon(test,
          polygon->GetPoints()->GetNumberOfPoints(), static_cast<double*>(
          polygon->GetPoints()->GetData()->GetVoidPointer(0)), bounds, n);

    return InVal;
}


vector<int> PathCBD::getPathPointInCheck(vector< vtkSmartPointer<vtkPolygon> > band, Path* p){
    // can handle multi length paths by replicating the last point

    vector<int> In;
    Point2D point = Point2D(0.0, 0.0);

    for(int i=0; i < E.max_path_length; i++){
        if( i >= p->get_size())
            point = Point2D(p->get(p->get_size()-1));
        else
            point = Point2D(p->get(i));
        In.push_back(max(double(PointInPolygonCheck(band[i], point)), 0.0));
    }
    return In;
}

// consutrcting the band in a proper format
vector< vtkSmartPointer<vtkPolygon> > PathCBD::getBand(vector<Path*> PSet){
    vector< vtkSmartPointer<vtkPolygon> > toReturn;
    vtkSmartPointer<vtkPolygon> polygon = vtkSmartPointer<vtkPolygon>::New();
    vector<Point2D> PointSet;

    for(int i=0; i < E.max_path_length; i++){
        PointSet.clear();
        for(int j=0; j<PSet.size(); j++){
            if(i >= PSet[j]->get_size())
                PointSet.push_back(Point2D(PSet[j]->get(PSet[j]->get_size()-1)));//TODO: not sure would work
            else
                PointSet.push_back(Point2D(PSet[j]->get(i)));//TODO: not sure would work
        }
        if(PointSet.size() == 3){
            Point2D p1 = PointSet[0];
            Point2D p2 = PointSet[1];
            Point2D p3 = PointSet[2];
            double check = (p2.x - p1.x)*(p3.y - p1.y) - (p2.y - p1.y)*(p3.x - p1.x);
            if (check < 0){
                PointSet.clear();
                PointSet.push_back(p3);
                PointSet.push_back(p2);
                PointSet.push_back(p1);
            }
        }
        polygon = getTriangle((PointSet));
        toReturn.push_back(polygon);
    }

    return toReturn;
}


vector< vtkSmartPointer<vtkPolygon> > PathCBD::getBandMinLength(vector<Path*> PSet){
    vector< vtkSmartPointer<vtkPolygon> > toReturn;
    vtkSmartPointer<vtkPolygon> polygon = vtkSmartPointer<vtkPolygon>::New();
    vector<Point2D> PointSet;

    int minL = 1000;

    for (int i=0; i < PSet.size(); i++)
        if(PSet[i]->get_size() < minL)
            minL = PSet[i]->get_size();

    for(int i=0; i < minL; i++){
        PointSet.clear();
        for(int j=0; j<PSet.size(); j++){
                PointSet.push_back(Point2D(PSet[j]->get(i)));
        }
        if(PointSet.size() == 3){
            Point2D p1 = PointSet[0];
            Point2D p2 = PointSet[1];
            Point2D p3 = PointSet[2];
            double check = (p2.x - p1.x)*(p3.y - p1.y) - (p2.y - p1.y)*(p3.x - p1.x);
            if (check < 0){
                PointSet.clear();
                PointSet.push_back(p3);
                PointSet.push_back(p2);
                PointSet.push_back(p1);
            }
        }
        polygon = getTriangle((PointSet));
        toReturn.push_back(polygon);
    }

    return toReturn;
}

void PathCBD::RunAnalysisFastMinLength(string date, string param){

#ifdef DEBUG
    cout << "running the min length analysis" << endl;
#endif

    Matrix<int> M; //combination matrix
    vector<Path*> PathSet; // holds the set of paths forming the band
    vector<int> In;
    stringstream ss;

#ifdef DEBUG
    ss << "analysis_sum_" << date << "_" << param << E.ESize << ".txt";
    ofstream data2(ss.str().c_str());
#endif
    vector< vtkSmartPointer<vtkPolygon> > band;


    /* original and exact code */
    M = getCombinationMatrix(E.ESize, 3);

#ifdef DEBUG
    cout << "combinatorial size: " << M.getXdim() << endl;
#endif

    for(int i=0; i<M.getXdim(); i++){ // goes over combination
        PathSet.clear();
        for(int j=0; j<M.getYdim(); j++){ //loops over choices
            PathSet.push_back(new Path(E.path_set[M(i,j)-1]));
        }

#ifdef DEBUG
        data2 << M(i,0) << ", " << M(i,1) << ", " << M(i,2) << ", ";
#endif
        band.clear();
        band = getBand(PathSet);
            for(int l=0; l<E.ESize; l++){ // goes through ensemble members
                if(IsElement(l+1, M.SubRows(vector<int>(1,i)))){ // checks if a value is in an int vector

#ifdef DEBUG
                    data2 << 0.0 << ", ";
#endif
                    continue;
                }else{
                    In = getPathPointInCheck(band, E.path_set[l]);
                }
                double sum = 0.0;
                for(int k=0; k<In.size(); k++){
                    Probs[l] += double(In[k])/double(In.size());
                    sum += double(In[k]);
                }
#ifdef DEBUG
                data2 << sum/double(In.size()) << ", ";
#endif
            }
#ifdef DEBUG
            data2 << endl;
#endif
    }

#ifdef DEBUG
    data2.close();
#endif

    for(int i=0; i<Probs.size(); i++)
        Probs[i] /= M.getXdim();

}
