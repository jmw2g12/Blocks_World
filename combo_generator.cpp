#include <iostream>
#include <string>

void create_combos(int depth, string line);

int main(int argc, char** argv){
    create_combos(0,"");
}

void create_combos(int depth, string line){
    string line_u =  line;
    line_u.push_back('U');
    string line_l =  line;
    line_l.push_back('L');
    string line_d =  line;
    line_d.push_back('D');
    string line_r =  line;
    line_r.push_back('R');

    if (depth > 5){
        cout << line << '\n';
    }else{
        create_combos(depth + 1, line_u);
        create_combos(depth + 1, line_l);
        create_combos(depth + 1, line_d);
        create_combos(depth + 1, line_r);
    }
}
