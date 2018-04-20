
cmake_minimum_required(VERSION 2.8)

PROJECT(curveboxplot)

find_package(Qt5Widgets REQUIRED)

# CGAL and its components
find_package( CGAL QUIET COMPONENTS  )

if ( NOT CGAL_FOUND )
  message(STATUS "This project requires the CGAL library, and will not be compiled.")
  return()
endif()

# include helper file
include( ${CGAL_USE_FILE} )


find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

add_executable(CurveBoxplot SVG.h SVG.cpp PathCBD.h PathCBD.cpp Matrix.h Utility.h Utility.cpp Point2D.h Point2D.cpp Path.h Path.cpp Ensemble.h Ensemble.cpp main.cpp)

if(VTK_LIBRARIES)
  target_link_libraries(CurveBoxplot ${VTK_LIBRARIES})
else()
  target_link_libraries(CurveBoxplot vtkHybrid)
endif()
