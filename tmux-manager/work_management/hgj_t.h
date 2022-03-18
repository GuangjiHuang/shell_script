#ifndef HGJ_T_H
#define HGJ_T_H

#include "utils.h"
#define TASK string("task")
#define RECORD string("record")
#define QUESTION string("question")
#define LEARN string("learn")

namespace fs = std::filesystem;
// the declaration
class File_t;
class Parser;
class Path_man;

/* class: File_t */
class File_t {
private:
    string _path;
    vector<string> _items; // buffer, vector to store each item
    int _items_num; // num
    vector<char> _states; // mark progress
    unordered_map<char, int> _states_num; // states num
    FILE* _p_file = nullptr;  // points to file
public:
    explicit File_t(const string& path, const char* open_mode);
    void readToBuffer(const char* open_mode);
    void flushBuffer();
    bool writeToBuffer(bool is_record=false, const string& cur_time={});
    void bufferToCout() const;
    void update(const string& path);
    vector<string>& refItems();
    vector<char>& refStates();
    int& refItemsNum();
    unordered_map<char, int>& refStateNum();
    // just public the items, and the states
    const vector<string>& getItems() const;
    const int& getItemsNum() const;
    const vector<char>& getStates() const;
    const int getStateNum(const char state) const;
    char getStateFromItem(const string& item) const;
    void subStateInItem(string& item, char state);
    //
private:
    void _closeFile();
    void _openFile(const char* open_mode="r+");
    void _readLines(vector<string>& lines);
    void _readItems();
    void _readStates();
    string _formatItem();
    bool _strIsSpace(const string& str);
};

/* class: Parser */
class Parser {
public:
    string input_str;
    string command;
    vector<string> arg_ls;
    int arg_num;
    // the constructor
    Parser() = default;
    explicit Parser(string input_str);
    Parser(Parser&& ) = default;
    ~Parser() = default;
    void update(string input_str);
    bool empty();
    // show, overload the << operation
    // split function
    static void split(const string& input_str, vector<string>& str_ls, const char* delimiter);
private:
    // renew attribute
    void _updateData(const vector<string>& arg_ls);
    // the friend
    friend ostream& operator<<(ostream& out, Parser& parser);
};

/* class: Path_num */
class Path_man {
private:
    // the attributes
    string date_str;
    vector<string> date_vec;
    string file_type;
    string cur_dir;
    string cur_path;
public:
    string parent_dir;
    static vector<string> defined_file_type;
public:
    // constructor
    explicit Path_man(string parent_dir);
    ~Path_man() = default;
    // update the data, but the parent_dir not change
    bool updateDate(string date_str);
    void updateFileType(string file_type);
    string getDate() const;
    string getCurPath() const;
    string getCurDir() const;
    string getFileType() const;
private:
    // date_vec
    bool _mkDateVec(const string& date_str);
    bool _mkCurDir();
    // current path
    bool _mkCurPath();
    // make dir
    void _mkdir();
    // touch file
    void _touch();
    // check if today
    bool _isToday();
    // check the date input if is valid
    bool _isValidDate(const vector<string>& date_vec);
};

/* struct: Show_arg */
class Show_arg {
private:
    int _state_total_num;
    int _state_show_num;
    char _state;
public:
    int least_show_num = 10;
    bool is_reverse = false; // you can set it without checking
    //static unordered_map<char, int> state_index_hash_table;
public:
    // the constructor
    Show_arg(File_t& file, const char state='a', const int state_show_num=INT_MAX) : _state(state) {
        _state_total_num = file.getStateNum(state);
        if (state_show_num>0 && state_show_num<=_state_total_num) {
            _state_show_num = state_show_num;
        }
        else {
            _state_show_num = _state_total_num;
        }
    }

    void update(const File_t& file) { // when the file change, update
        // _state will not change, but the num will change
        _state_total_num = file.getStateNum(_state);
        // compare with the least_show_num
        if (_state_show_num<least_show_num) _state_show_num = least_show_num; // that is very important, when file date toggle
        if (_state_show_num>0 && _state_show_num<_state_total_num) {
            ; // the state_show_num not change
        }
        else {
            _state_show_num = _state_total_num;
        }
    }

    int getStateShowNum() const {
        return _state_show_num;
    }

    int getStateShowTotalNum() const {
        return _state_total_num;
    }

    void setStateShowNum(int set_num, const File_t& file) {
        _state_total_num = file.getStateNum(_state);
        if (set_num>0 && set_num<_state_total_num) {
            _state_show_num = set_num;
        }
        else {
            _state_show_num = _state_total_num;
        }
    }

    char getState() const {
        return _state;
    }

    void setState(char set_state) {
        if (set_state==NOTDO || set_state==NOTFINISH || set_state==FINISH) {
            _state = set_state;
        }
        else {

            _state = 'a';
        }
    }

};

/* AliasParser */
class AliasParser {
private:
    string _path;
     unordered_map<string, string> _command_alias_tb;

public:
    AliasParser(const string& path); 
    void addAliasCommand(string& command); // can add the command to the hash table
    void parseAliasCommand(string& command); // check and then change to the new
    void showAlias();
    void saveAlias(const string& path);
    void reloadAlias(const string& path);

private:
    void _readComandFromFile(const string& path);
    void _lineToPair(const string& line);
    void _preDeal(string& line);
    void _stripSpace(string& line);
};
#endif

