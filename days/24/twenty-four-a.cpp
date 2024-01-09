#include <iostream>
#include <fstream>
#include <stdint.h>
#include <float.h>
#include <string>
#include <vector>

#define AREA_MIN 7.0LD
#define AREA_MAX 27.0LD

//#define AREA_MIN 200000000000000LL
//#define AREA_MAX 400000000000000LL

using namespace std;

struct Hailstone {
    double x, y, dx, dy;
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

bool testIntersection(Hailstone l, Hailstone r, pair<double, double>& intersection)
{
    auto m1 = l.xySlope();
    auto c1 = l.y - m1 * l.x;
    auto m2 = r.xySlope();
    auto c2 = r.y - m2 * r.x;

    if (m1 == m2) return false;

    intersection.first = (double)(c2 - c1) / (m1 - m2);
    intersection.second = m1 * intersection.first + c1;
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
        h.dx = stoll(slopeStrings[0]);
        h.dy = stoll(slopeStrings[1]);

        hailstones.push_back(h);
    }

    uint64_t totalIntersections = 0;
    for (int i = 0; i < hailstones.size(); i++)
    {
        for (int j = i + 1; j < hailstones.size(); j++)
        {
            Hailstone l = hailstones[i], r = hailstones[j];
            pair<double, double> intersection;
            if (testIntersection(l, r, intersection))
            {
                // Test outside of test area
                if (intersection.first  < AREA_MIN || intersection.first  > AREA_MAX ||
                    intersection.second < AREA_MIN || intersection.second > AREA_MAX)
                    continue; 

                // Test if intersection is in past
                if (l.dx > 0 && intersection.first < l.x) continue;
                if (r.dx > 0 && intersection.first < r.x) continue;
                if (l.dx < 0 && intersection.first > l.x) continue;
                if (r.dx < 0 && intersection.first > r.x) continue;

                totalIntersections++;
            }
        }
    }
    
    cout << endl << "Output: " << totalIntersections << endl;
}