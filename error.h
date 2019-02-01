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
IFEXPECTED=18,  
CONDITIONEXPECTED=19 , 
WHENOTHERSEXPECTED=20 , 
ARROWEXPECTED=21 ,
STATEMENTEXPECTED=22,
CASEEXPECTED=23, 
NONCATEGORIZED=24 } SyntacticErrorType;

static char * Tableau_sx_sm_erreurs[45] = {
"'procedure' expected\n" ,
"'identifier' expected\n" ,
"'is' expected\n" ,
"'begin' expected\n" ,
"'end'  expected\n" ,
"';' expected\n" ,
 "':' expected\n" ,
"',' expected\n" ,
"a type is expected \n" ,
"'(' expected \n" ,
"')' expected\n" ,
"a value expected\n" ,
"':=' expected \n" ,
"'loop' expected\n" ,
"'in' expected\n" ,
"missing or wrong for index type\n" ,
"'when' expected\n" ,
"'then' expected \n" ,
"'if' expected\n",
"missing or wrong condition\n" ,
"missing 'when others'\n" ,
"'=>' expected \n" ,
"a statement is expected\n",
"'case expected'\n",
"syntactic error\n" ,
"NotDeclared\n",
"AlreadyDeclared\n",
"NotInitialized\n",
"BadlyInitialised\n",
"IncompatibleAssignType\n",
"IncompatibleCompType\n",
"IncompatibleOperationType\n",
"IncompatibleForIndexType\n",
"IncompatibleForSwitch\n",
"MultipleValueForSwitch\n",
"DivisionbyZero\n"};

typedef enum {NotDeclared=25,
               //NotDefined, // variable utilisee sans définition : difficilement gérable car il faut détecter malgré la non inialisaton que la variable n'a pas été affecté par la suite
	      AlreadyDeclared=26,
	      NotInitialized=27,
	      BadlyInitialised=28,
	      IncompatibleAssignType=29,
	      IncompatibleCompType=30,
	      IncompatibleOperationType=31,
	      IncompatibleForIndexType=32,
	      IncompatibleForSwitch=33,
	      MultipleValueForSwitch=34,
	      DivisionbyZero=35} SemanticErrorType;

typedef enum {SyntacticError=36 , SemanticError=37} ErrorType;

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
