#include <text/text.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
  char tmp[256];
  memset(&tmp[0], 0, 256*sizeof(char));

  const char* hello = "HELLO_WORLD_123_THIS_IS_A_STRING";

  TextTokenizer tokenizer = text_tokenizer_init(hello, '_');


  TextToken* tok = 0;

  while ((tok = text_tokenizer_next(&tokenizer))) {
    char tmp[256] = {0};
    memcpy(&tmp[0], tok->ptr, tok->length);
    
    printf("%s\n", tmp);
  }

  const char* digit_text = "198278 F"; 
  printf("all_digits: %d\n", text_is_all_digits(digit_text, strlen(digit_text)));
  return 0;
}
