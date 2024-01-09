#include <iostream>
#include <fstream>
#include <stdint.h>
#include <float.h>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <iterator>
#include <iomanip>

using namespace std;

struct Hailstone {
    double x, y, z, dx, dy, dz;
    const double xySlope() const {
        if (dx == 0) return DBL_MAX;

        return (double)dy / (double)dx;
    }
};

vector<string> split(string s, string delim)
{
    vector<string> output;
    while (s.find(delim) != string::npos)
    {
        size_t pos = s.find(delim);
        output.push_back(s.substr(0, pos));
        s.erase(0, pos + delim.length());
    }
    output.push_back(s);
    return output;
}

set<int64_t> getVelocities(int64_t distance, int64_t hailVelocity)
{
    set<int64_t> valid;

    if (distance < 0) distance *= -1;
    for (int i = -500; i <= 500; i++)
    {
        if (i == hailVelocity) continue;
        if (distance % (hailVelocity - i) == 0)
        {
            valid.insert(i);
        }
    }

    return valid;
}

bool testIntersection(Hailstone l, Hailstone r, pair<double, double>& intersection)
{
    auto m1 = l.xySlope();
    auto c1 = l.y - m1 * l.x;
    auto m2 = r.xySlope();
    auto c2 = r.y - m2 * r.x;

    if (m1 == m2) return false;

    intersection.first = (double)(c2 - c1) / (m1 - m2);
    intersection.second = m1 * intersection.first + c1;

    return true;
}

int main()
{
    string line;
    ifstream input;
    vector<Hailstone> hailstones;

    input.open("input.txt");

    while (!input.eof())
    {
        getline(input, line);
        auto parts = split(line, " @ ");
        auto coordinateStrings = split(parts[0], ", ");
        auto slopeStrings = split(parts[1], ", ");

        Hailstone h;
        h.x = stoll(coordinateStrings[0]);
        h.y = stoll(coordinateStrings[1]);
        h.z = stoll(coordinateStrings[2]);
        h.dx = stoll(slopeStrings[0]);
        h.dy = stoll(slopeStrings[1]);
        h.dz = stoll(slopeStrings[2]);

        hailstones.push_back(h);
    }

    // Find the target X, Y, and Z values
    set<int64_t> xV, yV, zV;
    for (int i = 0; i < hailstones.size(); i++)
    {
        for (int j = i + 1; j < hailstones.size(); j++)
        {
            Hailstone l = hailstones[i], r = hailstones[j];
            int matchCount = 0;
            if (l.dx == r.dx)
            {
                auto pairX = getVelocities(r.x - l.x, r.dx);
                if (xV.size() == 0) xV = pairX;
                
                set<int64_t> xIntersect;
                set_intersection(xV.begin(), xV.end(), pairX.begin(), pairX.end(), inserter(xIntersect, xIntersect.begin()));
                xV = xIntersect;
            }

            if (l.dy == r.dy)
            {
                auto pairY = getVelocities(r.y - l.y, r.dy);
                if (yV.size() == 0) yV = pairY;
                
                set<int64_t> yIntersect;
                set_intersection(yV.begin(), yV.end(), pairY.begin(), pairY.end(), inserter(yIntersect, yIntersect.begin()));
                yV = yIntersect;
            }

            if (l.dz == r.dz)
            {
                auto pairZ = getVelocities(r.z - l.z, r.dz);
                if (zV.size() == 0) zV = pairZ;
                
                set<int64_t> zIntersect;
                set_intersection(zV.begin(), zV.end(), pairZ.begin(), pairZ.end(), inserter(zIntersect, zIntersect.begin()));
                zV = zIntersect;
            }
        }
    }

    // Assumption - Exactly one valid X, Y, and Z velocity was found
    Hailstone a = hailstones[0], b = hailstones[1];
    double dx = *xV.begin(), dy = *yV.begin(), dz = *zV.begin();
    double ma = (a.dy-dy)/(a.dx-dx);
    double mb = (b.dy-dy)/(b.dx-dx);
    double ca = a.y - (ma*a.x);
    double cb = b.y - (mb*b.x);
    double rx = (cb-ca)/(ma-mb);
    double ry = ma*rx + ca;
    double rt = (rx - a.x)/(a.dx-dx);
    double rz = a.z + (a.dz-dz) * rt;

    cout << endl << setprecision(20) << "Output: (" << rx << ", " << ry << ", " << rz << ") - [" << rx+ry+rz << "]" << endl;
}