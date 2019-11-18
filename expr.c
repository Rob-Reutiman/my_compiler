
#include "expr.h"
#include <stdlib.h>
#include <string.h>

struct expr * expr_create( expr_t kind, struct expr *left, struct expr *right ) {

	struct expr * e = malloc(sizeof(*e));

	e->kind = kind;
	e->left = left;
	e->right = right;

	return e;

}

struct expr * expr_create_name( const char *n ) {

	struct expr *e = expr_create(EXPR_NAME, 0, 0);
	e->name=strdup(n);

	return e;

}

struct expr * expr_create_integer_literal( int c ) {
	
	struct expr *e = expr_create(EXPR_INTEGER_LITERAL, 0, 0);
	e->literal_value=c;

	return e;

}

struct expr * expr_create_boolean_literal( int c ) {

	struct expr *e = expr_create(EXPR_BOOLEAN_LITERAL, 0, 0);
	e->literal_value=c;

	return e;

}

struct expr * expr_create_char_literal( char c ) {

	struct expr *e = expr_create(EXPR_CHAR_LITERAL, 0, 0);
	e->literal_value=c;

	return e;

}

struct expr * expr_create_string_literal( const char *str ) {

	struct expr *e = expr_create(EXPR_STRING_LITERAL, 0, 0);
	e->string_literal=strdup(str);

	return e;

}

void expr_print( struct expr *e , FILE* stream) {

	if(!e) return;

	switch(e->kind) {
		case EXPR_ADD:
			expr_print(e->left, stream);
			fprintf(stream, "+");
			expr_print(e->right, stream);
			break;
		case EXPR_SUB:
			expr_print(e->left, stream);
			fprintf(stream, "-");
			expr_print(e->right, stream);
			break;
		case EXPR_MUL:
			expr_print(e->left, stream);
			fprintf(stream, "*");
			expr_print(e->right, stream);
			break;
		case EXPR_DIV:
			expr_print(e->left, stream);
			fprintf(stream, "/");
			expr_print(e->right, stream);
			break;
		case EXPR_MOD:
			expr_print(e->left, stream);
			fprintf(stream, "%");
			expr_print(e->right, stream);
			break;
		case EXPR_INCREMENT:
			expr_print(e->left, stream);
			fprintf(stream, "++");
			break;
		case EXPR_DECREMENT:
			expr_print(e->left, stream);
			fprintf(stream, "--");
			break;
		case EXPR_EXPONENT:
			expr_print(e->left, stream);
			fprintf(stream, "^");
			expr_print(e->right, stream);
			break;
		case EXPR_GT:
			expr_print(e->left, stream);
			fprintf(stream, ">");
			expr_print(e->right, stream);
			break;
		case EXPR_GE:
			expr_print(e->left, stream);
			fprintf(stream, ">=");
			expr_print(e->right, stream);
			break;
		case EXPR_LT:
			expr_print(e->left, stream);
			fprintf(stream, "<");
			expr_print(e->right, stream);
			break;
		case EXPR_LE:
			expr_print(e->left, stream);
			fprintf(stream, "<=");
			expr_print(e->right, stream);
			break;
		case EXPR_EQ:
			expr_print(e->left, stream);
			fprintf(stream, "==");
			expr_print(e->right, stream);
			break;
		case EXPR_NEQ:
			expr_print(e->left, stream);
			fprintf(stream, "!=");
			expr_print(e->right, stream);
			break;
		case EXPR_AND:
			expr_print(e->left, stream);
			fprintf(stream, "&&");
			expr_print(e->right, stream);
			break;
		case EXPR_OR:
			expr_print(e->left, stream);
			fprintf(stream, "||");
			expr_print(e->right, stream);
			break;
		case EXPR_ASSIGN:
			expr_print(e->left, stream);
			fprintf(stream, "=");
			expr_print(e->right, stream);
			break;
		case EXPR_ARGLIST:
			expr_print(e->left, stream);
			if(e->right) printf(", ");
			expr_print(e->right, stream);
			break;
		case EXPR_FCALL:
			expr_print(e->left, stream);
			fprintf(stream, "()");	
			break;
		case EXPR_FCALL_ARGS:
			expr_print(e->left, stream);
			fprintf(stream, "(");
			expr_print(e->right, stream);
			fprintf(stream, ")");
			break;
		case EXPR_PAREN:
			expr_print(e->right, stream);
			break;
		case EXPR_REF:
			expr_print(e->left, stream);
			fprintf(stream, "[");
			expr_print(e->right, stream);
			fprintf(stream, "]");
			break;
		case EXPR_NOT:
			fprintf(stream, "!");
			expr_print(e->right, stream);
			break;
		case EXPR_NEG:
			fprintf(stream, "-");
			expr_print(e->right, stream);
			break;
		case EXPR_NAME:
			fprintf(stream, "%s", e->name);
			break;
		case EXPR_BOOLEAN_LITERAL:
			if(e->literal_value == 1) {
				fprintf(stream, "true");
			} else {
				fprintf(stream, "false");
			}
			break;
		case EXPR_CHAR_LITERAL:
			fprintf(stream, "\'");
			char_print(e->literal_value, stream);
			fprintf(stream, "\'");
			break;
		case EXPR_INTEGER_LITERAL:
			fprintf(stream, "%d", e->literal_value);
			break;
		case EXPR_STRING_LITERAL:
			fprintf(stream, "\"");
			string_print(e->string_literal, stream);
			fprintf(stream, "\"");
			break;
		default:
			break;
	}

}

void string_print(const char* c, FILE* stream) {

	while(*c) {
		char_print(*c, stream);
		c++;
	}

};

void char_print(char c, FILE* stream) {

	switch(c) {
		case '\a': fprintf(stream, "\\a"); break;
			
		case '\b': fprintf(stream, "\\b"); break;

		case '\e': fprintf (stream, "\\e"); break;

		case '\f': fprintf (stream, "\\f"); break;

		case '\n': fprintf (stream, "\\n"); break;

		case '\r': fprintf(stream, "\\r"); break;

		case '\t': fprintf(stream, "\\t"); break;

		case '\v': fprintf(stream, "\\v"); break;

		case '\\': fprintf(stream, "\\\\"); break;

		case '\'': fprintf(stream, "\\\'"); break;

		case '\"': fprintf(stream, "\\\""); break;
                      
		default: fprintf(stream, "%c", c); break;
	}
}

void expr_resolve( struct expr *e, struct hash_table *h) {

	if(!e) return;

	if( e->kind == EXPR_NAME ) {
		e->symbol = scope_lookup(h, e->name);
		if(e->symbol == NULL) {
			fprintf(stderr, "resolve error: %s is not defined\n", e->name);
			RESOLVE_ERROR = 0;
		}
	} else {
		expr_resolve( e->left, h );
		expr_resolve( e->right, h);
	}	

}

struct type * expr_typecheck( struct expr *e ) {

	if(!e) return 0;

	struct type *lt = expr_typecheck(e->left);
	struct type *rt = expr_typecheck(e->right);

	struct type *result;

	switch(e->kind) {
		case EXPR_ADD:
			if(lt->kind != TYPE_INTEGER || rt->kind != TYPE_INTEGER) {
				fprintf(stderr, "type error: cannot add a ");
				type_print(lt, stderr);
				fprintf(stderr, " (");
				expr_print(e->left, stderr);
				fprintf(stderr, ") to a ");
				type_print(rt, stderr);
				fprintf(stderr, " (");
				expr_print(e->right, stderr);
				fprintf(stderr, ")\n");
				TYPE_ERROR = 0;
			} 
			result = type_create(TYPE_INTEGER,0,0);
			break;

		case EXPR_SUB:
			if(lt->kind != TYPE_INTEGER || rt->kind != TYPE_INTEGER) {
				fprintf(stderr, "type error: cannot subtract a ");
				type_print(lt, stderr);
				fprintf(stderr, " (");
				expr_print(e->right, stderr);
				fprintf(stderr, ") from a ");
				type_print(rt, stderr);
				fprintf(stderr, " (");
				expr_print(e->left, stderr);
				fprintf(stderr, ")\n");
				TYPE_ERROR = 0;
			}
			result = type_create(TYPE_INTEGER,0,0);
			break;

		case EXPR_MUL:
			if(lt->kind != TYPE_INTEGER || rt->kind != TYPE_INTEGER) {
				fprintf(stderr, "type error: cannot mult a ");
				type_print(lt, stderr);
				fprintf(stderr, " (");
				expr_print(e->left, stderr);
				fprintf(stderr, ") to a ");
				type_print(rt, stderr);
				fprintf(stderr, " (");
				expr_print(e->right, stderr);
				fprintf(stderr, ")\n");
				TYPE_ERROR = 0;
			}
			result = type_create(TYPE_INTEGER,0,0);
			break;

		case EXPR_DIV:
			if(lt->kind != TYPE_INTEGER || rt->kind != TYPE_INTEGER) {
				fprintf(stderr, "type error: cannot div a ");
				type_print(lt, stderr);
				fprintf(stderr, " (");
				expr_print(e->right, stderr);
				fprintf(stderr, ") by a ");
				type_print(rt, stderr);
				fprintf(stderr, " (");
				expr_print(e->left, stderr);
				fprintf(stderr, ")\n");
				TYPE_ERROR = 0;
			}
			result = type_create(TYPE_INTEGER,0,0);
			break;

		case EXPR_MOD:
			if(lt->kind != TYPE_INTEGER || rt->kind != TYPE_INTEGER) {
				fprintf(stderr, "type error: cannot mod a ");
				type_print(lt, stderr);
				fprintf(stderr, " (");
				expr_print(e->left, stderr);
				fprintf(stderr, ") by a ");
				type_print(rt, stderr);
				fprintf(stderr, " (");
				expr_print(e->right, stderr);
				fprintf(stderr, ")\n");
				TYPE_ERROR = 0;
			}
			result = type_create(TYPE_INTEGER,0,0);
			break;

		case EXPR_INCREMENT:
			if(lt->kind != TYPE_INTEGER || rt->kind != TYPE_INTEGER) {
				fprintf(stderr, "type error: cannot increment a ");
				type_print(lt, stderr);
				fprintf(stderr, " (");
				expr_print(e->left, stderr);
				fprintf(stderr, ")\n");
				TYPE_ERROR = 0;
			}
			result = type_create(TYPE_INTEGER,0,0);
			break;

		case EXPR_DECREMENT:
			if(lt->kind != TYPE_INTEGER || rt->kind != TYPE_INTEGER) {
				fprintf(stderr, "type error: cannot decrement a ");
				type_print(lt, stderr);
				fprintf(stderr, " (");
				expr_print(e->left, stderr);
				fprintf(stderr, ")\n");
				TYPE_ERROR = 0;
			}
			result = type_create(TYPE_INTEGER,0,0);
			break;

		case EXPR_EXPONENT:
			if(lt->kind != TYPE_INTEGER || rt->kind != TYPE_INTEGER) {
				fprintf(stderr, "type error: cannot exponentiate a ");
				type_print(lt, stderr);
				fprintf(stderr, " (");
				expr_print(e->left, stderr);
				fprintf(stderr, ") with a ");
				type_print(rt, stderr);
				fprintf(stderr, " (");
				expr_print(e->right, stderr);
				fprintf(stderr, ")\n");
				TYPE_ERROR = 0;
			}
			result = type_create(TYPE_INTEGER,0,0);
			break;

		case EXPR_GT:	
			if(!type_equals(lt, rt)) {
				fprintf(stderr, "type error: cannot compare a ");
				type_print(lt, stderr);
				fprintf(stderr, " (");
				expr_print(e->left, stderr);
				fprintf(stderr, ") with a ");
				type_print(rt, stderr);
				fprintf(stderr, " (");
				expr_print(e->right, stderr);
				fprintf(stderr, ")\n");
				TYPE_ERROR = 0;
			} 
			if(lt->kind == TYPE_VOID || lt->kind == TYPE_ARRAY || lt->kind == TYPE_FUNCTION) {
				fprintf(stderr, "type error: cannot compare");
				type_print(lt, stderr);
				fprintf(stderr, " (");
				expr_print(e->left, stderr);
				fprintf(stderr, ") and ");
				type_print(rt, stderr);
				fprintf(stderr, " (");
				expr_print(e->right, stderr);
				fprintf(stderr, ") with > operator\n");
				TYPE_ERROR = 0;
			}
			result = type_create(TYPE_BOOLEAN,0,0);	
			break;

		case EXPR_GE:
			if(!type_equals(lt, rt)) {
				fprintf(stderr, "type error: cannot compare a ");
				type_print(lt, stderr);
				fprintf(stderr, " (");
				expr_print(e->left, stderr);
				fprintf(stderr, ") with a ");
				type_print(rt, stderr);
				fprintf(stderr, " (");
				expr_print(e->right, stderr);
				fprintf(stderr, ")\n");
				TYPE_ERROR = 0;
			} 
			if(lt->kind == TYPE_VOID || lt->kind == TYPE_ARRAY || lt->kind == TYPE_FUNCTION) {
				fprintf(stderr, "type error: cannot compare");
				type_print(lt, stderr);
				fprintf(stderr, " (");
				expr_print(e->left, stderr);
				fprintf(stderr, ") and ");
				type_print(rt, stderr);
				fprintf(stderr, " (");
				expr_print(e->right, stderr);
				fprintf(stderr, ") with >= operator\n");
				TYPE_ERROR = 0;
			}
			result = type_create(TYPE_BOOLEAN,0,0);	
			break;

		case EXPR_LT:
			if(!type_equals(lt, rt)) {
				fprintf(stderr, "type error: cannot compare a ");
				type_print(lt, stderr);
				fprintf(stderr, " (");
				expr_print(e->left, stderr);
				fprintf(stderr, ") with a ");
				type_print(rt, stderr);
				fprintf(stderr, " (");
				expr_print(e->right, stderr);
				fprintf(stderr, ")\n");
				TYPE_ERROR = 0;
			} 
			if(lt->kind == TYPE_VOID || lt->kind == TYPE_ARRAY || lt->kind == TYPE_FUNCTION) {
				fprintf(stderr, "type error: cannot compare");
				type_print(lt, stderr);
				fprintf(stderr, " (");
				expr_print(e->left, stderr);
				fprintf(stderr, ") and ");
				type_print(rt, stderr);
				fprintf(stderr, " (");
				expr_print(e->right, stderr);
				fprintf(stderr, ") with < operator\n");
				TYPE_ERROR = 0;
			}
			result = type_create(TYPE_BOOLEAN,0,0);	
			break;

		case EXPR_LE:
			if(!type_equals(lt, rt)) {
				fprintf(stderr, "type error: cannot compare a ");
				type_print(lt, stderr);
				fprintf(stderr, " (");
				expr_print(e->left, stderr);
				fprintf(stderr, ") with a ");
				type_print(rt, stderr);
				fprintf(stderr, " (");
				expr_print(e->right, stderr);
				fprintf(stderr, ")\n");
				TYPE_ERROR = 0;
			} 
			if(lt->kind == TYPE_VOID || lt->kind == TYPE_ARRAY || lt->kind == TYPE_FUNCTION) {
				fprintf(stderr, "type error: cannot compare");
				type_print(lt, stderr);
				fprintf(stderr, " (");
				expr_print(e->left, stderr);
				fprintf(stderr, ") and ");
				type_print(rt, stderr);
				fprintf(stderr, " (");
				expr_print(e->right, stderr);
				fprintf(stderr, ") with <= operator\n");
				TYPE_ERROR = 0;
			}
			result = type_create(TYPE_BOOLEAN,0,0);	
			break;

		case EXPR_EQ:
			if(!type_equals(lt, rt)) {
				fprintf(stderr, "type error: cannot compare a ");
				type_print(lt, stderr);
				fprintf(stderr, " (");
				expr_print(e->left, stderr);
				fprintf(stderr, ") with a ");
				type_print(rt, stderr);
				fprintf(stderr, " (");
				expr_print(e->right, stderr);
				fprintf(stderr, ")\n");
				TYPE_ERROR = 0;
			} 
			if(lt->kind == TYPE_VOID || lt->kind == TYPE_ARRAY || lt->kind == TYPE_FUNCTION) {
				fprintf(stderr, "type error: cannot compare");
				type_print(lt, stderr);
				fprintf(stderr, " (");
				expr_print(e->left, stderr);
				fprintf(stderr, ") and ");
				type_print(rt, stderr);
				fprintf(stderr, " (");
				expr_print(e->right, stderr);
				fprintf(stderr, ") with == operator\n");
				TYPE_ERROR = 0;
			}
			result = type_create(TYPE_BOOLEAN,0,0);	
			break;

		case EXPR_NEQ:
			if(!type_equals(lt, rt)) {
				fprintf(stderr, "type error: cannot compare a ");
				type_print(lt, stderr);
				fprintf(stderr, " (");
				expr_print(e->left, stderr);
				fprintf(stderr, ") with a ");
				type_print(rt, stderr);
				fprintf(stderr, " (");
				expr_print(e->right, stderr);
				fprintf(stderr, ")\n");
				TYPE_ERROR = 0;
			} 
			if(lt->kind == TYPE_VOID || lt->kind == TYPE_ARRAY || lt->kind == TYPE_FUNCTION) {
				fprintf(stderr, "type error: cannot compare");
				type_print(lt, stderr);
				fprintf(stderr, " (");
				expr_print(e->left, stderr);
				fprintf(stderr, ") and ");
				type_print(rt, stderr);
				fprintf(stderr, " (");
				expr_print(e->right, stderr);
				fprintf(stderr, ") with != operator\n");
				TYPE_ERROR = 0;
			}
			result = type_create(TYPE_BOOLEAN,0,0);	
			break;

		case EXPR_AND:

			break;

		case EXPR_OR:

			break;

		case EXPR_ASSIGN:

			break;

		case EXPR_ARGLIST:

			break;

		case EXPR_FCALL:
	
			break;

		case EXPR_FCALL_ARGS:

			break;

		case EXPR_PAREN:

			break;

		case EXPR_REF:

			break;

		case EXPR_NOT:

			break;

		case EXPR_NEG:

			break;

		case EXPR_NAME:
			result = type_create(e->symbol->type->kind,0,0);
			break; 

		case EXPR_BOOLEAN_LITERAL:
			result = type_create(TYPE_BOOLEAN,0,0);
			break;

		case EXPR_CHAR_LITERAL:
			result = type_create(TYPE_CHARACTER,0,0);
			break;

		case EXPR_INTEGER_LITERAL:
			result = type_create(TYPE_INTEGER,0,0);
			break;

		case EXPR_STRING_LITERAL:
			result = type_create(TYPE_STRING,0,0);
			break;

		default:
			break;

	} 

	type_delete(lt);
	type_delete(rt);

	return result;
}

void expr_delete(struct expr * e ) {

	if(!e) return;
	expr_delete(e->left);
	expr_delete(e->right);

	symbol_delete(e->symbol);

	free(e);

}
