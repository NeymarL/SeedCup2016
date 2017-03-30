/**
 * code struct
 */

#ifndef CODE_H
#define CODE_H

#include <string>

typedef struct code_struct {
    string code;        // 代码字符串
    int line_number;    // 行数
    // 代码属性，详见const.h
    char attribute;
} Code;

#endif
