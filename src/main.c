#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <ctype.h>
#include "compression.h"

// https://en.wikipedia.org/wiki/Byte_pair_encoding

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
