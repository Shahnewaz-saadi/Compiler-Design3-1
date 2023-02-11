#include <bits/stdc++.h>
using namespace std;
const std::set<std::string> key_word_list{"auto", "break", "case", "char", "const",
"continue", "default", "do", "double", "else", "enum", "extern", "float", "for",
"goto", "if", "inline", "int", "long", "register", "restrict", "return", "short",
"signed", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned",
"void", "volatile", "while"};
const std::set<std::string> binary_operator_list{"+", "-", "*", "/", "%", "&", "&&",
"|", "||", "<", "<<", ">", ">>", "=", "==", "^"};
const std::set<std::string> unary_operator_list{"!", "++", "--", "~"};
const std::set<char> double_char_operator_list{'+', '-', '&', '|', '<', '>', '='};
const std::set<char> parenthesis_list{'(', ')', '{', '}', '[', ']'};
const std::set<char> separator{'!', '%', '^', '&', '*', '(', ')', '{', '}', ',', '.',
'/', '<', '>', '?', ':', ';', '-', '=', '+', '[', ']', '~', '\'','\"'};
const std::set<char> ignorable{'\n', ' ', '\t'};
const std::vector<std::string> HEADER_FILE_LIST{"stdio.h", "string.h", "conio.h",
"stdlib.h", "math.h", "ctype.h", "time.h", "assert.h", "locale.h", "signal.h",
"setjmp.h", "stdarg.h", "errno.h", "complex.h", "fenv.h", "float.h", "inttypes.h",
"iso646.h", "limits.h", "setjmp.h", "signal.h", "stdalign.h", "uchar.h", "wchar.h",
"wctype.h"};
void trimmer(std::string &s) {
std::string t;
bool is_ok = false;
for(auto i : s) {
if(!is_ok && i != ' ') {
is_ok = true;
}
if (is_ok) {
t.push_back(i);
}
}
s = t;
t.clear();
is_ok = false;
for(int i = (int)s.length()-1; i >= 0; i--) {
if(!is_ok && s[i] != ' ') {
is_ok = true;
}
if (is_ok) {
t.push_back(s[i]);
}
}
s = t;
std::reverse(s.begin(), s.end());
}
bool is_header_file_correct(const std::string &s) {
bool hasStarted{false};
std::string t;
for (char i : s) {
if(i == '<' && !hasStarted) {
t.clear();
hasStarted = true;
continue;
} else if(i == '>' && hasStarted) {
break;
}
if(hasStarted) {
t.push_back(i);
}
}
trimmer(t);
auto it = std::find(HEADER_FILE_LIST.begin(), HEADER_FILE_LIST.end(), t);
return it != HEADER_FILE_LIST.end();
}
bool check_file_loader(const std::string &s) {
std::regex r(" *# *include *<[!-~]+ *> *");
if(std::regex_match(s, r)) {
return is_header_file_correct(s);
}
return false;
}

bool check_macro(const std::string &s) {
std::regex correct_macro_format(" *# *define +[!-~]+ +[!-~]+[ -~]+");
if(std::regex_match(s, correct_macro_format)) {
return true;
}
return false;
}
// cheak define
bool check_preprocessor(const std::string &s) {
std::regex file_loader_format(" *# *include.*");
std::regex macro_format(" *# *define +.*");
if(std::regex_match(s, file_loader_format)) {
return check_file_loader(s);
}
if(std::regex_match(s, macro_format)) {
return check_macro(s);
}
return false;
}
void enlist(std::vector<std::pair<std::string, std::string>> &record, std::string &s,
const std::string& comment) {
if(s.empty()) {
s.clear();
return;
}
//cheak operators
std::string type;
if(comment == "string" || comment == "char") {
type = "CONSTANTS";
} else if(key_word_list.count(s)) {
type = "KEY_WORD";
} else if(binary_operator_list.count(s)) {
type = "BINARY_OPERATOR";
} else if(unary_operator_list.count(s)) {
type = "UNARY_OPERATOR";
} else {
type = "Identifiers";
}
if(comment == "separator") {
if(parenthesis_list.count(s.at(0))) {
type = "PARENTHESIS";
}
}
record.emplace_back(s, type);
s.clear();
}
//close
//token
std::vector<std::pair<std::string, std::string>>
tokenize(std::vector<std::pair<std::string, int>> &record) {
std::vector<std::pair<std::string, std::string>> tokens;
std::pair<bool, char> string_checker{false, '0'};
std::string s;
for(auto & i : record) {
if(i.first[0] == '#' && !i.first.empty()) {
if(check_preprocessor(i.first)) {
std::cout << "line - " << i.second << " \"" << i.first << "\" is correct" << std::endl;
} else {
std::cout << "error on line - " << i.second << " \"" << i.first << "\""
<< std::endl;
return {}; }
continue;
}

for(int j = 0; j < i.first.size(); j++) {
if(i.first.at(j) == '\"') {
if(!string_checker.first) {
string_checker.first = true;
string_checker.second = i.first.at(j);
} else {
if(string_checker.second != i.first.at(j)) {
std::cout << "error on line - " << i.second << " \"" << i.first
<< "\"" << std::endl;
return {};
} else {
enlist(tokens, s, "string");
string_checker.first = false;
string_checker.second = '0';
}
}
continue;
} if(i.first.at(j) == '\'') {
if(!string_checker.first) {
string_checker.first = true;
string_checker.second = i.first.at(j);
} else {
if(string_checker.second != i.first.at(j)) {
std::cout << "error on line - " << i.second << " \"" << i.first
<< "\"" << std::endl;
return {};
} else {
if(s.size() > 1) {
std::cout << "error on line - " << i.second << " \"" <<
i.first << "\"" << std::endl;
return {};
}
enlist(tokens, s, "char");
string_checker.first = false;
string_checker.second = '0';
}
}
continue;
}
if(string_checker.first) {
s.push_back(i.first.at(j));
continue;
}
if(double_char_operator_list.count(i.first.at(j))) {
enlist(tokens, s, "default");
if(j+1 < i.first.size()) {
if(i.first.at(j+1) == i.first.at(j)) {
s.push_back(i.first.at(j));
s.push_back(i.first.at(j));
enlist(tokens, s, "default");
j += 2;
}
} else {
j++;
}
}
if(j >= i.first.size()) break;
if(separator.count(i.first.at(j)) || ignorable.count(i.first.at(j))) {
enlist(tokens, s, "default");
if(separator.count(i.first.at(j))) {
s.push_back(i.first.at(j));
}
enlist(tokens, s, "separator");
}
else {
s.push_back(i.first.at(j));
}
}
if(string_checker.first) {
s.push_back('\n');
}
else {
enlist(tokens, s, "default");
}
}
return tokens;
}
void comment_remover(std::vector<std::pair<std::string, int>> &record) {
bool is_multi_line{false};
for(auto &i : record) {
std::string s;
for(int j = 0; j < i.first.size(); j++) {
if(!is_multi_line && j+1 < i.first.size() && i.first.at(j) == '/' &&
i.first.at(j+1) == '/') {
break;
}
if(j+1 < i.first.size() && i.first.at(j) == '/' && i.first.at(j+1) == '*'){
is_multi_line = true;
j++;
continue;
}
if(j+1 < i.first.size() && i.first.at(j) == '*'&& i.first.at(j+1) == '/') {
is_multi_line = false;
j++;
continue;
}
if(!is_multi_line) {
s.push_back(i.first.at(j));
}
}
i.first = s;
}
}

void code_cleaner(std::vector<std::pair<std::string, int>> &record) {
comment_remover(record);
std::vector<std::pair<std::string, int>> temp;
for(auto &i : record) {
trimmer(i.first);
if(!i.first.empty()) {
temp.push_back(i);
}
}
record = temp;
}
vector<std::pair<string, int>> get_lines() {
vector<std::pair<string, int>> record;
string s;
int line_number{0};
while (getline(cin, s)) {
line_number++;
record.emplace_back(s, line_number);
}
code_cleaner(record);
return record;
}
vector<std::pair<string, string>> tokenization(vector<pair<string, int>> &record) {
return tokenize(record);
}

//close
int main() {
freopen("test.c", "r", stdin);
string s;
vector<std::pair<string, int>> each_line = get_lines();
for(const auto& i : each_line) {
cout << i.first << " " << endl;
}
for(const auto& i : tokenization(each_line)) {
cout << i.first << ", as : " << i.second << endl;
}
return 0;
}


