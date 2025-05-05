#include <stdio.h>
#include "c_unit.h"
#include "dir_in_c.h"
#include "test_main.h"

int test_make_dir_to_screen() {
  printf("test_make_dir_to_screen\n");
  struct Main_Screen screen = { .file_count = 0, .dir_count = 0 };
  make_dir_to_screen(&screen, "test_dir");
  int dir_position = get_dir_from_screen(&screen, "test_dir");
  assertEqualInt(dir_position,screen.dir_count - 1,null);
  assertEqualString(screen.dirs[dir_position].name,"test_dir",null);
  assertEqualInt(screen.dir_count, 1, null);
  return 0;
}

int test_make_file_to_screen() {
  printf("test_make_file_to_screen\n");
  struct Main_Screen screen = { .file_count = 0, .dir_count = 0 };
  make_file_to_screen(&screen, "test_file", "test");
  int file_position = get_file_from_screen(&screen, "test_file");
  assertEqualInt(file_position,screen.file_count - 1,null);
  assertEqualString(screen.files[file_position]->name,"test_file",null);
  assertEqualString(screen.files[file_position]->contents,"test",null);
  assertEqualInt(screen.file_count, 1, null);
  return 0;
}

int test_make_file_and_move_to_dir() {
  printf("test_make_file_and_move_to_dir\n");
  struct Main_Screen screen = { .file_count = 0, .dir_count = 0 };
  make_file_to_screen(&screen, "test_file", "test");
  make_dir_to_screen(&screen, "test_dir");
  move_file_to_dir(&screen, "test_dir","test_file");
  int dir_position = get_dir_from_screen(&screen, "test_dir");
  struct Directory target_dir = screen.dirs[dir_position];
  struct File *target_file = get_file_from_dir(&target_dir,"test_file");

  assertEqualInt(screen.file_count, 0, null);
  assertEqualInt(target_dir.file_count, 1, null);
  assertEqualString(target_file->name, "test_file",null);
  assertEqualString(target_file->contents, "test",null);

  return 0;
}

int test_remove_file_from_screen() {
  printf("test_remove_file_from_screen\n");
  struct Main_Screen screen = { .file_count = 0, .dir_count = 0 };
  make_file_to_screen(&screen, "test_file", "test");

  remove_file_from_screen(&screen, "test_file");
  read_file(&screen,NULL,"test_file");
  assertEqualInt(screen.file_count, 0, null);

  return 0;
}

int test_remove_file_from_dir() {
  printf("test_remove_file_from_dir\n");
  struct Main_Screen screen = { .file_count = 0, .dir_count = 0 };
  make_file_to_screen(&screen, "test_file", "test");
  make_dir_to_screen(&screen, "test_dir");
  move_file_to_dir(&screen, "test_dir","test_file");
  remove_file_from_screen(&screen, "test_file");

  int target_dir_position = get_dir_from_screen(&screen, "test_dir");
  struct Directory target_dir = screen.dirs[target_dir_position];

  assertEqualInt(target_dir.file_count, 0, null);
  read_file(&screen,NULL,"test_file");
  return 0;
}

int test_write_file() {
  printf("test_write_file\n");
  struct Main_Screen screen = { .file_count = 0, .dir_count = 0 };
  make_file_to_screen(&screen, "test_file", NULL);
  int file_position = get_file_from_screen(&screen, "test_file");
  struct File *file = screen.files[file_position];
  write_file_contents(file,"test");

  assertEqualString(file->contents, "test",null);

  return 0;
}

int total_test() {
  printf("total_test\n");
  test_make_dir_to_screen();
  test_make_file_to_screen();
  test_make_file_and_move_to_dir();
  test_remove_file_from_screen();
  test_remove_file_from_dir();
  test_write_file();
  return 0;
}


