%{
#include <stdio.h>
#include <stdlib.h>

int line_no = 1;
%}

%%

"auto"          { printf("Keyword: auto\n"); }
"break"         { printf("Keyword: break\n"); }
"case"          { printf("Keyword: case\n"); }
"char"          { printf("Keyword: char\n"); }
"const"         { printf("Keyword: const\n"); }
"continue"      { printf("Keyword: continue\n"); }
"default"       { printf("Keyword: default\n"); }
"do"            { printf("Keyword: do\n"); }
"double"        { printf("Keyword: double\n"); }
"else"          { printf("Keyword: else\n"); }
"enum"          { printf("Keyword: enum\n"); }
"extern"        { printf("Keyword: extern\n"); }
"float"         { printf("Keyword: float\n"); }
"for"           { printf("Keyword: for\n"); }
"goto"          { printf("Keyword: goto\n"); }
"if"            { printf("Keyword: if\n"); }
"int"           { printf("Keyword: int\n"); }
"long"          { printf("Keyword: long\n"); }
"register"      { printf("Keyword: register\n"); }
"return"        { printf("Keyword: return\n"); }
"short"         { printf("Keyword: short\n"); }
"signed"        { printf("Keyword: signed\n"); }
"sizeof"        { printf("Keyword: sizeof\n"); }
"static"        { printf("Keyword: static\n"); }
"struct"        { printf("Keyword: struct\n"); }
"switch"        { printf("Keyword: switch\n"); }
"typedef"       { printf("Keyword: typedef\n"); }
"union"         { printf("Keyword: union\n"); }
"unsigned"      { printf("Keyword: unsigned\n"); }
"void"          { printf("Keyword: void\n"); }
"volatile"      { printf("Keyword: volatile\n"); }
"while"         { printf("Keyword: while\n"); }

[a-zA-Z_][a-zA-Z0-9_]* { printf("Identifier: %s\n", yytext); }

[0-9]+\.[0-9]+       { printf("Floating point constant: %s\n", yytext); }
[0-9]+                { printf("Integer constant: %s\n", yytext); }

\"([^\"\\]|\\.)*\"    { printf("String literal: %s\n", yytext); }

\'[^\'\\]\'           { printf("Character constant: %s\n", yytext); }

"\\["          { printf("Special symbol: Left bracket [\n"); }
"\\]"          { printf("Special symbol: Right bracket ]\n"); }
"\\("          { printf("Special symbol: Left parenthesis (\n"); }
"\\)"          { printf("Special symbol: Right parenthesis )\n"); }
"\\{"          { printf("Special symbol: Left brace {\n"); }
"\\}"          { printf("Special symbol: Right brace }\n"); }
","            { printf("Special symbol: Comma ,\n"); }
":"            { printf("Special symbol: Colon :\n"); }
";"            { printf("Special symbol: Semicolon ;\n"); }
"\\*"          { printf("Special symbol: Asterisk *\n"); }
"="            { printf("Special symbol: Assignment operator =\n"); }
"#"            { printf("Special symbol: Pre-processor #\n"); }
"\\."          { printf("Special symbol: Period .\n"); }
"~"            { printf("Special symbol: Tilde ~\n"); }

"\\+\\+"       { printf("Unary operator: ++\n"); }
"--"           { printf("Unary operator: --\n"); }
"\\+"          { printf("Binary operator: +\n"); }
"-"            { printf("Binary operator: -\n"); }
"*"            { printf("Binary operator: *\n"); }
"/"            { printf("Binary operator: /\n"); }
"%"            { printf("Binary operator: %%\n"); }

"=="           { printf("Relational operator: ==\n"); }
"!="           { printf("Relational operator: !=\n"); }
"<"            { printf("Relational operator: <\n"); }
"<="           { printf("Relational operator: <=\n"); }
">"            { printf("Relational operator: >\n"); }
">="           { printf("Relational operator: >=\n"); }

"&&"           { printf("Logical operator: &&\n"); }
"||"           { printf("Logical operator: ||\n"); }
"!"            { printf("Logical operator: !\n"); }
"&"            { printf("Bitwise operator: &\n"); }
"\\|"          { printf("Bitwise operator: |\n"); }
"^"            { printf("Bitwise operator: ^\n"); }
"<<"           { printf("Bitwise operator: <<\n"); }
">>"           { printf("Bitwise operator: >>\n"); }

"\\?"          { printf("Ternary operator: ?\n"); }

"//".*         { /* Ignore single-line comment */ }
"/\\*([^*]|[\\r\\n]|(\\*+([^*/]|[\\r\\n])))*\\*/" { /* Ignore multi-line comment */ }

\n            { line_no++; }
[ \t]         { /* Ignore whitespace */ }

.              { printf("Lexical error: Invalid character '%s'\n", yytext); }

%%

int main(int argc, char **argv) {
    if (argc > 1) {
        FILE *infile = fopen(argv[1], "r");
        if (!infile) {
            perror("Error opening file");
            return 1;
        }
        yyin = infile;
    }

    yylex();

    printf("Total lines: %d\n", line_no);
    return 0;
}

