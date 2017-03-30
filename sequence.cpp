/**
 * 处理顺序结构代码
 */

#include "headers.h"

/**
 * 处理顺序语句
 */
void do_sequence()
{
    // 判断是否为printf
    if (IP->code.find("printf") == 0) {
        do_printf(IP->code);
    }
    else {
        handle_expression(IP->code);
    }
    // 指向下一句
    IP++;
}


/**
 * 计算表达式的值
 * @param  str 表达式
 * @return     [description]
 */
int calculate(string str)
{
    stack<string> opr;
    stack<int> num;
    str.append("#!");
    char op; 
    int a = 0, b = 0, i = 0;
    opr.push("#");
    trim(str);

    for (; str[i] != '!'; i++) {
        string c;
        c.insert(c.begin(), str[i]);
        // 处理正负号
        if (c == "-" || c == "+") {
            string tmp = str.substr(0, i);
            trim(tmp);
            if (tmp != "") {
                tmp = tmp.substr(tmp.length() - 1, tmp.length());
            }
            if (i == 0 || prior(tmp)) {
                while (str[i + 1] == ' ' || str[i + 1] == '\t') {
                    i++;
                }
                string number;
                number.append(1, c[0]);
                for (; str[i + 1] >= '0' && str[i + 1] <= '9'; i++) {
                    number.append(1, str[i + 1]);
                }
                num.push(string_to_int(number));
                continue;
            }
        }
        if (c == "+" || c == "-" || c == "*" || c == "/" || c == "#") {
            while (prior(c) <= prior(opr.top())) {
                if (opr.top() == "#") {
                    break;
                }
                b = num.top(); num.pop();
                a = num.top(); num.pop();
                op = opr.top().at(0); 
                opr.pop();
                switch (op)
                {
                    case '+':
                        a = a + b; 
                        break;
                    case '-':
                        a = a - b; 
                        break;
                    case '*':
                        a = a * b; 
                        break;
                    case '/':
                        a = a / b; 
                        break;
                    default:
                        break;
                }
                num.push(a);
            }
            opr.push(c);
        }
        else if (str[i] >= '0' && str[i] <= '9') {
            string number;
            number.append(1,str[i]);
            for (; str[i + 1] >= '0' && str[i + 1] <= '9'; i++) {
                number.append(1, str[i + 1]);
            }
            num.push(string_to_int(number));
        }
        else {
            continue;
        }
    }
    a = num.top();
    return a;
}

/**
 * 处理赋值、表达式、自增和自减
 * @param  exp [description]
 * @return     [description]
 */
int handle_expression(string& exp)
{
    int if_int = 0; //判断是否声明变量
    int index = 0;
    vector<string> v;
    
    string int_judge = exp.substr(0, 3);
    
    if (!int_judge.compare("int")) {
        if_int = 1;
    }
    //分割逗号
    int st,nd;
    int comma_cnt;
    
    if (if_int) {
        v = split(exp.substr(3), ",");
    }
    else {
        v = split(exp, ",");
    }
    vector<string> vv;
    map<char, int> token;
    token.clear();
    token['+'] = 1;
    token['-'] = 1;
    token['*'] = 1;
    token['/'] = 1;

    for (int i = 0; i < v.size(); i++) {
        trim(v[i]);
        string expression = v[i];
        if (expression.find("=") == -1 && expression.find("++") == -1 && expression.find("--") == -1) {
            // 单独处理
            int res = short_expression(expression);
            if (i == v.size() - 1) {
                return res;
            }
            continue;
        }
        if (expression.find("++") != -1 || expression.find("--") != -1) {
            // 单独处理自增，自减
            int res = add_self(expression);
            if (i == v.size() - 1) {
                return res;
            }
            continue;
        }
        vv = split(v[i], "=");
        if (vv.size() >= 2) {
            int index1 = 0;
            int index2 = 0;
            int is_to_cal = vv.size() - 1;
            string cur_ans;
            trim(vv[is_to_cal]);
            // 计算子表达式
            int res = short_expression(vv[is_to_cal]);
            for(int l = is_to_cal - 1; l >= 0; l --) {
                trim(vv[l]);
                if(if_int) {
                    GVS.set(vv[l], res);
                }
                else {
                    GVS.update(vv[l], res);
                }
            }
                if (i == v.size() - 1) {
                    return res;
                }
        }
        else {
            cout << "What the fuck ?!!!!" << endl;
        }
    }
    return 0;
}

/**
 * 处理不影响变量值的表达式
 * @param  exp [description]
 * @return     [description]
 */
int short_expression(string& exp)
{
    trim(exp);
    string replaced = "";   // 进行变量替换过的表达式
    int i = 0;
    while (i < exp.length()) {
        string token = "";
        while (i < exp.length() && exp[i] != ' ' && exp[i] != '\t' && exp[i] != ';'
            && exp[i] != '+' && exp[i] != '-' && exp[i] != '*' && exp[i] != '/') {
            token += exp[i++];
        }
        if (is_number(token)) {
            replaced += token;
        }
        else if (token.length() != 0) {
            trim(token);
            int value;
            GVS.get(token, value);
            replaced += int_to_string(value);
        }
        if (exp[i] == '+' || exp[i] == '-' || exp[i] == '*' || exp[i] == '/') {
            // 这个token是 + - * /
            replaced += exp[i];
        }
        i++;
    }
    return calculate(replaced);
}

/**
 * 处理自增，自减
 * @param  s [description]
 * @return   [description]
 */
int add_self(string s)
{
    trim(s);
    int index;
    string val_name;
    int flag = 0;   //1 add; 2 sub
    for(int i = 0; i < s.length(); i ++) {
        if(s[i] == '+') {
            flag = 1;
            val_name = s.substr(0,i);
            trim(val_name);
            break;
        }
        else if(s[i] == '-') {
            flag = 2;
            val_name = s.substr(0,i);
            trim(val_name);
            break;
        }
    }

    int res;
    GVS.get(val_name, res);
    if(flag == 1) {
        GVS.update(val_name, res + 1);
    }
    else if(flag == 2) {
        GVS.update(val_name, res - 1);
    }
    return res;
}

/**
 * 处理printf
 * @param  s [description]
 * @return   [description]
 */
int do_printf(string s)
{
    int first_quota = 0;
    int last_quota = 0;
    int st = 0;
    int length = 0;
    int out_cnt = 0;
    string temp_name;
    queue<string> val_name;

    for (int i = 0; i < s.length(); i++) {
        if (s[i] == '\"') {
            first_quota = i;
            break;
        }
    }

    for (int i = first_quota + 1; i < s.length(); i++) {
        if (s[i] == '\"') {
            last_quota = i;
            break;
        }
    }

    for (int i = first_quota + 1; i < last_quota; i++) {
        length++;
        if (s[i] == '%' && s[i + 1] == 'd') {
            out_cnt++;
        }
    }

    for (int i = last_quota + 1; i < s.length(); i++) {
        if (s[i] == ',') {
            st = i + 1;
            break;
        }
    }
    int sta = st;
    for (int i = st; i < s.length(); i++) {
        if (s[i] == ',' || s[i] == ')') {
            temp_name = s.substr(sta, i - sta );
            trim(temp_name);
            // int add_flag = KMP_index(temp_name, "++");
            int add_flag = temp_name.find("++");
            // int sub_flag = KMP_index(temp_name, "--");
            int sub_flag = temp_name.find("--");
            if(add_flag != -1) {
                add_self(temp_name);
                temp_name = temp_name.substr(0, add_flag);
            }
            else if(sub_flag != -1) {
                add_self(temp_name);
                temp_name = temp_name.substr(0, sub_flag);
            }
            val_name.push(temp_name);
            sta = i + 1;
        }
    }

    while (out_cnt--) {
        string cur_name;
        if (!val_name.empty()) {
            cur_name = val_name.front();
            val_name.pop();
        }
        int cur_val = 0;
        string cur_val_str;
        int cur_len = 0;
        GVS.get(cur_name, cur_val);
        stringstream ss;
        ss << cur_val;
        ss >> cur_val_str;
        cur_len = cur_val_str.length();
        length = length - 2 + cur_len;
    }
    return length;
}
