#ifndef UTIL_H
#define UTIL_H

/* put your function declaration in here */
#include<string>
#include<vector>
// the function's declaration
using namespace std;
int get_int_from_line(string &);
int sum_scores(const vector<char>&, const vector<int>& );
void deal_modify_todo_list(const string &, vector<char> &, int);
string itos(int, int);
string vitos(const vector<int>&, int);
void vctovi(const vector<char> & , vector<int> &, vector<int> &);

#endif
