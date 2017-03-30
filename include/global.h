/**
 * 声明全局变量
 */

#ifndef GLOBAL_H
#define GLOBAL_H

/* EXTERN is defined as extern except in global.cpp */
#ifdef  GLOBAL_VARIABLES_HERE
#undef  EXTERN
#define EXTERN
#endif

#include <vector>
#include "variable.h"
#include "code.h"

EXTERN  Variable         GVS;       // 全局处理变量的对象，global variable stack
EXTERN  std::vector<int> OUTPUT;    // 存放要输出行号的列表
EXTERN  Code*            IP;        // 指向带执行代码的指针

#endif
