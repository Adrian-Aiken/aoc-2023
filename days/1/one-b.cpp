#include <algorithm>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct DigitWord {
    string word;
    string digit;
};

vector<DigitWord> DigitMap = {
    {"zero", "z0ro"},
    {"one", "o1e"},
    {"two", "t2o"},
    {"three", "th3ee"},
    {"four", "fo4r"},
    {"five", "f5ve"},
    {"six", "s6x"},
    {"seven", "se7en"},
    {"eight", "ei8ht"},
    {"nine", "ni9e"}
};

bool isDigit(char c)
{
    return c >= '0' && c <= '9';
}

string undigitString(string s)
{
    string workingString = s;

    for (DigitWord dw: DigitMap)
    {
        size_t found = workingString.find(dw.word);
        while (found != string::npos)
        {
            workingString.replace(found, dw.digit.length(), dw.digit);
            found = workingString.find(dw.word);
        }
    }

    return workingString;
}

int main()
{
    string line;
    ifstream input;
    int sum;

    input.open("input.txt");

    // Set up digit map

    while (!input.eof())
    {
        getline(input, line);
        string cleanLine = undigitString(line);

        int num;
        string::iterator it = cleanLine.begin();

        // Left - 10s digit
        while (!isDigit(*it)) it++;
        num = *it - '0';
        num *= 10;

        // Right - 1s Digit
        string::reverse_iterator rit = cleanLine.rbegin();
        while (!isDigit(*rit)) rit++;
        num += *rit - '0';

        cout << num << " - " << left << setw(20) << cleanLine  << " (" << line << ')' << endl;

        sum += num;
    }
    
    cout << endl << "Total Sum: " << sum << endl;
}