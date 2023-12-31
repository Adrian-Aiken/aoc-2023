#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
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

struct Rule {
    char category, comparison;
    int64_t target;
    string destination;
};

struct PartSet {
    string workflow;
    pair<int64_t, int64_t> x, m, a, s;
};

int64_t SumParts(PartSet p)
{
    return (p.x.second - p.x.first + 1) *
        (p.m.second - p.m.first + 1) *
        (p.a.second - p.a.first + 1) *
        (p.s.second - p.s.first + 1);
}

int main()
{
    string line;
    ifstream input;
    map<string, vector<Rule>> workflows;

    input.open("sample.txt");

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
    queue<PartSet> parts;
    parts.push(PartSet{
        "in",
        pair<int, int>(1, 4000),
        pair<int, int>(1, 4000),
        pair<int, int>(1, 4000),
        pair<int, int>(1, 4000)
    });

    uint64_t sum = 0;
    while (!parts.empty())
    {
        auto part = parts.front(); parts.pop();

        // Check for valid ranges
        if (part.x.second < part.x.first || part.m.second < part.m.first || part.a.second < part.a.first || part.s.second < part.s.first)
        {
            continue;
        }

        if (part.workflow == "R") continue;
        if (part.workflow == "A")
        {
            sum += SumParts(part);
            continue;
        }

        for (Rule rule : workflows[part.workflow])
        {
            if (rule.target < 0)
            {
                part.workflow = rule.destination;
                parts.push(part);
                break;
            }

            PartSet left = PartSet(part);
            PartSet right = PartSet(part);

            switch (rule.comparison)
            {
                case '<':
                    switch (rule.category)
                    {
                        case 'x': left.x.second = rule.target - 1; right.x.first = rule.target; break;
                        case 'm': left.m.second = rule.target - 1; right.m.first = rule.target; break;
                        case 'a': left.a.second = rule.target - 1; right.a.first = rule.target; break;
                        case 's': left.s.second = rule.target - 1; right.s.first = rule.target; break;
                    }
                    left.workflow = rule.destination;
                    parts.push(left);
                    part = right;
                    break;

                case '>':
                    switch (rule.category)
                    {
                        case 'x': left.x.second = rule.target; right.x.first = rule.target + 1; break;
                        case 'm': left.m.second = rule.target; right.m.first = rule.target + 1; break;
                        case 'a': left.a.second = rule.target; right.a.first = rule.target + 1; break;
                        case 's': left.s.second = rule.target; right.s.first = rule.target + 1; break;
                    }
                    right.workflow = rule.destination;
                    parts.push(right);
                    part = left;
                    break;
            }
        }
    }
    
    cout << endl << "Output: " << sum << endl;
}