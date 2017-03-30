/**
 * 常量和宏定义
 */

#ifndef CONST_H
#define CONST_H

#define     INPUT_FILE          "input.txt"
#define     OUTPUT_FILE         "output.txt"

/* code attribute */
#define     EXEC_CODE           0   // 可执行语句，可能需输出行号
#define     DECLAIR_CODE        1   // 声明语句，不需声明行号
#define     INIT_CODE           2   // 初始化语句，可能需输出行号
#define     BLOCK_END           3   // 块结束，如果是 break; 则可能需输出行号
#define     OTHER_CODE          4   // 其他不输出行号语句
#define     IF_ELSE_CODE        5   // 分支语句
#define     LOOP_CODE           6   // 循环语句，for, while..

/* EXTERN is defined as extern except in global.cpp */
#define     EXTERN              extern

#endif
