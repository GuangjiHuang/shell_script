#define USE_READLINE
#include "utils.h"
#include "hgj_t.h"
#include "command.h"
#include <stack>
#include <locale>

using namespace std;
// the static vector file type
unordered_set<string> Path_man::defined_file_type {RECORD, QUESTION, TASK, LEARN};
// class declaration
class parser;
int main(int argc, char** argv) {
    // get the argument
    if (argc==1) {
        cout << "Error: must include the file parent dir!" << endl;
        exit(1);
    }
    string parent_dir(argv[1]);
    // the Path_man
    Path_man path_man(parent_dir);
    // the file
    string file_path(path_man.getCurPath()); // open the path_man's path
    File_t file(file_path, "r+");
    // the parser
    Parser parser;
    string input_command;
    // the AliasParser
    const string alias_path("alias_command.txt");
    AliasParser alias_parser(alias_path);
    // the show arg
    Show_arg show_arg(file, 'a', INT_MAX);

    // the var
    bool prompt_toggle {true};
    stack<string> record_time; // use to put the time begin inside the stack
    // for the prompt
    string prompt_str;
    char* prompt_char_p = nullptr;
    while (true) {
        if (prompt_toggle) {
            prompt_str = printPromptFileType(path_man, record_time);
        }
        else {
            prompt_str = printPromptTime(path_man, record_time);
        }
#ifdef USE_READLINE
        prompt_char_p = readline(prompt_str.c_str());
        input_command = string(prompt_char_p);
        free(prompt_char_p);
#else
        // ouput the prompt
        cout << prompt_str << endl;
        cout << 
        // parse the command
        getline(// change the path_man's file
            // show date and change date
            cmd_file(file, parser, path_man, show_arg);
        }
        else  if (command == "date") {
            // change the path_man's date
            cmd_date(file, parser, path_man, show_arg);
        }
        else if (command == "label") {
            cmd_label(file, parser, alias_parser, path_man, show_arg);
        }
        else if (command == "list") {
            cmd_list(file, path_man, parser);
        }
        else if (command == "alias") {
            cmd_alias(parser, alias_parser);
        }
        else if (command == "operation") {
            cmd_operation(file, parser);
        }
        else if (command == "timer") {
            cmd_timer(parser);
        }
        else if (command == "openInVim") {
            cmd_openInVim(file, path_man);
        }
        else if (command == "record_in" || command == "record_out" || command == "record_abort") {
            cmd_recordOperation(file, path_man, parser, record_time);
        }
        else if (command == "shell") {
            // if is the shell command, combine rest of the command as one
            stringstream ss;
            copy(parser.arg_ls.begin()+1, parser.arg_ls.end(), ostream_iterator<string>(ss, " "));
            try {
                system(ss.str().c_str());
            }
            catch (...) {
                cout << "[Warning]: your shell command is not right!" << endl;
            }
        }
        else if (command == "clear") {
            cls();
        }
        else if (command == "save") {
            cmd_save(file);
            cout << "save!" << endl;
        }
        else if (command == "reload") {
            file.readToBuffer("r+");
            cout << "[Tip]: reload successfully!" << endl;
        }
        else if (command == "prompt_toggle" || command == "pt") {
            prompt_toggle = !prompt_toggle;
            cls();
            printHeader(file, path_man, show_arg);
            printContent(file, show_arg);
        }
        else if (command == "exit") {
            cmd_save(file);
            cout << "exit!" << endl;
            break;
        }
        else if (command == "help") {
            cout << "Help information!" << endl;
        }
        else {
            cout << "You command is not right!" << endl;
        }
    }
}
