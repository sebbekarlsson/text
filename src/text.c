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

int64_t text_count_lines(const char* str) {
  if (!str) return 0;

  int64_t lines = 0;
  int64_t len = strlen(str);



  for (int64_t i = 0; i < len; i++) {
    char c = str[i];

    lines += (int64_t)(c == '\n');
  }

  return lines;
}


void text_shift_right(char* str, int64_t len, char c) {
  for (int64_t i = len-1; i > 0; i--) {
    str[i] = str[i-1];
  }

  str[0] = c;
}

int64_t text_get_line_length(char* line) {
  if (!line) return 0;
  int64_t line_len = 0;
  int64_t len = strlen(line);


  for (int64_t i = 0; i < len; i++) {
    if (line[i] == '\n') break;
    line_len++;
  }

  return line_len;
}

int text_with_line_numbers(char** str) {
  if (!str) return 0;
  char* value = *str;
  if (!value) return 0;

  int64_t lines = text_count_lines(value);
  if (lines <= 0) return 0;

  char* next_str = 0;
  char* ptr = &value[0];

  for (int64_t i = 0; i < lines; i++) {
    int64_t line_len = text_get_line_length(ptr);

    char line_str[256];
    sprintf(line_str, "%ld ", 1+i);
    text_append(&next_str, line_str);

    if (line_len > 0) {
      char* linebuff = (char*)calloc(line_len + 1, sizeof(char));
      memcpy(&linebuff[0], &ptr[0], line_len * sizeof(char));
      text_append(&next_str, linebuff);
      free(linebuff);
      linebuff = 0;
    }

    text_append(&next_str, "\n");


    ptr += line_len+1;
    if (ptr == 0 || (*ptr) == 0) break;
  }

  if (next_str != 0) {
    free(*str);
    *str = next_str;
  }
  return 1;
}

static bool text_compare_fuzzy_private(const char* a, const char* b) {
  if (a == b) return true;
  if (!a || !b) return false;
  if (strstr(a, b) || strstr(b, a)) return true;
  if (strcasecmp(a, b) == 0) return true;

  size_t a_len = strlen(a);
  size_t b_len = strlen(b);
  size_t len = MAX(a_len, b_len);

  if (len >= 1024) return false;



  int64_t idx = 0;
  char c = a[idx];
  while (idx < a_len && c != 0) {
    while (c == ' ') {
      c = a[++idx];
    }
    char tmp[b_len+1];
    memset(&tmp[0], 0, b_len+1);
    memcpy(&tmp[0], &a[idx], MIN(b_len, a_len-idx));
    if (strcasecmp(tmp, b) == 0) return true;
    c = a[++idx];
  }


  return false;
}

bool text_compare_fuzzy(const char* a, const char* b) {
  if (text_compare_fuzzy_private(a, b)) return true;
  return text_compare_fuzzy_private(b, a);
}
