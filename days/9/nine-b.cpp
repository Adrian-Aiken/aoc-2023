#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

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
    int64_t finalSum = 0;

    input.open("input.txt");

    while (!input.eof())
    {
        getline(input, line);

        auto readingLine = split(line, " ");
        reverse(readingLine.begin(), readingLine.end());

        vector<vector<int>> readings;
        readings.push_back(vector<int>());
        for (auto num: readingLine) readings[0].push_back(stoi(num));

        // Step 1: Add lines until all are 0
        bool allZero = false;
        while (!allZero)
        {
            allZero = true;
            vector<int> lastLine = readings.back();
            vector<int> newLine;

            for (int i = 1; i < lastLine.size(); i++)
            {
                int delta = lastLine[i] - lastLine[i-1];
                newLine.push_back(delta);
                if (delta != 0) allZero = false;
            }

            readings.push_back(newLine);
        }

        // Step 2: Work backwards to add the new values
        for (int i = readings.size() - 2; i >= 0; i--)
        {
            readings[i].push_back(readings[i].back() + readings[i+1].back());
        }

        // Step 3: Add final sum
        //cout << "New reading: " << readings[0].back() << endl;
        finalSum += readings[0].back();
    }

    cout << endl << "Output: " << finalSum <<  endl;
}