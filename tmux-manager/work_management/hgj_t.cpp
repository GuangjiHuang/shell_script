#include "utils.h"
#include "hgj_t.h"

// class: File_t
File_t::File_t(const string& path, const char* open_mode) : _path(path), _p_file(nullptr) {
    readToBuffer(open_mode);
}

// readToBuffer
void File_t::readToBuffer(const char* open_mode="r+") {
    _openFile(open_mode); // open file
    _readItems(); // read items to the buffer: _items
    _items_num = _items.size(); //_items_num
    _readStates(); // _states and _states_count
    _closeFile(); // close file, _p_file points to nullptr
}

// flushBuffer
void File_t::flushBuffer() {
    _openFile("w");  // open file again
    for (const auto& item : _items) {
        fputs((item+"\n").c_str(), _p_file); // add the \n to the end
    }
    _closeFile();
}

// writeToBuffer
bool File_t::writeToBuffer(bool is_record, const string& cur_time) {
GET_LINE:
    string item;
    cout << "->: ";
    getline(cin, item);
    // if key the ctrl+d, that is eof mark, means that you don't want to key this content
    if (cin.eof()) {
        cin.clear();
        cout << endl << "[rewrite]";
        goto GET_LINE;
    }
    // if item is empty, return
    if (_strIsSpace(item)) { // I think that is import!
        return false;
    }
    item += "\n";
    // concatenate the prefix
    if (is_record) {
        item = _formatItem() + string("<" + cur_time + "^> ") + item; // '^' is the mark
    }
    else {
        item = _formatItem() + item; // add the such as 1.(-) at the front of the item
    }
    // update the _items, _items_num and _states, _states_count
    _items.push_back(item);
    ++_items_num;
    ++_states_num.at(NOTDO); // not do add 1
    ++_states_num.at('a');  // all add 1
    _states.push_back(NOTDO);
    //
    return true;
}

// bufferToCout
void File_t::bufferToCout() const {
    if (_items.empty()) {
        cout << "[ Warning:]file is empty!" << endl;
    }
    else {
        copy(_items.begin(), _items.end(), ostream_iterator<string>(cout, "\n"));
    }
}

// update
void File_t::update(const string& path) {
    flushBuffer(); // flush buffer
    _path = path; // renew the path
    readToBuffer("r+");
}

// getItems
const vector<string>& File_t::getItems() const {
    return _items;
}

// refItems
vector<string>& File_t::refItems() {
    return _items;
}

// refStates
vector<char>& File_t::refStates() {
    return _states;
}

// refItemsNum
int& File_t::refItemsNum() {
    return _items_num;
}

// referStateNum
unordered_map<char, int>& File_t::refStateNum() {
    return _states_num;
}

// getItemsNum
const int& File_t::getItemsNum() const {
    return _items_num;
}


// getStates
const vector<char>& File_t::getStates() const {
    return _states;
}
// getStateFromItem
char File_t::getStateFromItem(const string& item) const {
    char ret_state {'x'}; // that default is the x
    auto it = item.begin();
    while (it!=item.end() && *it!='(') ++it;
    if (it!=item.end() && ++it!=item.end()) {
        ret_state = *it; // has been increment
    }
    // check
    if (ret_state!=NOTDO && ret_state!=NOTFINISH && ret_state!=FINISH) {
        ret_state = 'x'; // rectify
    }
    return ret_state;
}

// subStateInItem
void File_t::subStateInItem(string& item, char state) {
    // check the if the state is valid
    if (state!=NOTDO && state!=NOTFINISH && state!=FINISH) {
        return; // not change
    }
    auto it = item.begin();
    while (it!=item.end() && *it!='(') ++it;
    if (it!=item.end() && ++it!=item.end()) {
        *it = state;
    }
    // else not change
}

// getStateNum
const int File_t::getStateNum(const char state) const {
    // check
    if (_states_num.find(state) != _states_num.end()) {
        return _states_num.at(state);
    }
    else {
        return _states_num.at('a'); // not found, return all
    }
}

// _openFile
void File_t::_openFile(const char* open_mode) {
    _p_file = fopen(_path.c_str(), open_mode);
    // check if the file is opened
    if (!_p_file) {
        cerr << "Can not open the file: " << _path << endl;
        //fclose(_p_file); // now the _p_file is the null, can not close
    }
};

// _closeFile
void File_t::_closeFile() {
    if (fclose(_p_file) == 0) { // close successfully
        // set the _p_file to nullptr
        _p_file = nullptr;
    }
    else {
        cerr << "close the file error! " << "In " << __func__ << endl;
    }
}

// _readLines
void File_t::_readLines(vector<string>& lines) {
    const int buffer_size = 512;
    char c_line[buffer_size];
    while (fgets(c_line, buffer_size, _p_file)) {
        lines.push_back(string(c_line)); // c_string to string and then put it to the lines
    }
}

// readItems
void File_t::_readItems() {
    _items.clear(); // important! clear first
    vector<string> lines;
    _readLines(lines); // read lines store in lines vector
    string item;
    for (auto it=lines.begin(); it!=lines.end(); it++) {
        if (File_t::_strIsSpace(*it)) {
            // push back the item
            if (!item.empty()) {    // may continue to many space lines
                _items.push_back(item);
                item.clear();
            }
        }
        else {
            // combine the lines to the item
            item += *it;
        }
    }
    // at the end, put back the item
    if (!item.empty()) {
        _items.push_back(item);
    }
}

// _readStates
void File_t::_readStates() {
    // clear the vector first
    _states.clear();
    _states_num.clear();
    int num_not_do = 0;
    int num_not_finish = 0;
    int num_finish = 0;
    // read to the _states
    for (auto it=_items.begin(); it!=_items.end(); it++) {
        char state;
        state = getStateFromItem(*it);
        //
        if (state==NOTFINISH) {
            ++num_not_finish;
        }
        else if (state==FINISH)  {
             ++num_finish;
        }
        else {
            ++num_not_do;
        }
        _states.push_back(state);
    }
    // push back, order: not do, not finish, finish, and all
    _states_num.insert({NOTDO, num_not_do});
    _states_num.insert({NOTFINISH, num_not_finish});
    _states_num.insert({FINISH, num_finish});
    _states_num.insert({'a', num_not_finish + num_not_do + num_finish});
}

// writeItemFormat
string File_t::_formatItem() {
    // add the prefix before the item, for example: 1.(y)
    stringstream ss;
    int item_num = _items_num + 1; // add 1 represent the next item
    char state = NOTDO;
    ss << item_num << ".(" << state << ") ";
    // ss to string, renew the item's value
    return ss.str();
}

bool File_t::_strIsSpace(const string& str) {
    auto it = str.begin();
    while (it!=str.end() && isspace(*it)) ++it;
    return it==str.end() ? true : false;
}


/* class: Parser */
// public function
Parser::Parser(string input_str) : input_str(input_str) {
    // get the arg_ls
    split(input_str, arg_ls, " ");
    // update the data
    _updateData(arg_ls);
}
void Parser::update(string input_str) {
    split(input_str, arg_ls, " ");
    _updateData(arg_ls);
}
bool Parser::empty() {
    return command.empty();
}
// private function
void Parser::_updateData(const vector<string>& arg_ls) {
    int arg_ls_size = arg_ls.size();
    if (arg_ls_size==0) {
        arg_num = 0;
        command.clear();
    }
    else if (arg_ls_size==1) {
        arg_num = 0;
        command = arg_ls[0];
    }
    else {
        arg_num = arg_ls_size - 1;
        command = arg_ls[0];
    }
}
// static function
void Parser::split(const string& input_str, vector<string>& str_ls, const char* delimiter) {
    str_ls.clear(); // clear empty the vector
    // copy the input_src to the c_string
    char* c_input_str = new char[input_str.size() + 1];
    strcpy(c_input_str, input_str.c_str());
    // split
    string split_part;
    char* p = strtok(c_input_str, delimiter);
    while (p) {
        split_part.assign(p);
        if (!split_part.empty()) {  // must not be empty
            str_ls.push_back(split_part);
        }
        p = strtok(nullptr, delimiter);
    }
    // release the memory
    delete[] c_input_str;
}
// friend function; show, overload the << operation
ostream& operator<<(ostream& out, Parser& parser) {
    out << hyphen(20) << " arg information " << hyphen(20) << endl;
    out << "command: " << parser.command << endl;
    out << "args:(total " << parser.arg_num << ")" << endl;
    int arg_id {1};
    for (const auto &arg : parser.arg_ls) {
        out << "(" << arg_id++ << ")" <<  arg << " ";
    }
    out << endl;
    out << hyphen(55) << endl;
    return out;
}

/* class Path_man */ 
// public function
Path_man::Path_man(string parent_dir) : parent_dir(parent_dir), file_type("task") {
    // init the date_str
    ::getDate(0, date_str); // init to today, using the global function
    // construct the current path
    _mkDateVec(date_str); // [year, month, day], today be bound to the valid date

    _mkCurDir();
    _mkCurPath(); // parent_dir/2022/03/03-12/record.txt
}
bool Path_man::updateDate(string date_str) {
    bool is_update = false;
    if (_mkDateVec(date_str)) {        // mkdir and mkpath only if the date is valid
        string date_str_bd = this->date_str; // backed
        this->date_str = date_str; // only if date_str is valid, then renew this->date_str;
        if (!_mkCurDir()) {
            this->date_str = date_str_bd; // fail, recover
        }
        _mkCurPath(); // don't need to judge
        is_update = true; // update successfully
    }
    else {
        cout << "[Warning]: date: " << date_str << "is invalid, not update!" << endl;
    }
    return is_update;
}
void Path_man::updateFileType(string file_type) {
    // check if the file_type is valid
    if (find(defined_file_type.begin(), defined_file_type.end(), file_type)!=defined_file_type.end()) {
        // try to make current path
        string file_type_bd = this->file_type; // backen, once fail, recover
        this->file_type = file_type;
        if (!_mkCurPath()) { // here try to make, if not successfully, recover
            this->file_type = file_type_bd;
        }
    }
    else {
        cout << "Input file type is unexpected!" << endl;
        cout << "Expected file type are: ";
        for (const auto& f_t : defined_file_type) {
            cout << f_t << " ";
        }
        cout << endl;
    }
}
string Path_man::getDate() const {
    return date_str;
}
string Path_man::getCurPath() const {
    return cur_path;
}
string Path_man::getCurDir() const {
    return cur_dir;
}
string Path_man::getFileType() const {
    return file_type;
}
// private function
bool Path_man::_mkDateVec(const string& date_str) {
    // call the split function to split the -
    vector<string> temp_date_vec;
    Parser::split(date_str, temp_date_vec, "-");
    // check the day if valid, once not valid, it will return, and will not change the date_vec
    if (!_isValidDate(temp_date_vec)) {
        cout << "[Warning]: " << date_str << " is invalid! Cannot update!" << endl;
        return false;
    }
    // now the temp_date_vec is valid, so it can be assigned to the date_vec
    this->date_vec = temp_date_vec;
    return true;
}
bool Path_man::_mkCurDir() {
    // var
    bool is_mk = false;
    stringstream ss;
    string temp_cur_dir;
    // parent_dir/2022/03/03-12/
    string year(date_vec[0]);
    string month(date_vec[1]);
    string day(date_vec[2]);
    ss << parent_dir << "/" << year << "/" << month << "/" << (month + "-" + day);
    temp_cur_dir.assign(ss.str());
    // check if exists, if not, mkdir
    if (fs::exists(fs::path(temp_cur_dir))) {
        cur_dir = temp_cur_dir; // no need to make dir
        is_mk = true;
    }
    else {   // not exist, check if today
        if (_isToday()) {
            cur_dir = temp_cur_dir;
            _mkdir();
            is_mk = true;
        }
        else {
            is_mk = false;
            cout << "[warning]: No such dir: " << temp_cur_dir << endl;
        }
    }
    return is_mk;
}
bool Path_man::_mkCurPath() {
    // var
    bool is_mk = false;
    string temp_cur_path;
    // parent_dir/2022/03/03-12/record.txt
    temp_cur_path = cur_dir + "/" + file_type + ".txt";
    //
    if (fs::exists(fs::path(temp_cur_path))) { //
        cur_path = temp_cur_path;
        is_mk = true;
    }
    else {
        if (_isToday()) { // if not exists and today, touch the file
            cur_path = temp_cur_path;
            _touch();
            is_mk = true;
        }
        else {
            cout << "[warning]: No such path: " << temp_cur_path << endl;
            is_mk = false;
        };
    }
    return is_mk;
}
void Path_man::_mkdir() {
    string shell_command = "mkdir -p " + cur_dir;
    system(shell_command.c_str());
}
void Path_man::_touch() {
    string shell_command = "touch " + cur_path;
    system(shell_command.c_str());
}
bool Path_man::_isToday() {
    string date_today;
    ::getDate(0, date_today); // using the global function in utils.h
    return date_today==date_str;
}
bool Path_man::_isValidDate(const vector<string>& date_vec) {
    // check the size first
    const size_t year_month_day = 3;
    if (date_vec.size() != year_month_day) {
        cout << "[Warning]: not the valid date!" << endl;
        return false;
    }
    // all of str_num must be the number
    int year {0};
    int month {0};
    int day {0};
    try {
        year = stoi(date_vec.at(0)); // use the [] will occur error, no exception
        month = stoi(date_vec.at(1));
        day = stoi(date_vec.at(2));
    }
    catch (...) {
        //cout << "Your input date is not the number! Or not enough number!" << endl;
        return false;
    }
    return year>=2000 && month>=1 && month<=12 && day>=1 && day<=31;
}

/* AliasParser */
AliasParser::AliasParser(const string& path) : _path(path) {
    ifstream file(path);
    if (!file.good()) {
        cout << "[Warning]: can not open the file: " << path << endl;
        return;
    }
    string line;
    vector<string> l_r_cmd;
    while (getline(file, line)) {
        _preDeal(line);
        if (line.empty()) continue; // not deal the empty line
        Parser::split(line, l_r_cmd, "=");
        if (l_r_cmd.size() != 2) continue;
        // insert to the dict
        _stripSpace(l_r_cmd[0]); // very important
        _stripSpace(l_r_cmd[1]);
        _command_alias_tb[l_r_cmd[0]] = l_r_cmd[1];
    }
}

void AliasParser::addAliasCommand(string& line) {
    _preDeal(line);
    if (line.empty()) return;
    vector<string> l_r_cmd;
    Parser::split(line, l_r_cmd, "=");
    if (l_r_cmd.size()!=2) return;
    // insert to the dict
    _command_alias_tb.insert({l_r_cmd[0], l_r_cmd[1]});
}

void AliasParser::parseAliasCommand(string& raw_input) {
    _preDeal(raw_input);
    if (raw_input.empty()) return;
    // get command
    string command;
    auto it = raw_input.begin();
    while (it!=raw_input.end() && *it!=' ') ++it; // *it == ' '
    command.assign(raw_input.begin(), it); // not include the it
    if (_command_alias_tb.find(command) != _command_alias_tb.end()) {
        // found, then substitute the command
        string substitute_command = _command_alias_tb[command];
        raw_input.replace(0, command.size(), substitute_command);
    }
}

void AliasParser::showAlias() {
    cout << hyphen(22) << " ALIASES " << hyphen(22) << endl;
    int alias_id = 0;
    string alias_id_str;
    for (auto it = _command_alias_tb.begin(); it != _command_alias_tb.end(); ++it) {
        alias_id_str = to_string(++alias_id);
        if (alias_id_str.size()<2) alias_id_str += " ";
        cout << alias_id_str << ". " << (*it).first << " = " << (*it).second << endl;
    }
    cout << hyphen(55) << endl;
}

void AliasParser::saveAlias(const string& path="") {
    string save_path(path);
    if (save_path.empty()) {
        save_path = _path;
    }
    // how to solve the problem that the comment to write ?
    cout << "I am thinking how to implement now!" << endl;
}

void AliasParser::reloadAlias(const string& path="") {
    string load_path(path);
    if (load_path.empty()) {
        load_path = _path;
    }
    _command_alias_tb.clear(); // make the hash table empty
    ifstream file(load_path);
    if (!file.good()) {
        cout << "[Warning]: can not open the file: " << load_path << endl;
        return;
    }
    string line;
    vector<string> l_r_cmd;
    while (getline(file, line)) {
        _preDeal(line);
        if (line.empty()) continue; // not deal the empty line
        Parser::split(line, l_r_cmd, "=");
        if (l_r_cmd.size() != 2) continue;
        // insert to the dict
        _stripSpace(l_r_cmd[0]);
        _stripSpace(l_r_cmd[1]);
        _command_alias_tb.insert({l_r_cmd[0], l_r_cmd[1]});
    }
}

void AliasParser::_stripSpace(string &line) {
    int n = line.size();
    char *p_line = new char[n + 1];
    strcpy(p_line, line.c_str());
    // strip of the space
    char *p_l = p_line;
    char *p_r = p_line + n - 1;
    while (*p_l && isspace(*p_l)) ++p_l;
    while (p_r >= p_l && isspace(*p_r)) --p_r;
    *++p_r = '\0';
    line = string(p_l);
    delete[] p_line;
}

void AliasParser::_preDeal(string &line) {
    int n = line.size();
    char *p_line = new char[n + 1];
    strcpy(p_line, line.c_str());
    // strip of the space
    char *p_l = p_line;
    char *p_r = p_line + n - 1;
    while (*p_l && isspace(*p_l)) ++p_l;
    while (p_r >= p_l && isspace(*p_l)) --p_r;
    *++p_r = '\0';
    // strip of the comment
    char *p_c = p_l;
    while (*p_c) {
        if (*p_c == '#') {
            break;
        }
        else if (*p_c == '/') {
            if (*p_c && *(p_c + 1) == '/') {
                break;
            }
        }
        ++p_c;
    }
    *p_c = '\0';
    //
    line = string(p_l); // change the content of line
    delete[] p_line;
}