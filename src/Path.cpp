#include "Path.h"

Path::Path(){}

Path::Path(Path &P){
    pset.clear();
    length = P.get_size();

    for(int i=0; i<P.get_size(); i++){
        pset.push_back(new Point2D(P(i)->x, P(i)->y));
    }
}

Path::Path(vector<Point2D*> pset_){
   pset.clear();
   length = pset_.size();
   for (int i = 0; i < pset_.size(); i++)
       pset.push_back(pset_[i]);
}

Path::Path(const Path *P){
    pset.clear();
    length = P->get_size();

    for(int i=0; i<P->get_size(); i++){
        pset.push_back(new Point2D(P->get(i)));
    }
}

Path::Path(string filename){
    load_path(filename);
}

int Path::get_size() const{
    return length;
}

void Path::load_path(string filename){
    pset.clear();
    vector<Point2D> toAdd;

    ifstream data(filename.c_str());

    if(!data.is_open()){
        cerr<< "Bad file name: " << filename.c_str() << endl;
        exit(-1);
    }

    double dx, dy;
    data >> dx >> dy;
    while(!data.eof()){

        pset.push_back(new Point2D(dx, dy));
        data >> dx >> dy;

        //Point2D p = LambertConformalConicProj(dy*M_PI/180.0, dx*M_PI/180.0);
        //pset.push_back(new Point2D(p.x+725.0, 400.0-p.y)); //issac
        //pset.push_back(new Point2D(p.x+350, 300-p.y)); //sandy
    }

    data.close();
    length = pset.size();
}

void Path::print_path(){
    if(length == 0){
        cout << "This path is empty !!!" << endl;
        return;
    }

    for(int i=0; i<length; i++)
        cout << pset[i]->x << ", " << pset[i]->y << endl;
    cout<<endl;
}

void Path::write_path(string filename){
    ofstream data(filename.c_str());
    if(!data.is_open()){
        cerr << "Bad file name: " << filename.c_str() << endl;
        exit(-1);
    }

    if(length == 0){
        data << "This path is empty !!!" << endl;
        return;
    }

    for(int i=0; i<length; i++)
        data << pset[i]->x << " " << pset[i]->y << endl;
    cout<<endl;
    data.close();
}

void Path::write_path(ofstream& file){
    if(!file.is_open()){
        cerr << "Bad file name: " << endl;
        exit(-1);
    }

    if(length == 0){
        file << "This path is empty !!!" << endl;
        return;
    }

    for(int i=0; i<length; i++)
        file << pset[i]->x << " " << pset[i]->y << endl;
    file<<endl;
}

const Point2D* Path::operator()(int i) const{
    if(i<0 || i>=pset.size()){
        cerr << "path index bounds error" << i << endl;
        exit(-1);
    }
    return pset[i];
}

Point2D* Path::get(int i)const{
    if(i<0 || i>=pset.size()){
        cerr << "path index bounds error" << i << endl;
        exit(-1);
    }
    return pset[i];
}

void Path::arclen_reparam(){

    double arc_length = .5; //arc length

    double p[3] = {0.0, 0.0, 0.0};
    // Create a vtkPoints object and store the points in it
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkPolyLine> polyLine = vtkSmartPointer<vtkPolyLine>::New();
    polyLine->GetPointIds()->SetNumberOfIds(pset.size());

    for (int i = 0; i < pset.size(); i++){
        p[0] = pset[i]->x;
        p[1] = pset[i]->y;
        points->InsertNextPoint(p);

        polyLine->GetPointIds()->SetId(i,i);
    }

    // Create a cell array to store the lines in and add the lines to it
    vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
    cells->InsertNextCell(polyLine);

    vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
    polyData->SetPoints(points);
    polyData->SetLines(cells);

    vtkSmartPointer<vtkCardinalSpline> mySpline = vtkSmartPointer<vtkCardinalSpline>::New();
    mySpline->SetLeftConstraint(2);
    mySpline->SetLeftValue(0.0);
    mySpline->SetRightConstraint(2);
    mySpline->SetRightValue(0.0);

    //first spline filter to get the arc length of the path
    vtkSmartPointer<vtkSplineFilter> spline = vtkSmartPointer<vtkSplineFilter>::New();
    spline->SetSpline(mySpline);
    spline->SetInputData(polyData);
    spline->SetSubdivideToLength( );
    spline->SetLength( arc_length );
    spline->Update( );

    vtkSmartPointer<vtkPoints> pts = vtkSmartPointer<vtkPoints>::New();
    pts = spline->GetOutput()->GetPoints();

    double *pt = new double[3];

    //cout << pts->GetNumberOfPoints() << endl;

    pset.clear();
    for(int i=0; i< pts->GetNumberOfPoints(); i++){
        pt = pts->GetPoint(i);
        pset.push_back(new Point2D(pt[0], pt[1]));
        //data3 << pt[0] << " " << pt[1] << endl;//" " << pt[2] << endl;
    }
    length = pset.size();

    pt =  NULL;
    delete[] pt;
}

void Path::eqnum_reparam(){

    int L = 120; // total number of line segment - EqNum
    double arc_length = .5; //arc length

    double p[3] = {0.0, 0.0, 0.0};
    // Create a vtkPoints object and store the points in it
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkPolyLine> polyLine = vtkSmartPointer<vtkPolyLine>::New();
    polyLine->GetPointIds()->SetNumberOfIds(pset.size());

    for (int i = 0; i < pset.size(); i++){
        p[0] = pset[i]->x;
        p[1] = pset[i]->y;
        points->InsertNextPoint(p);

        polyLine->GetPointIds()->SetId(i,i);
    }

    // Create a cell array to store the lines in and add the lines to it
    vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
    cells->InsertNextCell(polyLine);

    vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
    polyData->SetPoints(points);
    polyData->SetLines(cells);

    vtkSmartPointer<vtkCardinalSpline> mySpline = vtkSmartPointer<vtkCardinalSpline>::New();
    mySpline->SetLeftConstraint(2);
    mySpline->SetLeftValue(0.0);
    mySpline->SetRightConstraint(2);
    mySpline->SetRightValue(0.0);

    //first spline filter to get the arc length of the path
    vtkSmartPointer<vtkSplineFilter> spline = vtkSmartPointer<vtkSplineFilter>::New();
    spline->SetSpline(mySpline);
    spline->SetInputData(polyData);
    spline->SetSubdivideToLength( );
    spline->SetLength( arc_length );
    spline->Update( );

    vtkSmartPointer<vtkPoints> pts = vtkSmartPointer<vtkPoints>::New();
    pts = spline->GetOutput()->GetPoints();

    double path_length = double(pts->GetNumberOfPoints()-1)*arc_length/double(L-1);

    //second spline filter to get the reparametrization of the path with length L
    vtkSmartPointer<vtkSplineFilter> splineReparam = vtkSmartPointer<vtkSplineFilter>::New();
    splineReparam->SetSpline(mySpline);
    splineReparam->SetInputData(polyData);
    splineReparam->SetSubdivideToLength( );
    splineReparam->SetLength( path_length );
    splineReparam->Update( );

    vtkSmartPointer<vtkPoints> ptsReparam = vtkSmartPointer<vtkPoints>::New();
    ptsReparam = splineReparam->GetOutput()->GetPoints();

    //cout << ptsReparam->GetNumberOfPoints() << endl;

    double *pt = new double[3];
    pset.clear();

    for(int i=0; i< ptsReparam->GetNumberOfPoints(); i++){
        pt = ptsReparam->GetPoint(i);
        pset.push_back(new Point2D(pt[0], pt[1]));
    }
    length = pset.size();

    pt = NULL;
    delete[] pt;
}

void Path::path_reparam(string param){
    if (param.compare("ArcLen") && param.compare("EqNum")){
        cout << "Path::Path_reparam -> The parameterization argument can be 'ArcLen' or 'EqNum' only!" << endl;
        return;
    }

    if (!param.compare("ArcLen"))
        arclen_reparam();
    else
        eqnum_reparam();
}

void Path::get_bounds(int& minLA, int& maxLA, int& minLO, int& maxLO){

    for (int i = 0; i < length; i++){
        if (pset[i]->x > maxLA)
            maxLA = pset[i]->x;
        if (pset[i]->y > maxLO)
            maxLO = pset[i]->y;
        if (pset[i]->x < minLA)
            minLA = pset[i]->x;
        if (pset[i]->y < minLO)
            minLO =pset[i]->y;
    }
}
