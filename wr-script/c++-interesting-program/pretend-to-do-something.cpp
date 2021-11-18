/* time: 2021.9.20.16:13 - xxx{{{
 * purpose: 
 * 1) make the progess and then show it
 * 2) pretend to do something when you are in the state of the moyu, hahaha!
 *//*}}}*/

#include<iostream>/*{{{*/
#include<string>
#include<chrono>
#include<thread>
#include<random>/*}}}*/

using namespace std;
// the global variable
namespace hgj_var
{
    int range_endPoint_count = 0;
}

// the function declaretion{{{
void wait(unsigned nms);
unsigned int get_random(unsigned min, unsigned max, string method);
void print_bar_renew(unsigned num_notation_showed, unsigned num_notation, unsigned line_length, char notation);
void print_bar(unsigned num_notation, unsigned line_length, unsigned nms, char notation);
string make_word(unsigned& c_num_min, unsigned& c_num_max, string method);
string make_paragraph(unsigned& w_num_min, unsigned& w_num_max,
                       unsigned& c_num_min, unsigned& c_num_max, string method);
void print_paragraph(const string& paragraph_content);/*}}}*/


int main(int argc, char ** argv)
{/*{{{*/
    // parse the arguments
    string mode_screen_size;
    string mode_speed;
    string mode_last_time;
    if (argc > 3)
    {
        mode_screen_size = argv[1];
        mode_speed = argv[2];
        mode_last_time = argv[3];
    }
    else if (argc == 3)
    {
        mode_screen_size = argv[1];
        mode_speed = argv[2];
        mode_last_time = "last";
    }
    else if(argc == 2)
    {
        mode_screen_size = argv[1];
        mode_speed = "slow";
        mode_last_time = "last";
    }
    else
    {
        mode_screen_size = "half";
        mode_speed = "slow";
        mode_last_time = "last";
    }
    // show the mode
    cout << "---------- MODE ----------" << endl;
    cout << "screen_size: " << mode_screen_size << endl;
    cout << "speed: " << mode_speed << endl;
    cout << "last_time: " << mode_last_time << endl;
    cout << "--------------------------" << endl;
    // check the arguments
    bool arg_check_mode_screen_size = mode_screen_size!="half" && mode_screen_size!="full";
    bool arg_check_mode_speed = mode_speed!="slow" && mode_speed!="quick";
    bool arg_check_mode_last_time = mode_last_time!="temporary" && mode_last_time!="last";
    if (arg_check_mode_screen_size || arg_check_mode_speed || arg_check_mode_last_time)
    {
        cout << "Your input is the: ";
        for (int i=0; i<argc; i++)
            cout << argv[i] << " ";
        cout << endl;
        cout << "Your argument is wrong!" << endl;
        cout << "The arugments can be the <full/half> [<quick/slow>] [<temporary/last>]." << endl;
        return -1;
    }
    // according to the mode to set the parameters of the program

    unsigned show_size[4];
    if (mode_screen_size == "half")
    {
        // show_size { range_min, range_max, w_num_min, w_num_max }
        unsigned show_size_temp[4] {10, 50, 10, 80};
        for (int i=0; i<4; i++)
            show_size[i] = show_size_temp[i];
    }
    else
    {
        // show_size { range_min, range_max, w_num_min, w_num_max }
        unsigned show_size_temp[4] {20, 100, 20, 160};
        for (int i=0; i<4; i++)
            show_size[i] = show_size_temp[i];
    }
    unsigned int speed;
    if (mode_speed == "slow")
        // set the nms_base's value
        speed = 30;
    else
        // set the nms_base's value
        speed = 5;

    bool flag_mode_last_time = false; // as the condition of the while loop in line 129.
    if (mode_last_time == "last")
        flag_mode_last_time = true;


    // the var: wait_time, notation, length
    char notation = '#';
    unsigned int range_min = show_size[0], range_max = show_size[1];
    string method = "default"; // "default", "normal_distribution", "others"

    unsigned int nms_base = speed;
    int multiply_eff;
    if (range_max < 30)
        multiply_eff = 2.5;
    else if(range_max > 30 && range_max < 70)
        multiply_eff = 2;
    else 
        multiply_eff = 1.5;
    //unsigned int line_length = (unsigned int) multiply_eff * range_max;
    unsigned int line_length = 1.5 * range_max;

    // in the loop and continue the output, break condition: rhgj_var::ange_endPoint_count > 20
    int threshold = 20;
    // the var about the paragraph
    unsigned c_num_min = 4;
    unsigned c_num_max = 12;
    unsigned w_num_min = show_size[2];
    unsigned w_num_max = show_size[3];
    string paragraph_method = "ohters";

    while(flag_mode_last_time || hgj_var::range_endPoint_count < threshold)
    {
        // make the paragraph
        string paragraph = make_paragraph(w_num_min, w_num_max, c_num_min, c_num_max, paragraph_method);
        // print the paragraph
        print_paragraph(paragraph);

        // generate the random number
        unsigned int num_notation = get_random(range_min, range_max, method);

        // print the line
        unsigned nms = nms_base * (rand() % 10 + 1);
        print_bar(num_notation, line_length, nms, notation);

        // renew the loop condition, rhgj_var::ange_endPoint_count
        if ((num_notation == range_min || num_notation == range_max) && (hgj_var::range_endPoint_count <= threshold))
        // the last condition is for the  flag_mode_last_time == true to prevent the last increasingof the hgj_var::rang_endPoint_count
            hgj_var::range_endPoint_count ++;
    }

    return 0;
}/*}}}*/

void wait(unsigned nms)
{/*{{{*/
    this_thread::sleep_for(chrono::milliseconds(nms));
}/*}}}*/

unsigned int get_random(unsigned min, unsigned max, string method)
{/*{{{*/
    unsigned int num_notation;
    if (method == "default")
    {
        unsigned seed = chrono::steady_clock::now().time_since_epoch().count();
        default_random_engine e(seed);

        uniform_int_distribution<int> distr(min, max);
        num_notation = distr(e);
    }
    else if(method == "normal_distribution")
    {
        num_notation = (unsigned int) (min + max) / 2;
    }
    else
    {
        //num_notation = (unsigned int) (min + max) / 2;
        num_notation = rand() % (max - min +  1) + min;
    }

    return num_notation;
}/*}}}*/

void print_bar_renew(unsigned num_notation_showed, unsigned num_notation, unsigned line_length, char notation)
{/*{{{*/
    // count the showed/show
    int show_percent = (double)num_notation_showed / (double)num_notation * 100;
    // show the: [, notation, ., ], percent in order
    cout << "[";
    for(int i=0; i<num_notation_showed; i++)
        cout << notation;
    for(int i=0; i<num_notation-num_notation_showed; i++)
        cout << ".";
    cout << "]";
    for (int i=0; i<line_length-num_notation-2; i++)
    {
        // if has the hgj_var::range_endPoint_count, output them, else output the space
        while(hgj_var::range_endPoint_count > 0 && i < hgj_var::range_endPoint_count)
        {
            cout << "*";
            i++;
        }
        cout << " ";

        /* --- obselete
        // output the "." * hgj_var::range_endPoint_count
        if(hgj_var::range_endPoint_count - i> 0)
            cout << "*";
        else
            // output the " " (space)
            cout << " ";
        --- obselete */
    }
    if (show_percent == 100)
        cout << "   " << "done!" << endl;
    else
        cout << "   " << show_percent << "%" << flush;
}/*}}}*/

void print_bar(unsigned num_notation, unsigned line_length, unsigned nms, char notation)
{/*{{{*/
    // recorrect the length of the line_length
    if (line_length <= num_notation)
    {
        cout << "The line_length is too small!" << endl;
        cout << "Change to the: " << (int) 1.5*line_length << endl;
    }
    // output the preview of the progress
    //for (int i=0; i<num_notation; i++)
    //{
    //    if (i==0)
    //        cout << "[";
    //    else if(i==line_length-1)
    //        cout << "]";
    //    else
    //        cout << ".";
    //}
    //cout << flush;
    // output the notation
    for (int i=1; i<=num_notation; i++)
    {
        print_bar_renew(i, num_notation, line_length, notation);
        wait(nms);
        cout << "\r" << flush;
    }
    cout << endl;
    /*  ----------- obselete -----------------
    cout << "\r" << flush; // return to the line begin
    for(int i=0; i<num_notation+2; i++)
    {
        if (i==0)
            cout << "[" << flush;
        else if(i==num_notation+1)
            cout << "]" << flush;
        else
        {
            cout << notation << "" << flush;
            wait(nms);
        }
    }
    ----------- obselete ----------------- */

    // output the ...done!
    /* ----------- obselete 
    cout << "\r" << flush;
    for(int i=0; i<line_length-num_notation; i++)
    {
        cout << "." << flush;
    }
    cout << " done!" << endl;
    */
}/*}}}*/

string make_word(unsigned& c_num_min, unsigned& c_num_max, string method)
{/*{{{*/
    // call the random function to get the excate number
    unsigned c_num = get_random(c_num_min, c_num_max, method);

    // get the character
    string word;
    for (int i=0; i<c_num; i++)
    {
        char c = get_random('a', 'z', method);
        word.push_back(c);
    }
    return word;
}/*}}}*/

string make_paragraph(unsigned& w_num_min, unsigned& w_num_max,
                       unsigned& c_num_min, unsigned& c_num_max, string method)
{/*{{{*/
    unsigned w_num = get_random(w_num_min, w_num_max, method);

    // get the paragraph
    string paragraph;
    for (int i=0; i<w_num; i++)
    {
        string word = make_word(c_num_min, c_num_max, method);
        word += " ";
        paragraph += word;
    }
    return paragraph;
}/*}}}*/

void print_paragraph(const string& paragraph_content)
{/*{{{*/
    cout << paragraph_content << endl;
}/*}}}*/
