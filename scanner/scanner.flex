%{
#include "token.h"
%}

DIGIT  [0-9]
LETTER [a-zA-Z]
%%
(" "|\t|\n|\r)  /* skip whitespace */

array  	   { return TOKEN_T_ARRAY; }
auto  	   { return TOKEN_T_AUTO; }
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

(_|{LETTER})({LETTER}|{DIGIT}|_)*  { return TOKEN_IDENT;  }

(\+){2}    { return TOKEN_INCREMENT; }

(\+){1}    { return TOKEN_ADD; }

.          { return TOKEN_ERROR;  }

%%
int yywrap() { return 1; }
