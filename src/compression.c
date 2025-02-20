#include "compression.h"
#include "file_reading.h"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

bool compress_file(const char* file_path) {
    char *buffer = read_file_into_mem(file_path);
	Token *tokens = NULL;
	size_t token_count = tokenize(buffer, &tokens);
	// allocating enough space in buffer for all the tokens to be written too.
	char new_buffer[token_count * 500];
    memset(new_buffer, 0, sizeof(new_buffer));
    char write_buffer[1000];
    char index_buffer[20];
	
	for (size_t i = 0; i < token_count; ++i) { 
	    memset(write_buffer, 0, sizeof(write_buffer));
	    snprintf(write_buffer, sizeof(write_buffer), "%s: %zu", tokens[i].key, tokens[i].value);
        
        if (tokens[i].value != 0) {
            for (size_t j = 0; j < tokens[i].value; ++j) {
                memset(index_buffer, 0, sizeof(index_buffer));
                snprintf(index_buffer, sizeof(index_buffer),"%zu,", tokens[i].indexs[j]);
                if (strlen(index_buffer) + strlen(write_buffer) < sizeof(new_buffer)) {
                    strcat(write_buffer, index_buffer);
                } else {
                    printf("Buffer overflow detected in index strcat");
                    return false;
                }
            }                
        }
        
        strcat(write_buffer, "\n");
        
        if (strlen(new_buffer) + strlen(write_buffer) < sizeof(new_buffer)) {
            strcat(new_buffer, write_buffer);
        } else {
            printf("Buffer overflow detected\n");
            return false;
        }
	}
	new_buffer[strlen(new_buffer)] = '\0';
    
	// create a new file called compressed.rep
	FILE *fp = fopen("compressed.rep", "w+");
	if (fp == NULL) {
	    perror("fopen");
	    return false;
	}
    
	fwrite(new_buffer, sizeof(char), strlen(new_buffer), fp);
    return true;
}

bool decompress_file(const char* file_path) {
    if (wrong_ext(file_path)) {
        printf("Wrong file extension for decompression, msut be a .rep file");
        return false;
    }
	char *buffer = read_file_into_mem(file_path);
	FILE *fp = fopen(file_path, "w");
	if (fp == NULL) {
	    perror("fopen");
	    return false;
	}
    
	printf("%s", buffer);
    return true;
}

bool wrong_ext(const char* file_path) {
    return true;
}
