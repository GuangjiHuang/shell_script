#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "util.h"

using namespace std;

int main()
{/*{{{*/
    /* the variables */
    // the path
    string todo_list_path;
    string cfg_path {"./date-path.txt"};
    string scores_path {"./scores.template"};
    // the vector to store the information
    vector<int> scores;
    vector<char> states;


    /* read the file */
    // get the todolist_path
    ifstream file_1(cfg_path);
    getline(file_1, todo_list_path);
    file_1.close();

    // get the scores table
    ifstream file_2(scores_path);
    string temp_line;
    while(getline(file_2, temp_line))
    {
        if (temp_line[0] != '(')
            continue;
        else
        {
            // get the arrary and then to the int
            int score = get_int_from_line(temp_line);
            scores.push_back(score);
        }
    }
    file_2.close();

    // get the state of todolist
    // if the platform is the window, you need to specify the path adding the d:/cygwin64/
    todo_list_path = "D:/cygwin64/" + todo_list_path;
    // >>>> debug code <<<
    //cout << "the path of the todo_list " << todo_list_path << endl;
    // >>>> debug code <<<
    ifstream file_3(todo_list_path);
    while(getline(file_3, temp_line))
    {
        if (temp_line[0] != '(')
            continue;
        else
        {
            // get the x or the y
            char state {temp_line[1]};
            states.push_back(state);
        }
    }
    file_3.close();

    // cout the scores and the state
    // first check the size, it must be equal
    if (states.size() != scores.size())
    {
        //throw "Error: the size of the scores and the states not equal! Please check the file!";
        cout << "ERROR in " << __FILE__ << ":" << __LINE__ << endl;
        cout << "the states size is: " << states.size() << endl;
        cout << "the scores size is: " << scores.size() << endl;
    }
    else
    {
        for (size_t i=0; i<states.size(); i++)
        {
            // >>> debug : cout the result 
            //cout << i+1 << ". " << scores[i] << " " << states[i] << endl;
            break;
        }
    }
    // then call the function to sum the total score
    int total_scores = sum_scores(states, scores);

    // >>> debug
    //cout << "the total scores is: " << total_scores << endl;

    // write the result to the file;
    deal_modify_todo_list(todo_list_path, states, total_scores);


    return 0;
}/*}}}*/
