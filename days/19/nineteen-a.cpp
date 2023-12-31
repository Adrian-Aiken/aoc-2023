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

struct Rule {
    char category, comparison;
    int target;
    string destination;
};

int main()
{
    string line;
    ifstream input;
    map<string, vector<Rule>> workflows;

    input.open("input.txt");

    // Parse rules
    getline(input, line);
    while (line != "")
    {
        auto i = line.find("{");
        string label = line.substr(0, i);
        auto ruleStrings = split(line.substr(i+1, line.length() - i - 2), ",");

        vector<Rule> rules;
        for (auto rule: ruleStrings)
        {
            auto parts = split(rule, ":");
            
            if (parts.size() == 1) {
                rules.push_back(Rule{'a', '>', -1, rule});
            } else {
                rules.push_back(Rule{ parts[0][0], parts[0][1], stoi(parts[0].substr(2)), parts[1] });
            }
        }

        workflows[label] = rules;
        getline(input, line);
    }

    // Parse workflows
    uint64_t sum = 0;
    while (!input.eof())
    {
        getline(input, line);
        auto workflow = split(line.substr(1, line.length() - 2), ",");

        int x = stoi(workflow[0].substr(2));
        int m = stoi(workflow[1].substr(2));
        int a = stoi(workflow[2].substr(2));
        int s = stoi(workflow[3].substr(2));

        string step = "in";
        while (true)
        {
            if (step == "R") break;
            if (step == "A")
            {
                sum += x + m + a + s;
                break;
            }

            for (Rule rule : workflows[step])
            {
                int checkVal;
                switch (rule.category)
                {
                    case 'x': checkVal = x; break;
                    case 'm': checkVal = m; break;
                    case 'a': checkVal = a; break;
                    case 's': checkVal = s; break;
                }

                bool passing = false;
                switch (rule.comparison)
                {
                    case '>': passing = checkVal > rule.target; break;
                    case '<': passing = checkVal < rule.target; break;
                }

                if (passing)
                {
                    step = rule.destination;
                    break;
                }
            }
        }
    }
    
    cout << "Output: " << sum << endl;
}