#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "dir_in_c.h"


char* get_current_time();

/*
 *  make directory and save it to target screen
 *  it can be fail  if the max_usage full
 *  if it success return 0
 */

int max_usage = 1024;

int make_dir_to_screen(struct Main_Screen *screen, char *dir_name) {
    if(max_usage <= 0 || dir_name == NULL) {
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
    if(max_usage <= 0) return -1;

    struct File *file = malloc(sizeof(struct File));
    strcpy(file->name, file_name);
    strcpy(file->contents, "");
    if(contents != NULL) {
        strcpy(file->contents, contents);
    }
    strcpy(file->createTime, get_current_time());

    screen->files[screen->file_count++] = file;
    max_usage -= FILE_USAGE * strlen(file->contents);
    return 0;
}

/*
 *  find directory from target screen by directory name
 *  and return target_directory position
 *  nothing return -1
 */
int get_dir_from_screen(struct Main_Screen *screen, char *dir_name) {
    int dir_limit = screen->dir_count;

    for(int i = 0; i < dir_limit; i++) {
        if(strcmp(screen->dirs[i].name, dir_name) == 0) {
            return i;
        }
    }
    return -1;
}

/*
 * find file from target screen by file name
 * and return file_position
 * nothing return -1
 */
int get_file_from_screen(struct Main_Screen *screen, char *file_name) {
    int file_limit = screen->file_count;
    for(int i = 0; i < file_limit; i++) {
        if(strcmp(screen->files[i]->name, file_name) == 0) {
            return i;
        }
    }
    return -1;
}

/*
 *  find directory and file by name
 *  and move file from screen to target directory and sort
 *  if success return 0
 */
int move_file_to_dir(struct Main_Screen *screen, char *dir_name, char *file_name) {
    int target_dir_position = get_dir_from_screen(screen, dir_name);
    int target_file_position = get_file_from_screen(screen, file_name);
    if (target_dir_position == -1 || target_file_position == -1) {
        return -1;
    }

    struct Directory *target_dir = &screen->dirs[target_dir_position];
    if (target_dir->file_count >= DIR_MAX_FILE) {
        return -1;
    }

    target_dir->files[target_dir->file_count] = screen->files[target_file_position];
    target_dir->file_count++;

    for (int i = target_file_position; i < screen->file_count - 1; i++) {
        screen->files[i] = screen->files[i + 1];
    }
    screen->file_count--;
    if (screen->file_count == 0) {
        screen->files[screen->file_count] = NULL;
    }

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
        struct Directory *dir = &screen->dirs[i];
        printf("📁 %s (files: %d)\n", dir->name, dir->file_count);
        for (int j = 0; j < dir->file_count; j++) {
            struct File *f = dir->files[j];
            printf("    └── 📄 %s | Size: %lu | Created: %s\n", f->name, strlen(f->contents), f->createTime);
        }
    }

    printf("\n[Files on Main Screen - %d]:\n", screen->file_count);
    for (int i = 0; i < screen->file_count; i++) {
        struct File *f = screen->files[i];
        printf("📄 %s | Size: %lu | Created: %s\n",
            f->name,
            strlen(f->contents),
            f->createTime);
    }
    printf("----------------------\n\n");
}

/*
 * print file's content
 */
void print_file_content(struct File *file) {
    printf("\n====📄 FILE: %s =====\n", file->name);
    printf("Created: %s\n", file->createTime);
    printf("-----------------------------\n");
    printf("%s\n", file->contents);
    printf("=============================\n\n");
}

/*
 * search by file name in the directory
 * if it find return file
 * or return NULL
 */
struct File *get_file_from_dir(struct Directory *dir, char *file_name) {
    for (int i = 0; i < dir->file_count; i++) {
        if (strcmp(dir->files[i]->name, file_name) == 0) {
            return dir->files[i];
        }
    }
    return NULL;
}

/*
 * search directory from the screen by filename
 * if it exists return found directory
 * or return NULL
 */
struct Directory *get_dir_from_screen_by_filename(struct Main_Screen *screen, char *file_name) {
    for (int i = 0; i < screen->dir_count; i++) {
        struct Directory *current_dir = &screen->dirs[i];
        for (int j = 0; j < current_dir->file_count; j++) {
            if (strcmp(current_dir->files[j]->name, file_name) == 0) {
                return current_dir;
            }
        }
    }
    return NULL;
}

int get_file_position_from_directory(struct Directory *dir, char *file_name) {
    for(int i = 0; i < dir->file_count; i++) {
        if(strcmp(dir->files[i]->name, file_name) == 0) {
            return i;
        }
    }
    return -1;
}

/*
 * it is called by remove_file_from_screen
 * running exactly the same
 */
int remove_file_from_screen_directory(struct Directory *target_dir, char *file_name) {
    if (target_dir == NULL) {
        return -1;
    }
    int target_dir_file_limit = target_dir->file_count;
    int remove_file_position = get_file_position_from_directory(target_dir, file_name);
    if (remove_file_position == -1) {
        return -1;
    }
    char *remove_file_contents = target_dir->files[remove_file_position]->contents;
    max_usage += strlen(remove_file_contents);
    free(target_dir->files[remove_file_position]);
    for (int i = remove_file_position; i < target_dir_file_limit; i++) {
        target_dir->files[i] = target_dir->files[i + 1];
    }
    target_dir->file_count--;
    if (target_dir->file_count == 0) {
        target_dir->files[target_dir->file_count] = NULL;
    }
    return 0;
}

int remove_directory_from_screen(struct Main_Screen *screen, char *dir_name) {
    int remove_dir_position = get_dir_from_screen(screen,dir_name);
    if (remove_dir_position == -1) {
        return -1;
    }
    struct Directory *current_dir = &screen->dirs[remove_dir_position];
    for (int j = 0; j < current_dir->file_count; j++) {
        free(current_dir->files[j]);
        max_usage += strlen(current_dir->files[j]->contents);
    }
    for(int i = remove_dir_position; i < screen->dir_count - 1; i++) {
        screen->dirs[i] = screen->dirs[i + 1];
    }
    screen->dir_count--;
    max_usage += DIR_USAGE;
    return 0;
}

/*
 * remove from the screen if it exists in screen then
 * remove it from this function
 * or call remove_file_from_screen_directory
 */
int remove_file_from_screen(struct Main_Screen *screen, char *file_name) {
    int remove_file_position = get_file_from_screen(screen, file_name);
    struct Directory *target_dir = get_dir_from_screen_by_filename(screen, file_name);
    if (remove_file_position == -1) {
       if(remove_file_from_screen_directory(target_dir, file_name) == 0) {
           return 0;
       }
        return -1;
    }
    char *remove_file_content = screen->files[remove_file_position]->contents;
    max_usage +=  strlen(remove_file_content);
    int file_limit = screen->file_count - 1;
    struct File *file = screen->files[remove_file_position];
    free(file);
    screen->file_count--;
    for(int i = remove_file_position; i < file_limit; i++) {
        screen->files[i] = screen->files[i + 1];
    }
    if(screen->file_count == 0) {
        screen->files[0] = NULL;
    }
    return 0;
}

/*
 * read the file from the screen by file name in any of directory
 * if there is no target directory in the screen
 * then search from the screen
 */
void read_file_from_screen(struct Main_Screen *screen, char *file_name) {
    struct Directory *target_dir = get_dir_from_screen_by_filename(screen, file_name);
    if (target_dir == NULL) {
        int target_file_position = get_file_from_screen(screen, file_name);
        if (target_file_position == -1) {
            printf("[SYSTEM] File not found\n");
            return;
        }
        struct File *file = screen->files[target_file_position];
        return print_file_content(file);
    }
    print_file_content(get_file_from_dir(target_dir, file_name));
}

/*
 * read file's contents that in the directory
 * if user don't insert directory then search from the screen
 */
void read_file(struct Main_Screen *screen, struct Directory *directory,  char *file_name) {
    if(directory == NULL) {
        return read_file_from_screen(screen, file_name);
    }
    return print_file_content(get_file_from_dir(directory, file_name));
}

/*
 * write contents into the file
 */
int write_file_contents(struct File *file, char *contents) {
    if(strlen(file->contents) == 0) {
        max_usage -= strlen(contents);
    }else {
        max_usage += strlen(file->contents);
        max_usage -= strlen(contents);
    }
    strcpy(file->contents, contents);
    return 0;
}

/*
 * explain commands
 */
void help_command() {
    printf("\n--- Command Help ---\n");
    printf("This Program has mkdir, touch, vim, mv, cat, rm, ls, exit\n");
    printf("mkdir dir_name\n");
    printf("touch file_name\n");
    printf("vim file_name\n");
    printf("if file exists in directory vim dir_name file_name\n");
    printf("mv file_name to_dir_name\n");
    printf("cat file_name\n");
    printf("rm file_name\n");
    printf("rmdir dir_name\n");


}

void execute_by_command(char *command, struct Main_Screen *screen) {
    char *cmd = strtok(command, " \n");
    if (cmd == NULL) {
        return;
    }
    char *arg = strtok(NULL, " \n");
    char *target =strtok(NULL, " \n");
    bool is_correct = false;

    /*
     * mkdir dir_name
     */
    if(strcmp(cmd, "mkdir") == 0) {
        is_correct = true;
        if(arg != NULL) {
            if(make_dir_to_screen(screen,arg) == 0) {
                printf("[SYSTEM] directory name='%s' created\n", arg);
            }else {
                printf("[SYSTEM] max usage overflowed remaining:%d\n",max_usage );
            }
        }else {
            printf("[SYSTEM] need to write file name\n");
        }
    }

    /*
     * touch file_name
     */
    if(strcmp(cmd, "touch") == 0) {
        is_correct = true;
        if(arg != NULL) {
            if(make_file_to_screen(screen,arg,NULL) == 0) {
                printf("[SYSTEM] file name='%s' created\n", arg);
            }else {
                printf("[SYSTEM] max usage overflowed remaining:%d\n",max_usage );
            }
        }else {
            printf("[SYSTEM] need to write file name\n");
        }
    }
    /*
     * vim file_name
     * vim dir_name file_name
     */
    if(strcmp(cmd, "vim") == 0) {
        is_correct = true;
        if(arg != NULL && target != NULL) {
            char contents[FILE_SIZE_MAX];
            int target_dir_position  = get_dir_from_screen(screen, arg );
            if(target_dir_position == -1) {
                printf("[SYSTEM] directory name=%s not found\n", arg);
                return;
            }
            struct Directory *target_dir = &screen->dirs[target_dir_position];
            struct File *file = get_file_from_dir(target_dir,target);
            if(file == NULL) {
                printf("[SYSTEM] file name=%s in directory name=%s not found\n",target, arg);
                return;
            }
            printf("[SYSTEM]  write contents:");
            fgets(contents,FILE_SIZE_MAX,stdin);
            write_file_contents(file,contents);
            printf("[SYSTEM] file name =%s contents wrote\n",arg);

        }
        if(arg!= NULL && target == NULL) {
            char contents[FILE_SIZE_MAX] ;
            int screen_file_position = get_file_from_screen(screen,arg);
            if(screen_file_position == -1) {
                printf("[SYSTEM] file name=%s not found\n", arg);
                return;
            }
            struct File *file = screen->files[screen_file_position];
            printf("[SYSTEM]  write contents:");
            fgets(contents,FILE_SIZE_MAX,stdin);
            write_file_contents(file,contents);
            printf("[SYSTEM] file name =%s contents wrote\n",arg);
        }
    }
    /*
     *  mv file_name to_dir_name
     */
    if(strcmp(cmd, "mv") == 0) {
        is_correct = true;
        if(arg!=NULL && target != NULL) {
            if(move_file_to_dir(screen,target,arg) == 0) {
                printf("[SYSTEM]  file name=%s moved into directory name=%s\n",arg,target);
            }else {
                printf("[SYSTEM]  move failed check command\n");
            }
        }
    }

    /*
     *  ls
     *  only one arg
     */
    if(strcmp(cmd,"ls") == 0) {
        is_correct = true;
        if(arg == NULL && target == NULL) {
            display_screen(screen);
        }
    }
    /*
     * cat file_name
     */
    if(strcmp(cmd,"cat") == 0) {
        is_correct = true;
        if(arg!=NULL && target == NULL) {
            read_file(screen,NULL,arg);
        }
    }

    if(strcmp(cmd,"exit") == 0) {
        is_correct = true;
        if(arg == NULL && target == NULL) {
            printf("Good Bye");
            exit(0);
        }
    }

    /*
     *  rm file_name
     */
    if(strcmp(cmd,"rm") == 0) {
        is_correct = true;
        if(arg != NULL && target == NULL) {
           if( remove_file_from_screen(screen,arg) == 0) {
               printf("[SYSTEM] file name=%s removed\n", arg);
           }else {
               printf("[SYSTEM] remove failed check command\n");
           }
        }
    }

    if(strcmp(cmd,"help") == 0) {
        is_correct = true;
        help_command();
    }

    /*
     * rmdir dir_name
     */
    if(strcmp(cmd,"rmdir") == 0) {
        is_correct = true;
        if(arg != NULL && target == NULL) {
            if(remove_directory_from_screen(screen,arg) == 0) {
                printf("[SYSTEM] directory name=%s removed\n", arg);
            }else {
                printf("[SYSTEM] remove failed check command\n");
            }
        }
    }
    if(!is_correct) {
        printf("\a[SYSTEM] not found command\n");
    }


}



