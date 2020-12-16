#include "tm.h"
#include "common.h"
#include "parse.h"

// vector<string> input = {"1", "0", "0", "1", "0", "0", "1"};
vector<string> input;


int main(int argc, char *argv[]) {
    // 读取已经格式化的输入并生成图灵机
    TM tm;
    Parser parser;
    parser.parser_main(argc, argv);
    
    // for(int i = 0; i < parser.IR.size(); i ++) cout << parser.IR[i] << endl;
    
    string opt, name;
    int n;

    // int cnt = 0;
    
    while(true) {
        opt = parser.next_token();
        if(opt == "") break;
        // cout << "opt is " << opt << endl;
        // cin >> opt;
        // cnt ++;
        if(opt == "#verbose") {
            tm.set_verbose();
        }
        else if(opt == "#input") {
            // cin >> name;
            name = parser.next_token();
            for(int i = 0; i < name.size(); i ++) {
                input.push_back(c2s(name[i]));
            }
            // cout << "input is " << name << endl;
            // cnt ++;
        }
        else if(opt == "#Q") {
            // cin >> n;
            string tmp = parser.next_token();
            // cout << tmp << endl;
            n = s2i(tmp);
            // cout << n << endl;
            // n = s2i(parser.next_token());
            // cnt += n;
            // cout << "states num is " << n << endl;
            while(n -- ) {
                // cin >> name;
                name = parser.next_token();
                tm.add_state(name);
            }
        }
        else if(opt == "#S") {
            // cin >> n;
            n = s2i(parser.next_token());
            // cout << n << "input symbols " << endl;
            // cnt += n;
            while(n--) {
                // cin >> name;
                name = parser.next_token();
                tm.add_inputsym(name);
            }
        }
        else if(opt == "#G") {
            // cin >> n;
            n = s2i(parser.next_token());
            // cout << n << "tape symbols " << endl;
            // cnt += n;
            while(n--) {
                // cin >> name;
                name = parser.next_token();
                tm.add_tapesym(name);
            }

        }
        else if(opt == "#q0") {
            // cin >> name;
            name = parser.next_token();
            tm.set_init_state(name);
            
            // cout << "q0 is" << name << endl;
            // cnt ++; 
        }
        else if(opt == "#B") {
            // cin >> name;
            name = parser.next_token();
            tm.set_blank(name);

            // cout << "blank is " << name << endl;
            // cnt ++;
        }
        else if(opt == "#F") {
            // cin >> n;
            n = s2i(parser.next_token());
            // cout << n << "final state " << endl;
            // cnt += n;
            while(n --) {
                // cin >> name;
                name = parser.next_token();
                tm.set_final_state(name);
            }
        }
        else if(opt == "#N") {
            // cin >> n;
            n = s2i(parser.next_token());
            tm.add_tapes(n);

            // cout << n << "tapes" << endl;
            // cnt ++;
        }
        else if(opt == "#delta") {
            string old_state, new_state;
            vector<string> old_syms, new_syms, directions;
            // // cin >> old_state;
            old_state = parser.next_token();
            // // cin >> old_syms
            //cin >> name;
            name = parser.next_token();
            for(int i = 0; i < name.size(); i ++) {
                old_syms.push_back(c2s(name[i]));
            }
            // // cin >> new_syms
            // cin >> name;
            name = parser.next_token();
            for(int i = 0; i < name.size(); i ++) {
                new_syms.push_back(c2s(name[i]));
            }
            // // cin >> directions
            // cin >> name;
            name = parser.next_token();
            for(int i = 0; i < name.size(); i ++) {
                directions.push_back(c2s(name[i]));
            }
            // cin >> new_state;
            new_state = parser.next_token();
            tm.add_transitem(old_state, new_state, old_syms, new_syms, directions);
            // cnt += 5;
        }
        else {
            cout << "invalid string : " << opt << endl;
            // cout << "cnt is " << cnt << endl;
            assert(0);
        }
    }
    
    // 运行图灵机
    // tm.show_tm_info();
    // vector<string> input = {"1", "0", "0", "A", "1", "A", "0", "0", "1"};
    
    tm.run(input);
    
}
