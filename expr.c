
#include "expr.h"
#include "param_list.h"
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
			fprintf(stream, " + ");
			expr_print(e->right, stream);
			break;
		case EXPR_SUB:
			expr_print(e->left, stream);
			fprintf(stream, " - ");
			expr_print(e->right, stream);
			break;
		case EXPR_MUL:
			expr_print(e->left, stream);
			fprintf(stream, " * ");
			expr_print(e->right, stream);
			break;
		case EXPR_DIV:
			expr_print(e->left, stream);
			fprintf(stream, " / ");
			expr_print(e->right, stream);
			break;
		case EXPR_MOD:
			expr_print(e->left, stream);
			fprintf(stream, " % ");
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
			fprintf(stream, " > ");
			expr_print(e->right, stream);
			break;
		case EXPR_GE:
			expr_print(e->left, stream);
			fprintf(stream, " >= ");
			expr_print(e->right, stream);
			break;
		case EXPR_LT:
			expr_print(e->left, stream);
			fprintf(stream, " < ");
			expr_print(e->right, stream);
			break;
		case EXPR_LE:
			expr_print(e->left, stream);
			fprintf(stream, " <= ");
			expr_print(e->right, stream);
			break;
		case EXPR_EQ:
			expr_print(e->left, stream);
			fprintf(stream, " == ");
			expr_print(e->right, stream);
			break;
		case EXPR_NEQ:
			expr_print(e->left, stream);
			fprintf(stream, " != ");
			expr_print(e->right, stream);
			break;
		case EXPR_AND:
			expr_print(e->left, stream);
			fprintf(stream, " && ");
			expr_print(e->right, stream);
			break;
		case EXPR_OR:
			expr_print(e->left, stream);
			fprintf(stream, " || ");
			expr_print(e->right, stream);
			break;
		case EXPR_ASSIGN:
			expr_print(e->left, stream);
			fprintf(stream, " = ");
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
				type_print(rt, stderr);
				fprintf(stderr, " (");
				expr_print(e->right, stderr);
				fprintf(stderr, ") from a ");
				type_print(lt, stderr);
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
				type_print(rt, stderr);
				fprintf(stderr, " (");
				expr_print(e->right, stderr);
				fprintf(stderr, ") by a ");
				type_print(lt, stderr);
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
			if(lt->kind != TYPE_INTEGER) {
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
			if(lt->kind != TYPE_INTEGER) {
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
			if(lt->kind == TYPE_VOID || lt->kind == TYPE_ARRAY || lt->kind == TYPE_FUNCTION) {
				fprintf(stderr, "type error: cannot logically AND ");
				type_print(lt, stderr);
				fprintf(stderr, " (");
				expr_print(e->left, stderr);
				fprintf(stderr, ") and ");
				type_print(rt, stderr);
				fprintf(stderr, " (");
				expr_print(e->right, stderr);
				fprintf(stderr, ")\n");
				TYPE_ERROR = 0;
			}
			result = type_create(TYPE_BOOLEAN,0,0);	
			break;

		case EXPR_OR:
			if(lt->kind == TYPE_VOID || lt->kind == TYPE_ARRAY || lt->kind == TYPE_FUNCTION) {
				fprintf(stderr, "type error: cannot logically OR ");
				type_print(lt, stderr);
				fprintf(stderr, " (");
				expr_print(e->left, stderr);
				fprintf(stderr, ") and ");
				type_print(rt, stderr);
				fprintf(stderr, " (");
				expr_print(e->right, stderr);
				fprintf(stderr, ")\n");
				TYPE_ERROR = 0;
			}
			result = type_create(TYPE_BOOLEAN,0,0);	
			break;

		case EXPR_ASSIGN:
			if(lt->kind == TYPE_AUTO) {
				lt->kind == rt->kind;
				fprintf(stderr, "notice: type of ");
				expr_print(e->left, stderr);
				fprintf(stderr, " is ");
				type_print(rt, stderr);
				fprintf(stderr, "\n");
			}
			if(lt->kind == TYPE_VOID) {
				fprintf(stderr, "type error: cannot assign a void (");
				expr_print(e->left, stderr);
				fprintf(stderr, ") to a value\n");
				TYPE_ERROR = 0;
			}
			if(!type_equals(lt, rt)) {
				fprintf(stderr, "type error: cannot assign a ");
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
			result = type_copy(lt);
			break;	

		case EXPR_ARGLIST:
			result = type_copy(lt);
			break;

		case EXPR_FCALL:
			result = type_copy(lt->subtype);
			break;

		case EXPR_FCALL_ARGS:
			result = type_copy(lt->subtype);
			break;

		case EXPR_PAREN:
			result = type_copy(rt);	
			break;

		case EXPR_REF:
			if(lt->kind == TYPE_ARRAY) {
				if(rt->kind != TYPE_INTEGER) {
					fprintf(stderr, "Must index array with integer");
					TYPE_ERROR = 0;	
				} 
				result = type_copy(lt->subtype);
			} else {
				fprintf(stderr, "Can only index an array");
				result = type_copy(lt);
			}
			break;

		case EXPR_NOT:
			if(rt->kind != TYPE_BOOLEAN) {
				fprintf(stderr, "type error: cannot logical not a ");
				type_print(rt, stderr);
				fprintf(stderr, " (");
				expr_print(e->right, stderr);
				fprintf(stderr, ")\n");
				TYPE_ERROR = 0;
			}
			result = type_create(TYPE_BOOLEAN,0,0);
			break;

		case EXPR_NEG:
			if(rt->kind != TYPE_INTEGER) {
				fprintf(stderr, "type error: cannot negate a ");
				type_print(rt, stderr);
				fprintf(stderr, " (");
				expr_print(e->right, stderr);
				fprintf(stderr, ")\n");
				TYPE_ERROR = 0;
			}
			result = type_create(TYPE_INTEGER,0,0);
			break;

		case EXPR_NAME:
			if(e->symbol->type->kind == TYPE_VOID) {
				fprintf(stderr, "type error: ident cannot be of type void\n");
				TYPE_ERROR = 0;
			}
			result = type_create(e->symbol->type->kind, e->symbol->type->subtype, e->symbol->type->params);
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

void expr_delete(struct expr * e) {

	if(!e) return;
	expr_delete(e->left);
	expr_delete(e->right);

	symbol_delete(e->symbol);

	free(e);

}

void expr_codegen (struct expr *e, FILE* stream) {

	if(!e) return;

	int scrtch, args;

	switch(e->kind) {
		case EXPR_ADD:
			expr_codegen(e->left, stream);
			expr_codegen(e->right, stream);
			fprintf(stream, "ADDQ %s, %s\n", scratch_name(e->left->reg), scratch_name(e->right->reg));
			e->reg = e->right->reg;
			scratch_free(e->left->reg);
			break;
		case EXPR_SUB:
			expr_codegen(e->left, stream);
			expr_codegen(e->right, stream);
			fprintf(stream, "SUBQ %s, %s\n", scratch_name(e->left->reg), scratch_name(e->right->reg));
			e->reg = e->right->reg;
			scratch_free(e->left->reg);
			break;
		case EXPR_MUL:
			expr_codegen(e->left, stream);
			expr_codegen(e->right, stream);
			fprintf(stream, "MOVQ %s, %rax\n", scratch_name(e->left->reg));
			fprintf(stream, "IMULQ %s\n", scratch_name(e->right->reg));
			fprintf(stream, "MOVQ %rax, %s\n",  scratch_name(e->right->reg));
			e->reg = e->right->reg;
			scratch_free(e->left->reg);
			break;
		case EXPR_DIV:
			expr_codegen(e->left, stream);
			expr_codegen(e->right, stream);
			fprintf(stream, "MOVQ %s, %rax\n", scratch_name(e->left->reg));
			fprintf(stream, "CQO\n");
			fprintf(stream, "IDIVQ %s\n", scratch_name(e->right->reg));
			fprintf(stream, "MOVQ %rax, %s\n",  scratch_name(e->right->reg));
			e->reg = e->right->reg;
			scratch_free(e->left->reg);
			break;
		case EXPR_MOD:
			expr_codegen(e->left, stream);
			expr_codegen(e->right, stream);
			fprintf(stream, "MOVQ %s, %rax\n", scratch_name(e->left->reg));
			fprintf(stream, "CQO\n");
			fprintf(stream, "IDIVQ %s\n", scratch_name(e->right->reg));
			fprintf(stream, "MOVQ %rdx, %s\n",  scratch_name(e->right->reg));
			e->reg = e->right->reg;
			scratch_free(e->left->reg);
			break;
		case EXPR_INCREMENT:
			expr_codegen(e->left, stream);
			fprintf(stream, "MOVQ %s, %rax\n", scratch_name(e->left->reg));
			fprintf(stream, "INCQ %rax\n");
			fprintf(stream, "MOVQ %rax, %s\n", scratch_name(e->left->reg));
			e->reg = e->left->reg;
			break;
		case EXPR_DECREMENT:
			expr_codegen(e->left, stream);
			fprintf(stream, "MOVQ %s, %rax\n", scratch_name(e->left->reg));
			fprintf(stream, "INCQ %rax\n");
			fprintf(stream, "MOVQ %rax, %s\n", scratch_name(e->left->reg));			
			e->reg = e->left->reg;
			break;
		case EXPR_EXPONENT:
			expr_codegen(e->left, stream);
			expr_codegen(e->right, stream);
			fprintf(stream, "MOVQ %s, %rdi\n", scratch_name(e->left->reg));
			fprintf(stream, "MOVQ %s, %rsi\n", scratch_name(e->right->reg));
			fprintf(stream, "CALL integer_power\n");
			fprintf(stream, "MOVQ %rax, %s\n", scratch_name(e->right->reg));
			e->reg = e->right->reg;
			scratch_free(e->left->reg);
			break;
		case EXPR_GT:
			expr_codegen(e->left, stream);
			expr_codegen(e->right, stream);
			fprintf(stream, "CMP %s, %s\n", scratch_name(e->left->reg), scratch_name(e->right->reg));
			int true_label = label_create();
			int done_label = label_create();
			fprintf(stream, "JGT %s\n", label_name(true_label));
			e->reg = scratch_alloc();
			fprintf(stream, "MOV $1, %s\n", scratch_name(e->reg));
			fprintf(stream, "JMP %s\n", label_name(done_label));
			fprintf(stream, "%s\n", label_name(true_label));
			fprintf(stream, "MOV $0, %s\n", scratch_name(e->reg));
			fprintf(stream, "%s\n", label_name(done_label));
			break;
		case EXPR_GE:
			expr_codegen(e->left, stream);
			expr_codegen(e->right, stream);
			fprintf(stream, "CMP %s, %s\n", scratch_name(e->left->reg), scratch_name(e->right->reg));
			true_label = label_create();
			done_label = label_create();
			fprintf(stream, "JGE %s\n", label_name(true_label));
			e->reg = scratch_alloc();
			fprintf(stream, "MOV $1, %s\n", scratch_name(e->reg));
			fprintf(stream, "JMP %s\n", label_name(done_label));
			fprintf(stream, "%s\n", label_name(true_label));
			fprintf(stream, "MOV $0, %s\n", scratch_name(e->reg));
			fprintf(stream, "%s\n", label_name(done_label));
			break;
		case EXPR_LT:
			expr_codegen(e->left, stream);
			expr_codegen(e->right, stream);
			fprintf(stream, "CMP %s, %s\n", scratch_name(e->left->reg), scratch_name(e->right->reg));
			true_label = label_create();
			done_label = label_create();
			fprintf(stream, "JLT %s\n", label_name(true_label));
			e->reg = scratch_alloc();
			fprintf(stream, "MOV $1, %s\n", scratch_name(e->reg));
			fprintf(stream, "JMP %s\n", label_name(done_label));
			fprintf(stream, "%s\n", label_name(true_label));
			fprintf(stream, "MOV $0, %s\n", scratch_name(e->reg));
			fprintf(stream, "%s\n", label_name(done_label));
			break;
		case EXPR_LE:
			expr_codegen(e->left, stream);
			expr_codegen(e->right, stream);
			fprintf(stream, "CMP %s, %s\n", scratch_name(e->left->reg), scratch_name(e->right->reg));
			true_label = label_create();
			done_label = label_create();
			fprintf(stream, "JLE %s\n", true_label);
			e->reg = scratch_alloc();
			fprintf(stream, "MOV $1, %s\n", scratch_name(e->reg));
			fprintf(stream, "JMP %s\n", done_label);
			fprintf(stream, "%s\n", true_label);
			fprintf(stream, "MOV $0, %s\n", scratch_name(e->reg));
			fprintf(stream, "%s\n", done_label);
			break;
		case EXPR_EQ:
			expr_codegen(e->left, stream);
			expr_codegen(e->right, stream);
			fprintf(stream, "CMP %s, %s\n", scratch_name(e->left->reg), scratch_name(e->right->reg));
			true_label = label_create();
			done_label = label_create();
			fprintf(stream, "JEQ %s\n", true_label);
			e->reg = scratch_alloc();
			fprintf(stream, "MOV $1, %s\n", scratch_name(e->reg));
			fprintf(stream, "JMP %s\n", done_label);
			fprintf(stream, "%s\n", true_label);
			fprintf(stream, "MOV $0, %s\n", scratch_name(e->reg));
			fprintf(stream, "%s\n", done_label);
			break;
		case EXPR_NEQ:
			expr_codegen(e->left, stream);
			expr_codegen(e->right, stream);
			fprintf(stream, "CMP %s, %s\n", scratch_name(e->left->reg), scratch_name(e->right->reg));
			true_label = label_create();
			done_label = label_create();
			fprintf(stream, "JNE %s\n", true_label);
			e->reg = scratch_alloc();
			fprintf(stream, "MOV $1, %s\n", scratch_name(e->reg));
			fprintf(stream, "JMP %s\n", done_label);
			fprintf(stream, "%s\n", true_label);
			fprintf(stream, "MOV $0, %s\n", scratch_name(e->reg));
			fprintf(stream, "%s\n", done_label);
			break;
		case EXPR_AND:
			expr_codegen(e->left, stream);
			expr_codegen(e->right, stream);
			fprintf(stream, "CMP $0, %s\n", scratch_name(e->left->reg));
			int fail_label = label_create();
			done_label = label_create();
			fprintf(stream, "JEQ %s\n", label_name(fail_label));
			fprintf(stream, "CMP $0, %s\n", scratch_name(e->right->reg));
			fprintf(stream, "JEQ %s\n", label_name(fail_label));
			e->reg = scratch_alloc();
			fprintf(stream, "MOV $0, %s\n", scratch_name(e->reg));
			fprintf(stream, "JMP %s\n", label_name(done_label));
			fprintf(stream, "%s\n", label_name(fail_label));
			fprintf(stream, "MOV $1, %s\n", scratch_name(e->reg));
			fprintf(stream, "%s\n", label_name(done_label));
			break;
		case EXPR_OR:
			expr_codegen(e->left, stream);
			expr_codegen(e->right, stream);
			fprintf(stream, "CMP $1, %s\n", scratch_name(e->left->reg));
			int success_label = label_create();
			done_label = label_create();
			fprintf(stream, "JEQ %s\n", label_name(success_label));
			fprintf(stream, "CMP $1, %s\n", scratch_name(e->right->reg));
			fprintf(stream, "JEQ %s\n", label_name(success_label));
			e->reg = scratch_alloc();
			fprintf(stream, "MOV $1, %s\n", scratch_name(e->reg));
			fprintf(stream, "JMP %s\n", label_name(done_label));
			fprintf(stream, "%s\n", label_name(success_label));
			fprintf(stream, "MOV $0, %s\n", scratch_name(e->reg));
			fprintf(stream, "%s\n", label_name(done_label));
			break;
		case EXPR_ASSIGN:
			expr_codegen(e->left, stream);
			expr_codegen(e->right, stream);
			fprintf(stream, "MOVQ %s, %s\n", scratch_name(e->right->reg), scratch_name(e->left->reg));
			if(e->left->kind == EXPR_NAME) {
				fprintf(stream, "MOVQ %s, %s\n", scratch_name(e->left->reg), symbol_codegen(e->left->symbol));
			} 
			scratch_free(e->right->reg);
			e->reg = e->left->reg;
			break;
		case EXPR_ARGLIST:
			expr_codegen(e->left, stream);
			fprintf(stream, "MOVQQ %s, %s\n", scratch_name(e->left->reg), func_labels(args));
			args++;
			while(e->right) {
				expr_codegen(e->right, stream);
				fprintf(stream, "MOVQQ %s\n, %", scratch_name(e->right->reg), func_labels(args));
				e=e->right;
				args++;
			} 
			break;
		case EXPR_FCALL:
			e->reg = scratch_alloc();
			fprintf(stream, "CALL %s\n", e->left->name);
			fprintf(stream, "MOV %rax, %s\n", scratch_name(e->reg));
			break;
		case EXPR_FCALL_ARGS:
			expr_codegen(e->left, stream);
			expr_codegen(e->right, stream);

			fprintf(stream, "PUSHQ %r10\n");
			fprintf(stream, "PUSHQ %r11\n");

			fprintf(stream, "CALL %s\n", e->left->name);

			fprintf(stream, "POPQ %r11\n");
			fprintf(stream, "POPQ %r10\n");

			fprintf(stream, "MOV %rax, %s\n", scratch_name(e->reg));
			break;
		case EXPR_PAREN:
			expr_codegen(e->right, stream);
			fprintf(stream, "MOV %rax, %s\n", scratch_name(e->reg));
			break;
		case EXPR_REF:
			expr_codegen(e->left, stream);
			fprintf(stream, "MOVQ %s, %rax\n", scratch_name(e->left->reg));
			fprintf(stream, "NOTQ %rax");
			break;
		case EXPR_NOT:
			expr_codegen(e->right, stream);
			fprintf(stream, "MOVQ %s, %rax\n", scratch_name(e->right->reg));
			fprintf(stream, "NOTQ %rax");
			fprintf(stream, "MOVQ %raw, %s\n", scratch_name(e->right->reg));
			break;
		case EXPR_NEG:
			expr_codegen(e->right, stream);
			scrtch = scratch_alloc();
			fprintf(stream, "MOVQ $-1, %s\n", scratch_name(scrtch));
			fprintf(stream, "MOVQ %s, %rax\n", scratch_name(e->right->reg));
			fprintf(stream, "IMULQ %s\n", scratch_name(scrtch));
			fprintf(stream, "MOVQ %rax, %s\n",  scratch_name(e->right->reg));
			scratch_free(scrtch);
			e->reg = e->right->reg;
			break;
		case EXPR_NAME:
			e->reg = scratch_alloc();
			if(e->symbol->type->kind == TYPE_STRING) {
				fprintf(stream, "MOVQ $%s, %s\n", symbol_codegen(e->symbol), scratch_name(e->reg));
			} else  {
				fprintf(stream, "MOVQ %s, %s\n", symbol_codegen(e->symbol), scratch_name(e->reg));
			}
			break;
		case EXPR_BOOLEAN_LITERAL:
		case EXPR_CHAR_LITERAL:
		case EXPR_INTEGER_LITERAL:
			e->reg = scratch_alloc();
			fprintf(stream, "MOVQ $%d, %s\n", e->literal_value, scratch_name(e->reg));
			break;
		case EXPR_STRING_LITERAL:
			fprintf(stream, "MOVQ $%s, %s\n", e->string_literal, scratch_name(e->reg));
			break;
		default:
			break;
	}

}
