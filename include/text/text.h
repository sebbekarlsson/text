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

typedef struct {
  char a;
  char b;
} TextCharTuple;

typedef struct {
  const char* ptr;
  int64_t length;
} TextToken;


const char *text_view_get_value(TextView *view);

void text_view_clear(TextView* view);

int text_split(const char* src, char delim, TextView* out, int64_t* out_length);

int text_append(char **inputstr, const char *other);


int text_slice_at(const char* str, uint64_t pos, TextSlice* slice);

char* text_replace(const char* str, const char* pattern,
                         const char* repl);


char* text_replace_all(const char* str, const char* pattern,
                         const char* repl);

int text_replace_char(char** str, char pattern, char repl);

int text_replace_chars(const char* input_str, TextCharTuple* replacements, int n_replacements, char* out);

int text_with_line_numbers(char** str);

int64_t text_count_lines(const char* str);
void text_shift_right(char* str, int64_t len, char c);

bool text_compare_fuzzy(const char *a, const char *b);


int text_truncate(const char *value, const char *ending, int64_t limit,
                  char *out);

int text_suffix(const char *value, const char *suffix, char *out,
                int64_t capacity);

int text_to_lowercase(const char *value, char *out, int64_t length,
                      int64_t capacity);


bool text_is_all_digits(const char* value, int64_t length);

typedef struct {
  TextToken token;
  char delim;
  int64_t length;
  int64_t i;
  const char* buff;
} TextTokenizer;

TextTokenizer text_tokenizer_init(const char *value, char delim);


TextToken* text_tokenizer_next(TextTokenizer* tokenizer);

#endif
