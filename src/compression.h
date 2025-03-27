#include <stddef.h>
#ifndef COMPRESSION_H
#define COMPRESSION_H

typedef struct {
    char pair[3];
    size_t count;
} Pair;

typedef struct {
    char pair[3];
    char byte;
} FinalPair;

typedef struct {
    Pair *pairs;
    size_t pair_count;
} PairStruct;

void handle_error(const char *error_string);
void read_file(char *file_path, char **buffer);
int exists(char *pair, Pair** pairs, size_t pair_count);
void find_pairs(char **buffer, PairStruct *pair_struct);
char *replace_byte_pair(char *buffer, char replacement, char *pair, size_t count);
int find_highest_freq(PairStruct *pair_struct);

#endif // COMPRESSION_H
