#include <text/text.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
  char tmp[256];
  memset(&tmp[0], 0, 256*sizeof(char));

  char stuff[] = ("hello this is           something               ");

  text_trim_right_in_place(stuff);

  printf("(%s)\n", stuff);

  return 0;
}
