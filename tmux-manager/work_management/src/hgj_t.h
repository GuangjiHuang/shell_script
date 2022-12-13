#ifndef HGJ_T_H
#define HGJ_T_H

#include "utils.h"
#define TASK string("task")
#define RECORD string("record")
#define QUESTION string("question")
#define LEARN string("learn")
#define STUDY string("study")

namespace fs = boost::filesystem;

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

/* class: Path_man */
class Path_man {
private:
    // the attributes
    string date_str;
    string file_type;
    string cur_dir;
    string cur_path;
public:
    vector<string> date_vec;
    string parent_dir;
    static unordered_set<string> defined_file_type;
public:
    // constructor
    explicit Path_man(string parent_dir, const string& file_type);
    ~Path_man() = default;
    // update the data, but the parent_dir not change
    bool updateDate(string date_str);
    bool updateFileType(string file_type);
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
    void vimAlias();
    void lessAlias();

private:
    void _readComandFromFile(const string& path);
    void _lineToPair(const string& line);
    void _preDeal(string& line);
    void _stripSpace(string& line);
};

/* StudyT */
class StudyT {
public:
    using json = nlohmann::json;
    // the path and the data
    string question_path;
    string answer_dir;
    json js_data;
    // topic1, topic2, question, link, score, num
    string topic1;
    string topic2;
    int question_id;
    vector<string> topic1_vec;
    vector<string> topic2_vec;
    vector<string> questions_vec;
    string question;
    string link;
    double score;
    int occur;
    /* constructor */
    StudyT(string& question_path, string& answer_dir, string& topic1, string& topic2, int question_id): question_path(question_path), answer_dir(answer_dir) {
        loadJson(question_path, js_data);
        initInfo(topic1, topic2, question_id);
    }
    /* init information */
    void initInfo(const string& c_topic1, const string& c_topic2, const int c_question_id) {
        string topic1(c_topic1);
        string topic2(c_topic2);
        int question_id = c_question_id;
        bool is_topic1_change = true;
        bool is_topic2_change = true;
        // check the topic1
        if (this->topic1 == topic1 && !this->topic1.empty()) {
            is_topic1_change = false;
        }
        else {
            // renew the topic1_vec, and try to find out the topic1
            topic1_vec.clear();
            for (auto it=js_data.begin(); it!=js_data.end(); ++it) {
                topic1_vec.emplace_back(it.key());
                if (topic1.empty()) {
                    topic1 = it.key();
                }
            }
            // find the topic1
            if (find(topic1_vec.begin(), topic1_vec.end(), topic1)==topic1_vec.end()) {
                // all the thing not change
                return;
            }
            else {
                this->topic1 = topic1;
            }
        }
        // check the topic2
        if (this->topic2 == topic2 && !this->topic2.empty() && !is_topic1_change) {
            is_topic2_change = false;
            ; // do nothing;
        }
        else {
            // renew the topic2_vec
            topic2_vec.clear();
            for (auto it=js_data[topic1].begin(); it!=js_data[topic1].end(); ++it) {
                topic2_vec.emplace_back(it.key());
                if (topic2.empty()) {
                    topic2 = it.key();
                }
            }
            if (find(topic2_vec.begin(), topic2_vec.end(), topic2)==topic2_vec.end()) {
                if (is_topic1_change) {
                    // empty the topic2 and the question
                    this->topic2 = this->question = "";
                    this->question_id = -1;
                    this->topic2_vec.clear();
                    this->questions_vec.clear();
                    this->link = "";
                    this->score = 0.0;
                    this->occur = 0;
                }
                return;
            }
            else {
                this->topic2 = topic2;
            }
        }
        if (this->question_id == question_id  && !is_topic2_change) {
            return; // do nothing
        }
        // check the question
        json tmp = js_data[topic1][topic2];
        natureSort(tmp, questions_vec);
        int len = js_data[topic1][topic2].size();
        if (len == 0) {
            this->question_id = -1;
            this->question = "";
            this->link = "";
            this->score = 0.0;
            this->occur = 0;
        }
        else if (question_id < len && question_id >= 0) {
            this->question_id = question_id;
            this->question = questions_vec[this->question_id];
            this->link = tmp[question]["link"];       
            this->score = tmp[question]["score"];
            this->occur = tmp[question]["occur"];
        }
        else if (question_id<0 || question_id>=len) {
            this->question_id = question_id<0 ? 0 : len - 1;
            this->question = questions_vec[this->question_id];
            this->link = tmp[question]["link"];       
            this->score = tmp[question]["score"];
            this->occur = tmp[question]["occur"];
        }
    }
    /* load data                */
    bool loadJson(string& question_path, json& js_data) {
        ifstream f_in(question_path);
        bool is_load = false;
        if (!f_in) {
            cout << "[Warning]: Can not read the file!" << question_path << endl;
        }
        else {
            try {
                js_data = json::parse(f_in);  
                is_load = true;
            }
            catch(...) {
                cout << "[Warning]: Can not parse the json file!" << endl;
            }
        }
        f_in.close();
        return is_load;
    }
    /* dump data                */
    void dumpJson(int dump_level, bool is_backen) {
        // if dump_leve is -1 or 0, that is no indent, otherwise, has the indent
        string save_str = this->js_data.dump(dump_level);
        if (is_backen) {
            string backen_command = "cp " + this->question_path + " " + this->question_path + ".bd";
            system(backen_command.c_str());
        }
        ofstream f_out(this->question_path);
        f_out << save_str;
        f_out.close();
    }
    /* add questions            */
    /* delete questions         */
    /* modify questions         */
    void setQuestion(const string& new_question) {
        string old_question(question);
        string old_link(link);
        // the original question value
        json q_val = js_data[topic1][topic2][old_question];
        // change the link and question
        link = new_question + ".md";
        question = new_question;
        q_val["link"] = link;
        js_data[topic1][topic2][new_question] = q_val;
        // delete the old question and rename the file name
        js_data[topic1][topic2].erase(old_question);
        string answer_old_path = answer_dir + "/" + topic1 + "/" + topic2 + "/" + old_link;     
        string answer_new_path = answer_dir + "/" + topic1 + "/" + topic2 + "/" + link;     
        if (fs::exists(fs::path(answer_old_path))) {
            // rename
            fs::rename(fs::path(answer_old_path), fs::path(answer_new_path));
        }
    }
    //
    void setOccure(int occure_val) {
        occur = occure_val>0 ? occure_val : 0;
    }
    //
    void setScore(double score_val) {
        if (score_val > 10.0) {
            score = 10.0;
        }
        else if (score_val < 0.0) {
            score = 0.0;
        }
        else {
            score = score_val;
        }
    }
    /* query topic1             */
    void queryTopic1(vector<string>& topic1_vec) {
        topic1.clear();
        for (auto it=js_data.begin(); it!=js_data.end(); ++it) {
            topic1_vec.emplace_back(it.key());
        }
    }
    /* query topic2             */
    void queryTopic2(string& topic1, vector<string>& topic2_vec) {
        if (!js_data.contains(topic1)) {
            cout << "[Warning]: Not contains the topic1: " << topic1 << endl;
            return;
        }
        topic2_vec.clear();
        for (auto it=js_data[topic1].begin(); it!=js_data[topic1].end(); ++it) {
            topic2_vec.emplace_back(it.key());
        }
    }
    /* query questions          */
    void queryQuestions(string& topic1, string& topic2, vector<string>& questions_vec) {
        if (js_data.contains(topic1) && js_data[topic1].contains(topic2)) {
            json tmp = js_data[topic1][topic2];
            for (auto it=tmp.begin(); it!=tmp.end(); ++it) {
                questions_vec.emplace_back(it.key());
            }
        }
        else if (!js_data.contains(topic1)) {
            cout << "[Warning]: Not contains the topic1: " << topic1 << endl;
        }
        else {
            cout << "[Warning]: Not contains the topic2: " << topic2 << endl;
        }
    }
    /* read and write the answer*/
    void readWriteAnswer() {

        ;
    }
};
#endif

