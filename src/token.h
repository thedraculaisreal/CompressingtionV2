#include <stdlib.h>

typedef struct {
    char* key;
    size_t value;
    size_t* indexs;
} Token;

size_t tokenize(char *buffer, Token **tokens);
Token create_token(char *word, size_t word_count);
bool is_white_space(char target);
