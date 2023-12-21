#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <numeric>
#include <map>
#include <sstream>

using namespace std;

map<string, uint64_t> observed;

string getState(string start, vector<int> runs)
{
    stringstream ss;

    auto strIndex = start.find('?');
    if (strIndex == string::npos) strIndex = 0;

    while (strIndex > 0 && start[strIndex-1] == '#') strIndex--;

    ss << start.substr(strIndex);
    for (int run : runs)
    {
        ss << run << ' ';
    }

    return ss.str();
}

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

bool isValid(string* row, vector<int> runs, vector<int>& outRuns)
{
    int i = 0, runIndex = 0, curRun = 0;
    bool inRun = false;

    while (i < row->length())
    {
        // Base-ish case. Unknown up to this point.
        // If run in still fine, assume fine
        if (row->at(i) == '?') 
        {
            bool toReturn = curRun <= runs[runIndex];
            //if (curRun == runs[runIndex] && row->at(i+1) == '?') runIndex++;
            outRuns.insert(outRuns.begin(), runs.begin()+runIndex, runs.end());
            return toReturn;
        }

        if (row->at(i++) == '#')
        {
            inRun = true;
            curRun++;
            continue;
        }

        // Current run does not match expected run
        if (inRun && curRun != runs[runIndex++]) return false;

        // All Runs accounted for
        if (runIndex == runs.size()) return true;

        inRun = false;
        curRun = 0;
    }

    return true;
}

uint64_t recurseCountValid(string *base, int toPlace, int missingCount, vector<int> runs)
{
    // Base 1 - if there's not enough spaces to place the springs, return
    if (toPlace > missingCount)
    {
        return 0;
    }

    // Base 2 - if no more springs place, fill in the blanks and test validity
    if (toPlace == 0)
    {
        string *check = new string(*base);
        for (int i = 0; i < check->size(); i++)
        {
            if (check->at(i) == '?') check->at(i) = '.';
        }

        vector<int> _;
        return isValid(check, runs, _) ? 1 : 0;
    }

    int missingIndex = base->find('?');

    // Check if we've seen the back half yet
    vector<int> baseRuns;
    isValid(base, runs, baseRuns);
    string baseState = getState(*base, baseRuns);
    if (observed.find(baseState) != observed.end())
        return observed[baseState];

    uint64_t validCount = 0;

    // Case - Broken spring
    base->at(missingIndex) = '#';
    vector<int> brokenRuns;
    if (isValid(base, runs, brokenRuns))
    {
        uint64_t brokenCount = recurseCountValid(base, toPlace-1, missingCount-1, runs);
        validCount += brokenCount;
    }

    // Case - Working spring
    base->at(missingIndex) = '.';
    vector<int> workingRuns;
    if (isValid(base, runs, workingRuns))
    {
        uint64_t workingCount = recurseCountValid(base, toPlace, missingCount-1, runs);
        validCount += workingCount;
    }

    base->at(missingIndex) = '?';

    observed[getState(*base, baseRuns)] = validCount;
    return validCount;
}

int main()
{
    string line;
    ifstream input;
    uint64_t validSum = 0;
    int rowCount = 1;

    input.open("input.txt");

    while (!input.eof())
    {
        getline(input, line);
        auto lineSplit = split(line, " ");

        string* row = new string(lineSplit[0] + '?' + lineSplit[0] + '?' + lineSplit[0] + '?' + lineSplit[0] + '?' + lineSplit[0]);
        row->push_back(' ');
        vector<int> runsegment, runs;
        for (auto s: split(lineSplit[1], ","))
        {
            runsegment.push_back(stoi(s));
        }
        for (int i = 0; i < 5; i++)
        {
            for (auto r: runsegment) runs.push_back(r);
        }

        int toFind = reduce(runs.begin(), runs.end());
        vector<int> unknowns;
        for (int i = 0; i < row->length(); i++)
        {
            switch (row->at(i))
            {
                case '#': toFind--; break;
                case '?': unknowns.push_back(i); break;
                case '.': break;
            }
        }

        uint64_t validCount = recurseCountValid(row, toFind, unknowns.size(), runs);
        validSum += validCount;

        cout << rowCount++ << ":\t" <<  validCount << " - " << line << endl;
    }
    
    cout << endl << "Output: " << validSum << endl;
}