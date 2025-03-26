#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include "compression.h"

typedef struct {
    char pair[3];
    char byte;
} BytePair;

typedef struct {
    char pair[3];
    size_t *index;
    size_t count;
} Pair;

static void handle_error(const char *error_string)
{
    fprintf(stderr, "ERROR: %s", error_string);
    exit(EXIT_FAILURE);
}

static void read_file(char *file_path, char **buffer)
{
    FILE *fd = fopen(file_path, "r");
    
    if (!fd) handle_error(strerror(errno));

    int ret = fseek(fd, 0L, SEEK_END);

    if (ret < 0) handle_error(strerror(errno));

    long size = ftell(fd);

    ret = fseek(fd, 0L, SEEK_SET);

    if (ret < 0) handle_error(strerror(errno));      
    
    (*buffer) = malloc(size + 1);
    (*buffer)[size] = '\0';

    ret = fread((*buffer), sizeof(char), size, fd);

    if (ret == 0) handle_error(strerror(errno));
    
    fclose(fd);
}

static int exists(char *pair, Pair** pairs, size_t pair_count)
{    
    for (size_t i = 0; i < pair_count; ++i)
    {
        if (strcmp(pair, (*pairs)[i].pair) == 0)
        {
            return i;
        }        
    }
    return -1;
}

int main(int argc, char **argv)
{
    char *buffer;
    read_file(argv[1], &buffer);
        
    Pair *pairs = NULL;
    
    size_t pair_count = 0;
    
    for (size_t i = 0; i < strlen(buffer) - 1; ++i)
    {        
        char pair[3];
        pair[2] = '\0';
        memcpy(pair, &buffer[i], 2);
        int index = exists(pair, &pairs, pair_count);

        if (index != -1)
        {            
            pairs[index].count++;
            pairs[index].index = (size_t *) realloc(pairs[index].index, sizeof(size_t) * (pairs[index].count + 1));
            pairs[index].index[pairs[index].count] = i;          
        }
        else
        {            
            pairs = (Pair *) realloc(pairs, sizeof(Pair) * (pair_count + 1));
            pairs[pair_count].index = malloc(sizeof(size_t));
            strcpy(pairs[pair_count].pair, pair);
            pairs[pair_count].count = 0;
            pairs[pair_count].index[pairs[pair_count].count] = i;
            pair_count++;
        }
    }

    for (size_t i = 0; i < pair_count; ++i)
    {        
        printf("%s -> ", pairs[i].pair);
        for (size_t j = 0; j < pairs[i].count; ++j)
        {
            printf("%zu, ",  pairs[i].index[j]);
        }
        printf("\n");
    }

    return 0;
}
