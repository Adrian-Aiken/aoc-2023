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

int MAX_RED = 12;
int MAX_GREEN = 13;
int MAX_BLUE = 14;

int main()
{
    string line;
    ifstream input;
    int sum;

    input.open("sample.txt");

    while (!input.eof())
    {
        getline(input, line);

        auto baseSplit = split(line, ": ");

        int gameNum = stoi(baseSplit[0].substr(5));
        bool valid = true;
        
        auto rounds = split(baseSplit[1], "; ");
        for (string round: rounds)
        {
            auto pulls = split(round, ", ");
            for (string pull: pulls)
            {
                auto parts = split(pull, " ");

                if      (parts[1] == "red"   && stoi(parts[0]) > MAX_RED)   valid = false;
                else if (parts[1] == "green" && stoi(parts[0]) > MAX_GREEN) valid = false;
                else if (parts[1] == "blue"  && stoi(parts[0]) > MAX_BLUE)  valid = false;

                if (!valid) break;
            }

            if (!valid) break;
        }

        cout << (valid ? "[Good] " : "[ Bad] ") << line << endl;

        if (valid) sum += gameNum;
    }
    
    cout << endl << "Output: " << sum << endl;
}