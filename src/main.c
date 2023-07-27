#include <text/text.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
  char tmp[256];
  memset(&tmp[0], 0, 256*sizeof(char));

  const char* hello = "HELLO_WORLD_123";

  text_to_lowercase(hello, tmp, strlen(hello), 256-1);

  printf("%s\n", tmp);
  
  
  return 0;
}
