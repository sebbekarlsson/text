#include <text/text.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
  const char* txt = "InputEvent hello john doe InputEvent InputEvent sarah doe";

  char* replaced = text_replace_all(txt, "InputEvent", "OtherText");

  if (replaced) {
    printf("%s\n", replaced);

    free(replaced);
    replaced = 0;
  }

  return 0;
}
