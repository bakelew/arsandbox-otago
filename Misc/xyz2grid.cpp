/* This code is freely released to the public domain. */
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Point3d
{
public:
    double x,y,z;
    Point3d(double x, double y, double z);
};

Point3d::Point3d(double x, double y, double z)
    :x(x),y(y),z(z)
{ }

Point3d getPoint(string line)
{
    Point3d result(0,0,0);
    sscanf(line.c_str(), "%lf %lf %lf", &result.x, &result.y, &result.z);
    return result;
}

int main(int argc, char *argv[])
{
    if(argc < 3) {
        cout << "tin2grid <inputfile> <outputfile>" << endl;
        return 0;
    }

    string line;
    string val;
    float fval;

    ifstream infile(argv[1]);
    ofstream outfile(argv[2], ios::binary);

    int h, w;
    w = 640;
    h = 480;

    outfile.write(reinterpret_cast<char*>(&w), sizeof(int));
    outfile.write(reinterpret_cast<char*>(&h), sizeof(int));

    float xmin=1000000.f, ymin=1000000.f, xmax=-1000000.f, ymax=-1000000.f;
    float zmin=1000000.f, zmax=-1000000.f;
    vector<Point3d> data;
    while(getline(infile,line))
	{
            Point3d newPoint = getPoint(line);
            if (newPoint.x < xmin) xmin = newPoint.x;
            if (newPoint.y < ymin) ymin = newPoint.y;
            if (newPoint.z < zmin) zmin = newPoint.z;
            if (newPoint.x > xmax) xmax = newPoint.x;
            if (newPoint.y > ymax) ymax = newPoint.y;
            if (newPoint.z > zmax) zmax = newPoint.z;
            data.push_back(newPoint);
	}
    cout << xmin << " " << xmax << " " << ymin << " " << ymax << " " << zmin << " " << zmax << endl;

    outfile.write(reinterpret_cast<char*>(&xmin), sizeof(float));
    outfile.write(reinterpret_cast<char*>(&ymin), sizeof(float));
    outfile.write(reinterpret_cast<char*>(&xmax), sizeof(float));
    outfile.write(reinterpret_cast<char*>(&ymax), sizeof(float));

    //next two lines are not needed, but we need to read them to skip them
    getline(infile, val);
    getline(infile, val);

    int numDataPoints = 0;
    double total = 0.0;
    int ival = 0;
    for (Point3d p : data)
	{
            fval = (99.2838 - p.z);
            outfile.write(reinterpret_cast<char*>(&fval), sizeof(float));
	}

    outfile.close();
    return 0;
}
