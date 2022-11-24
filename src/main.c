#include <text/text.h>
#include <stdio.h>

int main(int argc, char* argv[]) {

  char* str = 0;


  text_append(&str, "hello world\n");
  text_append(&str, "Duh!\n");

  printf("%s\n", str);

  return 0;
}
