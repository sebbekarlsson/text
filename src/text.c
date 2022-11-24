#include <text/text.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>


int text_append(char **inputstr, const char *other) {
  if (!other) return 0;
  if (!inputstr || !*inputstr) {
    *inputstr = (char *)calloc(2, sizeof(char));
  }
  char *str = *inputstr;

  uint32_t old_len = strlen(str);
  uint32_t other_len = strlen(other);
  uint32_t new_len = old_len + other_len;

  str = (char *)realloc(str, (new_len + 1) * sizeof(char));

  strcat(str, other);
  *inputstr = str;

  return 1;
}
