#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include "compression.h"

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

int main(int argc, char **argv)
{
    char *buffer;
    read_file(argv[1], &buffer);
    
    printf("%s", buffer);
    
    return 0;
}

