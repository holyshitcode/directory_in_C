#ifndef DIR_IN_C_H
#define DIR_IN_C_H

extern int max_usage;

#define SCREEN_MAX 10
#define FILE_SIZE_MAX 1000 //max file size
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

struct File {
    char name[NAME_SIZE_MAX];
    char contents[FILE_SIZE_MAX];
    char createTime[50];
};

struct Directory {
    char name[NAME_SIZE_MAX];
    struct File *files[DIR_MAX_FILE];
    int file_count;
};

struct Main_Screen {
    struct File *files[MAIN_SCREEN_FILE_MAX];
    struct Directory dirs[MAIN_SCREEN_DIR_MAX];
    int file_count;
    int dir_count;
};

char* get_current_time();
int make_dir_to_screen(struct Main_Screen *screen, char *dir_name);
int make_file_to_screen(struct Main_Screen *screen, char *file_name, char *contents);
int get_dir_from_screen(struct Main_Screen *screen, char *dir_name);
int get_file_from_screen(struct Main_Screen *screen, char *file_name);
int move_file_to_dir(struct Main_Screen *screen, char *dir_name, char *file_name);
char* get_current_time();
void display_screen(struct Main_Screen *screen);
void print_file_content(struct File *file);
struct File *get_file_from_dir(struct Directory *dir, char *file_name);
struct Directory *get_dir_from_screen_by_filename(struct Main_Screen *screen, char *file_name);
int get_file_position_from_directory(struct Directory *dir, char *file_name);
int remove_file_from_screen_directory(struct Directory *target_dir, char *file_name);
int remove_directory_from_screen(struct Main_Screen *screen, char *dir_name);
int remove_file_from_screen(struct Main_Screen *screen, char *file_name);
void read_file_from_screen(struct Main_Screen *screen, char *file_name);
void read_file(struct Main_Screen *screen, struct Directory *directory,  char *file_name);
int write_file_contents(struct File *file, char *contents);
void help_command();
void execute_by_command(char *command, struct Main_Screen *screen);


#endif
