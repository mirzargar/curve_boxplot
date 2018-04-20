#ifndef __ENSEMBLE_H_INCLUDED__
#define __ENSEMBLE_H_INCLUDED__

// *********
// The data structure to hold each ensmeble member (in this case it is a multivariate curve or path)
// *********

#include "Path.h"

class Ensemble{
private:
    int ESize;
    std::vector<Path*> path_set;
    int max_path_length;
    int min_lat;
    int max_lat;
    int min_lon;
    int max_lon;

    void set_max_path_length();

public:
    friend class PathCBD;

    Ensemble();
    Ensemble(Ensemble &e);
    Ensemble(string filename, int s);
    Ensemble(vector<Path*> paths);
    int get_size();
    int get_max_element_length();
    void load_ensemble(string filename, int s, string param);
    void load_ensemble(string filename, vector<int> s);
    void load_NCAR_dat_file(string filename, string modelname, string date, int lead_time);
    void reparam(string param);
    void print_ensemble();
    void write_ensemble(string filename);
    void set_ensemble_bounding_box();

    int get_min_lat () {return min_lat;}
    int get_max_lat () {return max_lat;}
    int get_max_lon () {return max_lon;}
    int get_min_lon () {return min_lon;}

    const Path* operator[](int i) const;
};

#endif
