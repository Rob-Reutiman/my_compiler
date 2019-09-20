#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
     TOKEN_EOF=0,
	 TOKEN_T_ARRAY,
	 TOKEN_T_AUTO,
	 TOKEN_T_BOOLEAN,
	 TOKEN_T_CHAR,
	 TOKEN_ELSE,
	 TOKEN_FALSE,
	 TOKEN_FOR,
	 TOKEN_FUNCTION,
	 TOKEN_IF,
	 TOKEN_T_INTEGER,
	 TOKEN_PRINT,
	 TOKEN_RETURN,
	 TOKEN_T_STRING,
	 TOKEN_TRUE,
	 TOKEN_VOID,
	 TOKEN_WHILE,
	 TOKEN_IDENT,
	 TOKEN_STRING_LITERAL,
	 TOKEN_CHAR_LITERAL,
	 TOKEN_INTEGER_LITERAL,
	 TOKEN_SEMICOLON,
	 TOKEN_COLON,
	 TOKEN_COMMA,
 	 TOKEN_LP,
	 TOKEN_RP,
	 TOKEN_LB,
	 TOKEN_RB,
	 TOKEN_LCB,
	 TOKEN_RCB,
	 TOKEN_INCREMENT,
	 TOKEN_DECREMENT,
	 TOKEN_NOT,
	 TOKEN_EXPONENT,
 	 TOKEN_MULT,
  	 TOKEN_DIV,
 	 TOKEN_MOD,
     TOKEN_ADD,
	 TOKEN_MINUS,
 	 TOKEN_GT,
 	 TOKEN_GE,
 	 TOKEN_LT,
 	 TOKEN_LE,
 	 TOKEN_EQ,
 	 TOKEN_NEQ,
 	 TOKEN_AND,
 	 TOKEN_OR,
 	 TOKEN_ASSIGN,
     TOKEN_ERROR
} token_t;

#endif