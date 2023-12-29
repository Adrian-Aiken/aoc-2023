#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>

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
    vector<string> field;
    int x = 0, y = 0;

    input.open("input.txt");
    field.push_back("#");

    while (!input.eof())
    {
        getline(input, line);
        auto parts = split(line, " ");

        int distance = stoi(parts[1]);
        for (int i = 0; i < distance; i++)
        {
            switch (parts[0][0])
            {
                case 'U': 
                    x--;
                    if (x < 0)
                    {
                        field.insert(field.begin(), string(field[0].size(), '.'));
                        x = 0;
                    }
                    break;

                case 'D': 
                    x++;
                    if (x == field.size())
                    {
                        field.push_back(string(field[0].size(), '.'));
                    }
                    break;

                case 'L': 
                    y--;
                    if (y < 0)
                    {
                        for (int j = 0; j < field.size(); j++) field[j] = '.' + field[j];
                        y = 0;
                    }
                    break;

                case 'R':                
                    y++;
                    if (y == field[0].size())
                    {
                        for (int j = 0; j < field.size(); j++) field[j] += '.';
                    }
                    break;
            }

            field[x][y] = '#';
        }
    }
    
    // Add ring of empty around field
    for (int j = 0; j < field.size(); j++) field[j] = '.' + field[j] + '.';
    field.insert(field.begin(), string(field[0].size(), '.'));
    field.push_back(string(field[0].size(), '.'));

    // Flood fill field
    queue<pair<int, int>> queue;
    queue.push(pair<int, int>(0, 0));
    while (!queue.empty())
    {
        auto space = queue.front(); queue.pop();

        if (space.first < 0 || space.first >= field.size() || space.second < 0 || space.second >= field[0].size()) continue;
        if (field[space.first][space.second] != '.') continue;
        field[space.first][space.second] = ' ';

        queue.push(pair<int, int>(space.first+1, space.second));
        queue.push(pair<int, int>(space.first-1, space.second));
        queue.push(pair<int, int>(space.first, space.second+1));
        queue.push(pair<int, int>(space.first, space.second-1));
    }

    for (int i = 0; i < field.size(); i++) for (int j = 0; j < field[i].size(); j++) if (field[i][j] == '.') field[i][j] = '#';
    
    //cout << "--------------" << endl;
    //for (auto s: field) cout << s << endl;

    int dugOut = 0;
    for (string s: field) for (char c: s) if (c == '#') dugOut++;

    cout << endl << "Output: " << dugOut << endl;
}