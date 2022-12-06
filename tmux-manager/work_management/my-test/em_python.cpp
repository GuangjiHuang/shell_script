#include <Python.h>
#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include "utils.h"
#include "hgj_t.h"

// the static vector file type
unorderedmap<string> Path_man::defined_file_type {RECORD, QUESTION, TASK, LEARN};
using namespace std;


int main() {
    // the var
    //const string dir_path("./data");
    //int year = 2022;
    //int month = 2;
    //const string file_type("task");
    //// call the function
    //printMonthDay(dir_path, year, month, file_type);

    //
    const string dir_path("data/2022/03/03-11");
    const vector<char>states_ls {'x', '-', 'y'};
    //printFileList(dir_path, Path_man::defined_file_type, states_ls);
    printFileList(dir_path);


    return 0;
}
