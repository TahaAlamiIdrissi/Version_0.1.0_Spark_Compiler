
#ifndef ANALYSER_H
#define ANALYSER_H


#include "ast.h"


typedef enum {
    INTEGER, BOOLEAN, FLOAT, NATURAL, RECORD, ARRAY, CONST, INDEX, STRING,
    WITH, IS, PROCEDURE, BODY, FUNCTION, TYPE, SUBTYPE, ACCESS,
    AND, NOT, OR,
    PRIVATE, PROJECT, NEW, PACKAGE, PROTECTED, LIMITED,
    PRAGMA, RANGE, OF, ENTRY, ABS, ELEMENT,
    BEG, END, EXIT,
    IF, THEN, ELSE, ELSIF,
    IN, OUT, IN_OUT,
    INPUT, OUTPUT, DECLARE,
    IN_OUT_, PROOF_IN,
    RAISE, GENERIC,
    RETURN, OTHERS, ALL, SOME,
    FALSE, TRUE, WHEN, WHEN_OTHERS,
    MOD, XOR,
    LOOP, WHILE, FOR, CASE,
    POSITIVE,
    TAGGED, OVERRIDING, TASK,
    SEMI_COLON, COMMA, COLON, POPEN, PCLOSE, POINT, APOSTROPHE,
    GREATER, EQUAL, LESS, LESS_OR_EQ, GREATER_OR_EQ,
    ARROW, AFFECTATION, INTERVAL,
    DIFFERENT, MINUS, PLUS, ORINRANGE, MULT, DIV , POWER, AMPERSAND,
    IDF, FNUMBER, INUMBER, CHARACTER, CHAR, CONSTANT, CSTR, PUT, PUT_LINE
    

} typetoken;

extern char* yytext;
extern int yylex();
extern int yylineno;

typetoken _read_token();

boolean _prog();

boolean _decl();
boolean _modifier();
boolean _list_idf();
boolean _add_assignment();
boolean _type();

boolean _list_decl();

boolean _loop();
boolean _loop_type();
boolean _for_loop();
boolean _while_loop();
boolean _other_loop();
boolean _index_type();
boolean _extra_statement();
boolean _loop_statement();
boolean _loop_condition();

boolean _list_inst();
boolean _list_inst_aux();
boolean _inst();
boolean _put_statement();
boolean _assignment_statement();
boolean _assignment();
boolean _expression();
boolean _value();
boolean _if_statement();
boolean _else_statement();

boolean _condition_bool();
boolean _condition_arith();
boolean _operator_arith();
boolean _cond();
boolean _list_cond();
boolean _list_cond_aux();
boolean _or_list_cond_aux();
boolean _xor_list_cond_aux();
boolean _and_list_cond_aux();
boolean _global_cond();
boolean _global_cond_aux();
boolean _or_global_cond_aux();
boolean _xor_global_cond_aux();
boolean _and_global_cond_aux();



boolean _case_statement();
boolean _case_body();

boolean _addsub(AST *past);
boolean _addsubaux(AST *past);
boolean _multdiv(AST *past);
boolean _multdivaux(AST *past);
boolean _aux(AST *past);

void set_idf_attributes(char *name, int line);
void set_number_attributes(double value);
void set_string_attributes(char * s);
void set_token_attributes(int line);

static boolean optimisationMode = false; // by default the optimizer is not ON

void set_Mode_Optimisation( boolean optimisationMode );
boolean get_Mode_Optimisation( );

static boolean leftAssociativity = false; // by default the semantic analyser takes leftAssociativity for +, -, *, /

void set_Left_Associativity( boolean leftAssociativity );
boolean get_Left_Associativity( );

#endif
