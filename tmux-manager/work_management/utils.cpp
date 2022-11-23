#include "utils.h"
#include "command.h"

enum {MINUTE=60, HOUR=3600, DAY=24*3600};

using namespace std;
namespace fs = std::filesystem;

void getFiles(const string& dir_path, vector<string>&files_path) {
    using namespace fs;
    const path _dir_path(dir_path);
    for (const auto& entry : directory_iterator(_dir_path)) {
        if (entry.is_regular_file()) {
            files_path.push_back(entry.path());
        }
    }
}

void getDirs(const string& dir_path, vector<string>& dirs_path) {
    using namespace fs;
    const path _dir_path(dir_path);
    for (const auto& entry : directory_iterator(_dir_path)) {
        if (entry.is_directory()) {
            dirs_path.push_back(entry.path());
        }
    }
}

bool isExists(const string& path) {
    using namespace fs;
    fs::path _path(path);
    return exists(_path);
};

void getDate(const int& offset, string& date) {
    // var
    time_t t_s_now;
    time_t t_s_offset;
    // count offset
    time(&t_s_now);
    t_s_offset = t_s_now + offset * DAY;
    assert(t_s_offset >= 0); // the t_s_offset must be le 0
    // get the c_str and to string
    const int DATE_SIZE{20};
    char c_date[DATE_SIZE];
    const char *format = "%Y-%m-%d";
    strftime(c_date, DATE_SIZE, format, localtime(&t_s_offset));
    date.assign(c_date);
}

int getCurYear() {
    int year;
    char str_year[5];
    time_t cur_time = time(nullptr);
    strftime(str_year, 5, "%Y", localtime(&cur_time));
    year = stoi(string(str_year));
    return year;
}

int getCurMonth() {
    int month;
    char str_month[3];
    time_t cur_time = time(nullptr);
    strftime(str_month, 3, "%m", localtime(&cur_time));
    month = stoi(string(str_month));
    return month;
}

int getCurMday() {
    int m_day;
    char str_m_day[3];
    time_t cur_time = time(nullptr);
    strftime(str_m_day, 3, "%d", localtime(&cur_time));
    m_day = stoi(str_m_day);
    return m_day;
}

int subDate(const string& date1, const string& date2) {
    /* date1 - date2, return the sub days */
    vector<string>v1;
    vector<string>v2;
    Parser::split(date1, v1, "-");
    Parser::split(date2, v2, "-");
    int days1 = dateToDays(stoi(v1.at(0)), stoi(v1.at(1)), stoi(v1.at(2)));
    int days2 = dateToDays(stoi(v2.at(0)), stoi(v2.at(1)), stoi(v2.at(2)));
    return days1 - days2;
}

int dateToDays(int year, int month, int day) {
    int days = 0;
    const int s_year = 1990;
    const int s_mon = 1;
    const int s_day = 1;
    // count year day
    for (int i = s_year; i < year; i++) {
        days += (is_leap(i)) ? 366 : 365;
    }
    // count month day
    for (int i = s_mon; i < month; i++) {
        switch (i) {
            case 1:
            case 3:
            case 5:
            case 7:
            case 8:
            case 10:
            case 12:
                days += 31;
                break;
            case 2:
                days += is_leap(year) ? 29 : 28;
                break;
            default:
                days += 30;
        }
    }
    // count day
    days += day - 1;
    return days;
}

string getWeekday(int year, int month, int day) {
    const int refer_week_day = 1; // 1990-1-1, Monday
    const vector<string> weekday_vec {"Sun", "Mon", "Tue", "Wed", "Thur", "Fri", "Sat"};
    int days = dateToDays(year, month, day);
    int weekday = days % 7 + refer_week_day;
    return weekday_vec[weekday%7];
}

bool is_leap(int year) {
    return (year%100==0 && year%400==0) || (year%4==0 && year%100!=0);
}

/* print the message */
string hyphen(int hyphen_num) {
    return string(hyphen_num, '-');
}

string asterisk(int asterisk_num) {
    return string(asterisk_num, '*');
}

string spaceStr(int space_num) {
    return string(space_num, ' ');
}

void printError(const string& error_info) {
    string error_info_prefix("[ W ] Error: ");
    cout << error_info_prefix << error_info << endl;
}

void printHeader(const File_t& file, const Path_man& path_man, const Show_arg& show_arg) {
    /* ----------deal with the line 1 ---------- */
    // get the state num
    int num_not_do;
    int num_not_finish;
    int num_finish;
    int num_total;
    {
        // assign the num of the state
        num_not_do = file.getStateNum(NOTDO);
        num_not_finish = file.getStateNum(NOTFINISH);
        num_finish =  file.getStateNum(FINISH);
        num_total = num_not_do + num_not_finish + num_finish;
    }
    // format the line 1
    string line_1;
    {
        const int space_num = 4;
        const int hyphen_num = 55;
        stringstream ss;
        ss << "[ " << CCOLOR(YELLOW, "Count Info:") << spaceStr(space_num);
        ss << NOTDO << ": " << num_not_do << spaceStr(space_num);
        ss << NOTFINISH << ": " << num_not_finish << spaceStr(space_num);
        ss << FINISH << ": " << num_finish << spaceStr(space_num);
        ss << "total" << ": " << num_total << spaceStr(space_num);
        ss << "\n";
        ss << endl;
        // assign the value to the line_1
        line_1 = ss.str();
    }
    /* ----------deal with the line 2 ---------- */
    // get the day_mark, weekday
    string day_mark;
    string weekday;
    {
        string today;
        getDate(0, today);
        if (today == path_man.getDate()) { // check if today
            day_mark = "*";
            // get the weekday
            {
                time_t tm = time(nullptr);
                const int buffer_size{10};
                char c_weekday[buffer_size];
                strftime(c_weekday, buffer_size, "%a", localtime(&tm));
                weekday = string(c_weekday); // copy the c_string to the weekday
            }
        } else {
            int sub_day = subDate(path_man.getDate(), today);
            day_mark = to_string(sub_day); // int -> string
            // get the weekday
            {
                vector<string> date_str;
                Parser::split(path_man.getDate(), date_str, "-");
                int year = stoi(date_str.at(0));
                int month = stoi(date_str.at(1));
                int day = stoi(date_str.at(2));
                weekday = getWeekday(year, month, day);
            }
        }
    }
    // get the state_verbose, show num and total_num
    string state_verbose;
    char state;
    int state_show_num;
    int state_total_num;
    {
        state = show_arg.getState();
        state_show_num = show_arg.getStateShowNum();
        state_total_num = show_arg.getStateShowTotalNum();
        getStateVerbose(state, state_verbose); // assign the value to the state_verbose
    }
    // format the line 2
    string line_2;
    {
        const int space_num = 2;
        // use the ss
        stringstream ss;
        ss << "[ " << CCOLOR(YELLOW, "Showing:") << spaceStr(space_num);
        ss << "(" << state << ":" << state_verbose << ")" << spaceStr(space_num);
        ss <<  "(" << state_show_num << "/" << state_total_num << ") " << spaceStr(space_num);
        ss << path_man.getDate() << spaceStr(1) << weekday << " ("  << day_mark << ")" << endl;
        // renew the line_how
        line_2 = ss.str();
    }
    /* ---------- print the line 1 and line 2 ---------- */
    const int hyphen_num = 55;
    cout << hyphen(hyphen_num) << endl;
    cout << line_1 << line_2;   // has included new line inside the string
    cout << hyphen(hyphen_num) << endl;
    // print the file type
    int space_num;
    string middle_title = "[ - " + path_man.getFileType() + " - ]";
    space_num = (hyphen_num - middle_title.size() - 2) / 2;
    cout << spaceStr(space_num) << " "\
         <<  SCOLOR(LIGHT_CYAN, middle_title) \
         << " " << spaceStr(space_num) << endl << endl;
}

void printTitle() {
    int const hyphen_num = 22;
    stringstream ss;
    ss << "[ ";
    ss << hyphen(hyphen_num);
    ss << CCOLOR(LIGHT_CYAN, " Work Management System ");
    ss << hyphen(hyphen_num);
    ss << " ]";
    ss << endl;
    cout << ss.str();
}

void printContent(const File_t& file, const Show_arg& show_arg, bool is_just_show_title) {
    vector<string> str_to_be_print;
    // the argument about the show
    char show_state = show_arg.getState();
    bool is_reverse = show_arg.is_reverse;
    int state_show_num = show_arg.getStateShowNum();
    // the state and the items, reference
    const vector<string>& items = file.getItems();
    const vector<char>& states = file.getStates();
    // print
    auto items_it = items.begin();
    auto states_it = states.begin();
    if (show_state == 'a') { // show_state is the 'a'
        str_to_be_print = items;
    }
    else { // show state is the NOTDO, NOTFINISH, FINISh
        for (; items_it!=items.end(); ++items_it, ++states_it) {
            if (*states_it == show_state) {
                str_to_be_print.push_back(*items_it);
            }
        }
    }
    if (is_reverse) reverse(str_to_be_print.begin(), str_to_be_print.end());
    // if just show title is true, extract the title
    if (is_just_show_title) {
        for (auto& item : str_to_be_print) {
            auto item_it = item.begin();
            while (item_it!=item.end() && *item_it!='\n') ++item_it;
            if (item_it!=item.end()) ++item_it;
            // renew the item
            item.assign(item.begin(), item_it);
        }
    }
    // print the content
    const int hyphen_num = 55;
    items_it = str_to_be_print.begin();
    while (--state_show_num>=0 && items_it!=str_to_be_print.end()) {
        cout << *items_it++ << endl;
    }
    cout << hyphen(hyphen_num) << endl;
}

void printPromptFileType(Path_man& path_man, const stack<string>& record_time) {
    string file_type(path_man.getFileType());
    string upper_file_type(file_type);
    for_each(upper_file_type.begin(), upper_file_type.end(), [](char &ch) {
        ch += 'A'-'a'; /* to uppercase */
    });
    string prompt = "[" + upper_file_type + "] "; // the command part
    prompt = SCOLOR(LIGHT_GREEN, prompt);
    if (file_type!="record" || (file_type=="record" && record_time.empty())) {
        // if not the record, or record && not in the record time
        prompt += CCOLOR(LIGHT_BLUE, "Please input the command: ");
        cout << endl;
        cout << prompt;
        return;
    }
    // the case: record and in the record time
    string start_time(record_time.top());
    string duration = timeDurationFromNow(start_time);
    duration = SCOLOR(LIGHT_RED, duration);
    prompt +=  start_time + " -> ... (U: " + duration + ") >>: ";
    cout << endl;
    cout << prompt;
}

void printPromptTime(Path_man& path_man, const stack<string>& record_time) {
    // the var
    string file_type(path_man.getFileType());
    // time c_str
    const int buffer_size {10};
    char time_str[buffer_size];
    time_t now = time(nullptr);
    strftime(time_str, buffer_size, "%H:%M", localtime(&now));
    // time string
    string prompt("[" + string(time_str) + "] ");
    prompt = SCOLOR(LIGHT_GREEN, prompt);
    //
    if (file_type!="record" || (file_type=="record" && record_time.empty())) {
        // if not the record, or record && not in the record time
        prompt += CCOLOR(LIGHT_BLUE, "Please input the command: ");
        cout << endl;
        cout << prompt;
        return;
    }
    // the case: record and in the record time
    string start_time(record_time.top());
    string duration = timeDurationFromNow(start_time);
    prompt +=  start_time + " -> ... (U: " + duration + ") >>: ";
    cout << endl;
    cout << prompt;
}

void getStateVerbose(const char state, string& state_verbose) {
    string str;
    switch (state) {
        case NOTDO:
            str = "Not Do";
            break;
        case NOTFINISH:
            str = "Not Finish";
            break;
        case FINISH:
            str = "Finish";
            break;
        default:
            str = "All";
    }
    state_verbose = str;
}

bool  isNum(const string& num_str) {
    if (num_str.empty()) return false;
    auto it = num_str.begin();
    for (; it!=num_str.end(); ++it) {
        if (*it<'0' || *it>'9') {
            break;
        }
    }
    // check
    return (it==num_str.end()) ? true : false;
}

void getDirFiles(const string& dir_path, vector<string>& files_path) {
    const char* py_command = "import sys;"
                             "sys.path.append('./py-script')"
                             "sys.path.append('../')";
    Py_Initialize(); // start
    PyRun_SimpleString(py_command); // set the python search path
    PyObject* p_module_name = PyUnicode_FromString("hgj_py");
    PyObject* p_module = PyImport_Import(p_module_name);
    PyObject* p_func = PyObject_GetAttrString(p_module, "getDirFiles");
    PyObject* arg = Py_BuildValue("(s)", dir_path.c_str());
    PyObject* p_list = PyObject_CallObject(p_func, arg); // the python function return value
    // convert p_list to the vector
    files_path.clear();
    Py_ssize_t ls_size = PyList_Size(p_list);
    for (auto i=0; i<ls_size; ++i) {
        PyObject* list_ele = PyList_GetItem(p_list, i);
        string ele_str = PyUnicode_AsUTF8(list_ele);  // str -> string
        files_path.push_back(ele_str);
    }
    Py_Finalize(); // end
}

void printMonthDay(const string& dir_path, int year, int month, const string& file_type, bool is_calendar) {
    const char* py_command = "import sys;"
                             "sys.path.append('./py-script');"
                             "sys.path.append('../')";
    Py_Initialize();
    PyRun_SimpleString(py_command);
    PyObject* p_module = PyImport_ImportModule("hgj_py"); // module
    PyObject* p_func = PyObject_GetAttrString(p_module, "printMonthDay");
    PyObject_CallFunction(p_func, "(siisi)",dir_path.c_str(), year, month, file_type.c_str(), (int)is_calendar); // function, call the function
    Py_Finalize();
}

string timeDurationFromNow(const string& history_time) {
    string duration;
    const char* py_command = "import sys;"
                             "sys.path.append('./py-script');"
                             "sys.path.append('../')";
    Py_Initialize();
    PyRun_SimpleString(py_command);
    PyObject* p_module = PyImport_ImportModule("hgj_py"); // module
    PyObject* p_func = PyObject_GetAttrString(p_module, "timeDurationFromNow");
    PyObject* p_str = PyObject_CallFunction(p_func, "s", history_time.c_str());
    // p_str is the string, convert to the string
    duration = string(PyUnicode_AsUTF8(p_str));
    Py_Finalize();
    //
    return duration;
}

void progressBar(int mins, bool is_hyphen_num_short) {
    const char* py_command = "import sys;"
                             "sys.path.append('./py-script');"
                             "sys.path.append('../')";
    Py_Initialize();
    PyRun_SimpleString(py_command);
    PyObject* p_module = PyImport_ImportModule("hgj_py");
    PyObject* p_func = PyObject_GetAttrString(p_module, "progressBar");
    PyObject_CallFunction(p_func, "ii", mins, static_cast<int>(is_hyphen_num_short));
    Py_Finalize();
}

void printFileList(const string& dir_path, const vector<string>& file_type_list, const vector<char>& states_ls) {
    // vector -> string, easy to share the data by the python
    string file_types_str;
    string states_str;
    for (auto it=file_type_list.begin(); it!=file_type_list.end(); ++it) {
        if (it != file_type_list.end()-1) {
            file_types_str += *it + ",";
        }
        else {
            file_types_str += *it;
        }
    }
    for (auto it=states_ls.begin(); it!=states_ls.end(); ++it) {
        if (it != states_ls.end()-1) {
            states_str += string(1, *it) + ",";
        }
        else {
            states_str += string(1, *it);
        }
    }
    if (file_types_str.empty()) file_types_str = TASK + "," + RECORD + "," + QUESTION + "," + LEARN;
    if (states_ls.empty()) states_str = "x,-,y";

    //
    const char* py_command = "import sys;"
                             "sys.path.append('./py-script');"
                             "sys.path.append('../')";
    const string arg_format;

    Py_Initialize();
    PyRun_SimpleString(py_command);
    PyObject* p_module = PyImport_ImportModule("hgj_py"); // module
    PyObject* p_func = PyObject_GetAttrString(p_module, "printFileList"); // function
    PyObject_CallFunction(p_func, "(sss)", dir_path.c_str(), file_types_str.c_str(), states_str.c_str());
    Py_Finalize();
}

void cls() {
#if defined(__linux__) || defined(__CYGWIN__)  /* linux or cygwin */
    system("clear");
#else /* window's cmd or powershell */
    system("cls");
#endif
}































