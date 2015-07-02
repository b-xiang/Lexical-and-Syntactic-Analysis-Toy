/**
    �ʷ���������.
    ��Դ����token��.

    token���Ĺ���д��bfn�����ļ���.
*/


#include "../base.h"
#include "ctype.h"
#include "Dfa.h"
#include "utilities.h"
using namespace std;

string trim(const string& str);
void split(vector<string>& ret, const string& str, const string& char_set);

/**
    @brief ����bfn�ļ��ڵĹ���token��input�ļ��ڵ�Դ��, �����output�ļ���.
    @return 0��ʾ�ɹ�, 1��ʾʧ��.
*/
int lexer(const char* bfn, const char* input, const char* output){
    ifstream fin_bfn(bfn);
    if(!fin_bfn.is_open()){
        error("can't open ", bfn);
        return 1;
    }
    ifstream fin_input(input);
    if(!fin_input.is_open()){
        error("can't open ", input);
        return 1;
    }
    ofstream fout_output(output);
    if(!fout_output.is_open()){
        error("can't open ", output);
        return 1;
    }

    // �ٴ�ע��, token���Ĺ���д��bfn�ļ���
    map<string, set<string>> reserve_words;
    map<string, string> regular_exps;
    int cnt = 0;
    string line;
    while(getline(fin_bfn, line)){
        ++ cnt;
        if(line.length() == 0) continue;
        if(line[0] == '#') continue; // ����ע����
        if(line[0] != '%' && line[0] != '$') continue; // ֻ�����token��صĹ���

        vector<string> strs;
        split(strs, line, " \t");

        if(strs[0] == "%"){ // %��ͷ��ʾ������
            if(strs.size() < 2) {
                error("line: ", cnt, " : ", line, " is invalid.");
                return 1;
            }
            for(size_t i=2;i<strs.size();++i) reserve_words[strs[1]].insert(strs[i]);
        }else if(strs[0] == "$"){  // $��ͷ��ʾ����ƥ��ʽ
            if(strs.size() != 3){
                error("line: ", cnt, " : ", line, " is invalid.");
                return 1;
            }
            regular_exps[strs[1]] = strs[2];
        }else{
            warning("line: ", cnt, " : ", line, " is invalid.");
        }
    }
    fin_bfn.close();

    // ���������������ʽ
    string reg_exp_str;
    for(auto& r : reserve_words) for(auto& s : r.second){
        if(s=="+" || s=="." || s=="*" || s=="(" || s==")")  reg_exp_str += ("|\\"+s);
        else reg_exp_str += ("|"+s);
    }
    for(auto& r : regular_exps)
        reg_exp_str += ("|"+r.second);
    reg_exp_str = reg_exp_str.substr(1);

    // Ϊÿ�����õ�����ʽ����DFA
    map<string, Dfa*> regular_dfa;
    for(auto& it : regular_exps)
        regular_dfa[it.first] = new Dfa(it.second);

    // ����: ȡ��ĳ��token������
    auto get_type = [&](const string& token){
        for(auto& it : reserve_words)
            if(it.second.count(token) > 0) return it.first;

        for(auto& it : regular_dfa)
            if(it.second->match(token)) return it.first;
    };

    // �����������õ�DFA
    Dfa dfa(reg_exp_str);

    // ��ʼ����ÿ��.
    int line_counter = 0;
    while(!(fin_input.eof() && !dfa.has_next_token())){
        if(!dfa.has_next_token()){
            getline(fin_input, line);
            dfa.set_str(line);
            ++ line_counter;
        }

        string token;
        int ret = dfa.get_next_token_longest(token);
        if(ret == 1) continue; // ��ǰ��һ�н������.
        else if(ret == -1) {  // ���� �������������֮���token.
            error("line: ", line_counter, " is invalid: ", line);
            fout_output<<line_counter<<" "<<"ERROR \""<<line<<"\""<<endl;
            fin_input.close();
            fout_output.close();
            return -1;
        }else{ // �����ǰ��token.
            fout_output<<line_counter<<" "<<get_type(token)<<" "<<token<<endl;
        }
    }

    fin_input.close();
    fout_output.close();

    return 0;
}
