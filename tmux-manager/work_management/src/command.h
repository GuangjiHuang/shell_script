#ifndef COMMAND_H
#define COMMAND_H

#include "utils.h"
#include "hgj_t.h"

/* command: add */
void cmd_add(File_t& file, Path_man& path_man);
/* command: append */
void cmd_append(File_t& file, Path_man& path_man, Parser& parser);
/* command: date */
void cmd_date(File_t& file, const Parser& parser,  Path_man& path_man, Show_arg& show_arg);
/* command: file */
void cmd_file(File_t& file, const Parser& parser, Path_man& path_man, Show_arg& show_arg);
/* command: show */
void cmd_show(File_t& file, Path_man& path_man, Parser& parser, Show_arg& show_arg);
/* command: save */
void cmd_save(File_t& file);
/* command: openInVim */
void cmd_openInVim(File_t& file, Path_man& path_man, Parser& parser, string& gui_plan_dir);
/* command: alias */
void cmd_alias(Parser& parser, AliasParser& alias_parser);
/* command: label */
void cmd_label(File_t& file, Parser& parser, AliasParser& alias_parser, const Path_man& path_man, const Show_arg& show_arg);
/* command: list */
void cmd_list(File_t& file, Path_man& path_man, Parser& parser);
/* command: record_in, record_out */
void cmd_recordOperation(File_t& file, Path_man& path_man, Parser& parser, stack<string>& record_time);
/* command: operation */
void cmd_operation(File_t& file, Parser& parser);
/* command: timer */
void cmd_timer(Parser& parser);
/* command just for study */
void cmd_just_for_study(string& questions_js_path, string& answers_path);
void cmd_study_topic1(Parser& parser, StudyT& study);
void cmd_study_topic2(Parser& parser, StudyT& study);
void cmd_study_questions(Parser& parser, StudyT& study);
void cmd_study_save(Parser& parser, StudyT& study);
void cmd_study_reload(Parser& parser, StudyT& study);
void cmd_study_bookmarks(Parser& parser, StudyT& study);
bool cmd_study_change_topic_question(Parser& parser, StudyT& study);
void cmd_timer_gui(Parser& parser, const string& pipeline_path);
void cmd_control_gui(Parser& parser, const string& pipeline_path, string& gui_help_path);

//void cmd_
#endif
