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
    if(max_usage <= 0) return -1;

    struct File *file = malloc(sizeof(struct File));
    strcpy(file->name, file_name);
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
    strcpy(file->contents, contents);
    return 0;
}

void execute_by_command(char *command, struct Main_Screen *screen) {
    char *cmd = strtok(command, " \n");
    char *arg = strtok(NULL, " \n");
    char *target =strtok(NULL, " \n");

    /*
     * mkdir dir_name
     */
    if(strcmp(cmd, "mkdir") == 0) {
        if(arg != NULL) {
            make_dir_to_screen(screen,arg);
        }else {
            printf("Need to write File name");
        }
    }

    /*
     * touch file_name
     */
    if(strcmp(cmd, "touch") == 0) {
        if(arg != NULL) {
            make_file_to_screen(screen,arg,NULL);
        }else {
            printf("Need to write File name");
        }
    }
    /*
     * vim file_name
     * vim dir_name file_name
     */
    if(strcmp(cmd, "vim") == 0) {
        if(arg != NULL && target != NULL) {
            char contents[FILE_SIZE_MAX];
            printf("Please write contents:");
            fgets(contents,FILE_SIZE_MAX,stdin);
            int target_dir_position  = get_dir_from_screen(screen, arg );
            struct Directory *target_dir = &screen->dirs[target_dir_position];
            struct File *file = get_file_from_dir(target_dir,target);
            write_file_contents(file,contents);
        }
        if(arg!= NULL && target == NULL) {
            char contents[FILE_SIZE_MAX] ;
            printf("Please write contents:");
            fgets(contents,FILE_SIZE_MAX,stdin);
            int screen_file_position = get_file_from_screen(screen,arg);
            struct File *file = screen->files[screen_file_position];
            write_file_contents(file,contents);
        }
    }
    /*
     *  mv file_name to_dir_name
     */
    if(strcmp(cmd, "mv") == 0) {
        if(arg!=NULL && target != NULL) {
            move_file_to_dir(screen,target,arg);
        }
    }

    /*
     *  ls
     *  only one arg
     */
    if(strcmp(cmd,"ls") == 0) {
        if(arg == NULL && target == NULL) {
            display_screen(screen);
        }
    }
    /*
     * cat file_name
     */
    if(strcmp(cmd,"cat") == 0) {
        if(arg!=NULL && target == NULL) {
            // int file_position = get_file_from_screen(screen,arg);
            // struct File *file = screen->files[file_position];
            read_file(screen,NULL,arg);
        }
    }

    if(strcmp(cmd,"exit") == 0) {
        if(arg == NULL && target == NULL) {
            printf("Good Bye");
            exit(0);
        }
    }
}


int main(void) {
    struct Main_Screen main_screen = { .file_count = 0, .dir_count = 0 };
    make_dir_to_screen(&main_screen, "Documents");
    make_file_to_screen(&main_screen, "hello.txt", "Hello World!");
    make_file_to_screen(&main_screen, "hello.txt2", "Hello World!");
    move_file_to_dir(&main_screen, "Documents", "hello.txt2");
    remove_file_from_screen(&main_screen, "hello.txt");
    make_file_to_screen(&main_screen, "hello.txt", "Hello World!");
    move_file_to_dir(&main_screen, "Documents", "hello.txt");
    remove_file_from_screen(&main_screen, "hello.txt");
    while(1) {
        char command[100];
        printf("> ");
        fflush(stdout);
        fgets(command, 100, stdin);
        command[strlen(command) - 1] = '\0';
        execute_by_command(command, &main_screen);
    }
    // // directory
    // make_dir_to_screen(&main_screen, "Documents");
    // make_dir_to_screen(&main_screen, "Images");
    //
    // // file
    // make_file_to_screen(&main_screen, "hello.txt", "Hello World!");
    // make_file_to_screen(&main_screen, "image.jpg", "FAKE_IMAGE_DATA");
    // make_file_to_screen(&main_screen, "notes.txt", "Important notes here.");
    //
    // // file move
    // move_file_to_dir(&main_screen, "Documents", "hello.txt");
    // move_file_to_dir(&main_screen, "Images", "image.jpg");
    //
    // // print
    // display_screen(&main_screen);
    // int dir_position = get_dir_from_screen(&main_screen,"Documents");
    // //read file
    // read_file(&main_screen,&main_screen.dirs[dir_position],"hello.txt");
    // read_file(&main_screen,NULL,"image.jpg");
    // read_file(&main_screen,NULL,"notes.txt");
}
