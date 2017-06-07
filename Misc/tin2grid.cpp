/* Start copying here */
/* This code is freely released to the public domain. */
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cfloat>
#include <algorithm>

using namespace std;

int main(int argc, char *argv[])
{
    if(argc < 3) {
        cout << "tin2grid <inputfile> <outputfile> [-mx] [-my]" << endl;
        return 0;
    }

    bool mirrorX = false, mirrorY = false;
    if(argc >= 4) {
        for (int i = 3; i < argc; ++i) {
            if (string(argv[i]).compare("-mx") == 0) {
                mirrorX = true;
            } else if (string(argv[i]).compare("-my") == 0) {
                mirrorY = true;
            } else {
                cout << "tin2grid <inputfile> <outputfile> [-mx] [-my]" << endl;
                return 0;
            }
        }
    }

    string line;
    string val;
    float fval;

    ifstream infile(argv[1]);
    ofstream outfile(argv[2], ios::binary);

    //first 4 lines need to be handled carefully
    int h, w;
    getline(infile, val);
    w = stoi(val.substr(6,6));
    getline(infile, val);
    h = stoi(val.substr(6,6));

    outfile.write(reinterpret_cast<char*>(&w), sizeof(int));
    outfile.write(reinterpret_cast<char*>(&h), sizeof(int));

    float xmin, ymin, xmax, ymax;
    getline(infile, val);
    xmin = stof(val.substr(10, 40));
    getline(infile, val);
    ymin = stof(val.substr(10, 40));

    getline(infile, val);
    double gridSize = stof(val.substr(9,40));
    
    getline(infile, val);
    int noDataVal = stoi(val.substr(13,40));

    w *= gridSize;
    h *= gridSize;
    xmax = xmin + w;
    ymax = ymin + h;

    outfile.write(reinterpret_cast<char*>(&xmin), sizeof(float));
    outfile.write(reinterpret_cast<char*>(&ymin), sizeof(float));
    outfile.write(reinterpret_cast<char*>(&xmax), sizeof(float));
    outfile.write(reinterpret_cast<char*>(&ymax), sizeof(float));

    double worldVolume = 0.0;
    double scaledVolume = 0.0;
    double sandVolume = 5600.0;
    double sandHeight = 8.0;
    double sandboxWidth = 1.0;
    double sandboxHeight = 0.7;
    
    //next line is not needed, but we need to read it to skip it
    //getline(infile, val);

    vector <vector <float> > elevations;
    float minElevation = DBL_MAX;
    for (int l = 0; getline(infile,line); ++l)
	{
            //skip empty lines
            if(line.empty())
                continue;
            
            stringstream ss(line);
            elevations.push_back(vector<float>());
            while(getline(ss, val, ' '))
		{
                    fval = stof(val);
                    if (int(fval) == noDataVal) fval = 0.0;
                    if (fval < minElevation) minElevation = fval;
                    elevations[l].push_back(fval);
		}
	}

    // mirror Y axis if necessary
    if (mirrorY) {
        reverse(elevations.begin(), elevations.end());
    }
    for (vector<float> el : elevations) {
        
        // mirror X axis if necessary
        if (mirrorX) {
            reverse(el.begin(), el.end());
        }

        // write the data
        for (float e : el) {
            if (minElevation < 0) e -= minElevation;
            worldVolume += (gridSize * gridSize * e);
            //scaledVolume += ((gridSize / w)*sandboxWidth)*((gridSize / h)*sandboxHeight)*(e);
            double hScale = (1.0 / 480.0) * 0.7;
            double wScale = (1.0 / 640.0) * 1.0;
            //scaledVolume += (gridSize * hScale) * (gridSize * wScale) * ((e / 640.0) * 1.0);
            outfile.write(reinterpret_cast<char*>(&e), sizeof(float));
        }
    }

    //scaledVolume = ((worldVolume / (w*h)) * (sandboxWidth*sandboxHeight));
    //scaledVolume = worldVolume / 1000000;
    
    cout << "Min: " << minElevation << endl;
    cout << "Grid size: " << gridSize << endl;
    cout << "Sand volume: " << sandVolume << " cm^3" << endl;
    cout << "World volume: " << worldVolume << " metres^3" << endl;
    cout << "Scaled volume: " << scaledVolume << " cm^3" << endl;

    double excessVolume = sandVolume - scaledVolume;
    double excessProportion = excessVolume / sandVolume;
    double verticalShift = -(sandHeight - (excessProportion * sandHeight)) / gridSize;
    cout << "Vertical shift: " << verticalShift << " cm" << endl;
    outfile.close();
    return 0;
}

/* Stop copying here */
