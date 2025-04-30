#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define SCREEN_MAX 10
#define FILE_SIZE_MAX 1024 //max file size
#define NAME_SIZE_MAX 50 //max name size
#define DIR_MAX_FILE 100 // max file count that directory can own
#define MAIN_SCREEN_DIR_MAX 24
#define MAIN_SCREEN_FILE_MAX 24
#define DIR_USAGE 4
#define FILE_USAGE 1

/*
 *  directory use size for 4;
 *  file use size 1 for contents size 1
 */
int max_usage = 1024;


struct File {
    char name[NAME_SIZE_MAX];
    char contents[FILE_SIZE_MAX];
    char createTime[50];
};

struct Directory {
    char name[NAME_SIZE_MAX];
    char *files[DIR_MAX_FILE];
    int file_count;
};

struct Main_Screen {
    struct File files[MAIN_SCREEN_FILE_MAX];
    struct Directory dirs[MAIN_SCREEN_DIR_MAX];
    int file_count;
    int dir_count;
};

char* get_current_time();

/*
 *  make directory and save it to target screen
 *  it can be fail  if the max_usage full
 *  if it success return 0
 */
int make_dir_to_screen(struct Main_Screen *screen, char *dir_name) {
    if(max_usage <= 0) {
        return -1;
    }
    struct Directory dir;
    strcpy(dir.name, dir_name);
    dir.file_count = 0;
    int dir_position = screen->dir_count;
    screen->dirs[dir_position] = dir;
    screen->dir_count++;
    max_usage -=  DIR_USAGE;
    return 0;
}

/*
 *  make file and save it to target screen
 *  it can be fail  if the max_usage full
 *  if it success return 0
 */
int make_file_to_screen(struct Main_Screen *screen, char *file_name, char *contents) {
    if(max_usage <= 0) {
        return -1;
    }
    struct File file;
    strcpy(file.name, file_name);
    strcpy(file.contents, contents);
    char *current_time = get_current_time();
    strcpy(file.createTime, current_time);
    int file_position = screen->file_count;
    screen->files[file_position] = file;
    screen->file_count++;
    max_usage -=  FILE_USAGE * strlen(file.contents);
    return 0;
}


/**
 *Usage
 * char *time = get_current_time();
 * get current time
 */
char* get_current_time() {
    static char buffer[100];
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", t);
    return buffer;
}

void display_screen(struct Main_Screen *screen) {
    printf("\n--- SCREEN STATUS ---\n");
    printf("Total Usage Remaining: %d\n", max_usage);

    printf("\n[Directories - %d]:\n", screen->dir_count);
    for (int i = 0; i < screen->dir_count; i++) {
        printf("  - %s (files: %d)\n", screen->dirs[i].name, screen->dirs[i].file_count);
    }

    printf("\n[Files - %d]:\n", screen->file_count);
    for (int i = 0; i < screen->file_count; i++) {
        printf("  - %s | Size: %lu | Created: %s\n",
            screen->files[i].name,
            strlen(screen->files[i].contents),
            screen->files[i].createTime);
    }
    printf("----------------------\n\n");
}


int main(void) {
    struct Main_Screen main_screen = { .file_count = 0, .dir_count = 0 };

    make_dir_to_screen(&main_screen, "Documents");
    make_dir_to_screen(&main_screen, "Images");

    make_file_to_screen(&main_screen, "hello.txt", "Hello World!");
    make_file_to_screen(&main_screen, "image.jpg", "FAKE_IMAGE_DATA");

    display_screen(&main_screen);
}
