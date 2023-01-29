#include <text/text.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
  TextView views[256] = {0};

  const char* txt = "hello world, my name is john!";

  int64_t count = 0;
  if (text_split(txt, ' ', views, &count)) {
    printf("%ld\n", count);
    for (int64_t i = 0; i < count; i++) {
    
      printf("%s\n", text_view_get_value(&views[i]));
    }
  }


  return 0;
}
