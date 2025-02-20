#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "compression.h"

int main(int argc, char **argv) {
    if (argc != 3) {
	    printf("%s {path_to_file} {-c or -d}\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    const char *file_path = argv[1];
    const char *option = argv[2];
    if ((strcmp("-c", option)) == 0) {        
        if (!compress_file(file_path)) {
            printf("File compression failed check logs for reasons why!!");
            exit(EXIT_FAILURE);
        }
    } else if ((strcmp("-d", option)) == 0) {
	    if (!decompress_file(file_path)) {
            printf("File decompression failed check logs for reasons why!!");
            exit(EXIT_FAILURE);
        }
    } else {
        
    }
    
    return 0;
}



