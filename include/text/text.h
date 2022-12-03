#ifndef TEXT_LIB_H
#define TEXT_LIB_H
#include <stdint.h>

typedef struct {
  char* left;
  char* right;
} TextSlice;


int text_append(char **inputstr, const char *other);


int text_slice_at(const char* str, uint64_t pos, TextSlice* slice);

char* text_replace(const char* str, const char* pattern,
                         const char* repl);

int text_replace_char(char** str, char pattern, char repl);


int text_with_line_numbers(char** str);

int64_t text_count_lines(const char* str);
void text_shift_right(char* str, int64_t len, char c);

#endif
