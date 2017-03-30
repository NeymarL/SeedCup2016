/**
 * 预处理过程
 */

#include "headers.h"

/**
 * 功能：
 *     1. 获取正确的行数
 *     2. 去掉空行和注释
 *     3. 一行一句排列
 *     4. 若if或loop只有一条语句没写{}，再加上
 *     5. 设置break; 和 } 的属性为 BLOCK_END
 *     6. C主程序最后加上BLOCK_END
 *     7. 设置声明语句
 * 返回值：
 *     vector<Code*> * output;
 *     处理好的代码列表
 */
vector<Code*>* preprocess()
{
    vector<string>* lines = read_file(INPUT_FILE);
    if (lines == NULL) {
        return NULL;
    }
    vector<Code*>* output = new vector<Code*>;
    bool in_for_loop = false;                // 标志这句话是否是for循环条件中的一句
    bool should_add_brackets = false;        // 标志这句话前后是否需要加 {}
    bool in_comment = false;                 // 标志是否在注释块里
    int line_num = 1;
    bool same_line = false;

    for (int i = 0; i < lines->size(); i++) {
        string line = (*lines)[i];
        trim(line);
        // 去掉注释
        discard_comment(line, in_comment);
        // 处理printf
        same_line = handle_printf(output, line, line_num, i, lines, should_add_brackets);
        // 匹配一个合法语句
        regex e("([A-Za-z0-9_\\+\\-\\*/= \t\\(\\)\"%\\>\\<\\!,]*)[;\\{\\}\\)(do)(else)]");
        smatch match;
        while (regex_search(line, match, e)) {
            auto x = match.begin();
            // one line code
            string s = x->str();
            // trim
            trim(s);
            if (fix_else(s, line_num, output)) {
                line = match.suffix().str();
                continue;
            }
            // 处理 { 与 if/else/for/while/do 分开一行写的情况
            if (s == "{") {
                if (should_add_brackets) {
                    (*output)[output->size() - 1]->code += "{";
                    should_add_brackets = false;
                } 
            }

            else if (s.length() != 0 && (s != ";" || in_for_loop)) {
                // 创建一个新的代码节点
                Code* code = new Code;
                code->attribute = EXEC_CODE;
                /* 设置声明语句 */
                if (s.find("int ") == 0 || s.find("int\t") == 0) {
                    if (s.find("=") == -1) {
                        code->attribute = DECLAIR_CODE;
                    }
                    else {
                        code->attribute = INIT_CODE;
                    }
                }
                if (s == "}" || s == "break;") {
                    /* 设置break; 和 } 的属性为 BLOCK_END */
                    code->attribute = BLOCK_END;
                }
                /* 若if或loop只有一条语句没写{}，再加上 */
                /* ---------------------------------------------------- */
                if (s.find("if", 0) == 0 || s.find("else", 0) == 0) {
                    // 判断 { 是否与 if/else 在同一行
                    if (s[s.length() - 1] != '{') {
                        should_add_brackets = true;  
                    }
                    code->attribute = IF_ELSE_CODE;
                } 
                else if (s.find("for", 0) == 0) {
                    // 处理for循环
                    in_for_loop = true;
                    code->attribute = LOOP_CODE;
                } 
                else if (in_for_loop) {
                    if (s.find(";") == -1) {
                        // 如果没有分号，那就是for循环的第三句
                        // 判断 { 是否在同一行
                        if (s[s.length() - 1] != '{') {
                            should_add_brackets = true;  
                        }
                        in_for_loop = false;
                    }
                    else if (s.find(")") != -1) {
                        // for 与循环体写在一行且没加 '{'
                        int pos = s.find_last_of(")");
                        string body = s.substr(pos + 1);
                        s = s.substr(0, pos + 1);
                        s += "{";
                        code->code = s;
                        code->attribute = LOOP_CODE;
                        code->line_number = line_num;
                        output->push_back(code);
                        // loop body
                        trim(body);
                        code = new Code;
                        code->code = body;
                        code->attribute = EXEC_CODE;
                        if (body == "break;") {
                            code->attribute = BLOCK_END;
                        }
                        code->line_number = line_num;
                        output->push_back(code);
                        // }
                        code = new Code;
                        code->attribute = BLOCK_END;
                        s = "}";
                        in_for_loop = false;
                    }
                    else if (s[s.length() - 1] == '{') {
                        in_for_loop = false;
                    }
                } 
                else if (s.find("do", 0) == 0) {
                    // 判断 { 是否在同一行
                    if (s[s.length() - 1] != '{') {
                        should_add_brackets = true;  
                    }
                    code->attribute = LOOP_CODE;
                } 
                else if (s.find("while", 0) == 0 && s[s.length() - 1] != ';') {
                    // 判断 { 是否在同一行
                    if (s[s.length() - 1] != '{') {
                        should_add_brackets = true;  
                    }
                    code->attribute = LOOP_CODE;
                } 
                else if (should_add_brackets) {
                    should_add_brackets = false;
                    // 在前一句后面加 {
                    (*output)[output->size() - 1]->code += "{";
                    code->code = s;
                    code->line_number = line_num;
                    // 加入到代码列表
                    output->push_back(code);
                    // 在下一句加入 }
                    code = new Code;
                    s = "}";
                    code->attribute = BLOCK_END;
                }
                /* ---------------------------------------------------- */
                code->code = s;
                code->line_number = line_num;
                // 加入到代码列表
                output->push_back(code);
            }
            line = match.suffix().str();
        }
        if (!same_line) {
            line_num++;
        }
        else {
            same_line = false;
        }
    }
    // 在主程序最后加上BLOCK_END
    Code* code = new Code;
    code->attribute = BLOCK_END;
    code->code = "}";
    code->line_number = -1;
    output->push_back(code);
    // 释放资源
    delete lines;

#ifdef DEBUG
    // 输出调试信息
    for (int i = 0; i < output->size(); i++) {
        cout << (*output)[i]->line_number << '\t';
        cout << (int)((*output)[i]->attribute) << '\t';
        cout << (*output)[i]->code << endl;
    }
#endif

    return output;
}

/**
 * 单独处理printf
 * 把printf匹配出来，把其他的成分返回
 */
bool handle_printf(vector<Code*>* output, string& line, int& line_number, 
                int i, vector<string>* lines, bool& should_add_brackets)
{
    regex get_printf("printf[ \t]*\\(\"[^\"]*\"[^;]*\\)[^;]*;");
    smatch match;
    string line_copy = line;
    while (regex_search(line_copy, match, get_printf)) {
        auto x = match.begin();
        // one line code
        string s = x->str();
        // 是不是第一个匹配，如果不是先处理前面的语句
        int pos = (int)line_copy.find(s);
        if (pos != 0) {
            string next_line = line_copy.substr(pos);
            line = line_copy.substr(0, pos);
            // 把 next_line 插入到下一句
            lines->insert(lines->begin() + i + 1, next_line);
            return true;
        }
        // trim
        trim(s);

        if (should_add_brackets) {
            (*output)[output->size() - 1]->code += "{";
        }
        // 把printf语句加入列表
        Code* code = new Code;
        code->attribute = EXEC_CODE;
        code->line_number = line_number;
        code->code = s;
        output->push_back(code);

        if (should_add_brackets) {
            should_add_brackets = false;
            Code* code = new Code;
            code->attribute = BLOCK_END;
            code->line_number = line_number;
            code->code = "}";
            output->push_back(code);
        }
        line_copy = match.suffix().str();
    }
    // 把printf去掉，返回剩下的
    line = regex_replace(line, get_printf, "");
    return false;
}

/**
 * 去注释
 * @param in_comment 语句是否在注释中
 */
void discard_comment(string& line, bool& in_comment)
{
    // 匹配 printf("..//..") 或 printf(".../*...") 的情况
    regex comment_in_printf("printf[ \t]*\\(\"(.|\n)*(//|/\\*|\\*/)+[^\"]*\"[^\\);\"]*\\)[^;]*;");
    smatch match_pf;
    if (!regex_search(line, match_pf,comment_in_printf)) {
        if (in_comment) {
            // 匹配注释尾
            regex comment_end("(.|\n)*\\*/");
            if (regex_search(line, comment_end)) {
                line = regex_replace(line, comment_end, "");
                in_comment = false;
            }
            else {
                line = "";
            }
        }
        // 去掉一行内的注释 /* ... */
        regex line_comment1("(/\\*(.|\n)*\\*/)");
        line = regex_replace(line, line_comment1, "");
        // 去掉一行内的注释 // .....
        regex line_comment2("//(.|\n)*");
        line = regex_replace(line, line_comment2, "");
        // 处理一行里只有 '/*' 而没有 '*/' 的情况
        regex comment_begin("(/\\*(.|\n)*)");
        if (regex_search(line, comment_begin)) {
            in_comment = true;
            line = regex_replace(line, comment_begin, "");
        }
    }
    else {
        // 逐字符判断是否为注释
        bool in_quote = false;
        for (int i = 0; i < line.length() - 1; i++) {
            if (line[i] == '/' && line[i + 1] == '/' && !in_quote) {
                // 判断 //... 
                line = line.substr(0, i);
                break;
            }
            else if (line[i] == '/' && line[i + 1] == '*' && !in_quote) {
                // 判断 /* ...
                line = line.substr(0, i);
                in_comment = true;
                break;
            }
            else if (line[i] == '*' && line[i + 1] == '/' && !in_quote && in_comment) {
                // 判断 ... */
                if (i + 2 < line.length()) {
                    line = line.substr(i + 2);
                }
                else {
                    line = "";
                }
                in_comment = false;
                i = -1;
            }
            else if (line[i] == '"') {
                if (!in_quote) {
                    in_quote = true;
                }
                else if (i == 0 || line[i - 1] == '\\') {
                    in_quote = false;
                }
            }
        }
    }
}

/**
 * 处理 else 语句匹配不到的情况
 */
bool fix_else(string& line, int line_number, vector<Code*>* output)
{
    if (line.find("else") == 0 && line[line.length() - 1] == ';') {
        // 插入 else{
        Code * code = new Code;
        code->code = "else{";
        code->attribute = IF_ELSE_CODE;
        code->line_number = line_number;
        output->push_back(code);
        // 插入 else 块内语句
        code = new Code;
        string s = line.substr(4);
        trim(s);
        code->code = s;
        code->attribute = EXEC_CODE;
        code->line_number = line_number;
        output->push_back(code);
        // 插入 }
        code = new Code;
        code->code = "}";
        code->attribute = BLOCK_END;
        code->line_number = line_number;
        output->push_back(code);
        return true;
    }
    return false;
}


