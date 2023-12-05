#include <algorithm>
#include <iostream>
#include <fstream>
#include <stdint.h>
#include <string>
#include <vector>
#include <queue>

using namespace std;

struct AlmMap {
    int64_t start, end, delta;
};

struct SeedRange {
    int64_t start, end;
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

vector<int64_t> castToint64_t(vector<string> stringValues)
{
    vector<int64_t> values;
    for (string s: stringValues) values.push_back(stoll(s));
    return values;
}

int main()
{
    string line;
    ifstream input;

    input.open("sample.txt");

    // Seeds
    getline(input, line);
    auto seedsVals = castToint64_t(split(line.substr(7), " "));
    queue<SeedRange> seeds;
    for (int i = 0; i < seedsVals.size(); i += 2)
    {
        seeds.push(SeedRange{seedsVals[i], seedsVals[i]+seedsVals[i+1]});
    }

    getline(input, line); // Blank line

    // Map processing
    while (!input.eof())
    {
        getline(input, line);
        
        // Parse map values
        vector<AlmMap> almanac;
        while (line != "")
        {
            getline(input, line);

            if (input.eof() || line == "") break;

            auto parsedLine = castToint64_t(split(line, " "));
            almanac.push_back( AlmMap{ parsedLine[1], parsedLine[1]+parsedLine[2], parsedLine[0]-parsedLine[1] });
        }

        // Convert new seed values
        queue<SeedRange> mappedSeeds;
        while (!seeds.empty())
        {
            SeedRange seed = seeds.front(); seeds.pop();
            bool overlapped = false;
            for (AlmMap almMap : almanac)
            {
                // Check if any overlap
                if ((seed.start <= almMap.end) && (almMap.start <= seed.end))
                {
                    overlapped = true;
                    // Remove before overlap
                    if (seed.start < almMap.start)
                    {
                        seeds.push(SeedRange{ seed.start, almMap.start-1 });
                        seed.start = almMap.start;
                    }

                    // Remove after overlap
                    if (seed.end > almMap.end)
                    {
                        seeds.push(SeedRange{ almMap.end+1, seed.end });
                        seed.end = almMap.end;
                    }

                    // Map the remaining
                    mappedSeeds.push(SeedRange{seed.start+almMap.delta, seed.end+almMap.delta });
                }
            }

            if (!overlapped) mappedSeeds.push(SeedRange{ seed.start, seed.end });
        }

        seeds = mappedSeeds;
    }

    int64_t minLocation = INT64_MAX;
    cout << "Final Ranges: ";
    while (!seeds.empty())
    {
        cout << seeds.front().start << "-" << seeds.front().end << "; ";
        minLocation = min(minLocation, seeds.front().start);
        seeds.pop();
    }

    cout << endl << endl << "Closest location: " << minLocation << endl;
}