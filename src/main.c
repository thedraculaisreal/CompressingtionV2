#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <ctype.h>
#include "compression.h"

// https://en.wikipedia.org/wiki/Byte_pair_encoding

typedef struct {
    char pair[3];
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

static char *replace_byte_pair(char *buffer, char *replacement, char *pair, size_t count)
{
    
    char *result = (char *) malloc(strlen(buffer) - count + 1);    

    size_t j = 0;
    for (size_t i = 0; i < strlen(buffer) - 1; ++i)
    {                
        if (strncmp(&buffer[i], pair, 2) == 0)
        {            
            strcpy(&result[j], replacement);            
            j++;
            i++; // skip next char
        }
        else
        {            
            result[j++] = buffer[i];
            result[j] = '\0';
        }
    }
    
    return result;
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
        if (isspace(buffer[i])) continue;
        int index = exists(pair, &pairs, pair_count);

        if (index != -1)
        {            
            pairs[index].count++;
        }
        else
        {            
            pairs = (Pair *) realloc(pairs, sizeof(Pair) * (pair_count + 1));            
            strcpy(pairs[pair_count].pair, pair);
            pairs[pair_count].count = 0;            
            pair_count++;
        }        
    }

    bool complete = false;
    size_t index;
    
    for (size_t i = 0; i < pair_count; ++i)
    {
        for (size_t j = 0; j < pair_count; ++j)
        {
            if (pairs[i].count < pairs[j].count)
            {
                complete = false;
                break;
            }
            else {
                complete = true;
            }
        }
        if (complete)
        {
            index = i;
            break;
        }
    }
        
    char *result = replace_byte_pair(buffer, "X", pairs[index].pair, pairs[index].count);

    printf("%s", result);
    
    return 0;
}
