#include <ctype.h>
#include <string.h>
#include "lexer.h"

const char *keywords[] = {
    "int", "float", "double", "char", "return", "if", "else", "while", "for", "void"
};

int is_keyword(const char *token) {
    for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); ++i) {
        if (strcmp(token, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int is_operator(const char *token) {
    const char *operators = "+-*/%&|^~<>!=";
    return strchr(operators, token[0]) != NULL;
}

const char *token_type_to_string(TokenType type) {
    switch (type) {
        case KEYWORD: return "Keyword";
        case IDENTIFIER: return "Identifier";
        case OPERATOR: return "Operator";
        case CONSTANT: return "Constant";
        case SYMBOL: return "Symbol";
        case PREPROCESSOR_DIRECTIVE: return "Preprocessor Directive";
        case STRING_LITERAL: return "String Literal";
        default: return "Lexical Error";
    }
}

TokenType classify_token(const char *token) {
    if (token[0] == '#' && strlen(token) > 1) 
        return PREPROCESSOR_DIRECTIVE;
    

    if (token[0] == '"' && token[strlen(token) - 1] == '"')
        return STRING_LITERAL;

    if (is_keyword(token))
        return KEYWORD;

    if (isalpha(token[0]) || token[0] == '_') {
        for (int i = 1; token[i] != '\0'; ++i) {
            if (!isalnum(token[i]) && token[i] != '_') 
                return UNKNOWN;
        }
        return IDENTIFIER;
    }

    if (is_operator(token)) 
        return OPERATOR;

    int has_dot = 0;
    for (int i = 0; token[i] != '\0'; ++i) {
        if (token[i] == '.') {
            if (has_dot) return UNKNOWN;
            has_dot = 1;
        } else if (!isdigit(token[i])) {
            return UNKNOWN;
        }
    }
    return CONSTANT;
}

void tokenize(FILE *file) {
    char c;
    char token[MAX_TOKEN_SIZE];
    int token_index = 0;
    int in_string_literal = 0;  

    while ((c = fgetc(file)) != EOF) {
        if (c == '"') {
            if (in_string_literal) {
                // End of string literal
                token[token_index++] = c;
                token[token_index] = '\0';
                printf("Token: %s, Type: %s\n", token, token_type_to_string(STRING_LITERAL));
                token_index = 0;
                in_string_literal = 0;  // Close string literal
            } else {
                // Beginning of string literal
                if (token_index > 0) {
                    token[token_index] = '\0';
                    TokenType type = classify_token(token);
                    printf("Token: %s, Type: %s\n", token, token_type_to_string(type));
                    token_index = 0;
                }
                in_string_literal = 1;  // Open string literal
                token[token_index++] = c;
            }
        } else if (in_string_literal) {
            token[token_index++] = c;  // Continue reading string literal
        } else if (isspace(c) || strchr(";,(){}[],", c)) {
            if (token_index > 0) {
                token[token_index] = '\0';
                TokenType type = classify_token(token);
                printf("Token: %s, Type: %s\n", token, token_type_to_string(type));
                token_index = 0;
            }
            if (strchr(";,(){}[],", c)) {
                token[0] = c;
                token[1] = '\0';
                printf("Token: %s, Type: %s\n", token, token_type_to_string(SYMBOL));
            }
        } else if (isalpha(c) || isdigit(c) || c == '_') {
            // Continue forming identifier, keyword, or constant
            token[token_index++] = c;
        } else if (c == '#' || is_operator((char[]){c, '\0'})) {
            // Handle preprocessor directive or operator
            if (token_index > 0) {
                token[token_index] = '\0';
                TokenType type = classify_token(token);
                printf("Token: %s, Type: %s\n", token, token_type_to_string(type));
                token_index = 0;
            }
            if (c == '#') {
                token[token_index++] = c;
                while ((c = fgetc(file)) != EOF && !isspace(c) && c != '\n') {
                    token[token_index++] = c;
                }
                token[token_index] = '\0';
                printf("Token: %s, Type: %s\n", token, token_type_to_string(PREPROCESSOR_DIRECTIVE));
                token_index = 0;
            } else {
                token[token_index++] = c;
            }
        } else {
            // Handle any unknown character
            if (token_index > 0) {
                token[token_index] = '\0';
                TokenType type = classify_token(token);
                printf("Token: %s, Type: %s\n", token, token_type_to_string(type));
                token_index = 0;
            }
        }
    }

    // Handle the last token if any
    if (token_index > 0) {
        token[token_index] = '\0';
        TokenType type = classify_token(token);
        printf("Token: %s, Type: %s\n", token, token_type_to_string(type));
    }
}

