#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "file_reading.h"
#include "token.h"
#include <math.h>

int main(int argc, char **argv) {
    if (argc != 3) {
	    printf("%s {path_to_file} {-c or -d}\n", argv[0]);
    }
    if ((strcmp("-c", argv[2])) == 0) {
	    const char *file_path = argv[1];
	    char *buffer = read_file_into_mem(file_path);
	    Token *tokens = NULL;
	    size_t token_count = tokenize(buffer, &tokens);
	    // allocating enough space in buffer for all the tokens to be written too.
	    char new_buffer[token_count * 150];
        memset(new_buffer, 0, sizeof(new_buffer));
        char write_buffer[1500];
        char index_buffer[100];
	    
	    for (size_t i = 0; i < token_count; ++i) { 
	        memset(write_buffer, 0, sizeof(write_buffer));
	        snprintf(write_buffer, sizeof(write_buffer), "%s: ", tokens[i].key);
            if (tokens[i].value != 0) {
                for (size_t j = 0; j < tokens[i].value; ++j) {
                    memset(index_buffer, 0, sizeof(index_buffer));
                    snprintf(index_buffer, sizeof(index_buffer),"%zu,", tokens[i].indexs[j]);
                    if (strlen(index_buffer) + strlen(write_buffer) < sizeof(new_buffer)) {
                        strcat(write_buffer, index_buffer);
                    } else {
                        printf("Buffer overflow detected in index strcat");
                        exit(EXIT_FAILURE);
                    }                  
                }                
            }
            
            strcat(write_buffer, "\n");
            
            if (strlen(new_buffer) + strlen(write_buffer) < sizeof(new_buffer)) {
                strcat(new_buffer, write_buffer);
            } else {
                printf("Buffer overflow detected\n");
                exit(EXIT_FAILURE);
            }
	    }
	    new_buffer[strlen(new_buffer)] = '\0';
        
	    // create a new file called compressed.rep
	    FILE *fp = fopen("compressed.rep", "w+");
	    if (fp == NULL) {
	        perror("fopen");
	        exit(EXIT_FAILURE);
	    }
        
	    fwrite(new_buffer, sizeof(char), strlen(new_buffer), fp);
        
    } else if ((strcmp("-d", argv[2])) == 0) {
	    // for now all im gonna have for decompression compression still needs indexing
	    // for all the individual words and there locations.
	    FILE *fp = fopen(argv[1], "w");
	    if (fp == NULL) {
	        perror("fopen");
	        exit(EXIT_FAILURE);
	    }
        
	    char *file_buffer = read_file_into_mem(argv[1]);
	    
	    printf("%s", file_buffer);
	    
    } else {
        
    }
    
    return 0;
}

