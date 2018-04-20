#include "PathCBD.h"
#include "Polygon.h"

using namespace std;

// NOTE: This main is defined based on the interface to NCAR TCP system

// @TODO: the outlier detection rule
// @TODO: map projection

// AP - NCEP GFS
// CP - Canadian
// FP - Navy

int main(int argn, char* argv[]){

    string filename, modelname, date, param, svg_name, coordinate_bound_filename;
    int lead_time = 0, palette_number = 0;

    if (argn < 8){
        cout << endl << "USAGE:"
             << endl << "     ./main date_filename modelname date param svg_name lead_time coordnate_bound_filename (color_palette_number)"
             << endl << endl << "Note:" << endl
             << "     The dat_filename should include .dat" << endl
             << "     The 'params' corresponds to curve parameterization and can only be specified as 'ArcLen' (short for arc length) and 'EqNum' " << endl
             << "     Specifying the color_paletter is optional - see below"
             << endl << endl
             << "The default color palette is:" << endl
             << "     median: #ffff00" << endl
             << "     50% band: #bc80bd" << endl
             << "     100% band: #fccde5" << endl
             << "     outlier(s): #8dd3c7" << endl << endl
             << "You have a couple of other option for your color palette and you can specify it by adding one more input paramter after 'coordnate_bound_filename' "
             << endl << endl
             << "color palette 2" << endl
             << "     median: #403876" << endl
             << "     50% band: #8888bb" << endl
             << "     100% band: #bbbbff" << endl
             << "     outlier(s): #e34a33"
             << endl << endl
             << "color palette 3" << endl
             << "     median: #2f6e35" << endl
             << "     50% band: #88bb88" << endl
             << "     100% band: #bbffbb" << endl
             << "     outlier(s): #008080" << endl << endl
             << endl;

        return(-1);
    }else if (argn == 8){
        filename = argv[1];
        stringstream model;
        model << " " << argv[2];
        modelname = model.str();
        stringstream d;
        d << " " << argv[3];
        date = d.str();
        param = argv[4];
        svg_name = argv[5];
        lead_time = atoi(argv[6]);
        coordinate_bound_filename = argv[7];
        palette_number = 1;
    }else if (argn == 9){
        filename = argv[1];
        stringstream model;
        model << " " << argv[2];
        modelname = model.str();
        stringstream d;
        d << " " << argv[3];
        date = d.str();
        param = argv[4];
        svg_name = argv[5];
        lead_time = atoi(argv[6]);
        coordinate_bound_filename = argv[7];
        palette_number = atoi(argv[8]);
    }else{
        cerr << "UNKNOWN PARAMTERS!!!" << endl;
        return -1;
    }

    cout << filename << ", " << modelname << ", " << date << ", " << param << ", " << svg_name << ", " << lead_time << endl;
    Ensemble e;

    e.load_NCAR_dat_file(filename, modelname, date, lead_time);

#ifdef DEBUG
    e.print_ensemble();
#endif

    e.reparam(param);

    PathCBD cbd;
    cbd = PathCBD(e, argv[3], param, svg_name, coordinate_bound_filename, palette_number); //do the analysis

//#ifdef DEBUG
    cout << "Done !!!" << endl;
//#endif

    return(0);
}
