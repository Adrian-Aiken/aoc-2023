#include <algorithm>
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

    input.open("input.txt");

    while (!input.eof())
    {
        getline(input, line);

        auto baseSplit = split(line, ": ");

        int gameNum = stoi(baseSplit[0].substr(5));
        int red = 0, green = 0, blue = 0;
        
        auto rounds = split(baseSplit[1], "; ");
        for (string round: rounds)
        {
            auto pulls = split(round, ", ");
            for (string pull: pulls)
            {
                auto parts = split(pull, " ");

                if      (parts[1] == "red")     red = max(  red, stoi(parts[0]));
                else if (parts[1] == "green") green = max(green, stoi(parts[0]));
                else if (parts[1] == "blue")   blue = max( blue, stoi(parts[0]));
            }
        }

        int power = red * green * blue;
        sum += power;
        
        cout << "Game " << gameNum << ": Power: " << power <<", Red: " << red << ", Green: " << green << ", Blue: " << blue << endl;
    }
    
    cout << endl << "Output: " << sum << endl;
}