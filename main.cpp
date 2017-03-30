/**
 * 主程序逻辑
 */

#include "headers.h"

using namespace std;

int main()
{
    vector<Code*>* codes = preprocess();        // 预处理，详细见preprocess.cpp
    if (!codes) {
        cout << "FILE " << INPUT_FILE << " NOT EXISTS!" << endl;
        return 0;
    }
    /*Code* codes_array = vector_to_array(codes); // 把 vector 转为数组，方便使用指针
    IP = codes_array;                           // 设置IP指向代码段首句话
    GVS.push_table();                           // 把变量表加入到全局变量表堆栈
    do_block();                                 // 处理这段代码
    write_output(false);*/                        // 输出到文件
    return 0;
}

/**
 * 处理一块代码段
 */
void do_block()
{
    bool meet_block_end = false;

    while (!meet_block_end) {

#ifdef DEBUG
        cout << IP->line_number << '\t';
        cout << (int)(IP->attribute) << '\t';
        cout << "|" << IP->code << "|" << endl;
        GVS.print_table();
#endif
        do_output();        // 输出行号，如果需要的话
        // 处理不同情况的代码
        switch (IP->attribute) {
            // handle block end
            case (BLOCK_END) :
                meet_block_end = true;
                do_exit();
                break;

            // handle branch
            case (IF_ELSE_CODE) :
                do_if();
                break;

            // handle loop
            case (LOOP_CODE) :
                do_loop();
                break;

            // handle sequence
            default :
                do_sequence();
                break;
        }
    }
}

/**
 * 释放资源，退出程序段
 */
void do_exit()
{
    GVS.pop_table();  // 弹出最外层变量表
}
