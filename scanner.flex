%{
#include "token.h"
int ident_check(char* yytext);
int string_check(char* yytext);
int char_check(char* yytext);
int int_check(char* yytext);
void q_strip(char* yytext);
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

(_|{LETTER})({LETTER}|{DIGIT}|_)*  { return ident_check(yytext);   }
\"[^\"]*\" 						  { return string_check(yytext);  }
\'(\.|.)\'     					  { return char_check(yytext);    }
{DIGIT}+  						  { return int_check(yytext);     }

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

.     { return TOKEN_SCAN_ERROR;  }
%%
int ident_check(char* yytext) {
	int length = 0;
	
	return TOKEN_IDENT;	
};

int string_check(char* yytext) {

	q_strip(yytext);
	return TOKEN_STRING_LITERAL;
};

int char_check(char* yytext) {
	q_strip(yytext);

	if(*yytext == '\\') {
		if(strlen(yytext) == 2) {
			return TOKEN_CHAR_LITERAL;
		} else {
			return TOKEN_CHAR_ERROR;
		}
	} else {
		if(strlen(yytext) == 1) {
		return TOKEN_CHAR_LITERAL; 
		} else {
			return TOKEN_CHAR_ERROR;
		}
	}

};

int int_check(char* yytext) { 

	return TOKEN_INTEGER_LITERAL;
};

void q_strip(char* yytext) {

    char* c = yytext+strlen(yytext)-1;

    if(*c == '\'' || '\"') {    /* eliminates quotes from end */
        *c='\0';
    } 

//	while(*prev != '\'' && *c != '\"') {
		
 //   if(*temp == '\'' || '\"') {    /* eliminates quotes from front */
//		 char* temp = *prev;
//		 *c
			
  //  } 

    
};

int yywrap() { return 1; }
