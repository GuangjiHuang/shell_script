#include <iostream>
#include <fstream>
#include <regex>
#include <iterator>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

int main() {
    // read the line
    string line;
    const string path("./data/2022/03/03-11/task.txt");
    ifstream file(path);
    if (!file.is_open()) {
        cout << "Can not open the file!" << endl;
        return 1;
    }
    regex re("^ *[0-9]{1,2} *\\. *\\(([a-z-])\\).*");
    smatch sm;
    while (getline(file, line)) {
        if (line.empty()) {
            continue;
        }
        if (regex_match(line, sm, re)) {
            cout << line;
            cout << "(" << sm.str(1) << ")" << endl;
        }
        else {
            //cout << " -> not match!" << endl;
            ;
        }
    }
    //
    return 0;
}
