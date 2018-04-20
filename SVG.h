#ifndef SVG_H
#define SVG_H

// *********
// Provide utility functions to support SVG output
// *********

#include "Utility.h"
#include "Matrix.h"

/***************************** SVG Utilities *****************************/


void InitializeSVG(string SVGfname, string background_filename, int width, int height);
void AddSVGBG(char* SVGfname, char* BGfname, int x, int y, int width, int height);
void DrawSVGIntPath(string SVGfname, vector<IntPairVector> Pset, char* strokeColor, double strokeWidth, char* StrokeType, char* fill, bool smooth);
void DrawPath(string SVGfname, Points Pset, string strokeColor, double strokeWidth, string StrokeType, char *fill, double window_width, double window_height);
vector<IntPairVector> SmoothPath(vector<IntPairVector> v);
void FillPolygon(string SVGfname, vector<IntPairVector> Pset, char* fillColor, char* StrokeColor, double StrokeWidth, double opacity, bool smooth);
void FillPolygon(string SVGfname, Points Pset, string fillColor, string StrokeColor, double StrokeWidth, double opacity, bool smooth, double window_width, double window_height);
void CloseSVGFile(string SVGfname);

#endif // SVG_H
