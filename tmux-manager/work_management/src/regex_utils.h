#ifndef REGEX_UTILS_H
#define REGEX_UTILS_H

#include <regex>
#include "utils.h"
#include "hgj_t.h"

using namespace std;

//
static void formatItemPrefix(const int& id, const char& state, string& prefix);
struct Item_info {
    bool has_state;
    char state;
};

// check the state
void getItemInfo(string& item, Item_info& item_info);

// search the number
void getLineNumber(string& line, vector<int>& num_vec) {
    num_vec.clear();
    regex re(" +([1-9][0-9]*) +");  // for example  1 2 3
    smatch sm;
    string str_num;
    for (sregex_iterator it(line.begin(), line.end(), re), end_it; it!=end_it; ++it) {
        str_num = it->str();
        num_vec.push_back(stoi(str_num));
    }
}

// replace the item
void dealItemsPrefix(File_t& file) {
    vector<string>& items = file.refItems();
    const vector<char>& states = file.getStates();
    int items_size = file.getItemsNum();
    // replace to the ""
    regex re("^ *[1-9][0-9]* *\\. *\\(([a-z-])\\)");

    for (auto& item : items) {
        // replace
        regex_replace(item, re, "");
    }
    // and then add the prefix
    char state;
    string prefix;
    string new_item;
    for (int i=0; i<items_size; ++i) {
        state = states[i];
        formatItemPrefix(i+1, state, prefix); // get the prefix
        // try to replace the prefix
        new_item = regex_replace(items[i], re, prefix);
        if (new_item == items[i]) { // not replace
            items[i] = prefix + items[i];
        }
        else {
            items[i] = new_item;
        }
    }
}

static void formatItemPrefix(const int &id, const char &state, string& prefix) {
    stringstream ss;
    // the number part
    if (id<10) {
        ss << " " << id;
    }
    else {
        ss << id;
    }
    // the state
    ss << ".(" << state << ")";
    prefix = ss.str();
}

#endif