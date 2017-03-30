/**
 * variable类的声明
 */

#ifndef VARIABLE_H
#define VARIABLE_H

#include <map>
#include <string>
#include <vector>

using namespace std;

class Variable
{
    vector<map<string, int>*> variable_stack;   // "global" variable stack
public:
    Variable();
    ~Variable();
    bool get(const string& name, int& value);   // 取得某个变量的值, 变量值放在value中，返回成功失败
    void set(const string& name, int value);    // 设置(初始化)某个变量的值
    void update(const string& name, int value); // 更新某个变量的值
    void push_table();                          // push一张变量表
    void pop_table();                           // pop一张变量表
    void print_table();                         // 打印变量表
};

#endif
