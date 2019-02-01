#ifndef ERROR_H
#define ERROR_H

// Types nécessaires à la gestion des erreurs

typedef enum {PROCEDUREEXPECTED=0 , 
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
WHILEEXPECTED=14 , 
FOREXPECTED=15 , 
INEXPECTED=16 , 
INDEXTYPEEXPECTED=17 , 
EXITEXPECTED=18 , 
WHENEXPECTED=19 , 
IFEXPECTED=20 , 
THENEXPECTED=21 , 
ELSEEXPECTED=22 , 
ELSIFEXPECTED=23 , 
CONDITIONEXPECTED=24 , 
COMPARATOREXPECTED=25 , 
WHENOTHERSEXPECTED=26 , 
ARROWEXPECTED=27 , 
CASEEXPECTED=28 , 
MINUSEXPECTED=29 , 
PLUSEXPECTED=30 , 
MULTEXPECTED=31 , 
DIVEXPECTED=32 ,
NONCATEGORIZED=33 } SyntacticErrorType;

char * Tableau_sx_sm_erreurs[45] = {"missing 'procedure'" , "missing 'identifier'" , "missing 'is'" , "missing 'begin'" , "missing 'end'" , "missing ';'" , "missing ':'" , "missing ','" , "missing 'type'" , "missing ')'" , "missing '('" , "missing 'value'" , "missing ':='" , "missing 'loop'" , "missing 'while'" , "missing 'for'" , "missing 'in'" , "missing 'for index type'" , "missing 'exit'" , "missing 'when'" , "missing 'if'" , "missing 'then'" , "missing 'elsif'" , "missing 'condition'" , "missing 'comparator'" , "missing 'when others'" , "missing '=>'" , "missing 'case'" , "missing '-'" , "missing '+'" , "missing '*'" , "missing '/'" , "non categorized" ,"NotDeclared","AlreadyDeclared","NotInitialized","BadlyInitialised","IncompatibleAssignType","IncompatibleCompType",
"IncompatibleCompType","IncompatibleOperationType","IncompatibleForIndexType","IncompatibleForIndexType","MultipleValueForSwitch","DivisionbyZero"};

typedef enum {NotDeclared=34,
               //NotDefined, // variable utilisee sans définition : difficilement gérable car il faut détecter malgré la non inialisaton que la variable n'a pas été affecté par la suite
	      AlreadyDeclared=35,
	      NotInitialized=36,
	      BadlyInitialised=37,
	      IncompatibleAssignType=38,
	      IncompatibleCompType=39,
	      IncompatibleOperationType=40,
	      IncompatibleForIndexType=41,
	      IncompatibleForSwitch=42,
	      MultipleValueForSwitch=43,
	      DivisionbyZero=44} SemanticErrorType;

typedef enum {SyntacticError=45 , SemanticError=46} ErrorType;

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
