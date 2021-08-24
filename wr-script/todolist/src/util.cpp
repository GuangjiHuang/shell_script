/* the implement of the function */

#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<cctype>
#include<memory>
#include "util.h"
#define USE_RATE

using namespace std;
int get_int_from_line(string &s)
{/*{{{*/
    int result{0};
    bool digit_start {false};
    bool digit_end {false};
    bool digit_in {false};
    for (auto i : s)
    {
        // set the flag
        if (i == '(')
        {
            digit_start = true;
            continue;
        }
        else if (i == ')')
        {
            digit_end = true;
            break;
        }
        digit_in = digit_start && (!digit_end);

        if (digit_in && isdigit(i))
        {
            // renew the result
            result = result * 10 + (i - '0');
        }
    }
    return result;
}/*}}}*/

int sum_scores(const vector<char>& states, const vector<int>& scores)
{/*{{{*/
    int total_scores = 0;
    int get_scores = 0;
    for (size_t i=0; i<states.size(); i++)
    {
        int state_to_int;
        if (states[i] == 'y')
            state_to_int = 1;
        else if(states[i] == 'x')
            state_to_int = 0;
        else
        {
            //throw "The state is not the y or the x";
            //cout << "ERROR in " << __FILE__ << ":" << __LINE__ << endl;
            //cout << "the state is not the y or the x";
            state_to_int = 0;
        }
        get_scores += state_to_int * scores[i];
        total_scores += scores[i];
    }
#ifdef USE_RATE
        get_scores = (double)get_scores / total_scores * 100;
#endif
    return get_scores;
}/*}}}*/

void deal_modify_todo_list(const string &path, vector<char> &states, int total_scores)
{/*{{{*/
    // deal the modify string: line-5, line-7 and the line-9
    string line5_append;
    string line7_append;
    string line9_append;

    vector<int>line_5_y, line_7_x;
    vctovi(states, line_5_y, line_7_x);

    line5_append = vitos(line_5_y, 10);
    line7_append = vitos(line_7_x, 10);
    line9_append = itos(total_scores, 10);

    vector<string> all_lines;
    string line_temp;
    int line_number = 0;

    // creat the ifstream to read the line
    ifstream is(path);

    while(getline(is, line_temp))
    {
        line_number++;
        // preprocess with the line 5, 7, 9 ( get the pre 8 character)
        if (line_number == 5 || line_number == 7 || line_number == 9)
            line_temp = line_temp.substr(0, 8);

        if (line_number == 5)
        {
            line_temp += (" " + line5_append);
        }
        else if(line_number == 7)
        {
            line_temp += (" " + line7_append);
        }
        else if(line_number == 9)
        {
            line_temp += (" " + line9_append);
        }
        // add the newline at the end of the line
        line_temp += "\n";
        all_lines.push_back(line_temp);
    }
    is.close();

    string str_block;
    for (auto i : all_lines)
        str_block += i;
    // >>>>> debug the code <<<<<<
    //cout << " the content is as the follow: " << endl;
    //cout << str_block;

    // creat the ofstream to write the content that has been modifyed to the file
    ofstream os(path);
    os << str_block << flush;

    os.close();
}/*}}}*/

string itos(int x, int base)
{/*{{{*/
    if (base <2 || base > 16)
    {
        cout << "the base you input is: " << base << " Not qualify!" << endl;
        return "";
    }
    // the vars
    int digit_num = 0;
    char arr_char[8];
    string s_result;

    for(int quotient=x; quotient!=0; digit_num++)
    {
        int mode = quotient % base;
        quotient /= base;
        // the single interger to the char
        arr_char[digit_num] = mode + '0';
    }
    // check the number of the digit
    if (digit_num > 8)
    {
        cout << "ERROR in " << __FILE__ << ":" << __LINE__ << endl;
        cout << "Error: digit number too long!" << endl;
        return "";
    }
    else
    {
        while(--digit_num >= 0)
        {
            s_result.push_back(arr_char[digit_num]);
        }
    }

    // return the string
    return s_result;
}/*}}}*/

string vitos(const vector<int> &vi, int base)
{/*{{{*/
    // the var
    string s_result;

    for (size_t i=0; i<vi.size(); i++)
    {
        // call the itos to convert the interger to the string
        s_result += itos(vi[i], base);
        // add the , as the separate sign
        if (i != vi.size() - 1)
            s_result += ", ";
    }

    return s_result;
}/*}}}*/

void vctovi(const vector<char> &vc, vector<int> &vi_y, vector<int> &vi_x)
{/*{{{*/
    for(size_t i=0; i<vc.size(); i++)
    {
        if (vc[i]=='x')
            vi_x.push_back(i+1);
        else
            vi_y.push_back(i+1);
    }
}/*}}}*/
