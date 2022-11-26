#include <text/text.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {


  char* name = strdup("John Doe");

  text_replace_char(&name, 'o', 'Y');

  printf("%s\n", name);


  return 0;
}
