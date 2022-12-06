#include<cstdio>
#include<malloc.h>
#include<readline/readline.h>
#include<iostream>

using namespace std;

int main() {
    char *line = nullptr;
    while (1) {
        //line = readline("enter text: ");
        line = readline("\033[31menter text: \033[0m");
        rl_clear_message();
        //line = readline("\e[31menter text: \e[0m");
        cout << line << endl;
        free(line);
    }
}

