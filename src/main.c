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

typedef struct {
    Pair *pairs;
    size_t pair_count;
} PairStruct;

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

static void find_pairs(char **buffer, PairStruct *pair_struct)
{
    for (size_t i = 0; i < strlen(*buffer) - 1; ++i)
    {        
        char pair[3];
        pair[2] = '\0';
        memcpy(pair, &(*buffer)[i], 2);
        if (isspace((*buffer)[i])) continue;
        int index = exists(pair, &pair_struct->pairs, pair_struct->pair_count);

        if (index != -1)
        {            
            pair_struct->pairs[index].count++;
        }
        else
        {            
            pair_struct->pairs = (Pair *) realloc(pair_struct->pairs, sizeof(Pair) * (pair_struct->pair_count + 1));            
            strcpy(pair_struct->pairs[pair_struct->pair_count].pair, pair);
            pair_struct->pairs[pair_struct->pair_count].count = 0;            
            pair_struct->pair_count++;
        }        
    }
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

static int find_highest_freq(PairStruct *pair_struct)
{
    bool complete = false;    
    
    for (size_t i = 0; i < pair_struct->pair_count; ++i)
    {
        for (size_t j = 0; j < pair_struct->pair_count; ++j)
        {
            if (pair_struct->pairs[i].count < pair_struct->pairs[j].count)
            {
                complete = false;
                break;
            }
            else
            {
                complete = true;
            }
        }
        if (complete)
        {            
            return i;
            break;
        }
    }
    
    return -1;
}

int main(int argc, char **argv)
{
    char *replacements[] = { "X", "Y", "Q", "R", "J", "L", "P" };
    
    char *buffer;
    read_file(argv[1], &buffer);
    
    for (size_t i = 0; i < 7; ++i)
    {
        PairStruct pair_struct;
        pair_struct.pairs = NULL;
        pair_struct.pair_count = 0;

        find_pairs(&buffer, &pair_struct);
        
        int index = find_highest_freq(&pair_struct);
        if (index == -1) handle_error("Failed to read index");
        
        char *result = replace_byte_pair(buffer, replacements[i], pair_struct.pairs[index].pair, pair_struct.pairs[index].count);
        free(buffer);
        buffer = result;
    }            
    
    printf("%s", buffer);
    
    return 0;
}
