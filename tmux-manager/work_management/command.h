#ifndef COMMAND_H
#define COMMAND_H

#include "utils.h"
#include "hgj_t.h"

/* command: add */
void cmd_add(File_t& file, Path_man& path_man);
/* command: file */
void cmd_date(File_t& file, const Parser& parser,  Path_man& path_man, Show_arg& show_arg);
/* command: date */
void cmd_file(File_t& file, const Parser& parser, Path_man& path_man, Show_arg& show_arg);
/* command: show */
void cmd_show(File_t& file, Path_man& path_man, Parser& parser, Show_arg& show_arg);
/* command: save */
void cmd_save(File_t& file);
/* command: openInVim */
void cmd_openInVim(File_t& file, Path_man& path_man);
/* command: alias */
void cmd_alias(Parser& parser, AliasParser& alias_parser);
/* command: label */
void cmd_label(File_t& file, Parser& parser, AliasParser& alias_parser, const Path_man& path_man, const Show_arg& show_arg);
/* command: list */
void cmd_list(File_t& file, Path_man& path_man, Parser& parser);
/* command: record_in, record_out */
void cmd_recordOperation(File_t& file, Path_man& path_man, Parser& parser, stack<string>& record_time);
#endif
