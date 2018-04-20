#ifndef __PATHCBD_H_INCLUDED__
#define __PATHCBD_H_INCLUDED__

// *********
// The heart of the code that is in charge of doing the band depth analysis
// *********

#include "Ensemble.h"
#include "Utility.h"
#include "SVG.h"

// NOTE: These parameters has been hard coded for NCAR integration - not required for general usage
const string BACKGROUND_FILENAME = "eps_track_boxplot_late_base_map.png";
const double window_width = 1024.0;
const double window_height = 1024.0;
const string median_color[3] = {"#ffff00", "#2171b5", "#6a51a3"}; //, "#403876", "#2f6e35"};
const string band_color[3] = {"#bc80bd", "#6baed6", "#9e9ac8"}; //"#74c476"}; //, "#8888bb", "#88bb88"};
const string envelop_color[3] = {"#fccde5", "#c6dbef", "#dadaeb"}; //"#c7e9c0"}; //, "#bbbbff", "#bbffbb"};
const string outlier_color[3] = {"#8dd3c7", "#fb9a99", "#fb9a99"}; //, "#e34a33", "#008080"};

using namespace std;

class PathCBD{
private:
     vector<double> Probs;
     double PThreshold;
     Ensemble E;
public:

     PathCBD(){}
     PathCBD(Ensemble e, string date, string param, string svg_name, string coordinate_bound_filename, int palette_number);
     vector<int> getPathPointInCheck(vector<vtkSmartPointer<vtkPolygon> > band, Path *p);
     void RunAnalysisFastMinLength(string date, string param);
     vtkSmartPointer<vtkPolygon> getTriangle(vector<Point2D> v);
     int PointInPolygonCheck(vtkSmartPointer<vtkPolygon> polygon, Point2D p);
     vector< vtkSmartPointer<vtkPolygon> > getBand(vector<Path*> PSet);
     vector< vtkSmartPointer<vtkPolygon> > getBandMinLength(vector<Path*> PSet);

     void genSVG(vector<int> Indx, string filename);
     void genSVG_continous(vector<int> Indx, string filename, string coordinate_bound_filename, int palette_number); //forms the band over time and space
     void genSVG_paths(vector<int> Indx, string filename, string coordinate_bound_filename);

     void get_BG_img_info(double& minlat, double& latwidth, double& minlon, double& lonwidth, double& VPX, double& VPY, double& VPW, double& VPH, string coordinate_bound_filename);

};

#endif
