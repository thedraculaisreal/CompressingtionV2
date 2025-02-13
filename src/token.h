#include <stdlib.h>

typedef struct {
    char* key;
    size_t value;
} Token;

size_t tokenize(char *buffer, Token **tokens);
Token create_token(char *word);
bool contains_white_space(char* buffer);
