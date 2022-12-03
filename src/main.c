#include <text/text.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {

    FILE *fp = fopen(argv[1], "r");
  char *buffer = NULL;
  size_t len;
  ssize_t bytes_read = getdelim(&buffer, &len, '\0', fp);
  if (bytes_read == -1) {
    return 0;
  }

  fclose(fp);





  text_with_line_numbers(&buffer);

  printf("%s\n", buffer);



  return 0;
}
