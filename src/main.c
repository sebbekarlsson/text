#include <text/text.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
  char tmp[256];
  if (!text_suffix("gAlbedo", 0, tmp, 256)) {
    printf("bad\n");
  }
  printf("%s\n", tmp);
  return 0;
}
