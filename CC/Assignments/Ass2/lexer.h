#ifndef FUNC_H
#define FUNC_H

#include <stdio.h>

#define MAX_TOKEN_SIZE 100

typedef enum {
    KEYWORD,
    IDENTIFIER,
    OPERATOR,
    CONSTANT,
    SYMBOL,
    PREPROCESSOR_DIRECTIVE,
    STRING_LITERAL,
    UNKNOWN
} TokenType;

void tokenize(FILE *file);

#endif 