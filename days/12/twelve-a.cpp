#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <numeric>

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

vector<string> recurseFillSprings(string base, int toPlace, int missingCount)
{
    // Base 1 - if there's not enough spaces to place the springs, return
    if (toPlace > missingCount) return vector<string>();

    // Base 2 - if no more springs place, fill rest of string and return that
    if (toPlace == 0)
    {
        vector<string> toReturn;
        for (int i = 0; i < base.length(); i++)
        {
            if (base[i] == '?') base[i] = '.';
        }

        toReturn.push_back(base);
        return toReturn;
    }

    int missingIndex = base.find('?');

    // Case - Broken spring
    base[missingIndex] = '#';
    auto brokenStrings = recurseFillSprings(base, toPlace-1, missingCount-1);

    // Case - Working spring
    base[missingIndex] = '.';
    auto workingStrings = recurseFillSprings(base, toPlace, missingCount-1);

    brokenStrings.insert(brokenStrings.end(), workingStrings.begin(), workingStrings.end());
    return brokenStrings;
}

bool isValid(string row, vector<int> runs)
{
    int i = 0, runIndex = 0, curRun = 0;
    bool inRun = false;
    row += ' ';

    while (i < row.length())
    {
        if (row[i++] == '#')
        {
            inRun = true;
            curRun++;
            continue;
        }

        // Current run does not match expected run
        if (inRun && curRun != runs[runIndex++]) return false;

        inRun = false;
        curRun = 0;
    }

    //return runIndex == runs.size();
    return true;
}

int main()
{
    string line;
    ifstream input;
    int validSum = 0;
    int rowCount = 1;

    input.open("sample.txt");

    while (!input.eof())
    {
        getline(input, line);
        auto lineSplit = split(line, " ");

        string row = lineSplit[0];
        vector<int> runs;
        for (auto s: split(lineSplit[1], ","))
        {
            runs.push_back(stoi(s));
        }

        int toFind = reduce(runs.begin(), runs.end());
        vector<int> unknowns;
        for (int i = 0; i < row.length(); i++)
        {
            switch (row[i])
            {
                case '#': toFind--; break;
                case '?': unknowns.push_back(i); break;
                case '.': break;
            }
        }

        auto fullStrings = recurseFillSprings(row, toFind, unknowns.size());

        int validCount = 0;
        for (string s: fullStrings)if (isValid(s, runs)) validCount++;
        validSum += validCount;

        cout << rowCount++ << ":\t" <<  validCount << " - " << line << endl;
    }
    
    cout << endl << "Output: " << validSum << endl;
}