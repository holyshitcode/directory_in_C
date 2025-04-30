#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define FILE_SIZE_MAX 1024 //max file size
#define NAME_SIZE_MAX 50 //max name size
#define DIR_MAX_FILE 100 // max file count that directory can own

/*
 *  directory use size for 4;
 *  file use size 1 for contents size 1
 */
int max_usage = 1024;

struct File {
    char name[NAME_SIZE_MAX];
    char contents[FILE_SIZE_MAX];
    char *createTime;
};

struct Directory {
    char name[NAME_SIZE_MAX];
    char *files[DIR_MAX_FILE];
};



/**
 *Usage
 * char *time = get_current_time();
 * get current time
 */
const char* get_current_time() {
    static char buffer[100];
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", t);
    return buffer;
}



int main(void) {

    return 0;
}
