# Curve Boxplot

Computing the curve boxplot for an ensmeble of 2D curves in [NCAR's .dat format](http://hurricanes.ral.ucar.edu/repository/). For more details regarding the curve boxplot technique which is a nonparamteric technique to summarize the main features of an ensmeble of multivariate paths consult the [curve boxplot paper](http://www.cs.miami.edu/home/mirzargar/papers/curve_boxplot.pdf)

## Requirements
```
CMake 2.8+
C++11 compiler
CGAL4+
VTK7+
```
## Building
```
cd curve_boxplot
mkdir build
cd build
cmake ../src/ or cmake ../src/ -DVTK_DIR=<VTK build directory> -DCGAL_DIR=<CGAL build directory>
make
```

## Running
* Note that this code has been optimized for intergation to [NCAR's TCGP project](http://hurricanes.ral.ucar.edu/)

```
./curveboxplot <data_filename> <modelname> <date> <param> <svg_name> <lead_time> <coordnate_bounds> [color_palette_number]
```

[Example output from TCGP project's archive (Hurricane Irma)](http://hurricanes.ral.ucar.edu/realtime/plots/northatlantic/2017/al112017/eps_track_gefs_boxplot_late/aal11_2017090318_eps_track_gefs_boxplot_late.png)
