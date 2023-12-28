#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

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

int HASH(string s)
{
    int value = 0;
    for (char c : s)
    {
        value += c;
        value *= 17;
        value %= 256;
    }
    return value;
}

int main()
{
    string line;
    ifstream input;
    vector<string> sequence;
    map<int, vector<string>> boxes;

    input.open("input.txt");
    getline(input, line);
    sequence = split(line, ",");

    for (string step : sequence)
    {
        if (step.back() == '-')
        {
            string prefix = step.substr(0, step.length() - 1);
            int hash = HASH(prefix);
            
            int removeIndex = -1;
            for (int i = 0; i < boxes[hash].size(); i++)
            {
                if (boxes[hash][i].substr(0, prefix.length()) == prefix)
                {
                    removeIndex = i;
                    break;
                }
            }

            if (removeIndex >= 0) boxes[hash].erase(boxes[hash].begin() + removeIndex);
        }
        else
        {
            auto parts = split(step, "=");
            int hash = HASH(parts[0]);

            int removeIndex = -1;
            for (int i = 0; i < boxes[hash].size(); i++)
            {
                if (boxes[hash][i].substr(0, parts[0].length()) == parts[0])
                {
                    removeIndex = i;
                    break;
                }
            }

            if (removeIndex >= 0) boxes[hash][removeIndex] = step;
            else boxes[hash].push_back(step);
        }
    }

    // Get lens focus
    uint64_t sum = 0;
    for (int i = 0; i < 256; i++)
    {
        for (int j = 0; j < boxes[i].size(); j++) sum += (i+1) * (j+1) * (boxes[i][j].back() - '0');
    }
    
    cout << endl << "Output: " << sum << endl;
}