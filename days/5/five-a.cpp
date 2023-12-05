#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct AlmMap {
    int64_t destination, source, range;
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
    auto seeds = castToint64_t(split(line.substr(7), " "));

    getline(input, line); // Blank line

    // Map processing
    while (!input.eof())
    {
        getline(input, line);
        cout << "Processing " << line << endl;
        
        // Parse map values
        vector<AlmMap> almanac;
        while (line != "")
        {
            getline(input, line);

            if (input.eof() || line == "") break;

            auto parsedLine = castToint64_t(split(line, " "));
            almanac.push_back(AlmMap{parsedLine[0], parsedLine[1], parsedLine[2]});
        }

        // Convert new seed values
        vector<int64_t> mappedSeeds;
        for (int64_t seed: seeds)
        {
            bool mapped = false;
            for (AlmMap almMap : almanac)
            {
                if (seed >= almMap.source && seed <= (almMap.source + almMap.range))
                {
                    int64_t delta = almMap.destination - almMap.source;
                    mappedSeeds.push_back(seed + delta);
                    mapped = true;
                    break;
                }
            }

            if (!mapped) mappedSeeds.push_back(seed);
        }

        // Print64_t out seed values
        for (int64_t i : mappedSeeds) cout << i << " ";
        cout << endl << endl;

        seeds = mappedSeeds;
    }

    cout << "---------------------------------------------" << endl << endl;
    cout << "Closest location: " << *min_element(seeds.begin(), seeds.end()) << endl;;
}