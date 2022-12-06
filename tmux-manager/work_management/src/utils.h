#ifndef UTILS_H
#define UTILS_H

/* include the header */
// the c lib
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <iostream>
#include <ctime>
#include <cassert>
#include <climits>
#include "readline/readline.h"
#include "readline/history.h"

// the c++ lib
#include <iostream>
#include <fstream>
#include <sstream>
#include "boost/filesystem.hpp"
#include <regex>
// stl
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
// python.h
#include <Python.h>

// others
#include "color.h"

// third part
#include "json.hpp"

// the macro
#define NOTDO 'x'
#define NOTFINISH '-'
#define FINISH 'y'
using namespace std;
typedef unsigned int u_int;
typedef long long ll_int;

// class declaration
class File_t;
class Parser;
class Path_man;
class Show_arg;
class StudyT;


// function declare

/* about files and the dirs */
void getFiles(const string&, vector<string>&);
void getDirs(const string&, vector<string>&);
bool isExists(const string&);

/* deal the time */
void getDate(const int&, string&);
int getCurYear();
int getCurMonth();
int getCurMday();
int subDate(const string&, const string&);
int dateToDays(int, int, int);
string getWeekday(int, int, int);
bool is_leap(int );

/* print the message */
string hyphen(int);
string asterisk(int asterisk_num);
void printError(const string& );
void printHeader(const File_t& file, const Path_man& path_man, const Show_arg& show_arg);
void printTitle();
void printContent(const File_t& file, const Show_arg& show_arg, bool is_just_show_title=false);
string printPromptTime(Path_man& path_man, const stack<string>& record_time);
string printPromptStudy(const StudyT& study, bool is_toggle);
void renewStudyPrompt(string&prompt, const string&add_content, const char mark);
string printPromptFileType(Path_man& path_man, const stack<string>& record_time);
void getStateVerbose(const char state, string& state_verbose);
void printWarning(const string& your_type);
void printExe(const string& );

/* parse the input command */
void parseInput(string&, vector<string>&); // to
bool  isNum(const string& );
void printVecString(vector<string>& vec);
bool parseRange(string&, vector<int>&);
void strip(string& str);

/* the stl, container, */
//template <class T_con, class T_ele>
//bool isInVec(const T_con<T_ele>& container, const T_ele& val) {
//    return find(container.begin(), container.end(), val) != container.end();
//};
template <class T_ele>
bool isInVec(const vector<T_ele>& vec, const T_ele& val) {
    return find(vec.begin(), vec.end(), val) != vec.end();
}
using json = nlohmann::json;
void natureSort(json& questions_key_val, vector<string>& question);

/* call the os */
void cls();

/* embed the python code */
void getDirFiles(const string& dir_path, vector<string>& files_path);
void printMonthDay(const string& dir_path, int year, int month, const string& file_type, bool is_calendar);
void printFileList(const string& dir_path, const unordered_set<string>& file_type_list={}, const vector<char>& states_ls={});
void progressBar(int mins, bool is_hypen_num_short=false);
string timeDurationFromNow(const string& history_time);
#endif
