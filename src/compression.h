#include <stdbool.h>
#include <stddef.h>

typedef struct {
    char *key;
    size_t index;
} new_token;

bool compress_file(const char* file_path);
bool decompress_file(const char* file_path);
bool wrong_ext(const char* file_path);
bool split_at_comma(char letter);
new_token create_new_token(char *buffer, size_t num);


