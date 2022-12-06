#include<cstdio>
#include<malloc.h>
#include<readline/readline.h>
#include<iostream>

using namespace std;

int main() {
    char *line = nullptr;
    while (1) {
        line = readline("enter text: ");
        cout << line << endl;
        free(line);
    }
}

