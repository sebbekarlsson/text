#include <text/text.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {

  printf("%s\n", text_compare_fuzzy("hello my name is john doe", "Doe") ? "yes" : "no");


  return 0;
}
