/**
 * 实现KMP算法
 */

#include "headers.h"

int nxt[1000];

/**
 * 以下为KMP算法匹配字符串
 * @param S [description]
 * @param T [description]
 */
void get_next(string S, string T)
{
    int j, k;
    unsigned long tlen = T.length();
    unsigned long slen = S.length();
    j = 0;
    k = -1;
    nxt[0] = -1;
    while (j < tlen){
        if (k == -1 || T[j] == T[k]) {
            nxt[++j] = ++k;
        }
        else {
            k = nxt[k];
        }
    }
}

/**
 * 返回模式串T在主串S中首次出现的位置
 * 返回的位置是从0开始的。
 * @param  S [description]
 * @param  T [description]
 * @return   [description]
 */
int KMP_index(string S, string T)
{
    int i = 0, j = 0;
    unsigned long slen = S.length();
    unsigned long tlen = T.length();
    get_next(S, T);
    
    while (i < slen && j < tlen) {
        if (j == -1 || S[i] == T[j]) {
            i++;
            j++;
        }
        else {
            j = nxt[j];
        }
    }

    if (j == tlen) {
        return i - tlen;
    }
    else {
        return -1;
    }
}

/**
 * 返回模式串在主串S中出现的次数
 * @param  S [description]
 * @param  T [description]
 * @return   [description]
 */
int KMP_count(string S, string T)
{
    int slen = S.length();
    int tlen = T.length();
    int ans = 0;
    int i, j = 0;
    
    if (slen == 1 && tlen == 1) {
        if (S[0] == T[0]) {
            return 1;
        }
        else {
            return 0;
        }
    }
    get_next(S, T);
    
    for (i = 0; i < slen; i++) {
        while (j > 0 && S[i] != T[j]) {
            j = nxt[j];
        }
        if (S[i] == T[j]) {
            j++;
        }
        if (j == tlen) {
            ans++;
            j = nxt[j];
        }
    }
    return ans;
}
