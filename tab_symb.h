#ifndef TAB_SYMB_H
#define TAB_SYMB_H

#include "cfg.h"

extern int NBRMAXIDF; // nombre max des idf dans une declaration ex : a, b, ..., z : integer := 5; 
extern varvalueType varattribute[100];
// select *
extern void afficherTS();

// pr�dicat de recherche
extern boolean inTS(char * varname, int* rangvar);

extern boolean inListIdf(char * varname);

// constructeur d'une nouvelle variable
extern void ajouter_nouvelle_variable_a_TS(varvalueType newvar);

extern void incrementer_nombre_declarations(int rangvar);
extern char* name(int rang);
extern Type typevar(int rang);
extern boolean est_definie(int rang);
extern boolean est_utilisee(int rang);

extern void definir(int rang);

extern int nombre_variables();

// additional support functions
/* reverse: reverse string s in place */
extern void reverse(char s[]);

/* myitoa: convert n to characters in s */
extern char * myitoa(int);

/* RAZ de la TS */
extern void reinitialiserTS();

//extern boolean inPreviousCases(casevaluelinstType * cases, int nbcases, int value);

extern char * lowercase(char * s);
// pr�-requis : e.etype == DBL;


extern Element value(char *idf);

// évalation par parcours postfixé de l'arbre abstrait
// précondition : ast<> NULL 
extern double evaluer (AST ast);

extern Element valinit(int rang);

extern void set_svalinit(int rang, char *s);

extern void set_svalinit(int rang, char *s);

extern char *substring(char *string, int position, int length);

#endif
