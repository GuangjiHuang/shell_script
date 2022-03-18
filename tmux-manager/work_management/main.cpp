#include "utils.h"
#include "hgj_t.h"
#include "command.h"


using namespace std;

// the static vector file type
vector<string> Path_man::defined_file_type {RECORD, QUESTION, TASK, LEARN};
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
    while (true) {
        if (prompt_toggle) {
            printPromptFileType(path_man, record_time);
        }
        else {
            printPromptTime(path_man, record_time);
        }
        // parse the command
        getline(cin, input_command);
        if (cin.eof()) {
            cin.clear();
            continue;
        }
        alias_parser.parseAliasCommand(input_command);
        parser.update(input_command);
        string command(parser.command);

        // key enter
        if (parser.empty()) {
            cls();
            printTitle();
            continue;
        }
        // check the command
        if (command == "show") {
            cls();
            // show the file
            //file.bufferToCout();
            cmd_show(file, path_man, parser, show_arg); // will change the show_arg
        }
        else if (command == "add") {
            // enter the input state, what you input will store in the file._items
            cmd_add(file, path_man);
        }
        else if (command == "file") {
            // change the path_man's file
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
