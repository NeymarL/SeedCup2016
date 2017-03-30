/**
* 处理循环结构代码
*/
#include "headers.h"

/**
 * 处理循环结构
 */
void do_loop()
{
    string expression = "";
    string expression1 = "";
    string expression2 = "";
    string expression3 = "";
    Code* blockbegin = IP;
    Code* blockend = match_bracket();
    if (IP->code.find("for") == 0) {
        //处理for循环头部
        int numofline = 1;
        for (; numofline<4; IP++, numofline++) {
            expression = cutexpr(numofline);
            if (numofline == 1) {
                expression1 = expression;
            }
            else if (numofline == 2) {
                expression2 = expression;
            }
            else if (numofline == 3) {
                expression3 = expression;
            }
        }
        if (expression2 == "") {
            expression2 = "1";
        }
    }
    else if (IP->code.find("while") == 0) {
        //处理while循环头部
        expression2 = cutexpr(1);
        IP++;
    }
    else if (IP->code.find("do") == 0) {
        IP++;
        Code *tmpIP = IP;
        GVS.push_table();   // 新建变量表
        do_block();
        if (IP->code.find("break") != -1) {
            IP = blockend;
            return;
        }
        IP++;
        expression2 = cutexpr(1);
        IP = tmpIP;
    }
    Code* tmpIP = IP;
    if (expression1 != "") {
        handle_expression(expression1);
    }
    IP = blockend + 1;
    for (; output(blockbegin), if_continue(expression2);) {
        IP = tmpIP;
        GVS.push_table();
        do_block();
        if (IP->code.find("break") != -1) {
            IP = blockend;
            return;
        }
        if (expression3 != "") {
            handle_expression(expression3);
        }
    }
}

/**
 * 剪切循环判断中的表达式
 * @param  numofline [description]
 * @return           [description]
 */
string cutexpr(int numofline)
{
    bool in_para = false;
    int i = 0;
    string expression;
    for (; IP->code[i] != ')' && IP->code[i] != ';'; i++) {
        //截取表达式
        if (numofline==1) {
            if (IP->code[i] == '(' && in_para == false) {
                in_para = true;
            }
            else if (in_para) {
                expression += IP->code[i];
            }
        }
        else if (numofline > 1) {
            expression += IP->code[i];
        }
    }
    trim(expression);
    if(expression.find("printf") == 0) {
        expression = expression + ")";
    }

    return expression;
}

/**
 * 判断是不是比较表达式
 * @param  exrpression [description]
 * @return             [description]
 */
bool is_compare_exp(string exrpression)
{
    if (exrpression.find(">") == -1 &&
        exrpression.find("<") == -1 &&
        exrpression.find("==") == -1 &&
        exrpression.find("!=") == -1) {
        return false;
    }
    else {
        return true;
    }
}

/**
 * 判断循环是否要继续
 * @param  expression [description]
 * @return            [description]
 */
int if_continue(string expression)
{
    vector<string> list;
    string former_exp = "";
    string value_exp = "";
    int i = 0;
    int res = 0;
    int is_printf = 0;
    if (expression.find("printf") == 0) {
        is_printf = 1;
    }
    if (is_printf == 0) {
        list = split(expression, ",");
    }
    if (list.size() > 1) {
        for (; i < list.size() - 2; i++) {
            former_exp += list[i];
            former_exp+=",";
        }
        former_exp += list[i];
        value_exp = list[list.size() - 1];
        handle_expression(former_exp);
    }
    else{
        value_exp = expression;
    }
    if (is_printf == 0 && is_compare_exp(value_exp)) {
        res = handle_compare(value_exp);
    }
    else {
        if (is_printf == 1) {
            res = do_printf(value_exp);
        }
        else {
            res = handle_expression(value_exp);
        }
    }
    return res;
}

/**
 * 输出循环头
 * @param blockbegin [description]
 */
void output(Code* blockbegin)
{
    Code* tmpIP = NULL;
    tmpIP = IP;
    IP = blockbegin;
    do_output();
    IP = tmpIP;
}
