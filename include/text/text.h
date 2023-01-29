#ifndef TEXT_LIB_H
#define TEXT_LIB_H
#include <stdint.h>
#include <stdbool.h>

typedef struct {
  char* left;
  char* right;
} TextSlice;

#define TEXT_VIEW_CAP 128

typedef struct {
  char tmp[TEXT_VIEW_CAP];
  const char* ptr;
  int64_t length;
} TextView;

const char *text_view_get_value(TextView *view);

void text_view_clear(TextView* view);

int text_split(const char* src, char delim, TextView* out, int64_t* out_length);

int text_append(char **inputstr, const char *other);


int text_slice_at(const char* str, uint64_t pos, TextSlice* slice);

char* text_replace(const char* str, const char* pattern,
                         const char* repl);

int text_replace_char(char** str, char pattern, char repl);


int text_with_line_numbers(char** str);

int64_t text_count_lines(const char* str);
void text_shift_right(char* str, int64_t len, char c);

bool text_compare_fuzzy(const char* a, const char* b);

#endif
