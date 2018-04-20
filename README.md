# Curve_boxplot

Computing the curve boxplot for an ensmeble of 2D curves in [NCAR's .dat format](http://hurricanes.ral.ucar.edu/repository/). For more details regarding the curve boxplot technique which is a nonparamteric technique to summarize the main features of an ensmeble of multivariate paths consult the [curve boxplot paper](http://www.cs.miami.edu/home/mirzargar/papers/curve_boxplot.pdf)

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

[Example output for Hurricane Irma from TCGP project's archive](http://hurricanes.ral.ucar.edu/realtime/plots/northatlantic/2017/al112017/eps_track_gefs_boxplot_late/aal11_2017090318_eps_track_gefs_boxplot_late.png)
