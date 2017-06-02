/* Start copying here */
/* This code is freely released to the public domain. */
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
    if(argc < 3) {
        cout << "tin2grid <inputfile> <outputfile>" << endl;
        return 0;
    }

    std::string line;
    std::string val;
    float fval;

    std::ifstream infile(argv[1]);
    std::ofstream outfile(argv[2], ios::binary);

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
    xmax = xmin + w;
    ymax = ymin + h;

    outfile.write(reinterpret_cast<char*>(&xmin), sizeof(float));
    outfile.write(reinterpret_cast<char*>(&ymin), sizeof(float));
    outfile.write(reinterpret_cast<char*>(&xmax), sizeof(float));
    outfile.write(reinterpret_cast<char*>(&ymax), sizeof(float));

    //next two lines are not needed, but we need to read them to skip them
    getline(infile, val);
    getline(infile, val);

    while(getline(infile,line))
	{
            //skip empty lines
            if(line.empty())
                continue;
            stringstream ss(line);
            while(getline(ss, val, ' '))
		{
                    fval = stof(val);
                    outfile.write(reinterpret_cast<char*>(&fval), sizeof(float));
		}

	}

    outfile.close();
    return 0;
}

/* Stop copying here */
