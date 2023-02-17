#include "utils.h"
#include "hgj_t.h"
#include "command.h"
#include <stack>
#include "yaml-cpp/yaml.h"
//#include "yaml.h"

using namespace std;

extern bool FILE_IS_STUDY;

// the static vector file type
unordered_set<string> Path_man::defined_file_type {RECORD, QUESTION, TASK, LEARN, STUDY};
// class declaration
class parser;
int main(int argc, char** argv) {
    // set the prompt
    /* ------------------------------ */
    // no need the argv, instead of the yaml-config
    string config_path("./config/config.yml");
    YAML::Node config = YAML::LoadFile(config_path);
    //path
    string data_dir = config["data_dir"].as<string>();
    string answer_path = config["answer_path"].as<string>();
    string question_path = config["question_path"].as<string>();
    string alias_path = config["alias_path"].as<string>();
    string study_help_path = config["study_help_path"].as<string>();
    string pipeline_path = config["timer_gui_pipeline_path"].as<string>();
    string record_help_path = config["record_help_path"].as<string>();
    string gui_help_path = config["gui_help_path"].as<string>();
    string gui_plan_dir = config["gui_plan_dir"].as<string>();
    /* occure the bug, try to fix the bug, next time
    // replace the gui_plan_dir and the pipeline_path' hgj with the $USER
    char* user_name = getenv("USER");
    if (user_name){
        string string_user_name = string(user_name);
        size_t start_pos_1 = pipeline_path.find("hgj");
        size_t start_pos_2 = gui_plan_dir.find("hgj");
        pipeline_path.replace(start_pos_1, start_pos_1+3, string_user_name);
        gui_plan_dir.replace(start_pos_2, start_pos_2+3, string_user_name);
    }
    */
    // file type
    string file_type = config["file_type"].as<string>();
    FILE_IS_STUDY = file_type=="study" ? true : false;
    // study init data
    string topic1 = config["topic1"].as<string>();
    string topic2 = config["topic2"].as<string>();
    int question_id = config["question_id"].as<int>();
    /* ------------------------------ */

    // the Path_man
    Path_man path_man(data_dir, file_type=="study"?"task":file_type);
    // the file
    string file_path(path_man.getCurPath()); // open the path_man's path
    File_t file(file_path, "r+");
    // the parser
    Parser parser;
    string input_command;
    // the AliasParser
    AliasParser alias_parser(alias_path);
    // the show arg
    Show_arg show_arg(file, 'a', INT_MAX);
    // the StduyT
    string answer_dir("data/answers");
    StudyT study(question_path, answer_dir, topic1, topic2, question_id);

    // the var
    bool prompt_toggle {true};
    stack<string> record_time; // use to put the time begin inside the stack
    // for the prompt
    string prompt_str;
    char* prompt_char_p = nullptr;
    while (true) {
        /* print the prompt by the file type */
        if (!FILE_IS_STUDY) { // not the study file
            if (prompt_toggle) {
                prompt_str = printPromptFileType(path_man, record_time);
            }
            else {
                prompt_str = printPromptTime(path_man, record_time);
            }
        }
        else {
            prompt_str = printPromptStudy(study, prompt_toggle);
        }
        /* import part */
        // here get the intput_command 
        prompt_char_p = readline(prompt_str.c_str());
        if (prompt_char_p) {
            input_command = string(prompt_char_p);
            if (*prompt_char_p && strlen(prompt_char_p)<25) {
                add_history(prompt_char_p);
            }
            free(prompt_char_p);
        }
        else {
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
        /* import part */

        /* this part for the study filetype */
        bool is_execute_common = false;
        if (FILE_IS_STUDY) {
            if (command == "file") {
                cls();
                printTitle();
                // same as the other filetype
                cmd_file(file, parser, path_man, show_arg);
            }
            else if (command == "topic1") {
                cls();
                printTitle();
                cmd_study_topic1(parser, study);
                // list, set
                //renewStudyPrompt(prompt_str, "topic1", '?');
                //cout << prompt_str << endl;
            }
            else if (command == "topic2") {
                cls();
                printTitle();
                cmd_study_topic2(parser, study);
                // list set 
                //renewStudyPrompt(prompt_str, "topic2", '?');
                //cout << prompt_str << endl;
            }
            else if (command == "questions") {
                //cls();
                //printTitle();
                cmd_study_questions(parser, study);
                // show(id), set(question/score,occur), open
                ;
            }
            else if (command == "save") {
                cls();
                printTitle();
                cmd_study_save(parser, study);
            }
            else if (command == "reload") {
                cls();
                printTitle();
                cmd_study_reload(parser, study);
            }
            else if (command == "edit") {
                string file_dir = study.answer_dir + "/" + study.topic1 + "/" + study.topic2;
                string file_path = file_dir + + "/" + study.link;
                cout << file_path << endl;
                // check if the path is exists 
                int _;
                if (!fs::exists(fs::path(file_dir))) {
                    _ = system(("mkdir -p '"+file_dir + "'").c_str());
                }
                if (!fs::exists(fs::path(file_path))) {
                    _ = system(("touch '"+file_path + "'").c_str());
                }
                string x_x = "vim '" + file_path + "'";
                _ = system(x_x.c_str());
            }
            else if (command == "help") {
                cls();
                printTitle();
                ifstream ifs(study_help_path);
                if (!ifs) {
                    cout << "Can not open the file " << study_help_path << endl;
                }
                string content(istreambuf_iterator<char>(ifs), {});
                cout << content << endl;
            }
            else {
                // directly using the command 
                cls();
                printTitle();
                is_execute_common = ! cmd_study_change_topic_question(parser, study);
            }
        }
        /* this part is just for record, learn, task and question */
        else {
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
            else if (command == "append") {
                cmd_append(file, path_man, parser);
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
            else if (command == "operation") {
                cmd_operation(file, parser);
            }
            else if (command == "record_in" || command == "record_out" || command == "record_abort") {
                cmd_recordOperation(file, path_man, parser, record_time);
            }
            else if (command == "save") {
                cmd_save(file);
                cout << "save!" << endl;
            }
            else if (command == "reload") {
                file.readToBuffer("r+");
                cout << "[Tip]: reload successfully!" << endl;
            }
            else if (command == "help") {
                cls();
                printTitle();
                //ifstream ifs(record_help_path);
                //if (! ifs.is_open()) {
                //    cout << "Can not open the file: " << record_help_path << endl;
                //}
                //cout << ifs.rdbuf() << endl;
                /* using the less to show */
                string print_command = "cat " + record_help_path + " | less";
                system(print_command.c_str());
            }
            else {
                is_execute_common = true;
            }
        }
        if (! is_execute_common) continue;
        /* this is the common part  */
        if (command == "list") {
            cmd_list(file, path_man, parser);
        }
        else if (command == "alias") {
            cmd_alias(parser, alias_parser);
        }
        else if (command == "timer") {
            cls();
            cmd_timer(parser);
        }
        else if (command == "timerStop") {
            cls();
            printTitle();
            cmd_timer_gui(parser, pipeline_path);
        }
        else if (command == "sh") {
            // if no argument, just open the interactive bash
            if (parser.arg_num == 0) {
                system("bash -i");
                continue;
            }
            // if is the shell command, combine rest of the command as one
            // add the bash login shell and interactive bash
            stringstream ss;
            ss << "bash -l -c ";
            ss << "\"";
            copy(parser.arg_ls.begin()+1, parser.arg_ls.end(), ostream_iterator<string>(ss, " "));
            ss << "\"";
            try {
                //cout << ss.str() << endl;
                system(ss.str().c_str());
            }
            catch (...) {
                cout << "[Warning]: your shell command is not right!" << endl;
            }
        }
        else if (command == "clear") {
            cls();
        }
        else if (command == "prompt_toggle" || command == "pt") {
            prompt_toggle = !prompt_toggle;
            cls();
            if (FILE_IS_STUDY) {
                printTitle();
            }
            else {
                printHeader(file, path_man, show_arg);
                printContent(file, show_arg);
            }
        }
        else if (command == "gui_control") {
            // control the GUI 
            cmd_control_gui(parser, pipeline_path, gui_help_path);
            ;
        }
        else if (command == "exit") {
            cmd_save(file);
            cout << "exit!" << endl;
            break;
        }
        else if (command == "openInVim") {
            cmd_openInVim(file, path_man, parser, gui_plan_dir);
        }
        else {
            string warning_info = string("unknown command: <") + SCOLOR(CYAN, parser.input_str) + ">!";
            printWarning(warning_info);
        }
    }
}
