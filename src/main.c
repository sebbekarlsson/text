#include <text/text.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
  char tmp[256];
  memset(&tmp[0], 0, 256*sizeof(char));

  const char* stuff = ("hello this is           something               ");

  char* trimmed =text_trim_right(stuff, ' ');

  printf("(%s)\n", trimmed);

  free(trimmed);
  return 0;
}
