#include <text/text.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
  const char* txt = "InputEvent hello john doe InputEvent InputEvent sarah doe";
  int64_t len = strlen(txt);
  
  char buff[len];
  memset(&buff[0], 0, len*sizeof(char));

  if (text_truncate(txt, "...", 9, buff)) {
    printf("%s\n", buff);
  }
  
  return 0;
}
