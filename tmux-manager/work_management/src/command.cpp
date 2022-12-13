#include "command.h"
#include "regex_utils.h"
#include <unordered_set>

using json = nlohmann::json;

bool FILE_IS_STUDY = false;

/* command: add */
void cmd_add(File_t& file, Path_man& path_man) {
    if (path_man.getFileType() == "record") {
        stringstream ss;
        ss << "\"record\" can not use the add command, please use the in command!";
        printExe(ss.str());
        return;
    }
    file.writeToBuffer(); // write item to buffer
}

/* command: append */
void cmd_append(File_t& file, Path_man& path_man, Parser& parser) {
    const int arg_num = parser.arg_num;
    if (arg_num < 2) {
        stringstream ss;
        ss << "append command except 2 arguments!";
        printWarning(ss.str());
        return;
    }
    if (!isNum(parser.arg_ls[2])) {
        stringstream ss;
        ss << parser.arg_ls[2] << " is not a num!";
        printExe(ss.str());
        return;
    }
    int item_id = stoi(parser.arg_ls[2]);
    if (item_id<0 || item_id>file.getItemsNum()) {
        stringstream ss;
        ss << "iteams id out of range! ";
        printWarning(ss.str());
        return;
    }
    if (parser.arg_ls[1]!="newline" && parser.arg_ls[1]!="space") {
        stringstream ss;
        ss << "append type except the newline or the space!";
        printWarning(ss.str());
        return;
    }
    string arg_append_type = parser.arg_ls[1];
    // get the item and then append
    string& deal_item = file.refItems()[item_id-1]; // the reference type
    // prompt the input
    string content;
    string delimiter;
    //cout << "(append)" << CCOLOR(LIGHT_GREEN, "->: ");
    //getline(cin, content, '&');
    // combine the new item
    string append_prompt("(append) ->: ");
    char* get_str = readline(append_prompt.c_str());
    if (get_str) {
        content = string(get_str);
        free(get_str);
    }
    if (content.empty()) return;
    // content strip of the space and the newline
    strip(content);
    strip(deal_item);
    if (arg_append_type == "newline") {
        deal_item += "\n" + content + "\n";
    }
    else {
        deal_item += " " + content + "\n";
    }
}

/* command: date */
void cmd_date(File_t& file, const Parser& parser,  Path_man& path_man, Show_arg& show_arg) {
    const int arg_num = parser.arg_num;
    if (arg_num == 0) {
        cout << "\"date\" no argument!" << endl;
        return;
    }
    string arg_1 = parser.arg_ls[1];
    // change and show
    if (arg_1 == "show") {
        cls();
        cout << "[Tip]: date: " << path_man.getDate() << endl;
        // will not change the argument, just show the items
        printHeader(file, path_man, show_arg);
        printContent(file, show_arg);
        return;
    }
    // change the date
    string new_date_str;
    if (arg_1 == "previous" || arg_1 == "next") {
        int offset_date;
        if (arg_num == 1) { // no number
            offset_date = 1;
        }
        else {
            try {
                offset_date = stoi(parser.arg_ls.at(2)); // may fail
            }
            catch (...) { // if occur exception, let the offset_date equal to 1
                offset_date = 1;
            }
        }
        // renew the offset_date
        if (arg_1 == "previous") offset_date = -offset_date;
        // get the new_date_str
        {
            // cur_show_date's offset relative to the today
            string today;
            string cur_show_date(path_man.getDate());
            getDate(0, today);
            int cur_show_date_offset_relative_to_today = subDate(cur_show_date, today);
            offset_date += cur_show_date_offset_relative_to_today;
        }
        getDate(offset_date, new_date_str);
    }
    else if (arg_1 == "today") {
        getDate(0, new_date_str);
    }
    else if (arg_1 == "update") {
        if (arg_num == 1) {
            cls();
            stringstream ss;
            ss << "expected \"date update year-month-day\", lack date args!";
            printExe(ss.str());
            return;
        }
        new_date_str = parser.arg_ls[2];
    }
    else if (arg_1 == "go_month_day") {
        // get year and then combine to the new_date_str
        {
            const int size = 10;
            char c_str_year[size];
            time_t ts = time(nullptr);
            strftime(c_str_year, size, "%Y-", localtime(&ts));
            // update the new_date_str
            new_date_str = string(c_str_year) + parser.arg_ls[2];
        }
    }
    else if (arg_1 == "go_day") {
        // get year and month and then combine to the new_date_str
        {
            const int size = 10;
            char c_str_year_month[size];
            time_t ts = time(nullptr);
            strftime(c_str_year_month, size, "%Y-%m-", localtime(&ts));
            // update the new_date_str
            new_date_str = string(c_str_year_month) + parser.arg_ls[2];
        }
    }
    else {
        stringstream ss;
        ss << "date command is wrong!" << endl;
        printExe(ss.str());
    }
    // update, update successfully: true
    if (path_man.updateDate(new_date_str)) {
        //cout << "Ok, date update to the: " << path_man.getDate() << endl;
        file.update(path_man.getCurPath()); // update the file
        show_arg.update(file);
        // at the end, print the header and the content
        cls();
        printHeader(file, path_man, show_arg);
        printContent(file, show_arg);
    }
}

/* command: file */
void cmd_file(File_t& file, const Parser& parser, Path_man& path_man, Show_arg& show_arg) {
    const int arg_num = parser.arg_num;
    if (parser.arg_num == 0) {
        cout << "\"file\" no argument!" << endl;
        return;
    }
    string original_file_type = path_man.getFileType();
    // change file type
    string arg_1 = parser.arg_ls[1];
    // here add the stuy file type, very special
    if (arg_1 == "study") { // just set the mark
        cls();
        printTitle();
        FILE_IS_STUDY = true;
    }
    else { // record, question, ....
        if (!path_man.updateFileType(arg_1)) { // not changed, return
            return;
        }
        file.update(path_man.getCurPath());
        show_arg.update(file);
        // show the new file type's content
        // or solve the debug that study switch to other file type.
        if (original_file_type != path_man.getFileType() || FILE_IS_STUDY) { // if the file type change
            cls();
            printHeader(file, path_man, show_arg);
            printContent(file, show_arg);
            FILE_IS_STUDY = false; // renew the mark
        }
    }
}


/* command: show */
void cmd_show(File_t& file, Path_man& path_man, Parser& parser, Show_arg& show_arg) {
    // the var
    bool is_just_show_title = false;
    /* get the arg to the show_arg */
    const int arg_num = parser.arg_num;
    if (arg_num == 0) {
        printHeader(file, path_man, show_arg);
        printContent(file, show_arg);
        return;
    }
    //
    string arg_1 = parser.arg_ls[1];
    int set_num;
    if (arg_1 == "number") {
        if (arg_num == 1) { // without the number
            set_num = INT_MAX;
        }
        else {
            string arg_2 = parser.arg_ls[2];
            try {
                set_num = stoi(arg_2); // try to assign the value
            }
            catch(...) {
                set_num = INT_MAX; // once occur the exception, set to the INT_MAX
            }
        }
        show_arg.setStateShowNum(set_num, file);
    }
    else if (arg_1 == "state") {
        char state;
        if (arg_num == 1) { // without the specified state
            state = 'a'; // show all
        }
        else {
            try {
                state = parser.arg_ls.at(2)[0];
            }
            catch (...) {
                state = 'a'; // once occur exception, set state to 'a'
            }
        }
        show_arg.setState(state); 
    }
    else if (arg_1 == "title") {
        is_just_show_title = true;
    }
    else if (arg_1 == "reverse") {
        // toggle the reverse
        show_arg.is_reverse = !show_arg.is_reverse;
    }
    else if (arg_1 == "id") {
        // just show the id specified
        if (arg_num == 1) {
            cout << "[Warning]: not specify the items id! " << endl;
            return;
        }
        if (! isNum(parser.arg_ls[2])) {
            cout << "[Warning]: " << parser.arg_ls[2] << " is not the number!" << endl;
            return;
        }
        int specified_item_id = stoi(parser.arg_ls[2]);
        if (specified_item_id<0 || specified_item_id>file.getItemsNum()) {
            cout << "[Warning]: id out of range!" << endl;
            return;
        }
        // print the header and the content
        show_arg.setState('a');
        show_arg.setStateShowNum(1, file);
        printHeader(file, path_man, show_arg);
        string item_to_be_print = file.getItems()[specified_item_id-1];
        cout << item_to_be_print << endl;
        cout << hyphen(55) << endl;
        return; // here end the function
    }
    else {
        cout << "[warning]: no such show command!" << endl;
    }
    /* here to print, print header and the file content */
    printHeader(file, path_man, show_arg);
    printContent(file, show_arg, is_just_show_title);
}

/* command: save */
void cmd_save(File_t& file) {
    file.flushBuffer();
}

/* command: openInVim */
void cmd_openInVim(File_t& file, Path_man& path_man, Parser& parser, string& gui_plan_dir) {
    const int arg_num = parser.arg_num;
    string open_vim_path;
    //
    if (arg_num>0 && parser.arg_ls[1] == "gui") {
        stringstream ss;
        // /2022/03/03-12/
        string year(path_man.date_vec[0]);
        string month(path_man.date_vec[1]);
        string day(path_man.date_vec[2]);
        ss << year << "/" << year << "-" << month << "/" << (month + "-" + day);
        //
        open_vim_path = gui_plan_dir + ss.str() + "/plan.txt";
#ifdef __linux__
        cout << "Remote machine file!" << endl;
        if (!fs::exists(fs::path(open_vim_path))) {
            string scp_command = "scp gjsq@202.38.214.68:" + open_vim_path + " ./tmp >/dev/null";
            int _ = system(scp_command.c_str());
            //
            if (!_) open_vim_path = "./tmp/plan.txt";
        }
#endif

    }
    else if (FILE_IS_STUDY) return; // stuy not execute
    else {
        open_vim_path = path_man.getCurPath();
    }
    // 1. save the buffer to the file; 2. use the vim to open it
    file.flushBuffer();
    try {
        string shell_command("vim " + open_vim_path);
        system(shell_command.c_str());
    }
    catch (...) {
        cout << "There are something wrong! Can not open in vim!" << endl;
    }
    return;
}

/*command: alias */
void cmd_alias(Parser& parser, AliasParser& alias_parser) {
    const int arg_num = parser.arg_num;
    if (arg_num == 0) {
        return;
    }
    string arg_1 = parser.arg_ls[1];
    if (arg_1 == "show") {
        cls();
        alias_parser.showAlias();
    }
    else if (arg_1 == "add") {
        string input_alias;
        getline(cin, input_alias);
        alias_parser.addAliasCommand(input_alias);
    }
    else if (arg_1 == "reload") {
        alias_parser.reloadAlias("");
    }
    else if (arg_1 == "save") {
        alias_parser.saveAlias("");
    }
    else if (arg_1 == "vim") {
        alias_parser.vimAlias();
        // after saving, and then reload
        alias_parser.reloadAlias("");
    }
    else if (arg_1 == "less") {
        alias_parser.lessAlias();
    }
    else {
        cout << "[Warning]: no such alias command!" << endl;
    }
}

void cmd_list(File_t& file, Path_man& path_man, Parser& parser) {
    const int arg_num = parser.arg_num;
    if (arg_num == 0) {
        return;
    }
    string arg_1 = parser.arg_ls[1];
    if (arg_1 == "file") {
        cls();
        // print the date
        cout << hyphen(55) << endl;
        cout <<  string(16, ' ') << "Date: ";
        cout << path_man.getDate() << endl;
        // print the file
        printFileList(path_man.getCurDir(), Path_man::defined_file_type);
    }
    else if (arg_1 == "date" || arg_1 == "calendar") {
        int year;
        int month;
        if (arg_num == 1) { // without the year and month
            year = getCurYear();
            month = getCurMonth();
        }
        else if (arg_num == 2) { // just month, without year
            year = getCurYear();
            if (isNum(parser.arg_ls[2])) {
                month = stoi(parser.arg_ls[2]);
                month = (month>0 && month<13) ? month : getCurMonth(); // the valid month
            }
            else {
                month = getCurMonth();
            }
        }
        else { // with the year and the month
            if (isNum(parser.arg_ls[2]) && isNum(parser.arg_ls[3])) {
                year = stoi(parser.arg_ls[2]);
                month = stoi(parser.arg_ls[3]);
                // check valid
                year = (year>=1990&&year<=9999) ? year : getCurYear();
                month = (month>0&&month<13) ? month : getCurMonth();
            }
            else {
                year = getCurYear();
                month = getCurMonth();
            }
        }
        // pass the year, month to print
        cls();
        bool is_calendar = false;
        if (arg_1=="calendar") {
            is_calendar = true;
        }
        printMonthDay(path_man.parent_dir, year, month, path_man.getFileType(), is_calendar);
    }
    else {
        cout << "[Warning]: no such list command!" << endl;
    }
}

void cmd_label(File_t& file, Parser& parser, AliasParser& alias_parser, const Path_man& path_man, const Show_arg& show_arg) {
    const int arg_num = parser.arg_num;
    if (arg_num == 0) {
        return;
    }
    string arg_1 = parser.arg_ls[1];
    char state = arg_1.at(0);
    if (state==NOTDO || state==NOTFINISH || state==FINISH) {
        // in the cin mode
        string line;
        vector<string> vec_id_str;
        vector<int> vec_id_int;
        cout << "(label index)To " << state << CCOLOR(LIGHT_GREEN, "->: ");
        getline(cin, line);
        Parser::split(line, vec_id_str, " ");
        for (auto it=vec_id_str.begin(); it!=vec_id_str.end(); ++it) {
            if (isNum(*it)) {
                vec_id_int.push_back(stoi(*it)); // try to convert
            }
        }
        // replace the state
        for (auto it=vec_id_int.begin(); it!=vec_id_int.end(); ++it) {
            if (*it<=0 || *it>file.getItemsNum()) {
                continue;
            }
            string& be_changed_item = file.refItems()[*it-1]; // reference to the string
            auto str_it= be_changed_item.begin();
            while (*str_it!='(') ++str_it;
            char original_state = *(++str_it); // get the original state
            *str_it = state; // change
            // update the state, and state count
            file.refStates().at(*it-1) = state;
            file.refStateNum().at(state)++;  // new state num add 1
            file.refStateNum().at(original_state)--; // odd state num sub 1
        }
        // at last, show the result
        cls();
        printHeader(file, path_man, show_arg);
        printContent(file, show_arg);
    }
    else {
        cout << "[Warning]: no such list command!" << endl;
    }
}

void cmd_recordOperation(File_t& file, Path_man& path_man, Parser& parser, stack<string>& record_time) {
    if (path_man.getFileType() != "record") {
        cout << "[Warning]: Not in the record type!" << endl;
        return;
    }
    // in the record type
    string arg_0 = parser.arg_ls[0];
    if (arg_0 == "record_in") {
        // check if in the record_in state, can not modify
        if (!record_time.empty()) {
            cout << "[Warning]: Not finish the last item yet! You have to use the <out> first!" << endl;
            return;
        }
        // push the time to the stk, and then write the item to the buffer
        string cur_time;
        {
            const int size = 10;
            char c_cur_time[size];
            time_t ts = time(nullptr);
            strftime(c_cur_time, size, "%H:%M", localtime(&ts));
            cur_time = string(c_cur_time);
        }
        // write to the buffer
        if (file.writeToBuffer(true, cur_time)) {
            record_time.push(cur_time);
        }
    }
    else if (arg_0 == "record_out") {
        // the var
        const char mark {'^'};
        string& last_item = file.refItems().back(); // refer to the last item
        auto str_it = last_item.begin();
        while (str_it!=last_item.end() && *str_it!=mark) ++str_it;
        if (str_it!=last_item.end() && (str_it+1)!=last_item.end()) {
            *str_it = '-';
        }
        else {
            cout << "[Warning]: record_out command fail!" << endl;
            return; // fail to do so!
        }
        // get the time
        string cur_time;
        {
            const int size = 10;
            char c_cur_time[size];
            time_t ts = time(nullptr);
            strftime(c_cur_time, size, "%H:%M", localtime(&ts));
            cur_time = string(c_cur_time);
        }
        // renew the item
        last_item.insert(str_it+1, cur_time.begin(), cur_time.end());
        // clear the time record stk, then write renew the buffer's item
        record_time.pop();
    }
    else if (arg_0 == "record_abort") {
        // empty the stack and erase the vector's last item
        file.refItems().pop_back(); // pop the item
        file.refStates().pop_back(); //  pop the state
        --file.refItemsNum(); // the items num must sub 1
        --file.refStateNum()[NOTDO]; // sub the NOTDO
        --file.refStateNum()['a']; // all sib 1
        record_time.pop(); // pop the time
    }
    else {
        cout << "[Warning]: no such record command!" << endl;
    }
}

void cmd_operation(File_t& file, Parser& parser) {
    const int arg_num = parser.arg_num;
    if (arg_num == 0) {
        return;
    }
    string arg_1 = parser.arg_ls[1];
    if (arg_1 == "delete") {
        // get the line
    AGAIN:
        string line;
        cout << "(del index)->: ";
        getline(cin, line);
        if (cin.eof()) {
            cin.clear();
            cout << endl << "[rewrite]->: ";
            goto AGAIN;
        }
        if (line.empty()) {
            return; // end
        }
        // the var
        vector<int> del_index;
        getLineNumber(line, del_index); // use the regular expression to extract the number
        vector<string>& items_vec = file.refItems();
        vector<char>& states_vec = file.refStates();
        unordered_map<char, int> states_num = file.refStateNum();
        int items_num = file.getItemsNum();
        //
        remove_if(del_index.begin(), del_index.end(), [&items_num](int i) { // filter
            return i<0 || i>items_num;
        });
        unordered_set<int> del_index_set(del_index.begin(), del_index.end());
        //
        vector<string> items_temp;
        vector<char> states_temp;
        for (int i=0; i<items_vec.size(); ++i) {
            if (del_index_set.find(i)==del_index_set.end()) {
                // not found
                items_temp.push_back(items_vec[i]);
                states_temp.push_back(states_vec[i]);
            }
            else { // found, the state number sub 1
                --states_num[states_vec[i]];
                --states_num['a']; // all sub 1
            }
        }
        // renew the items_vec, states_vec
        items_vec.clear();
        states_vec.clear();
        items_vec = move(items_temp);
        states_vec = move(states_temp);
        // renew the number and the items state and number
        dealItemsPrefix(file); // reorder the item of file
        // then renew the states, and the states_num
    }
    else if (arg_1 == "recover") {
        dealItemsPrefix(file);
    }
    else {
        cout << "[Warning]: no such operation command!" << endl;
    }
}

void cmd_timer(Parser& parser) {
    const int arg_num = parser.arg_num;
    bool is_hyphen_num_short = false;
    // the sleep_min;
    int sleep_min;
    if (arg_num == 0) {
        sleep_min = 30;
    }
    else {
        string arg_1 = parser.arg_ls[1];
        if (isNum(arg_1)) {
            sleep_min = stoi(arg_1);
        }
        else {
            sleep_min = 30;
        }
        // if has the arg_2, set the is_hyphen_num_short true
        if (arg_num == 2) {
            is_hyphen_num_short = true;
        }
    }
    // call the function
    progressBar(sleep_min, is_hyphen_num_short);
}
void cmd_timer_gui(Parser& parser, const string& pipeline_path) {
    const int arg_num = parser.arg_num;
    int sleep_min;
    if (arg_num == 0) {
        sleep_min = 30;
    }
    else {
        string arg_1 = parser.arg_ls[1];
        sleep_min = isNum(arg_1) ? stoi(arg_1) : 30;
    }
    // open the piple line path and the write the time to the file
    ofstream ofs(pipeline_path);
    if (!ofs.is_open()) {
        string warning_info("file " + pipeline_path + " no exists! Or try again!");
        printWarning(warning_info);
    }
    else {
        // write the sleep time to the file
        //cout << pipeline_path << endl;
        ofs << sleep_min << std::flush; 
        ofs.close();
        cout << "Timer: " << sleep_min << "min" << endl;
    }
}

/* command just for study */
void cmd_just_for_study(string& questions_js_path, string& answers_path) {
    auto f_is = ifstream(questions_js_path);
    if (!f_is) {
        cout << "[Warning]: Can not open the file: " << questions_js_path << endl;
        return;
    }
    FILE_IS_STUDY = true;
    // load the json file, just load once time
    static json data = json::parse(f_is);
    f_is.close();
    cout << "Has loaded the json file!" << endl;
}

void cmd_study_topic1(Parser& parser, StudyT& study) { 
    /* get the arg to the show_arg */
    const int arg_num = parser.arg_num;
    if (arg_num == 0) {
        // just show the id's question
        return;
    }
    //
    string arg_1 = parser.arg_ls[1];
    if (arg_1 == "list") {
        cout << endl;
        cout << hyphen(31) << CCOLOR(LIGHT_RED, " Topic1 ") << hyphen(31);
        printVecString(study.topic1_vec, true);
    }
    else if (arg_1 == "add") {
        if (arg_num == 1) {
            printWarning("Need add the arg: <topic1> following!");
            return;
        }
        string arg_2 = parser.arg_ls[2];
        if (find(study.topic1_vec.begin(), study.topic1_vec.end(), arg_2)!=study.topic1_vec.end()) {
            printWarning("Cannot add, has been includding the topic1!" + arg_2);
            return;
        }
        study.topic1_vec.push_back(arg_2);
        json tmp;
        tmp[arg_2] = {};
        study.js_data.update(tmp);
    }
    else {
        printWarning("No such command: <" +  parser.input_str + ">");
    }
}

void cmd_study_topic2(Parser& parser, StudyT& study) { 
    /* get the arg to the show_arg */
    const int arg_num = parser.arg_num;
    if (arg_num == 0) {
        // just show the id's question
        return;
    }
    //
    string arg_1 = parser.arg_ls[1];
    if (arg_1 == "list") {
        cout << endl;
        cout << hyphen(31) << CCOLOR(LIGHT_RED, " Topic2 ") << hyphen(31);
        printVecString(study.topic2_vec);
    }
    else if (arg_1 == "add") {
        if (arg_num==1) {
            printWarning("Need add the arg: <topic2> following!");
            return;
        }
        string arg_2 = parser.arg_ls[2];
        if (find(study.topic2_vec.begin(), study.topic2_vec.end(), arg_2)!=study.topic2_vec.end()) {
            printWarning("Cannot add, has been including the topic2_vec " + arg_2);
            return;
        }
        study.topic2_vec.push_back(arg_2);
        json tmp;
        tmp[arg_2] = {};
        study.js_data[study.topic1].update(tmp);
    }
    else if (arg_1 == "delete") {
        if (arg_num==1) return;
        string arg_2 = parser.arg_ls[2];
        auto f_it=find(study.topic2_vec.begin(), study.topic2_vec.end(), arg_2);
        if (f_it == study.topic2_vec.end()) { //not found
            printWarning("No Such topic2 " + arg_2);
            return;
        }
        cout << "[ ! ]: Do you want to delete the topic2: " << arg_2 << "? [y/n]: ";
        char ch;
        cin >> ch;
        if (ch == 'y' || ch == 'Y') {
            study.topic2_vec.erase(f_it);
            study.js_data[study.topic1].erase(arg_2);
            cout << "Has been delete the topic2: " << arg_2 << endl;
            study.initInfo(study.topic1, "", 0);
        }
    }
    else if (arg_1 == "rename") {
        if (arg_num==1) return;
        string arg_2 = parser.arg_ls[2];
        auto f_it=find(study.topic2_vec.begin(), study.topic2_vec.end(), arg_2);
        if (f_it != study.topic2_vec.end()) { //not found
            printWarning("Has including topic2 " + arg_2);
            return;
        }
        study.topic2_vec.erase(find(study.topic2_vec.begin(), study.topic2_vec.end(), study.topic2));
        study.topic2_vec.emplace_back(arg_2);
        study.js_data[study.topic1].erase(arg_2);
        json old_topic2 = study.js_data[study.topic1][study.topic2];
        study.js_data[study.topic1].update(old_topic2);
    }
    else {
        printWarning("No such command: <" +  parser.input_str + ">");
    }
}

void cmd_study_questions(Parser& parser, StudyT& study) {
    /* get the arg to the show_arg */
    const int arg_num = parser.arg_num;
    if (arg_num == 0) {
        // just show the id's question
        return;
    }
    //
    string arg_1 = parser.arg_ls[1];
    if (arg_1 == "show") {
        if (arg_num == 1) {
            // show all
            //printVecString(study.questions_vec);
            cout << study.question << endl;
            return;
        }
        // with the number id, range(num1:num2)
        string arg_2 = parser.arg_ls[2];
        if (isNum(arg_2)) {
            int show_id = stoi(arg_2);
            string question_show;
            if (show_id>=0 && show_id<study.questions_vec.size()) {
                question_show = study.questions_vec[show_id];
            }
            else if (show_id < 0) {
                question_show = study.questions_vec.front();
            }
            else {
                question_show = study.questions_vec.back();
            }
            // then show 
            cout << question_show << endl;
        }
        else if (arg_2 == "all") {
            const int len_threshold = 15;
            int print_len = study.questions_vec.size(); 
            if (print_len < len_threshold) {
                printVecString(study.questions_vec);
            }
            else {
                // use the less to show 
                lessVecString(study.questions_vec, true);
            }
        }
        else {
            printExe("show the questions range!");
        }
    }
    else if (arg_1 == "edit") {
        string command_edit = "vim " + study.question_path;
        int _ = system(command_edit.c_str());
        if (_) {
            printWarning("Fail to execute the command: " + command_edit);
        }
    }
    else if (arg_1 == "add") {
        string question_content;
        // get the question
        string prompt_add = "(add question)-> : ";
        char* rl = readline(prompt_add.c_str());
        if (!rl) return;
        question_content = string(rl);
        strip(question_content);
        free(rl);
        //cout << question_content;
        // append the question to the js_data
        if (!study.topic1.empty() && !study.topic2.empty()) {
            study.questions_vec.push_back(question_content);
            string link_path(question_content + ".md");
            json tmp;
            tmp["link"] = link_path;
            tmp["score"] = 0;
            tmp["occur"] = 0;
            study.js_data[study.topic1][study.topic2][question_content] = tmp;
            // not save!
        }
        else {
            cout << "topic1 or topic2 is empty! Can not add the new question!" << endl;
        }
    }
    else if (arg_1 == "delete") {
        cout << "Has not implementation!" << endl;
        ;
    }
    else if (arg_1 == "score") {
        stringstream ss;
        ss << "the score is the : " << study.score;
        printExe(ss.str());
    }
    else if (arg_1 == "occur") {
        stringstream ss;
        ss << "the occur is the : " << study.occur;
        printExe(ss.str());
    }
    else if (arg_1 == "link") {
        stringstream ss;
        ss << "the link is the: " << study.link;
        printExe(ss.str());
    }
    else {
    }
}
void cmd_study_save(Parser& parser, StudyT& study) { 
    /* get the arg to the show_arg */
    const int arg_num = parser.arg_num;
    int dump_level = 4;
    if (arg_num >= 1) {
        dump_level = stoi(parser.arg_ls[1]);
    }
    // backen the file before saving! Because that is very important!
    study.dumpJson(dump_level, true);
    printExe("Save indent: " + to_string(dump_level) + ", successfully!");
} 
void cmd_study_reload(Parser& parser, StudyT& study) { 
    /* get the arg to the show_arg */
    study.loadJson(study.question_path, study.js_data);
    // get the question id
    study.initInfo(study.topic1, study.topic2, 0);
    printExe("Reload successfully!");
}

bool cmd_study_change_topic_question(Parser& parser, StudyT& study) {
    bool is_change_success = false;
    string command = parser.arg_ls[0];
    // check if the number
    if (isNum(command)) {
        int question_id = stoi(command);
        study.initInfo(study.topic1, study.topic2, question_id);
        is_change_success = true;
    }
    else if (find(study.topic1_vec.begin(), study.topic1_vec.end(), command)!=study.topic1_vec.end()) { // found
        study.initInfo(command, "", 0);
        is_change_success = true;
    }
    else {
        for (auto& try_topic1 : study.topic1_vec) {
            if (study.js_data[try_topic1].contains(command)) {
                study.initInfo(try_topic1, command, 0);
                is_change_success = true;
                break;
            }
        }
    }
    //if (!is_change_success) {
    //    string warning_info = string("unknown command: <") + SCOLOR(CYAN, parser.input_str) + ">!";
    //    printWarning(warning_info);
    //}
    return is_change_success;
}

void cmd_study_bookmarks(Parser& parser, StudyT& study) { 
    /* get the arg to the show_arg */
    const int arg_num = parser.arg_num;
    if (arg_num == 0) {
        // just show the id's question
        return;
    }
    //
    string arg_1 = parser.arg_ls[1];
}

void cmd_control_gui(Parser& parser, const string& pipeline_path, string& gui_help_path) {
    const int arg_num = parser.arg_num;
    if (arg_num == 0) {
        stringstream ss;
        ss << "Need at least one argument!";
        printWarning(ss.str());
        return;
    }
    string arg_1 = parser.arg_ls[1];
    if (arg_1 == "help") {
        // print the help information
        ifstream ifs(gui_help_path);
        if (! ifs.is_open()) {
            cout << "Could not open the help file: " << gui_help_path << endl;
            return;
        }
        cout << ifs.rdbuf() << endl;
    }
    // open the piple line path and the write the time to the file
    ofstream ofs(pipeline_path);
    if (!ofs.is_open()) {
#ifdef __linux__
        cout << "remote writing!" << endl;
        // ssh $temp_path $gjsq:$pipeline_path
        string tmp_path("./data/tmp.txt");
        ofstream ofs_tmp(tmp_path);
        ofs_tmp << arg_1 << std::flush;
        ofs_tmp.close();
        //
        string scp_command = "scp " + tmp_path + " gjsq@202.38.214.68:" + pipeline_path + \
                              " >/dev/null";
        //cout << scp_command << endl;
        if (!system(scp_command.c_str())) return;
#endif
        string warning_info("file " + pipeline_path + " no exists! Or try again!");
        printWarning(warning_info);
    }
    else {
        ofs << arg_1 << std::flush; 
        ofs.close();
        cout << "add the control field: " << arg_1 << endl;
    }
}
