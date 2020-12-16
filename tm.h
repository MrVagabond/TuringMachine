#ifndef TM_H
#define TM_H

#include <string>
#include <iostream>
#include <cstring>
#include <vector>
#include <stdlib.h>
#include <assert.h>
#include <iomanip>
#include <stdio.h>
#include "common.h"

using namespace std;

const int TAPE_START = 1024 * 8;

class State
{
public:
    string name;
    bool is_final;
    /* constructor */
    State(string _name)
    {
        name = _name;
        is_final = false;
    }
};

class TransItem
{
public:
    string cur_state, nxt_state;
    vector<string> old_syms;
    vector<string> new_syms;
    vector<string> directions;

    TransItem(string cur, string nxt, vector<string> olds, vector<string> news, vector<string> dir) {
        cur_state = cur, nxt_state = nxt, old_syms = olds, new_syms = news, directions = dir;
    }
    void show_transitem_info() {
        cout << cur_state << " " << nxt_state << " ";
        for(int i = 0; i < old_syms.size(); i ++) cout << old_syms[i] << " ";
        cout << endl;
        for(int i = 0; i < new_syms.size(); i ++) cout << new_syms[i] << " ";
        cout << endl;
        for(int i = 0; i < directions.size(); i ++) cout << directions[i] << " ";
        cout << endl;
    }

};

class Tape
{
private:
    vector<string> tape; // 对于单个纸带还是应该用字符
    int head;
    int h0; // 记录head的初始位置
public:
    /* constructor */
    Tape()
    {
        tape.push_back("_");
        head = 0;
        h0 = 0;
    }
    void set_tape(vector<string> input) {
        tape = input;
        head = 0;
        h0 = 0;
    }
    string get_content() {
        assert(0 <= head && head < tape.size());
        return tape[head];
    }
    void set_content(string str) {
        assert(0 <= head && head < tape.size());
        tape[head] = str;
    }
    void clean() {
        int i = 0;
        while(i < head && tape[i] == "_") {
            head --;
            tape.erase(tape.begin());
        }
        while(head < tape.size() - 1 && tape.back() == "_") tape.pop_back();
    }
    void move(string dir) {
        if(dir == "l") {
            if(head == 0) {
                tape.insert(tape.begin(), "_");
                h0 ++;
            } else head --;
        } else if(dir == "r") {
            if(head == tape.size() - 1) {
                tape.push_back("_");
            }
            head ++;
        } else if(dir == "*") {

        } else {
            cout << "invalid move" << endl;
            return;
        }
    }

    void print_tape_info() {
        for(int i = 0; i < tape.size(); i ++) {
            cout << tape[i] << " ";
        }
        cout << endl;
    }
    void print_tape_info_noBlank() {
        for(int i = 0; i < tape.size(); i ++) {
            if(tape[i] != "_") cout << tape[i];
        }
        cout << endl;
    }
    void print_tape_info_final(int i) {
        // clean();
        // 计算需要打印的区间
        int wid = get_width(tape.size());
        int l = 0, r = tape.size() - 1;
        while(tape[l] == "_" && l < head) l ++;
        while(tape[r] == "_" && r > head) r --;

        printf("Index%d\t: ", i);
        for(int i = l; i <= r; i ++) {
            cout << left << setw(wid) << abs(i - h0) << " ";
        }
        cout << endl;
        // cout << "Tape" << i << " : ";
        printf("Tape%d\t: ", i);
        for(int i = l; i <= r; i ++) {
            cout << left << setw(wid) << tape[i] << " ";
        }
        cout << endl;
        // cout << "Head" << i << " : ";
        printf("Head%d\t: ", i);
        for(int i = l; i <= r; i ++) {
            if(i == head) cout << left << setw(wid) << "^" << " ";
            else cout << left << setw(wid) << " " << " ";
        }
        cout << endl;
    }
    int get_tape_head() {
        return head;
    }
    

};



class TM
{
private:
    vector<State> states;
    int q0;
    vector<Tape> tapes;
    vector<TransItem> transitems;
    vector<string> inputsyms;
    vector<string> tapesyms;
    int blank;
    int cur_state;
    vector<int> tape_head;
    bool verbose;
public:
    // 生成函数
    TM() {
        verbose = false;
    }
    void set_verbose() {
        verbose = true;
    }
    void add_inputsym(string c) {
        inputsyms.push_back(c);
    }
    void add_tapesym(string c) {
        tapesyms.push_back(c);
    }
    void set_blank(string c) {
        int i;
        for(i = 0; i < tapesyms.size(); i ++) {
            if(tapesyms[i] == c) {
                blank = i;
                return;
            }
        }
        assert(i < tapesyms.size());
    }
    void add_state(string name) {
        states.push_back(State(name));
    }
    void add_tapes(int n) {
        for(int i = 0; i < n; i ++) {
            tapes.push_back(Tape());
            tape_head.push_back(0);
        }
    }
    void add_transitem(string old_state, string new_state,
                       vector<string> old_syms, vector<string> new_syms, vector<string> directions) {
        transitems.push_back(TransItem(old_state, new_state, old_syms, new_syms, directions));
    }
    int find_state(string name) {
        for(int i = 0; i < states.size(); i ++) {
            if(name == states[i].name) return i;
        }
        cout << "wrong state name" << endl;
        exit(0);
    }
    void set_final_state(string name) {
        states[find_state(name)].is_final = true;
    }
    void set_init_state(string name) {
        q0 = find_state(name);
        cur_state = q0;
    }
    
    void show_tm_info() {
        cout << "*****************print turing machine info" << endl;
        cout << "state set: ";
        for(int i = 0; i < states.size(); i ++) cout << states[i].name << " ";
        cout << endl;
        cout << "input alphabet: ";
        for(int i = 0; i < inputsyms.size(); i ++) cout << inputsyms[i] << " ";
        cout << endl;
        cout << "tape alphabet: ";
        for(int i = 0; i < tapesyms.size(); i ++) cout << tapesyms[i] << " ";
        cout << endl;
        cout << "delta functions: " << endl;;
        for(int i = 0; i < transitems.size(); i ++) {
            transitems[i].show_transitem_info();
        }
        cout << "blank symbol: " << tapesyms[blank] << endl;
        cout << "initial state: " << states[q0].name << endl;
        cout << "final states: ";
        for(int i = 0; i < states.size(); i ++) {
            if(states[i].is_final) cout << states[i].name << " ";
        }
        cout << endl;
    }

    void snapshot() {
        if(!verbose) return;

        static int cnt = 0;
        cout << "Step\t: " << cnt << endl;
        for(int i = 0; i < tapes.size(); i ++) {
            tapes[i].print_tape_info_final(i);
        }
        cout << "State\t: " << states[cur_state].name << endl;
        cnt ++;
        cout << "---------------------------------------------" << endl;
    }

    // 运行函数
    bool checkinput(string c) {
        for(int i = 0; i < inputsyms.size(); i ++) {
            if(c == inputsyms[i]) return true;
        }
        return false;
    }

    int lookup_transitem() {
        for(int i = 0; i < transitems.size(); i ++) {
            if(transitems[i].cur_state == states[cur_state].name) {
                bool flag = true;
                assert(transitems[i].old_syms.size() == tapes.size());
                for(int j = 0; j < transitems[i].old_syms.size(); j ++) {
                    if(transitems[i].old_syms[j] != tapes[j].get_content()) {
                        flag = false;
                    }
                }
                if(flag) return i;
            }
        }
        return -1;
    }
    
    // 根据transitems[i]来更新图灵机的状态
    void update(int i) {
        assert(0 <= i && i < transitems.size());
        cur_state = find_state(transitems[i].nxt_state);
        assert(transitems[i].new_syms.size() == tapes.size());
        for(int j = 0; j < transitems[i].new_syms.size(); j ++) {
            tapes[j].set_content(transitems[i].new_syms[j]);
            tapes[j].move(transitems[i].directions[j]);
        }
    }
    bool step_once() {
        // 根据cur_stata、tape_head[]、sym查找转移函数
        int i = lookup_transitem();
        if(i < 0) return false;
        else {
            update(i);
            snapshot();
            if(states[cur_state].is_final) return false; // 结束状态需要停止
            return true;
        }
    }
    void run(vector<string> input) {
        for(int i = 0; i < input.size(); i ++) {
            if(!checkinput(input[i])) {
                cout << "==================== ERR ====================" << endl;
                cout << "error: '" << input[i] << "' was not declared in the set of input symbols" << endl;
                cout << "Input: ";
                for(int i = 0; i < input.size(); i ++) cout << input[i];
                cout << endl;
                cout << "==================== END ====================" << endl;
                return;
            }
        }
        // 将input写在第0条纸带上
        // cout << "==================== RUN ====================" << endl;
        tapes[0].set_tape(input);
        snapshot();
        while(step_once()) ;
        // cout << "Result: ";
        tapes[0].print_tape_info_noBlank();
        // for(int i = 0; i < tapes.size(); i ++) {
        //     cout << "Result: ";
        //     tapes[i].print_tape_info_noBlank();
        // }
        // cout << "==================== END ====================" << endl;
    }

};



#endif