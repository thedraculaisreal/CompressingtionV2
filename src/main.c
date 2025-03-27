#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <ctype.h>
#include "compression.h"

// https://en.wikipedia.org/wiki/Byte_pair_encoding

#define ARRAY_SIZE(a) sizeof(a)/sizeof(a[0]) 

int main(int argc, char **argv)
{        
    char replacements[] = { 'X', 'Y', 'Q', 'R', 'J', 'L', 'P' };

    size_t replacements_size = ARRAY_SIZE(replacements);
    
    FinalPair *final_pairs = NULL;    
    final_pairs = (FinalPair *)malloc(replacements_size * sizeof(FinalPair) + 1);
    
    char *buffer;
    read_file(argv[1], &buffer);
    
    for (size_t i = 0; i < replacements_size; ++i)
    {
        PairStruct pair_struct;
        pair_struct.pairs = NULL;
        pair_struct.pair_count = 0;

        find_pairs(&buffer, &pair_struct);
        
        int index = find_highest_freq(&pair_struct);
        if (index == -1) handle_error("Failed to read index");

        strcpy(final_pairs[i].pair, pair_struct.pairs[index].pair);
        final_pairs[i].byte = replacements[i];
        
        char *result = replace_byte_pair(buffer, replacements[i], pair_struct.pairs[index].pair, pair_struct.pairs[index].count);
        free(buffer);
        buffer = result;
    }

    // assign memory for the appended text
    buffer = (char *)realloc(buffer, strlen(buffer) + (replacements_size * 5));
    strcat(buffer, "\n");

    for (size_t i = 0; i < replacements_size; ++i)
    {
        char buf[10];
        sprintf(buf, "%c=%s\n", final_pairs[i].byte, final_pairs[i].pair);
        strcat(buffer, buf);
    }
            
    FILE* fd = fopen("compressed.rep", "w+");

    fwrite(buffer, sizeof(char), strlen(buffer), fd);    
        
    return 0;
}
