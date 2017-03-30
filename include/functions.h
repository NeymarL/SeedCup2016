/**
 * 所有函数的声明
 */

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "code.h"
#include "variable.h"

/* util.cpp */
vector<string>* read_file(string filename);
void trim(string& str);
void do_output();
Code* vector_to_array(vector<Code*>* vec);
bool write_output(bool print);
bool handle_compare(string code);
bool is_compare_op(const char c);
int string_to_int(const string& str);
vector<string> split(string str, string pattern);
int prior(string opr);
bool is_leagal_var(const char c);
bool is_number(const string& str);
string int_to_string(int value);
Code* match_bracket();

/* KMP.cpp */
void get_next(string S, string T);
int KMP_index(string S, string T);
int KMP_count(string S, string T);

/* preprocess.cpp */
vector<Code*>* preprocess();
void discard_comment(string& line, bool& in_comment);
bool handle_printf(vector<Code*>* output, string& line, int& line_number, int i,
    vector<string>* lines, bool& should_add_brackets);
bool fix_else(string& line, int line_number, vector<Code*>* output);

/* main.cpp */
void do_block();
void do_exit();

/* branch.cpp */
void do_if();
bool jump_block();

/* loop.cpp */
void do_loop();
string cutexpr(int numofline);
bool is_compare_exp(string exrpression);
int if_continue(string expression);
void output(Code* blockbegin);

/* sequence.cpp */
void do_sequence();
int handle_expression(string& exp);
int short_expression(string& exp);
int add_self(string s);
int calculate(string str);
int do_printf(string s);

#endif
