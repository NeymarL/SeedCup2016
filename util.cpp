/**
 * 一些通用的工具性函数
 */

#include "headers.h"

using namespace std;

/**
 * 按行读取文件，返回包含代码的列表
 */
vector<string>* read_file(string filename)
{
    ifstream input(filename.c_str());
    if (!input.is_open()) {
        return NULL;
    }
    vector<string>* lines = new vector<string>;
    string line;
    // 逐行读入文件
    while (getline(input, line)) {    
        lines->push_back(line);
    }
    return lines;
}

/**
 * 把行号列表 OUTPUT 输出到文件 OUTPUT_FILE
 * @param  print    是否打印到屏幕
 * @return          写入成功返回true，否则返回false
 */
bool write_output(bool print = false)
{
    ofstream output(OUTPUT_FILE, ios::out|ios::trunc);
    if (output.is_open()) {
        for (int i = 0; i < OUTPUT.size(); ++i) {
            output << OUTPUT[i];
            if (i != OUTPUT.size() - 1) {
                output << ' ';
            }
            if (print) {
                cout << OUTPUT[i] << " ";
            }
        }
        output.close();
        return true;
    }
    else {
        cout << "打开文件失败！请检查是否有足够的权限" << endl;
        return false;
    }
}

/**
 * 去除str两边空白字符
 * @param str
 */
void trim(string& str)
{
    // trim trailing spaces
    size_t endpos = str.find_last_not_of(" \t");
    if (string::npos != endpos ) {
        str = str.substr(0, endpos + 1);
    }
    // trim leading spaces
    size_t startpos = str.find_first_not_of(" \t");
    if (string::npos != startpos ) {
        str = str.substr(startpos);
    }

    if (str == " " || str == "\t") {
        str = "";
    }
}

/**
 * 把 vector 转换成 数组
 */
Code* vector_to_array(vector<Code*>* vec)
{
    int n = vec->size();
    Code* codes_array = new Code[n];
    for (int i = 0; i < n; i++) {
        codes_array[i].code = (*vec)[i]->code;
        codes_array[i].line_number = (*vec)[i]->line_number;
        codes_array[i].attribute = (*vec)[i]->attribute;
    }
    delete vec;
    return codes_array;
}

/**
 * 判断IP指向的语句是否需要输出行号
 * 如果需要就把行号加入OUTPUT列表
 */
void do_output()
{
    // 排除不输出的情况
    if (IP->attribute == DECLAIR_CODE || IP->attribute == OTHER_CODE) {
        // 声明语句不输出
        return;
    }
    if (IP->attribute == IF_ELSE_CODE) {
        // 单行 else 不输出
        regex match_else("else[ \t]*\\{");
        if (regex_match(IP->code, match_else)) {
            return;
        }
    }
    if (IP->attribute == LOOP_CODE) {
        // 单行 do 不输出
        regex match_do("do[ \t]*\\{");
        if (regex_match(IP->code, match_do)) {
            return;
        }
    }
    if (IP->attribute == BLOCK_END && IP->code == "}") {
        // 单独的 } 不输出
        return;
    }
    if (OUTPUT.size() > 0 && IP->line_number == OUTPUT[OUTPUT.size() - 1]) {
        // 相同行号只输出一次
        return;
    }
    // 把行号加入带输出列表
    OUTPUT.push_back(IP->line_number);
}

/**
 * 处理比较表达式
 * @param  code 表达式
 * @return      比较结果
 */
bool handle_compare(string code)
{
    string op;              // 比较符号
    string var;             // 变量名
    string val;             // 值
    int value;              // 变量名对应的值
    bool result = false;    // 比较结果
    // 去除首尾空格
    trim(code);
    int i = 0;
    // 获取变量名
    while (code[i] != ' ' && code[i] != '\t' && !is_compare_op(code[i])) {
        var += code[i];
        i++;
    }
    // 略掉中间空白字符
    while (code[i] == ' ' || code[i] == '\t') {
        i++;
    }
    // 获取比较符号
    while (is_compare_op(code[i])) {
        op += code[i];
        i++;
    }
    // 略掉中间空白字符
    while (code[i] == ' ' || code[i] == '\t') {
        i++;
    }
    // 获取值
    while ((code[i] >= '0' && code[i] <= '9') || code[i] == '-') {
        val += code[i];
        if (i < code.length()) {
            i++;
        }
        else {
            break;
        }
    }
    value = string_to_int(val);
    int left_value = 0;
    if (!GVS.get(var, left_value)) {
        cout << "NOT FOUND VARIABLE " << var << endl;
    }
    // 匹配比较符号进行比较
    if (op == "<") {
        result = left_value < value;
    }
    else if (op == ">") {
        result = left_value > value;
    }
    else if (op == "<=") {
        result = left_value <= value;
    }
    else if (op == ">=") {
        result = left_value >= value;
    }
    else if (op == "==") {
        result = left_value == value;
    }
    else if (op == "!=") {
        result = left_value != value;
    }
    return result;
}

/**
 * 判断c是不是比较操作符
 * @param  c 
 * @return 
 */
bool is_compare_op(const char c)
{
    if (c == '<' || c == '>' || c == '!' || c == '=') {
        return true;
    }
    else {
        return false;
    }
}

/**
 * 判断字符c是否属于合法变量的字符集
 * @param  c [description]
 * @return   [description]
 */
bool is_leagal_var(const char c)
{
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || 
        (c >= '0' && c <= '9') || c == '_') {
        return true;
    }
    else {
        return false;
    }
}

/**
 * 把字符串转化为整数
 * @param  str [description]
 * @return     [description]
 */
int string_to_int(const string& str)
{
    int value = 0;
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == '-') {
            continue;
        }
        value *= 10;
        value += str[i] - '0';
    }
    if (str[0] == '-') {
        value = -value;
    }
    return value;
}

/**
 * 把整数转化为字符串
 * @param  value [description]
 * @return       [description]
 */
string int_to_string(int value)
{
    string res = "";
    if (value < 0) {
        res += "-";
        value = -value;
    }
    else if (value == 0) {
        return "0";
    }
    stack<char> tmp;
    while (value) {
        char c = value % 10 + '0';
        tmp.push(c);
        value /= 10;
    }
    while (tmp.size()) {
        res += tmp.top();
        tmp.pop();
    }
    return res;
}

/**
 * 自己实现的split函数 
 */
vector<string> split(string str, string pattern)
{
    string::size_type pos;
    vector<string> result;
    str += pattern;//扩展字符串以方便操作
    int size = str.size();
    
    for (int i = 0; i < size; i++) {
        pos = str.find(pattern, i);
        if (pos < size) {
            string s = str.substr(i, pos - i);
            result.push_back(s);
            i = pos + pattern.size() - 1;
        }
    }
    return result;
}

/**
 * 计算运算符优先级
 * @param  opr 运算符
 */
int prior(string opr)
{
    if (opr == "+" || opr == "-") {
        return 1;
    }
    else if (opr == "*" || opr == "/") {
        return 2;
    }
    else {
        return 0;
    }
}

/**
 * 判断s t r是否都由数字组成
 * @param  str [description]
 * @return     [description]
 */
bool is_number(const string& str)
{
    for (int i = 0; i < str.length(); i++) {
        if ((str[i] < '0' || str[i] > '9') && (str[i] != '-' || str[i] != '+')) {
            return false;
        }
    }
    return true;
}

/**
* 匹配花括号
* 返回与当前行左花括号匹配的右花括号所在行的行号(code*)
*/
Code* match_bracket()
{
    int bracket = 1;
    Code* tmpIP = IP;
    while (tmpIP->code.find("{") == -1) {
        tmpIP++;
    }
    tmpIP++;
    for (;bracket;tmpIP++) {
        if (tmpIP->code == "}") {
            bracket--;
        }
        else if (tmpIP->code[tmpIP->code.length() - 1] == '{') {
            bracket++;
        }
    }
    return tmpIP;
}
