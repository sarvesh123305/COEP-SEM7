%{
    #include "y.tab.h"
    #include<string.h>
%}

DIGIT [0-9]+
ID [a-zA-Z_][a-zA-z0-9_]*

%%
[Ss][Ee][Ll][Ee][Cc][Tt]  { return SELECT; }
[Ii][Nn][Ss][Ee][Rr][Tt]  { return INSERT; }
[Ff][Rr][Oo][Mm]          { return FROM; }
[Ww][Hh][Ee][Rr][Ee]      { return WHERE; }
[Aa][Nn][Dd]              { return AND; }
[Oo][Rr]                  { return OR; }
[Ii][Nn][Tt][Oo] 	  { return INTO; }
"*"            {return STAR;}
{DIGIT} { yylval.number = atoi(yytext); return NUMBER; }
{ID} { yylval.variable = strdup(yytext); return IDENTIFIER; }
"="            { return EQUALS; }
"!="            { return NOTEQUALS; }
"<>"            { return NOTEQUALS; }
";"            { return SEMICOLON; }
[ \t\n]+       { /* Ignore whitespace */ }
.              { return yytext[0]; }
 
%%
