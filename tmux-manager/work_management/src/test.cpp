#include <iostream>
#include "json.hpp"
#include <fstream>
#include <cctype>

using namespace std;
using json = nlohmann::json;

void strip(string& str) {
    int len = str.size();
    int l = 0, r = len - 1;
    bool flag_l = false, flag_r = false;
    for (int i=0; i<len; ++i) {
        char c = str[i];
        // mark the left
        if (!flag_l && !isspace(c) && (i-1>=0 && isspace(str[i-1]) || i==0)) {
            l = i;
            flag_l = true;
        }
        // mark the right 
        if (!flag_r && !isspace(c) && (i+1<len && isspace(str[i+1]) || i==len)) {
            r = i;
            flag_r = true;
        }
    }
    str = str.substr(l, r-l+1);
}

int main() {
    string file_path("src/questions.json");
    ifstream f(file_path);
    if (!f) {
        cout << "Can not open the file!" << endl;
    }

    json data = json::parse(f);
    //
    for (auto topic1=data.begin(); topic1!=data.end(); ++topic1) {
        cout << topic1.key() << endl;
    }
    string topic1, topic2, question;
    json topic1_js, topic2_js, question_js;
    string prompt_t1("[?] input topic1: ");
    string prompt_t2;
    GOLINE:
    cout << prompt_t1;
    string input_t1;
    string input_t2;
    while (getline(cin, input_t1)) {
        strip(input_t1);
        cout << "the input_t1:" << input_t1 << "#" << endl;
        // check
        if (! data.contains(input_t1)) {
            cout << prompt_t1;
            continue;
        }
        topic1 = input_t1;
        topic1_js = data[topic1];
        // renew the prompt_t1 show
        cout <<  "\r";
        cout << "[ " << topic1 << " ] input topic2: ";
        // get the topic2
        while (getline(cin, input_t2)) {
            strip(input_t2);
            if (input_t2 == "<") {
                cout << "\r";
                cout << prompt_t1;
                break;
            }
            if (! data[topic1].contains(input_t2)) {
                cout << "[Warning:] No exist the topic2: " << topic2 << endl;
                cout << "[ " << topic1 << " ] input topic2: ";
                continue;
            }
            topic2 = input_t2;
            // cout the result
            topic2_js = data[topic1][topic2];
            int i = 1;
            for (auto it=topic2_js.begin(); it!=topic2_js.end(); ++it) {
                cout << it.key() << endl;
            }
            cout << "----------------------------------------------------------------------" << endl;
            cout << "[ " << topic1 << " ] input topic2: ";
        }
        if (cin.eof()) {
            cout << prompt_t1 << "ctrl + D";
            clearerr(stdin);
        }
    }
    // check if contains
    string check_key("tool");
    cout << data.contains(check_key) << endl;

    // save the json to the file
    string save_str = data.dump(4);
    ofstream f_out(file_path);
    f_out << save_str;
    f_out.close();
    return 0;
}
