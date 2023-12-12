#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
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

string label = "AKQJT98765432";

struct Hand
{
    string cards;
    int bid, rank;
};

int rankHand(string cards)
{
    map<char, int> counts;
    for (char c: cards) counts[c]++;

    bool threes = false;
    int pairs = 0;

    for (auto card: counts)
    {
        switch (card.second)
        {
            case 5: return 1; // 5 of a kind
            case 4: return 2; // 4 of a kind
            case 3: threes = true; break;
            case 2: pairs++; break;
        }
    }

    if (threes)
    {
        if (pairs == 1) return 3; // Full House
        else return 4; // Three of a kind
    }

    if (pairs == 2) return 5; // Two Pair
    if (pairs == 1) return 6; // One Pair

    return 7; // High card
}

bool compareHands(Hand left, Hand right)
{
    if (left.rank != right.rank) return left.rank > right.rank;
    
    for (int i = 0; i < 5; i++)
    {
        if (left.cards[i] == right.cards[i]) continue;

        return label.find(left.cards[i]) > label.find(right.cards[i]);
    }

    return false;
}

int main()
{
    string line;
    ifstream input;
    vector<Hand> hands;

    input.open("sample.txt");

    while (!input.eof())
    {
        getline(input, line);
        auto handLine = split(line, " ");
        hands.push_back(Hand{ handLine[0], stoi(handLine[1]), rankHand(handLine[0]) });
    }

    sort(hands.begin(), hands.end(), compareHands);

    for (auto hand: hands) cout << hand.cards << ": " << hand.rank << endl;

    int64_t winnings = 0;
    for (int i = 0; i < hands.size(); i++)
    {
        winnings += hands[i].bid * (i+1);
    }
    
    cout << endl << "Output: " << winnings << endl;
}