#include <text/text.h>
#include <stdio.h>

int main(int argc, char* argv[]) {

  char* str = 0;


  text_append(&str, "hello world\n");
  text_append(&str, "Duh!\n");


  char* replaced = text_replace(str, "Duh!\n", "john");

  if (replaced != 0) {
    printf("%s\n", replaced);
  }

  return 0;
}
