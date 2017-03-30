/**
 * 处理分支结构代码
 */

#include "headers.h"

/**
 * 处理分支结构
 */
void do_if()
{
    bool finish = false;    // 标志分支执行完毕
    while (!finish) {
        string expression = "";  // 记录表达式
        int i = 0;
        // 获取表达式的值
        bool in_para = false;
        bool single_else = false;
        while (1) {
            if (IP->code[i] == ')') {
                break;
            }
            else if (IP->code[i] == '(') {
                in_para = true;
            } 
            else if (in_para) {
                expression += IP->code[i];
            }
            i++;
            if (i >= IP->code.length()) {
                break;
            }
        }
        if (expression == "" && IP->code.find("else") == 0) {
            single_else = true;
        }
        // 判断表达式
        if (single_else || handle_compare(expression)) {
            IP++;               // IP 指向段首
            GVS.push_table();   // 新建变量表
            do_block();         // 执行这段代码，执行完毕IP指向 }
            if (IP->code.find("break") != -1) {
                return;
            }
            IP++;               // 指向下一句
            finish = true;
            // 跳过后面所有的else
            while (IP->code.find("else") == 0) {
                // do_output();
                jump_block();
            }
        }
        else {
            // 判断失败就跳到下一个block
            jump_block();
            do_output();
            if (IP->code.find("else") != 0) {
                // 后面没有 else 了
                finish = true;
            }
        }
    }
}

/**
 * 跳过一个if/else if/else代码块
 * @return 让IP指向下一个 if/else if/else 段开始 或者 false
 */
bool jump_block()
{
    int bracket = 0;
    if (IP->code.find("if") == 0 || IP->code.find("else") == 0) {
        bracket = 1;
        IP++;
        while (bracket != 0) {
            if (IP->code == "}") {
                bracket -= 1;
            }
            else if (IP->code[IP->code.length() - 1] == '{') {
                bracket += 1;
            }
            IP++;
        }
        return true;
    }
    else {
        return false;
    }
}

