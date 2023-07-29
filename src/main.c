#include <text/text.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
  char tmp[256];
  memset(&tmp[0], 0, 256*sizeof(char));

  const char* haystack = "hello world, my name is john doe. I like Apples and bananas.wav";
  const char* needle = "apples";


  if (text_includes(haystack, needle, true)) {
    printf("yes\n");
  }
  
  return 0;
}
