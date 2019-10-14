%{
#include "parser.h"
int ident_check(char* yytext);
int string_check(char** yytext);
int char_check(char** yytext);
int int_check(char* yytext);
void q_strip(char** yytext);
void escape_fix(char** yytext);
char escp_pick(char c);
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

(_|{LETTER})({LETTER}|{DIGIT}|_)*  	{ return ident_check(yytext);   }
\"(([^\"\\\n]|\\[^\s])*)\"     { return string_check(&yytext);  }   
\'(\\[^\s]|[^\\\n])\'  			    { return char_check(&yytext);    }
{DIGIT}+  						    { return int_check(yytext);     }

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

\!    { return TOKEN_NOT; }

\&\&  { return TOKEN_AND;    }
\|\|  { return TOKEN_OR;     }
\=    { return TOKEN_ASSIGN; }

.     { return TOKEN_SCAN_ERROR;  }
%%
int ident_check(char* yytext) {

	if(strlen(yytext) < 255 ) {
		return TOKEN_IDENT;
	} else {
		return TOKEN_IDENT_ERROR;
	}
	
};

int string_check(char** yytext) {

	q_strip(yytext);

	escape_fix(yytext); 	

	if(strlen(*yytext) < 255 ) {
		return TOKEN_STRING_LITERAL;
	} else {
		return TOKEN_STRING_ERROR;
	}
};

int char_check(char** yytext) {
	q_strip(yytext);

	escape_fix(yytext);

	if(strlen(*yytext) == 1 || strlen(*yytext) == 0) {
		return TOKEN_CHAR_LITERAL; 
	} else {
			return TOKEN_CHAR_ERROR;
	}
};

int int_check(char* yytext) {

	int64_t max = 9223372036854775807;

	if(atoi(yytext) <= max && strlen(yytext) <= 19) {
		return TOKEN_INTEGER_LITERAL;
	} else {
		return TOKEN_INTEGER_ERROR;
	}
};

void q_strip(char** yytext) {

    char* c = *yytext+strlen(*yytext)-1;

    if(*c == '\'' || '\"') {    /* eliminates quotes from end */
        *c='\0';
    } 

	(*yytext)++;

};

void escape_fix(char** yytext) {

	int jumped = 0;
	char* c = *yytext;
	char* b = *yytext;
	int len = strlen(c);

	while(*c) {

		if(*c == '\\') {
			jumped++;
			c++;
			*c = escp_pick(*c);
		}

		if(jumped) {
			*(c-jumped) = *c;
		}

		c++;
	}

	b+=len-jumped;
	while(*b) {
		*b = '\0';
	}	

};

char escp_pick(char c) {

	switch(c) {
		case 'a': return '\a';
			
		case 'b': return '\b';

		case 'e': return '\e';

		case 'f': return '\f';

		case 'n': return '\n';

		case 'r': return '\r';

		case 't': return '\t';

		case 'v': return '\v';

		case '\\': return '\\';

		case '\'': return '\'';

		case '\"': return '\"';

		case '?': return '\?';

		case '0': return '\0';

		default: return c;

	}

};

int yywrap() { return 1; }
