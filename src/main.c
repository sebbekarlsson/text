#include <text/text.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
  char tmp[256];
  memset(&tmp[0], 0, 256*sizeof(char));

  text_replace_chars("hello world this is cool {test}", (TextCharTuple[]){
      {' ', '_'},
      {'{', '-'},
      {'}', '-'}
    }, 3, tmp);
  
  printf("%s\n", tmp);
  
  return 0;
}
