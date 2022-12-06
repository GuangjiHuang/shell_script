#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<cstring>

using namespace std;

int main() {
    vector<string> vec {"0.abc", "1.abc", "19.abc", "20.abc", "2.abc", "200.abc"};
    // cout
    for (auto ele : vec) cout << ele << ", ";
    cout << endl;
    auto sort_func = [](string&a, string&b) {
        return stoi(a.c_str()) < stoi(b.c_str());
    };
    sort(vec.begin(), vec.end(), sort_func);
    // cout
    for (auto ele : vec) cout << ele << ", ";
    cout << endl;
    //
    return 0;
}
