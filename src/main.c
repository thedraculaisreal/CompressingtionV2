#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "file_reading.h"

typedef struct {
    char* key;
    size_t value;
} Token;

size_t tokenize(char *buffer, Token **tokens);
Token create_token(char *word);
bool contains_white_space(char* buffer);

int main() {
    const char *file_path = "black.txt";
    char *buffer = read_file_into_mem(file_path);
    Token *tokens = NULL;
    size_t token_count = tokenize(buffer, &tokens);
    for (size_t i = 0; i < token_count; ++i) {
	if (tokens[i].value > 0) {
	    printf("%s:%zu\n", tokens[i].key, tokens[i].value);
	}
    }
    return 0;
}

int is_word_present(Token **tokens, char *buffer, size_t *token_count) {
    for (size_t i = 0; i < (*token_count); ++i) {
	int ret = strcmp((*tokens)[i].key, buffer);
	if (ret == 0) {
	    return (int)i;
	}
    }
    return -1;
}

// turns all words into a token and stores them within an array.
size_t tokenize(char *buffer, Token **tokens) {
    // start at 1 because cant be 0 bytes.
    size_t token_count = 0;
    size_t counter = 0;
    size_t previous_num = 0;
    for (size_t i = 0; i < strlen(buffer); ++i && ++counter) {
	if (is_white_space(buffer[i])) {
	    if (counter > 1) {
		char* new_word = malloc(counter + 1); 
		memcpy(new_word, &buffer[previous_num], counter);
		new_word[counter] = '\0';
		if (contains_white_space(new_word)) {
		    // do nothing
		} else {
		    int ret = is_word_present(tokens, new_word, &token_count);
		    if (ret >= 0) {
			// increment individual token_value
			(*tokens)[ret].value += 1;
		    } else {
			// create new token
			Token new_token = create_token(new_word);
			*tokens = (Token *)realloc(*tokens, (token_count + 1) * sizeof(Token));
			(*tokens)[token_count] = new_token;
			token_count++;
		    }
		}
		counter = 0;
		// start at next word.
		previous_num = i + 1;
	    }
	}
    }
    return token_count;
}

Token create_token(char *word) {
    Token new_token;
    new_token.key = malloc(strlen(word) + 1);
    strcpy(new_token.key, word);
    new_token.value = 0;
    return new_token;
}

bool contains_white_space(char* buffer) {
    for (size_t i = 0; i < (strlen(buffer) - 1) ; ++i) {
	if (buffer[i] == ' ' || buffer[i] == '\n' || buffer[i] == '\r' || buffer[i] == '\t') {
	    return true;
	}
    }
    return false;
}
