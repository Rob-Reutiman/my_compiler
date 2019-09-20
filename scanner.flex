%{
#include "token.h"
%}

DIGIT  [0-9]
LETTER [a-zA-Z]
%%
(" "|\t|\n|\r)  /* skip whitespace */
\/\/.* 	
\/\*(\/[^\*]|[^\/]\*|\*[^\/]|\/[^\*]|[^\/\*][^\/\*]|[^\/\*])*\*\/

array  	   { return TOKEN_T_ARRAY; }
auto 	   { return TOKEN_T_AUTO; }
boolean    { return TOKEN_T_BOOLEAN; }
char  	   { return TOKEN_T_CHAR; }
else  	   { return TOKEN_ELSE; }
false  	   { return TOKEN_FALSE; }
for  	   { return TOKEN_FOR; }
function   { return TOKEN_FUNCTION; }
if  	   { return TOKEN_IF; }
integer    { return TOKEN_T_INTEGER; }
print  	   { return TOKEN_PRINT; }
return 	   { return TOKEN_RETURN; }
string 	   { return TOKEN_T_STRING; }
true  	   { return TOKEN_TRUE; }
void  	   { return TOKEN_VOID; }
while      { return TOKEN_WHILE;  }

(_|{LETTER})({LETTER}|{DIGIT}|_){0,256}  { return TOKEN_IDENT;  }
\"[^\"]{0,256}\0\" { return TOKEN_STRING_LITERAL; }
\'(\.|.)\'     { return TOKEN_CHAR_LITERAL; }

\; 		   { return TOKEN_SEMICOLON; }
\: 		   { return TOKEN_COLON; }

{DIGIT}+  { return TOKEN_INTEGER_LITERAL; }

(\(){1} 		  { return TOKEN_LP; }
(\)){1} 		{ return TOKEN_RP;  }
(\[){1} 		{ return TOKEN_LB; }
(\]){1} 		{ return TOKEN_RB; }
(\{){1} 		{ return TOKEN_LCB; }
(\}){1} 		{ return TOKEN_RCB; }
(\,){1} 		{ return TOKEN_COMMA; }

(\+){2}    { return TOKEN_INCREMENT; }
(\-){2}    { return TOKEN_DECREMENT; }


(\!){1}[^=]    { return TOKEN_NOT; }

(\^){1}    { return TOKEN_EXPONENT; }
(\*){1}    { return TOKEN_MULT; }
(\/){1}    { return TOKEN_DIV; }
(\%){1}    { return TOKEN_MOD; }

(\+){1}    { return TOKEN_ADD; }
(\-){1}    { return TOKEN_MINUS; }

(\>){1}    { return TOKEN_GT; }
(\>\=){1}    { return TOKEN_GE; }
(\<){1}    { return TOKEN_LT; }
(\<\=){1}    { return TOKEN_LE; }
(\=\=){1}    { return TOKEN_EQ; }
(\!\=){1}    { return TOKEN_NEQ; }

(\&\&){1} 	{ return TOKEN_AND; }
(\|\|){1}  { return TOKEN_OR;  }
(\=){1}    { return TOKEN_ASSIGN; }


.          { return TOKEN_ERROR;  }

%%
int yywrap() { return 1; }
