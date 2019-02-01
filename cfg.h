#ifndef CFG_H
#define CFG_H

#include "ast.h"


#define NULL ((void *)0)

typedef enum {DBL, STR} elementType;


typedef union{
	char * svalue;
	double dvalue;
} elementValue;


typedef struct Elt {
	elementType etype;
	elementValue evalue;
} Element;


//Added some more instruction like (Raise, While ,Case , Loop )
typedef enum {PrintIdf, PrintString, AssignArith, AssignBool, IfThenArith, IfThenElseArith,
 IfThenElsifArith,IfThenElsifElseArith,For,Raise,While,Case,LoopNoExitWhen, LoopEwMiddle,
 LoopEwBegin, LoopEwEnd} Type_INST ;

typedef struct {
  char *name;      
  int  nbdecl;     
  Type typevar;    
  boolean initialisation; // attribut nécessaire à l'analyseur sémantique 
  Element valinit;      
  boolean utilisation; // attribut nécessaire à l'optimisateur de code (la non utilisation n'est pas boquante pour l'analyseur sémantique)
  int line; 
} varvalueType;

typedef struct {
  Type typename;    // CONST_IB.typename
  double valinit;      // CONST_IB.valinit
} constvalueType;

typedef struct {
  char *value;
} stringvalueType;

typedef struct {
  int line;  //TOKEN.line
} tokenvalueType;

typedef struct {
  Type typename;    // TYPE.typename
} typevalueType;

struct INST; // pré déclaration de la structure de stockage d'une instruction

struct LIST_INST;// pré déclaration de la structure de stockage d'une liste d'instruction

struct CASE_BODY; // pré déclaration de la structure de stockage d'un case de switch

struct WHEN_BODY;

struct LIST_ELSEIF;

typedef struct INST {
  Type_INST typeinst;
  union  {
    // PRINT idftoprint
    struct  {
      int rangvar; // cas (print IDF) indice de l'idf, à afficher, dans la table des symboles
      char * str; // cas (print STRING)
    } printnode;
    // left := right
    struct  {
      int rangvar; // indice de l'idf, où il faut affecter, dans la table des symboles
      //int right0; // la valeur boolenne à affecter // a nettoyer
      AST right; // l'expression à affecter (arithmétique ou booléenne)
    } assignnode;
    // IF ... THEN 
    struct  {
      int rangvar; // indice de l'idf à comparer, dans la table des symbole
      AST right; // l'expression à comparer
      int nbelseif;
      struct LIST_INST * thenlinst; // then list of instructions
      struct LIST_ELSEIF *listelsif;   //elsif list of instruction 
      struct LIST_INST * elselinst; // else list of instructions
    } ifnode;
    // for (index:= exp_min..exp_max) loop list_inst end loop;
    struct {
      int rangvar; // indice de l'index de la boucle
      int borneinf; // l'expression borne inf
      int bornesup; // l'expression borne sup
      struct LIST_INST * forbodylinst; // for body list of instructions
    } fornode;
      //while condition loop statement end loop; 
      struct{
        int rangvar;//indice de l'idf à comparer, dans la table des symbole
        AST right;//expression à comparer
        struct LIST_INST * whilebodylinst;
      }whilenode;
      // case ( x ) when 1 : ...  ; when 20 : ...  ; .... when others : ...  ; end case;
    struct {
      int rangvar; // indice de la variable du switch
      int nbwhens;
      struct WHEN_BODY *whens ; // pour les cases (SWITCH_BODY), tableau dynamique non trié de couples val- liste
      struct LIST_INST * defaultbodylinst ; // la liste d'instructions par défaut du case ( when others)
    } casenode ;
      //loop list_inst exit when condition list_inst end loop
    struct{
      struct LIST_INST * listinstavantexit; //liste des instruction avant exit ça peut etre null 
                                          //dans le cas ou exit est directement apres loop
      int rangvar; // indice de l'idf à comparer, dans la table des symbole = -1 si exit n'existe pas
      //double right; // la valeur de comparaison
      AST right;// l'expression à comparer null si exit n'existe pas
      struct LIST_INST * listinstapresexit;//liste des instruction apres exit ça peut etre null 
                                          //dans le cas ou exit est directement avant end loop
    } loopnode;
  } node;
} instvalueType;


/* typedef struct CASE_BODY {
	int value ; // la valeur du cas (doit être >= 0)
	struct LIST_INST * casebodylinst; // la liste d'instructions du cas
} casevaluelinstType; */

//WHEN_BODY ===> like the case in the switch statement
typedef struct WHEN_BODY {
	int value ; // la valeur du cas (doit être >= 0)
	struct LIST_INST * whenbodylinst; // la liste d'instructions du cas
} whenvaluelinstType;

typedef struct LIST_ELSEIF{
  int rangvar; // indice de l'idf à comparer, dans la table des symbole
  AST right; // l'expression à comparer
  struct LIST_INST * elsiflinst;
}elseifinstType;

typedef struct LIST_INST {
  struct INST first;
  struct LIST_INST * next;
} listinstvalueType;

typedef union {
  varvalueType varattribute;            //IDF.varattribute
  constvalueType constattribute;        // CONST_IB.constattribute
  Type typename;                        // TYPE.typename
  instvalueType instattribute;          // INST.instattribute
  listinstvalueType listinstattribute;  // LIST_INST.listinstattribute
  //AST expattribute;                     // EXP.expattribute
} valueType;

//#define YYSTYPE valueType

extern instvalueType* creer_instruction_printIdf(int rangvar);
extern instvalueType* creer_instruction_printString(char * s);
extern instvalueType* creer_instruction_affectation(int rangvar, AST * past);
//extern instvalueType* creer_instruction_if(int rangvar, constvalueType constattribute, listinstvalueType * plistthen, listinstvalueType * plistelse);
extern instvalueType* creer_instruction_if_then(int rangvar, AST * past,listinstvalueType * plistthen);
extern instvalueType* creer_instruction_if_then_else(int rangvar, AST * past,listinstvalueType * plistthen, listinstvalueType * plistelse);
extern instvalueType* creer_instruction_if_then_elsif(int rangvar, AST * past,int nbelsif, listinstvalueType * plistthen, listinstvalueType * plistelsif);
extern instvalueType* creer_instruction_if_then_elsif_else(int rangvar, AST * past,int nbelsif,listinstvalueType * plistelse, listinstvalueType * plistthen, listinstvalueType * plistelsif);

extern instvalueType* creer_instruction_for(int rangvar, int borneinf, int bornesup, listinstvalueType *plistfor);
// extern instvalueType* creer_instruction_switch(int rangvar, int nbcases, casevaluelinstType *cases, listinstvalueType *plistswitchdefaultbody);
extern instvalueType* creer_instruction_while(int rangvar, AST * past, listinstvalueType * plistwhile);
extern instvalueType* creer_instruction_case(int rangvar, int nbwhens, whenvaluelinstType *whens, listinstvalueType *plistcasedefaultbody);

extern instvalueType* creer_instruction_loopnoexitwhen(listinstvalueType * plistavantexit);
extern instvalueType* creer_instruction_loopewbegin(int rangvar, AST * past, listinstvalueType * plistapresexit);
extern instvalueType* creer_instruction_loopewmiddle(int rangvar, AST * past, listinstvalueType * plistavantexit, listinstvalueType * plistapresexit);
extern instvalueType* creer_instruction_loopewend(int rangvar, AST * past, listinstvalueType * plistavantexit);



extern void inserer_inst_en_queue(listinstvalueType * listinstattribute, instvalueType instattribute);

extern void inserer_inst_en_tete(listinstvalueType ** listinstattribute, instvalueType instattribute);

extern void afficher_inst(instvalueType instattribute);

extern void afficher_list_inst(listinstvalueType * plistinstattribute);

// extern void interpreter_inst(instvalueType instattribute);

// extern void interpreter_list_inst(listinstvalueType * listinstattribute);

extern double dvalue(Element e);

// pré-requis : e.etype == STR;
extern char * svalue(Element e);

extern void setdvalue(Element *e, double d);

extern void setsvalue(Element *e, char * s);

#endif
 
