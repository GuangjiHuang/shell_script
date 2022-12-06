#include "command.h"
#include "regex_utils.h"
#include <unordered_set>

/* command: add */
void cmd_add(File_t& file, Path_man& path_man) {
    if (path_man.getFileType() == "record") {
        cout << "[Warning]: \"record\" can not use the add command, please use the in command!" << endl;
        return;
    }
    file.writeToBuffer(); // write item to buffer
}

/* command: append */
void cmd_append(File_t& file, Path_man& path_man, Parser& parser) {
    const int arg_num = parser.arg_num;
    if (arg_num < 2) {
        cout << "[Warning]: append command except 2 arguments!" << endl;
    }
    if (!isNum(parser.arg_ls[2])) {
            cout << "[Warning]: " << parser.arg_ls[2] << " is not a num!" <<endl;
            return;
    }
    int item_id = stoi(parser.arg_ls[2]);
    if (item_id<0 || item_id>file.getItemsNum()) {
        cout << "[Warning]: itms id out of range! " << endl;
        return;
    }
    if (parser.arg_ls[1]!="newline" && parser.arg_ls[1]!="space") {
        cout << "[Warning]: append type except the newline or the space!" << endl;
        return;
    }
    string arg_append_type = parser.arg_ls[1];
    // get the item and then append
    string& deal_item = file.refItems()[item_id-1]; // the reference type
    // prompt the input
    string content;
    string delimiter;
    cout << "(append)" << CCOLOR(LIGHT_GREEN, "->: ");
    getline(cin, content, '&');
    // combine the new item
    if (arg_append_type == "newline") {
        deal_item += content + "\n";
    }
    else {
        deal_item = deal_item.substr(0, deal_item.size()-1) + " " + content + "\n";
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
            cout << "[Warning]: expected \"date update year-month-day\", lack date args!" << endl;
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
        cout << "[Warning]: date command is wrong!" << endl;
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
    path_man.updateFileType(arg_1);
    file.update(path_man.getCurPath());
    show_arg.update(file);
    // show the new file type's content
    if (original_file_type != path_man.getFileType()) { // if the file type change
        cls();
        printHeader(file, path_man, show_arg);
        printContent(file, show_arg);
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
void cmd_openInVim(File_t& file, Path_man& path_man) {
    // 1. save the buffer to the file; 2. use the vim to open it
    file.flushBuffer();
    try {
        string shell_command("vim " + path_man.getCurPath());
        system(shell_command.c_str());
    }
    catch (...) {
        cout << "There are something wrong! Can not open in vim!" << endl;
    }
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
