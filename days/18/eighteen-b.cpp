#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

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

int main()
{
    string line;
    ifstream input;
    vector<pair<uint64_t, uint64_t>> points;
    uint64_t x = 0, y = 0, trench = 0;

    input.open("input.txt");

    while (!input.eof())
    {
        getline(input, line);
        auto parts = split(line, " ");

        uint64_t dist = stoi(parts[2].substr(2, 5), nullptr, 16);
        trench += dist;

        switch(parts[2][7])
        {
            case '0': y += dist; break;
            case '1': x += dist; break;
            case '2': y -= dist; break;
            case '3': x -= dist; break;
        }

        points.push_back(pair<uint64_t, uint64_t>(x, y));
    }

    uint64_t area = 0;
    int j = points.size() - 1;
    for (int i = 0; i < points.size(); i++)
    {
        // 2x2 matrix determinant
        area += (points[i].first * points[j].second) - (points[j].first * points[i].second);
        j = i;
    }
    if (area < 0) area *= -1;

    // Pick's Formula (to add trench area in)
    area = (area / 2) + (trench / 2) + 1;
    
    cout << endl << "Output: " << area << endl;
}