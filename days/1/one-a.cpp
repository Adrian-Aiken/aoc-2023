#include <iostream>
#include <fstream>
#include <string>

using namespace std;

bool isDigit(char c)
{
    return c >= '0' && c <= '9';
}

int main()
{
    string line;
    ifstream input;
    int sum;

    input.open("sample.txt");

    while (!input.eof())
    {
        getline(input, line);

        int num;
        string::iterator it = line.begin();

        // Left - 10s digit
        while (!isDigit(*it)) it++;
        num = *it - '0';
        num *= 10;

        // Right - 1s Digit
        string::reverse_iterator rit = line.rbegin();
        while (!isDigit(*rit)) rit++;
        num += *rit - '0';

        cout << num << " - " << line << endl;

        sum += num;
    }
    
    cout << endl << "Total Sum: " << sum << endl;
}