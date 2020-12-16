#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <assert.h>
#include <stdlib.h>
#include <vector>
using namespace std;

class Parser {
public:
    vector<string> IR; // 用于记录中间表示
private:
    char buffer[1024];

    /* 在str的当前位置cur之后查找第一个属于delim中的字符的位置，找不到返回-1 */
    int strloc(string str, int cur, string delim) {
        for(int i = cur; i < str.size(); i ++) {
            for(int j = 0; j < delim.size(); j ++) {
                if(str[i] == delim[j]) return i;
            }
        }
        // assert(0);
        return -1;
    }

    /* 将str[i-j]子串截取成新的字符串 */
    string genstr(string str, int i, int j) {
        assert(0 <= i && i <= j && j < str.size());
        string res;
        for(int k = i; k <= j; k ++) {
            res.push_back(str[k]);
        }
        return res;
    }

    bool charcheck(char c) {
        if(33 <= c && c <= 126) return true; // 所有可显示字符

        if('a' <= c && c <= 'z') return true;
        if('A' <= c && c <= 'Z') return true;
        if('0' <= c && c <= '9') return true;
        if(c == '_') return true;
        if(c == '*') return true;
        return false;
    }

    string myi2a(int i) {
        string res;
        while(i) {
            int digit = i % 10;
            res.insert(0, 1, '0' + digit);
            i /= 10;
        }
        return res;
    }
public:
    void parse(string line)
    {
        // 当你不能一下子进行信息的转换时，可以适当借助中间表示，分步骤进行转换。
        /* parse负责将复杂的.tm文件格式化为格式化输入
        * 1. 过滤掉所有注释，=，{，}，空格等字符
        * 2. 每一行或者是字符串，或者是整数
        * 3. 格式如下：
        * Q
        * 2
        * q1
        * q2
        * ...
        * 4. 我觉得还是要记录输入字符集和纸带字符集
        */
        int i = strloc(line, 0, ";"), j;
        if(i >= 0) line.erase(i);
        if(line.size() == 0) return;
        
        i = strloc(line, 0, "#");
        if(i >= 0) {
            j = strloc(line, i, " ");
            string opt = genstr(line, i, j - 1);
            IR.push_back(opt);
            if(opt == "#Q" || opt == "#S" || opt == "#G" || opt == "#F") {
                // 计算总标记数
                int cnt = 0;
                for(int k = 0; k < line.size(); k ++) if(line[k] == ',') cnt ++;
                IR.push_back(myi2a(cnt + 1));
                
                i = strloc(line, j, "{") + 1;
                if(i < 0 || !charcheck(line[i])) assert(0);
                while(i < line.size()) {
                    j = strloc(line, i, ",}");
                    if(j < 0) assert(0);
                    IR.push_back(genstr(line, i, j - 1));
                    i = j + 1;
                    if(line[i] == '\0') break;
                    if(!charcheck(line[i])) {
                        cout << "line[i] is " << (int)line[i] << endl;
                        assert(0);
                    }
                }
            } else if(opt == "#q0" || opt == "#B" || opt == "#N") {
                i = strloc(line, j + 1, " ") + 1;
                if(i < 0 || !char16_t(line[i])) assert(0);
                j = strloc(line, i, " \0\n");
                if(j < 0) j = line.size();
                IR.push_back(genstr(line, i, j - 1));
            } else {
                cout << "非法命令" << endl;
                assert(0);
            }

        } else {
            // 说明是转移函数
            IR.push_back("#delta");
            i = 0;
            while(i < line.size()) {
                if(!charcheck(line[i])) {
                    cout << "line[i] is " << (int)line[i] << endl;
                    assert(0);
                }
                j = strloc(line, i, " \0\n");
                if(j < 0) j = line.size();
                IR.push_back(genstr(line, i, j - 1));
                i = j + 1;
            }
        }

        // cout << line << endl;
    }

    int parser_main(int argc, char *argv[]) {
        if(argc < 3) {
            cout << "usage: turing [-v|--verbose] [-h|--help] <tm> <input>" << endl;
            exit(0);
        }
        
        fstream file;
        for(int i = 1; i < argc - 1; i ++) {
            string opt(argv[i]);
            if(opt[0] == '-') {
                if(opt == "-v" || opt == "--verbose") 
                    IR.push_back("#verbose");
                else if(opt == "-h" || opt == "--help") {
                    cout << "usage: turing [-v|--verbose] [-h|--help] <tm> <input>" << endl;
                    exit(0);
                }
                else {
                    cout << "command error" << endl;
                    cout << "usage: turing [-v|--verbose] [-h|--help] <tm> <input>" << endl;
                    exit(0);
                }
                
            }
            else {
                file.open(argv[i], ios::in);
            }
        }
        IR.push_back("#input");
        IR.push_back(string(argv[argc-1]));
        
        // 打开文件，依次读取每行
        if(!file.is_open()) {
            cerr << "file open failed : " << argv[1] << endl;
            exit(0);
        }
        // 对每行进行解析
        while(file.getline(buffer, sizeof(buffer))) {
            string str(buffer);
            parse(str);
        }
        // 输出中间表示
        // for(int i = 0; i < IR.size(); i ++) cout << IR[i] << endl;

        return 0;
    }

    string next_token() {
        static int i = 0;
        if(i == IR.size()) {
            return "";
        }
        else{
            i ++;
            return IR[i - 1];
        }
    }


};


#endif