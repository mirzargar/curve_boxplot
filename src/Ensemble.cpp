#include "Ensemble.h"


Ensemble::Ensemble(){}

Ensemble::Ensemble(Ensemble &e){
    ESize = e.ESize;
    path_set.clear();
    for(int i=0; i<e.ESize; i++){
        path_set.push_back(e.path_set[i]);
    }

    max_path_length = e.max_path_length;
    set_ensemble_bounding_box();
}


Ensemble::Ensemble(string filename, int s){
    Path p;

    ESize = s;
    path_set.clear();
    max_path_length = 0;

    stringstream ss;
    for(int i=0; i<ESize; i++){
        ss.str(string());
        ss.clear();

        ss << filename << i+1 << ".txt";
        cout<<ss.str()<<endl;
        p.load_path(ss.str());
        path_set.push_back(&p);
        if(p.get_size() > max_path_length)
            max_path_length = p.get_size();
    }
    set_ensemble_bounding_box();
}


Ensemble::Ensemble(vector<Path*> paths){

    ESize = paths.size();
    int max_path_length = 0;
    path_set.clear();

    for (int i = 0; i < ESize; i++){
        path_set.push_back(paths[i]);
        if (paths[i]->get_length() > max_path_length)
            max_path_length = paths[i]->get_length();
    }
    set_ensemble_bounding_box();
}

int Ensemble::get_size(){
    return ESize;
}

int Ensemble::get_max_element_length(){
    return max_path_length;
}

void Ensemble::set_max_path_length(){
    max_path_length = 0;
    for (int i = 0; i< path_set.size(); i++){
        if (path_set[i]->get_length() > max_path_length)
            max_path_length = path_set[i]->get_length();
    }
}

void Ensemble::load_NCAR_dat_file(string filename, string modelname, string date, int lead_time){
    int min_path_length = 5;
    
    ifstream infile( filename.c_str() );
    
    bool path_start = false;
    vector <Point2D*> pset;
    string current_model;
    
    while (infile){
        string s;
        if (!getline( infile, s ))
            break;
        
        istringstream ss( s );
        vector <string> record;
        
        while (ss){
            string s;
            if (!getline( ss, s, ',' ))
                break;
            record.push_back( s );
        }
        
        bool modelcheck;
        if (modelname.compare(" ALL") == 0){
            if (record[4].compare(" CARQ") == 0)
                modelcheck = true;
            else
                modelcheck = false;
        }else{
            modelcheck = record[4].compare(0, modelname.size(), modelname);
        }

        if (!modelcheck && !record[2].compare(date)){
            if (!path_start){
                path_start = true;
                current_model = record[4];
            }
            
            if (record[4].compare(current_model)){
                if (pset.size() > min_path_length)
                    path_set.push_back(new Path(pset));
#ifdef DEBUG
                cout << "******* " << current_model << " " << pset.size() << " (" << pset[0]->x << "," << pset[0]->y <<
                     ") " << " (" << pset[pset.size()-1]->x << "," << pset[pset.size()-1]->y << ") " << endl;
#endif
                pset.clear();
                current_model = record[4];
            }

            int current_leadtime = atoi(record[5].c_str());
            
            int lat = atoi(record[6].c_str());
            int lon;
            if ( *(record[7].end() - 1) == 'E'){
                lon = atoi(record[7].c_str());
            }else{
                lon = -atoi(record[7].c_str());
            }
            if (current_leadtime < lead_time)
                pset.push_back(new Point2D(lat, lon));
        }else{
            if (path_start == true){
#ifdef DEBUG
                cout << "******* " << current_model << " " << pset.size() << " (" << pset[0]->x << "," << pset[0]->y <<
                     ") " << " (" << pset[pset.size()-1]->x << "," << pset[pset.size()-1]->y << ") " << endl;
#endif
                path_start = false;
                if (pset.size() > min_path_length)
                    path_set.push_back(new Path(pset));
                pset.clear();
                current_model = "NONE";
            }
        }
    }
    
    if (path_start == true){
#ifdef DEBUG
                cout << "******* " << current_model << " " << pset.size() << " (" << pset[0]->x << "," << pset[0]->y <<
                     ") " << " (" << pset[pset.size()-1]->x << "," << pset[pset.size()-1]->y << ") " << endl;
#endif
        if (pset.size() > min_path_length)
            path_set.push_back(new Path(pset));
    }
    
    ESize = path_set.size();
    set_max_path_length();
    
    set_ensemble_bounding_box();
    
}

void Ensemble::reparam(string param){
    for (int i = 0; i < ESize; i++){
        path_set[i]->path_reparam(param);
    }
    set_max_path_length();
}

void Ensemble::load_ensemble(string filename, int s, string param){

    ESize = s;
    path_set.clear();
    max_path_length = 0;

    stringstream ss;
    for(int i=0; i < ESize; i++){
        ss.str(string());
        ss.clear();

        ss << filename << i+1 << "_" << param << "_rep.txt"; //originall i+1
        cout<<"filename: "<<ss.str()<<endl;
        path_set.push_back(new Path(ss.str()));
        if(path_set[i]->get_size() > max_path_length)
            max_path_length = path_set[i]->get_size();
    }

    set_ensemble_bounding_box();

}

void Ensemble::load_ensemble(string filename, vector<int> s){

    ESize = s.size();
    path_set.clear();
    max_path_length = 0;

    stringstream ss;
    for(int i=0; i < s.size(); i++){
        ss.str(string());
        ss.clear();

        ss << filename << 123-s[i] << "_rep.txt"; //originallly s+1
        cout<<"filename: "<<ss.str()<<endl;
        path_set.push_back(new Path(ss.str()));
        if(path_set[i]->get_size() > max_path_length)
            max_path_length = path_set[i]->get_size();
    }

    set_ensemble_bounding_box();
}

void Ensemble::print_ensemble(){
    if(ESize == 0){
        cout << "This ensemble is empty !!!" << endl;
        return;
    }

    cout << "Ensemble size: " << ESize << endl;
    set_ensemble_bounding_box();
    cout << "bounds: " << min_lat << ", " << max_lat << ", " << min_lon << ", " << max_lon << endl;
    for(int i=0; i<ESize; i++){

        cout << "Path " << i+1 << " -> length: " << path_set[i]->get_length() << " (" << path_set[i]->get(0)->x << ", " << path_set[i]->get(0)->y << ") ("
             << path_set[i]->get(path_set[i]->get_length()-1)->x << ", " << path_set[i]->get(path_set[i]->get_length()-1)->y << ")" << endl;
    }
}

void Ensemble::write_ensemble(string filename){
    ofstream data(filename.c_str());
    if(!data.is_open()){
        cerr << "Bad file name: " << filename.c_str() << endl;
    }
    data << "Ensemble size: " << ESize << endl << endl;

    for(int i=0; i<ESize; i++){
        data << "Path " << i+1 << " :" << endl;
        path_set[i]->write_path(data);
    }
    data.close();
}

const Path* Ensemble::operator[](int i) const{
    if(i<0 || i>=ESize){
        cerr << "ensemble index bouns error" << i << endl;
        exit(-1);
    }
    return path_set[i];
}

void Ensemble::set_ensemble_bounding_box(){
    int minLA = 10000, minLO = 10000;
    int maxLA = 0, maxLO = 0;

    min_lat = minLA, min_lon = minLO, max_lat = maxLA, max_lon = maxLO;

    //cout << minLA << " " << maxLA << endl;

    for (int i = 0; i < ESize; i++){
        path_set[i]->get_bounds(minLA, maxLA, minLO, maxLO);
        if (minLA < min_lat)
            min_lat = minLA;
        if (minLO < min_lon)
            min_lon = minLO;
        if (maxLA > max_lat)
            max_lat = maxLA;
        if (maxLO > max_lon)
            max_lon = maxLO;
    }

}
