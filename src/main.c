#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "file_reading.h"
#include "token.h"

int main(int argc, char **argv) {
    if (argc != 3) {
	printf("%s {path_to_file} {-c or -d}", argv[0]);
    }
    if ((strcmp("-c", argv[2])) == 0) {
	const char *file_path = argv[1];
	char *buffer = read_file_into_mem(file_path);
	Token *tokens = NULL;
	size_t token_count = tokenize(buffer, &tokens);
	for (size_t i = 0; i < token_count; ++i) {
	    if (tokens[i].value > 0) {
		printf("%s:%zu\n", tokens[i].key, tokens[i].value);
	    }
	}
    }    
    return 0;
}

