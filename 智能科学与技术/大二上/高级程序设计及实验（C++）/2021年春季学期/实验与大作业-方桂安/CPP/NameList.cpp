#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

using namespace std;

class NameList{
private:
    vector<string> names;
public:
    NameList() {}
    NameList(const NameList &o) : names(o.names) {}
    ~NameList() {}
    NameList operator=(const NameList &o) {names = o.names;}
    void addName(const string &str) {names.push_back(str);}
    void deleteName(const string &str) {
        auto iter = names.begin();
        while (iter != names.end()) {
            if (*iter == str) iter = names.erase(iter);
            else ++iter;
        }
    }
    vector<string> search(const string &substr) {
        vector<string> res;
        for (auto &str: names) {
            if (str.find(substr) != -1) res.push_back(str);
        }
        return res;
    }
    void print(int order = 0) {
        if (order == 0) {
            for (auto &str: names) cout << str << endl;
        }
        else {
            vector<string> _names = names;
            if (order == 1) {
                sort(_names.begin(), _names.end());
                for (auto &str: _names) cout << str << endl;
            }
            else {
                if (order == 2) {
                    sort(_names.begin(), _names.end(), greater<string>());
                    for (auto &str: _names) cout << str << endl;
                }
            }
        }
    }

};

int main() {
    NameList nl;
    nl.addName("One");
    nl.addName("Two");
    nl.addName("Three");
    nl.addName("Four");
    nl.addName("Five");
    NameList nl2(nl);
    nl2.print(); printf("\n");
    nl.addName("Six");
    nl.addName("Seven");
    nl.print(1);
    printf("\n");
    nl.print(2);
    printf("\n");
    for (auto &t: nl.search("o")) cout << t << endl;
}