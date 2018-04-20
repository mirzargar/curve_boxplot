# Curve_boxplot

Computing the curve boxplot for an ensmeble of curves in [NCAR's .dat format](http://hurricanes.ral.ucar.edu/repository/)

## Requirements
CMake 2.8+

C++11 compiler

CGAL4+

VTK7+

## Building
cd curve_boxplot
mkdir build
cd build
cmake ../
make

## Running
* Note this code has been optimized for intergation to [NCAR's TCGP project](http://hurricanes.ral.ucar.edu/)

./curveboxplot date_filename modelname date param svg_name lead_time coordnate_bound_filename (color_palette_number)
