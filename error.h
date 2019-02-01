#ifndef ERROR_H
#define ERROR_H

// Types nécessaires à la gestion des erreurs

typedef enum {
PROCEDUREEXPECTED=0 , 
IDFEXPECTED=1 , 
ISEXPECTED=2 , 
BEGINEXPECTED=3 , 
ENDEXPECTED=4 , 
SEMICOLONEXPECTED=5 , 
COLONEXPECTED=6 , 
COMMAEXPECTED=7 , 
TYPEEXPECTED=8 , 
POPENEXPECTED=9 , 
PCLOSEEXPECTED=10, 
VALUEEXPECTED=11 , 
AFFECTATIONEXPECTED=12 , 
LOOPEXPECTED=13 , 
INEXPECTED=14 , 
INDEXTYPEEXPECTED=15 , 
WHENEXPECTED=16 , 
THENEXPECTED=17 ,  
CONDITIONEXPECTED=18 , 
WHENOTHERSEXPECTED=19 , 
ARROWEXPECTED=20 , 
NONCATEGORIZED=22 } SyntacticErrorType;

static char * Tableau_sx_sm_erreurs[45] = {
"'procedure' expected" ,
"'identifier' expected" ,
"'is' expected" ,
"'begin' expected" ,
"'end'  expected" ,
"';' expected" ,
 "':' expected" ,
"',' expected" ,
"a type is expected " ,
"'(' expected " ,
"')' expected" ,
"a value expected" ,
"':=' expected " ,
"'loop' expected" ,
"'in' expected" ,
"missing or wrong for index type" ,
"'when' expected" ,
"'then' expected " ,
"'if' expected",
"missing or wrong condition" ,
"missing 'when others'" ,
"'=>' expected " ,
"syntactic error" ,
"NotDeclared",
"AlreadyDeclared",
"NotInitialized",
"BadlyInitialised",
"IncompatibleAssignType",
"IncompatibleCompType",
"IncompatibleOperationType",
"IncompatibleForIndexType",
"IncompatibleForSwitch",
"MultipleValueForSwitch",
"DivisionbyZero"};

typedef enum {NotDeclared=23,
               //NotDefined, // variable utilisee sans définition : difficilement gérable car il faut détecter malgré la non inialisaton que la variable n'a pas été affecté par la suite
	      AlreadyDeclared=24,
	      NotInitialized=25,
	      BadlyInitialised=26,
	      IncompatibleAssignType=27,
	      IncompatibleCompType=28,
	      IncompatibleOperationType=29,
	      IncompatibleForIndexType=30,
	      IncompatibleForSwitch=31,
	      MultipleValueForSwitch=32,
	      DivisionbyZero=33} SemanticErrorType;

typedef enum {SyntacticError=34 , SemanticError=35} ErrorType;

typedef struct {
  char *name;
  int line;
  SemanticErrorType smerrort;
} smerror;

typedef struct {
  int line;
  SyntacticErrorType sxerrort;
} sxerror;


typedef union{
	sxerror * sxerr ;
	smerror * smerr ;
} errorAttribute;

typedef struct{
ErrorType errortype;
errorAttribute errorattribute;
} error ;


extern void creer_sx_erreur(SyntacticErrorType et, int line);

extern void afficher_sx_erreur(SyntacticErrorType et, int line);

extern int nombre_sx_erreurs();

extern void creer_sm_erreur(SemanticErrorType et, int line, char * name);



extern void afficher_sm_erreur(SemanticErrorType et, int line, char* name);

extern int nombre_sm_erreurs();

extern void afficher_sx_erreurs();
extern void afficher_sm_erreurs();
extern void afficher_erreurs();

#endif
