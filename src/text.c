#include <text/text.h>
#include <text/macros.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>


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


#define TEXT_REPLACE_ALL_MAX_ITER 1024

char* text_replace_all(const char *str, const char *pattern,
                         const char *repl) {
  if (!str || !pattern || !repl) return 0;

  char* new_str = text_replace(str, pattern, repl);
  if (!new_str) return 0;

  if (strstr(new_str, pattern) != 0) {
    int iterations = 0;
    while (true) {
      char* next_str = text_replace(new_str, pattern, repl);
      if (next_str == 0) {
	return new_str;
      }

      free(new_str);

      if (strstr(next_str, pattern) == 0) {
	return next_str;
      }

      new_str = next_str;

      if (iterations >= TEXT_REPLACE_ALL_MAX_ITER) {
	return new_str;
      }

      iterations++;
    }
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

int text_replace_chars(const char *input_str, TextCharTuple *replacements,
                       int n_replacements, char *out) {
  if (input_str == 0 || replacements == 0 || n_replacements <= 0 || out == 0) return 0;

  int64_t len = strlen(input_str);
  if (len <= 0) return 0;

  for (int64_t i = 0; i < len; i++) {
    char c = input_str[i];
    for (int j = 0; j < n_replacements; j++) {
      TextCharTuple t = replacements[j];
      if (c == t.a) {
        c = t.b;
      }
    }
    out[i] = c;
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

const char *text_view_get_value(TextView *view) {
  if (!view || view->ptr == 0 || view->length <= 0) return 0;
  if (view->length >= TEXT_VIEW_CAP-1) {
    fprintf(stderr, "(text): Not enough space in temporary buffer.\n");
    return 0;
  }
  memset(&view->tmp, 0, TEXT_VIEW_CAP * sizeof(char));
  memcpy(&view->tmp[0], view->ptr, view->length * sizeof(char));
  return view->tmp;
}

void text_view_clear(TextView *view) {
  if (!view) return;
  memset(&view->tmp, 0, TEXT_VIEW_CAP * sizeof(char));
  view->ptr = 0;
  view->length = 0;
}

int text_split(const char* src, char delim, TextView* out, int64_t* out_length) {
  if (!src || !out || !out_length || !delim) return 0;


  TextView current = {0};
  text_view_clear(&current);
  current.ptr = src;

  int64_t len = strlen(src);
  int64_t i = 0;
  char c = src[i];

  int64_t count = 0;

  while (c != 0 && i < len) {
    c = src[i];
    text_view_clear(&current);
    current.ptr = src+MAX(0, i-1);
    while (c != delim && c != 0 && i < len) {
      current.length += 1;
      c = src[i++];
    }
    out[count++] = current;

    while (c == delim) {
      c = src[i++];
    }
  }

  *out_length = count;
  return out_length > 0;
}

int text_truncate(const char* value, const char* ending, int64_t limit, char* out) {
  if (!value || !out) return 0;
  ending = ending ? ending : "...";
  int64_t end_length = strlen(ending);
  int64_t actual_limit = limit - end_length;
  if (actual_limit <= 0) return 0;
  int64_t len = strlen(value);
  if (len <= 0 || len < actual_limit) return 0;
  memcpy(&out[0], &value[0], actual_limit * sizeof(char));
  memcpy(&out[actual_limit], &ending[0], end_length * sizeof(char));
  return 1;
}

int text_suffix(const char *value, const char *suffix, char *out,
                int64_t capacity) {
  if (!value || !out || capacity <= 0) return 0;

  memset(&out[0], 0, capacity * sizeof(char));

  int64_t value_length = strlen(value);
  int64_t suffix_length = suffix == 0 ? 0 : strlen(suffix);

  if (value_length + suffix_length >= capacity) return 0;

  memcpy(&out[0], &value[0], value_length * sizeof(char));

  if (suffix != 0) {
    memcpy(&out[value_length], &suffix[0], suffix_length * sizeof(char));
  }

  return 1;
}

int text_to_lowercase(const char* value, char* out, int64_t length, int64_t capacity) {
  if (value == 0 || out == 0 || length <= 0) return 0;
  if (length > capacity) return 0;

  for (int64_t i = 0; i < length; i++) {
    out[i] = tolower((unsigned char)value[i]);
  }

  return 1;
}

bool text_is_all_digits(const char* value, int64_t length) {
  if (value == 0 || length <= 0) return false;

  for (int64_t i = 0; i < length; i++) {
    if (!isdigit(value[i])) return false;
  }

  return true;
}

TextTokenizer text_tokenizer_init(const char *value, char delim) {
  if (value == 0 || delim == 0) return (TextTokenizer){0};
  int64_t len = strlen(value);
  if (len <= 0) return (TextTokenizer){0};
  return (TextTokenizer){
    .buff = value,
    .delim = delim,
    .i = 0,
    .length = len,
    .token = (TextToken){ .ptr = value, .length = 0 }
  };
}


TextToken *text_tokenizer_next(TextTokenizer *tokenizer) {
  if (tokenizer == 0 || tokenizer->length <= 0 || tokenizer->buff == 0 || tokenizer->delim == 0) return 0;
  TextToken* token = &tokenizer->token;
  token->length = 0;
  token->ptr = 0;

  if (tokenizer->i >= tokenizer->length) return 0;

  char c = tokenizer->buff[tokenizer->i];
  if (c == 0) return 0;

  token->ptr = &tokenizer->buff[tokenizer->i];


  while (tokenizer->i < tokenizer->length && c != 0) {
    c = tokenizer->buff[tokenizer->i++];
    if (c == tokenizer->delim || c == 0) break;
    token->length += 1;
  }

  if (token->ptr == 0 || token->length <= 0) return 0;
  
  return token;
}


static int64_t get_next_lowercase_chunk(const char **buff_ptr, int64_t buff_len, char *out, int64_t capacity) {
  if (buff_ptr == 0) return 0;
  const char* buff = *buff_ptr;
  if (buff == 0 || out == 0 || capacity <= 0 || buff_len <= 0 || buff[0] == 0) return 0;

  int64_t min_len = MIN(buff_len, capacity);
  if (min_len <= 0) return 0;
  
  memset(&out[0], 0, capacity*sizeof(char));
  if (!text_to_lowercase(&buff[0], &out[0], buff_len, capacity)) return 0;

  buff += min_len;

  return buff != 0;
}

#define TEXT_INCLUDES_CAP PATH_MAX

bool text_includes(const char *haystack, const char *needle,
                   bool case_insensitive) {
  if (haystack == 0 || needle == 0) return false;
  if (haystack == needle) return true;
  
  
  if (!case_insensitive) {
    return strstr(haystack, needle) != 0;
  }

  int64_t len_haystack = strlen(haystack);
  int64_t len_needle = strlen(needle);

  if (len_needle > len_haystack || len_needle >= TEXT_INCLUDES_CAP) return false;

  char needle_lower[TEXT_INCLUDES_CAP] = {0};
  if (!text_to_lowercase(needle, needle_lower, len_needle, TEXT_INCLUDES_CAP-1)) return false;
  
  char chunk[TEXT_INCLUDES_CAP] = {0};
  int64_t n_chars_left = strlen(haystack);

  const char* haystack_ptr = &haystack[0];

  while (haystack_ptr != 0 && n_chars_left > 0 && haystack_ptr[0] != 0) {
    int64_t len = strlen(haystack_ptr);
    int64_t min_len = MIN(len, (TEXT_INCLUDES_CAP-1));
    
    if (min_len <= 0) return false;
    memset(&chunk[0], 0, TEXT_INCLUDES_CAP*sizeof(char));
    if (!text_to_lowercase(haystack_ptr, chunk, min_len, TEXT_INCLUDES_CAP-1)) return false;


    if (strstr(chunk, needle_lower) != 0) return true;
    
    haystack_ptr += min_len;
    n_chars_left = len;
  }

  return false;
}

#undef TEXT_INCLUDES_CAP




static int trim_right(char *str) {
  int len = strlen(str);

  while(len > 0 && (isspace(str[len - 1]) || str[len - 1] > 127)) {
    len--;
  }

  str[len] = '\0';

  return 1;
}

int text_trim_right_in_place(char* value) {
  if (!value) return 0;
  trim_right(value);
  return 1;
}

char* text_trim_right(const char* value) {
  if (value == 0) return 0;
  char* new_value = strdup(value);
  if (new_value == 0) return 0;
  if (!text_trim_right_in_place(new_value)) return 0;
  return new_value;
}
