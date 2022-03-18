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

// the c++ lib
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <regex>
// stl
#include <map>
#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
// python.h
#include <Python.h>

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
void printError(const string& );
void printHeader(const File_t& file, const Path_man& path_man, const Show_arg& show_arg);
void printTitle();
void printContent(const File_t& file, const Show_arg& show_arg);
void printPromptTime(Path_man& path_man, const stack<string>& record_time);
void printPromptFileType(Path_man& path_man, const stack<string>& record_time);
void getStateVerbose(const char state, string& state_verbose);

/* parse the input command */
void parseInput(string&, vector<string>&); // to
bool  isNum(const string& );

/* the stl, container, */
//template <class T_con, class T_ele>
//bool isInVec(const T_con<T_ele>& container, const T_ele& val) {
//    return find(container.begin(), container.end(), val) != container.end();
//};
template <class T_ele>
bool isInVec(const vector<T_ele>& vec, const T_ele& val) {
    return find(vec.begin(), vec.end(), val) != vec.end();
}

/* call the os */
void cls();

/* embed the python code */
void getDirFiles(const string& dir_path, vector<string>& files_path);
void printMonthDay(const string& dir_path, int year, int month, const string& file_type, bool is_calendar);
void printFileList(const string& dir_path, const vector<string>& file_type_list={}, const vector<char>& states_ls={});
string timeDurationFromNow(const string& history_time);
#endif
