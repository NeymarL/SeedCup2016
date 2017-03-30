/**
 * 涉及变量的存储与跟踪
 * variable类的实现
 */

#include "headers.h"

Variable::Variable()
{
    // 好像什么都不用做
}

Variable::~Variable()
{
    // 好像也什么都不用做
}

/**
 * 取得某个变量的值
 * @param  name  变量名
 * @param  value 获取的变量值
 * @return       true：找到该变量；false：没找到
 */
bool Variable::get(const string& name, int& value)
{
    for (int i = variable_stack.size() - 1; i >= 0; i--) {
        map<string, int>* table = variable_stack[i];
        map<string, int>::iterator it = table->find(name);
        if (it != table->end()) {
            value = it->second;
            return true;
        }
    }
    // 没找到默认置0
    value = 0;
    return false;
}

/**
 * 设置某个变量的值
 * @param name  变量名
 * @param value 值
 */
void Variable::set(const string& name, int value)
{
    map<string, int>* table = variable_stack[variable_stack.size() - 1];
    (*table)[name] = value;
}

/**
 * 更新某个变量的值
 * @param name  变量名
 * @param value 值
 */
void Variable::update(const string& name, int value)
{
    int tmp = 0;
    if (get(name, tmp)) {
        for (int i = variable_stack.size() - 1; i >= 0; i--) {
            map<string, int>* table = variable_stack[i];
            map<string, int>::iterator it = table->find(name);
            if (it != table->end()) {
                (*table)[name] = value;
            }
        }
    }
    else {
        set(name, value);
    }
}

/**
 * push一张变量表
 * @param table 变量表
 */
void Variable::push_table()
{
    map<string, int>* table = new map<string, int>;
    variable_stack.push_back(table);
}

/**
 * pop一张变量表
 */
void Variable::pop_table()
{
    map<string, int>* table = variable_stack[variable_stack.size() - 1];
    variable_stack.pop_back();
    delete table;
}

/**
 * 打印变量表
 */
void Variable::print_table()
{
    map<string, int>::iterator iter;
    for (int i = 0; i < variable_stack.size(); i++) {
        cout << "{" << endl;
        for (iter = variable_stack[i]->begin(); iter != variable_stack[i]->end(); iter++) {
            cout << iter->first << " = " << iter->second << endl;
        }
        cout << "}" << endl;
    }
}
