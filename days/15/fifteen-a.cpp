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

    input.open("sample.txt");
    getline(input, line);
    sequence = split(line, ",");

    uint64_t sum = 0;
    for (string step : sequence) sum += HASH(step);
    
    cout << endl << "Output: " << sum << endl;
}