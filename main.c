#include <stdio.h>
#include <string.h>
#include "dir_in_c.h"

int main(void) {
    struct Main_Screen main_screen = { .file_count = 0, .dir_count = 0 };

    while(1) {
        char command[100];
        printf("> ");
        fflush(stdout);
        fgets(command, 100, stdin);
        command[strlen(command) - 1] = '\0';
        execute_by_command(command, &main_screen);
    }

}