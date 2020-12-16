#ifndef COMMON_H
#define COMMON_H

/*
* 该头文件封装了一些独立的小型操作
*/
#include <string>
#include <assert.h>
using namespace std;

static string c2s(char c) {
    string str;
    str.push_back(c);
    return str;
}

static int get_width(int i) {
    int res = 0;
    while(i) {
        res ++;
        i /= 10;
    }
    return res;
}

static int s2i(string str) {
    int res = 0;
    for(int i = 0; i < str.size(); i ++) {
        assert(isdigit(str[i]));
        res = res * 10 + (str[i] - '0');
    }
    return res;
}

#endif