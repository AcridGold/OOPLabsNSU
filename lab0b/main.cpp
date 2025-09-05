#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <list>
#include <vector>

using namespace std;

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        cout << "Usage: ./lab0b [input] [output]\n";
        return 1;
    }

    ifstream fin(argv[1]);
    ofstream fout(argv[2]);
    string line;
    list<string> lines;
    map<string, int> wordCount;
    int totalWords = 0;

    while (getline(fin, line)) {
        string word;
        for (size_t i = 0; i <= line.size(); i++) {
            char c = (i < line.size()) ? line[i] : ' ';
            if (iswalnum(c))
            {
                word += c;
            } else
            {
                if (!word.empty()) {
                    wordCount[word]++;
                    totalWords++;
                    word.clear();
                }
            }
        }
    }

    // for (auto it = wordCount.begin(); it != wordCount.end(); ++it) {
    //     cout << "Word: " << it->first << ", Count: " << it->second << endl;
    // }

    vector<pair<string, int>> words(wordCount.begin(), wordCount.end());
    sort(words.begin(), words.end(), [](const auto& a, const auto& b) {
        if (a.second == b.second) {
            return a.first < b.first;
        }
        return a.second > b.second;
    });

    fout << "Word,Frequency,Frequency(%)\n";
    for (const auto& pair : words) {
        double percentage = (static_cast<double>(pair.second) / totalWords) * 100.0;
        fout << pair.first << "," << pair.second << "," << (int)percentage << "\n";
    }
    return 0;
}
