#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "token.h"
#include "file_reading.h"

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
    size_t word_count = 0;
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
			// code a bit disgusting but basically we are dynamically allocating memory for our size_t indexs pointer each
			// time the size of the array needs to increase, for that particular token
			(*tokens)[ret].indexs = (size_t *)realloc((*tokens)[ret].indexs, ((*tokens)[ret].value + 1) * sizeof(size_t));
			(*tokens)[ret].indexs[(*tokens)[ret].value] = word_count;
		    } else {
			// create new token
			Token new_token = create_token(new_word, word_count);
			*tokens = (Token *)realloc(*tokens, (token_count + 1) * sizeof(Token));
			(*tokens)[token_count] = new_token;
			
			token_count++;
		    }
		    word_count += 1;
		}
		counter = 0;
		// start at next word.
		previous_num = i + 1;
	    }
	}
    }
    return token_count;
}

Token create_token(char *word, size_t word_count) {
    Token new_token;
    new_token.key = malloc(strlen(word) + 1);
    new_token.indexs = malloc(sizeof(size_t));
    strcpy(new_token.key, word);
    new_token.value = 0;
    new_token.indexs[0] = word_count;
    return new_token;
}

bool contains_white_space(char* buffer) {
    for (size_t i = 0; i < (strlen(buffer) - 1) ; ++i) {
	if (buffer[i] == ' ' || buffer[i] == '\n' || buffer[i] == '\r' || buffer[i] == '\t' || buffer[i] == '?' || buffer[i] == ',' || buffer[i] == '.') {
	    return true;
	}
    }
    return false;
}

