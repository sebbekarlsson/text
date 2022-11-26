#include <text/text.h>
#include <text/macros.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


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

int text_slice_at(const char *str, uint64_t pos,
                          TextSlice *slice) {
  slice->left = 0;
  slice->right = 0;
  if (!str) {
    return 0;
  }
  uint64_t len = strlen(str);
  if (!len) {
    return 0;
  }
  pos = pos % len;

  const char *right = &str[pos];
  if (!right) {
    return 0;
  }
  uint64_t right_len = strlen(right);
  int64_t left_len = len - right_len;


  char *r = strdup(right);
  char* l = 0;

  if (left_len > 0) {
    l = (char *)calloc(left_len + 1, sizeof(char));
    memcpy(&l[0], &str[0], left_len * sizeof(char));
  }

  slice->left = l;
  slice->right = r;

  return 1;
}

char* text_replace(const char *str, const char *pattern,
                         const char *repl) {
  if (!str || !pattern || !repl) return 0;

  char *match = strstr(str, pattern);
  if (!match) return 0;

  int64_t index = MAX(0, match - str);

  TextSlice slice = {0};

  if (!text_slice_at(str, index, &slice)) {
    fprintf(stderr, "Slice error.\n");
    return 0;
  }

  char *new_str = slice.left;


  text_append(&new_str, repl);

  if (slice.right != 0) {
    text_append(&new_str, &slice.right[strlen(pattern)]);
    free(slice.right);
    slice.right = 0;
  }

  return new_str;
}

int text_replace_char(char** str, char pattern, char repl) {
  if (!str || !pattern || !repl) return 0;
  char* value = *str;
  if (!value) return 0;
  int64_t len = strlen(value);
  if (len <= 0) return 0;

  for (int64_t i = 0; i < len; i++) {
    char c = value[i];
    if (c != pattern) continue;
    value[i] = repl;
  }

  return 1;
}
