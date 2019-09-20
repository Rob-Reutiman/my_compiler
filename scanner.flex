%{
#include "token.h"
int ident_check();
int string_check();
int char_check();
int int_check();
char* q_strip();
%}

DIGIT  [0-9]
LETTER [a-zA-Z]

%%
(" "|\t|\n|\r)  /* skip whitespace */
\/\/.* 	        /* skip comment type 1 */
\/\*(\/[^\*]|[^\/]\*|\*[^\/]|\/[^\*]|[^\/\*][^\/\*]|[^\/\*])*\*\/ /* skip comment type 2 */

array  	   { return TOKEN_T_ARRAY;   }
auto 	   { return TOKEN_T_AUTO;    }
boolean    { return TOKEN_T_BOOLEAN; }
char  	   { return TOKEN_T_CHAR;    }
else  	   { return TOKEN_ELSE;      }
false  	   { return TOKEN_FALSE;     }
for  	   { return TOKEN_FOR; 		 }
function   { return TOKEN_FUNCTION;  }
if  	   { return TOKEN_IF; 		 }
integer    { return TOKEN_T_INTEGER; }
print  	   { return TOKEN_PRINT;     }
return 	   { return TOKEN_RETURN;    }
string 	   { return TOKEN_T_STRING;  }
true  	   { return TOKEN_TRUE; 	 }
void  	   { return TOKEN_VOID; 	 }
while      { return TOKEN_WHILE;     }

(_|{LETTER})({LETTER}|{DIGIT}|_)  { return ident_check();   }
\"[^\"]*\" 						  { return string_check();  }
\'(\.|.)\'     					  { return char_check();    }
{DIGIT}+  						  { return int_check();     }

\; 		{ return TOKEN_SEMICOLON; }
\: 		{ return TOKEN_COLON;     }
\, 		{ return TOKEN_COMMA;     }

\( 		{ return TOKEN_LP;  }
\) 		{ return TOKEN_RP;  }
\[ 		{ return TOKEN_LB;  }
\] 		{ return TOKEN_RB;  }
\{ 		{ return TOKEN_LCB; }
\} 		{ return TOKEN_RCB; }

(\+){2}    { return TOKEN_INCREMENT; }
(\-){2}    { return TOKEN_DECREMENT; }

\![^=]    { return TOKEN_NOT; }

\^    { return TOKEN_EXPONENT; }
\*    { return TOKEN_MULT;     }
\/    { return TOKEN_DIV;      }
\%    { return TOKEN_MOD;      }

\+    { return TOKEN_ADD;   }
\-    { return TOKEN_MINUS; }

\>    { return TOKEN_GT;  }
\>\=  { return TOKEN_GE;  }
\<    { return TOKEN_LT;  }
\<\=  { return TOKEN_LE;  }
\=\=  { return TOKEN_EQ;  }
\!\=  { return TOKEN_NEQ; }

\&\&  { return TOKEN_AND;    }
\|\|  { return TOKEN_OR;     }
\=    { return TOKEN_ASSIGN; }

.     { return TOKEN_ERROR;  }
%%
int ident_check() {

};

int string_check() {

};

int char_check() {

};

int int_check() { 

};

char* q_strip() {

};

int yywrap() { return 1; }
