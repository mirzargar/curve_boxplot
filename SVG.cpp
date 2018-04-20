#include "SVG.h"

// initializes a SVG file and setup the window parameters
void InitializeSVG(string SVGfname, string background_filename, int width, int height){

    ofstream data(SVGfname.c_str());

    if(!data.is_open()){
        cerr << "bad SVG Filename: " << SVGfname << endl;
        exit(-1);
    }

    data << "<?xml version=\"1.0\" standalone=\"no\"?>" << endl;
    data << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"" << endl;
    data << "  \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">" << endl;
    data << "<svg width=\"" << width <<"\" height=\"" << height << "\"" << " viewBox=\"" << 0 << " " << 0 << " " << width << " " << height << "\" " << endl;
    data << "     xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink= \"http://www.w3.org/1999/xlink\" version=\"1.1\">" << endl << endl;
    data << "<image x=\"0\" y=\"0\" width=\"" << width << "\" height=\"" << height << "\" xlink:href=\"" << background_filename <<  "\"/>" << endl;
    //data << "<image x=\"-100\" y=\"-5\" width=\"1025\" height=\"750\" xlink:href=\"hurricane_map_copy.png\"/>" << endl;

    data.close();
}

// if the output SVG file requires a background image, this function can set it up
void AddSVGBG(char* SVGfname, char* BGfname, int x, int y, int width, int height){
    //ofstream data(SVGfname);
    ofstream data;
    data.open(SVGfname, std::ofstream::out | std::ofstream::app);

    if(!data.is_open()){
        cerr << "Bad SVG file name: " << SVGfname << endl;
        exit(-1);
    }

    data << "<image x=\"" << x <<"\" y=\"" << y <<
            "\" width=\"" << width <<"\" height=\"" << height <<
            "\" xlink:href=\"./" << BGfname << "\"/>" <<endl;// << test_map.svg" transform="scale(.4)"/>" << endl;
    data.close();
}

//////////////////
// Drawying utilities - the function names are self explanatory
//////////////////
void DrawPath(string SVGfname, Points Pset, string strokeColor, double strokeWidth, string StrokeType, char* fill, double window_width, double window_height){
    //ofstream data(SVGfname);
    if(Pset.size()<=1)
        return;

    ofstream data;
    data.open(SVGfname.c_str(), std::ofstream::out | std::ofstream::app);

    if(!data.is_open()){
        cerr << "Bad SVG Filename: " << SVGfname << endl;
        exit(-1);
    }

    if(strcmp(StrokeType.c_str(), "dash") == 0)
        data << "<path stroke-dasharray=\"2,2\" fill = \"" << fill <<
                "\" d=\"M" << Pset[0].x() << "," << Pset[0].y();// << "\"" << endl;
    else
        data << "<path  fill = \"" << fill <<
                "\" d=\"M" << Pset[0].x() << "," << Pset[0].y();// << endl;

    //cout << "pset size: " << Pset.size() << endl;
        for(int i=1; i<Pset.size(); i++)
            data << endl << "            L" << Pset[i].x() << ", " << Pset[i].y();// << endl;

    //make it conncet to the first point
        data << endl << "            L" << Pset.back().x() << ", " << Pset.back().y() << "\"" << endl;

    data << "            stroke = \"" << strokeColor << "\" stroke-width = \"" <<
            strokeWidth << "\" stroke-linejoin=\"round\" transform = \"rotate(-90 " << window_width/2.0 << " " << window_height/2.0 << ")\"/>" << endl;
    data.close();
}

void DrawSVGIntPath(string SVGfname, vector<IntPairVector> PS, char* strokeColor, double strokeWidth, char* StrokeType, char* fill, bool smooth){
    //ofstream data(SVGfname);
    if(PS.size()<=1)
        return;

    ofstream data;
    data.open(SVGfname.c_str(), std::ofstream::out | std::ofstream::app);

    if(!data.is_open()){
        cerr << "Bad SVG Filename: " << SVGfname << endl;
        exit(-1);
    }

    vector<IntPairVector> Pset;

    //TODO: optimize
    if(smooth)
        Pset = SmoothPath(PS);
    else{
        for(int i=0; i<PS.size(); i++)
        Pset.push_back(PS[i]);
    }

    if(strcmp(StrokeType, "dash") == 0)
        data << "<path stroke-dasharray=\"2,2\" fill = \"" << fill <<
                "\" d=\"M" << Pset[0].first << "," << Pset[0].second;// << "\"" << endl;
    else
        data << "<path  fill = \"" << fill <<
                "\" d=\"M" << Pset[0].first << "," << Pset[0].second;// << endl;

    //cout << "pset size: " << Pset.size() << endl;
        for(int i=1; i<Pset.size(); i++)
            data << endl << "            L" << Pset[i].first << ", " << Pset[i].second;// << endl;

    //make it conncet to the first point
    data << endl << "            L" << Pset.back().first << ", " << Pset.back().second << "\"" << endl;

    data << "            stroke = \"" << strokeColor << "\" stroke-width = \"" <<
            strokeWidth << "\" stroke-linejoin=\"round\"/>" << endl;
    data.close();
}

void FillPolygon(string SVGfname, vector<IntPairVector> PS, char* fillColor, char* StrokeColor, double StrokeWidth, double opacity, bool smooth){
    if( PS.size()<=2 )
        return;

    //TODO: optimize
    vector<IntPairVector> Pset;
    if(smooth)
        Pset = SmoothPath(PS);
    else{
        for(int i=0; i<PS.size(); i++)
        Pset.push_back(PS[i]);
    }

    ofstream data;
    data.open(SVGfname.c_str(), std::ofstream::out | std::ofstream::app);

    if(!data.is_open()){
        cerr << "Bad SVG Filename: " << SVGfname << endl;
        exit(-1);
    }

    data << endl << "<polygon fill=\"" << fillColor << "\" stroke=\"" << StrokeColor << "\" stroke-width=\"" << StrokeWidth
         << "\" stroke-linejoin=\"round\" stroke-opacity=\"" << opacity << "\" fill-opacity=\"" << opacity << "\""
         //<< " transform = \"rotate(-90 276 348) translate(20, 70)\"\n"
         << endl;
    data << "    points=\"" << Pset[0].first << "," << Pset[0].second << endl;
    for(int i=1; i<Pset.size(); i++)
        data << "     " << Pset[i].first << "," << Pset[i].second << endl;

    //data << "\"/>" << endl;
    data << "      " << Pset.back().first << "," << Pset.back().second << "\"/>" << endl;
    data.close();
}

void FillPolygon(string SVGfname, Points Pset, string fillColor, string StrokeColor, double StrokeWidth, double opacity, bool smooth, double window_width, double window_height){
    if( Pset.size()<=2 )
        return;

    ofstream data;
    data.open(SVGfname.c_str(), std::ofstream::out | std::ofstream::app);

    if(!data.is_open()){
        cerr << "Bad SVG Filename: " << SVGfname << endl;
        exit(-1);
    }

    data << endl << "<polygon fill=\"" << fillColor << "\" stroke=\"" << StrokeColor << "\" stroke-width=\"" << StrokeWidth
         << "\" stroke-linejoin=\"round\" stroke-opacity=\"" << opacity << "\" fill-opacity=\"" << opacity << "\""
         //<< " transform = \"rotate(-90 276 348) translate(20, 70)\"\n"
         << endl;
    data << "    points=\"" << Pset[0].x()<< "," << Pset[0].y() << endl;
    //cout << Pset[0].x() << ", " << Pset[0].y() << endl;
    for(int i=1; i<Pset.size(); i++)
        data << "     " << Pset[i].x() << "," << Pset[i].y() << endl;

    //data << "\"/>" << endl;
    data << "      " << Pset.back().x() << "," << Pset.back().y() << "\" transform = \"rotate(-90 " << window_width/2.0 << " " << window_height/2.0 << ")\"/>" << endl; //<< "\"/>" << endl;
    data.close();
}

// a simple fast nearest neighbor smoothing of the paths
vector<IntPairVector> SmoothPath(vector<IntPairVector> v){
    vector<IntPairVector> temp;
    temp.push_back(v[0]);

    for(int i=1; i<v.size()-2; i++)
        temp.push_back(make_pair((v[i-1].first+v[i+1].first)/2,(v[i-1].second+v[i+1].second)/2));
    temp.push_back(v.back());
    return temp;
}

// finalizing the SVG files nad closing the handle
void CloseSVGFile(string SVGfname){
    ofstream data;
    data.open(SVGfname.c_str(), std::ofstream::out | std::ofstream::app);

    if(!data.is_open()){
        cerr << "Bad SVG Filename: " << SVGfname << endl;
        exit(-1);
    }

    data << endl << "</svg>";
    data.close();
}

