%{
#include <stdio.h>
#include <stdlib.h>
int yylex(void);
extern int yylineno;
extern char* yytext;
void yyerror(const char *s) { 
    printf("Error: %s at line %d near '%s'\n", s, yylineno, yytext);
}
%}

%union{
    int number;
    char* variable;
}
%token <variable> SELECT FROM WHERE IDENTIFIER STAR OR AND INSERT INTO
%token <number> NUMBER
%token EQUALS SEMICOLON NOTEQUALS

%type <variable> columnOrStar columns condition conditionAND conditionOR simpleCondition 

%%
query: 
     SELECT columnOrStar FROM IDENTIFIER where_clause SEMICOLON { printf("Valid SQL Query\n"); }
     | INSERT INTO IDENTIFIER where_clause SEMICOLON  { printf("Valid SQL Query\n"); }
;

columnOrStar: columns  
        | STAR { printf("All Columns Selected\n"); };

columns:
     IDENTIFIER { printf("Column: %s\n", $1); $$ = $1; }
     | columns ',' IDENTIFIER { printf("Column: %s\n", $3); $$ = $3; }
;

where_clause:
     | WHERE condition
;

condition : 
    conditionOR;

conditionOR : 
    conditionAND
    | conditionOR OR conditionAND;

conditionAND : 
    simpleCondition
    | conditionAND AND simpleCondition; 

simpleCondition : 
    IDENTIFIER EQUALS NUMBER { printf("Condition: %s = %d\n", $1, $3); }
    | IDENTIFIER NOTEQUALS NUMBER { printf("Condition: %s != %d\n", $1, $3); }
;
%%


int main(){
    yyparse();
    return 0;
}
