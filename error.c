#include "error.h"
#include "analyser.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define NBERRMAX 100

static int NBERRORS = 0;

// invariant : NBERRORS == NBERRSX + NBERRSM

static int NBERRSM = 0;
static int NBERRSX = 0;


/*static int NBERRDECL = 0;
static int NBERRINST = 0;*/


static error * ERRORS[NBERRMAX*3];


/*static smerror * ERDECL[NBERRMAX];
static smerror * ERINST[NBERRMAX];
static sxerror * ERSX[NBERRMAX];
*/

void afficher_sm_erreur(SemanticErrorType et, int line, char* name){
  if (name != NULL) printf("line %d : %s : %s ", line, Tableau_sx_sm_erreurs[et] , name);
  else printf("line %d : SemanticError ", line);
}

void creer_sm_erreur(SemanticErrorType et, int line, char* name){
  smerror * e = (smerror*) malloc (sizeof (smerror) );

  if (name != NULL){
	  e->name = (char *) malloc (strlen(name));
	  strcpy(e->name, name);
  }

  e->line = line;
  e->smerrort = et;

  ERRORS[NBERRORS] = (error*) malloc (sizeof (error) );

  ERRORS[NBERRORS]->errortype=SemanticError;      
  ERRORS[NBERRORS]->errorattribute.smerr= e   ;

  NBERRORS++;
  NBERRSM++;
}

void creer_sx_erreur(SyntacticErrorType et, int line){
    sxerror * e = (sxerror*) malloc (sizeof (sxerror) );
    e->line = line;
    e->sxerrort = et;

    //ERSX[NBERRSX++]= e;
    
    ERRORS[NBERRORS] = (error*) malloc (sizeof (error) );
    ERRORS[NBERRORS]->errortype=SyntacticError;      
    ERRORS[NBERRORS]->errorattribute.sxerr= e;

    NBERRORS++;
    NBERRSX++; // on dénombre le nombres des erreurs syntaxiques
}


void afficher_sx_erreur(SyntacticErrorType et, int line){
  printf("line %d : Syntaxic error : %s", line , Tableau_sx_sm_erreurs[et]);
}

// obsolete
void afficher_sx_erreurs(){
/*int nberr = 0;
  while (nberr < NBERRSX) {
    afficher_sx_erreur(ERSX[nberr]->errort, ERSX[nberr]->line);
    nberr++;
  }*/
   int i;
   for (i=0; i<NBERRORS; i++){
	switch (ERRORS[ i ]->errortype){
	case	SyntacticError: afficher_sx_erreur(ERRORS[i]->errorattribute.sxerr->sxerrort, ERRORS[i]->errorattribute.sxerr->line); break;
	}
   }
}

int nombre_sx_erreurs(){
 return NBERRSX;
}

void afficher_sm_erreurs(){
/*  int i = 0;

  while (i < NBERRORS) {
    afficher_sm_erreur(ERDECL[idecl]->errort, ERDECL[idecl]->line, ERDECL[idecl]->name);
    idecl++;
  }

 while (iinst < NBERRINST) {
    afficher_sm_erreur(ERINST[iinst]->errort, ERINST[iinst]->line, ERINST[iinst]->name);
    iinst++;
  }*/
    int i;
    for (i=0; i<NBERRORS; i++){
	switch (ERRORS[ i ]->errortype){
	case	SemanticError :     afficher_sm_erreur(ERRORS[i]->errorattribute.smerr->smerrort, ERRORS[i]->errorattribute.smerr->line, ERRORS[i]->errorattribute.smerr->name);break;
	}
    }
}

/*void creer_sm_erreur_instruction(SemanticErrorType et, int line, char* name){
    //ERINST[NBERRINST++]= creer_sm_erreur(et, line, name);   

    ERRORS[NBERRORS++]->errortype=SemanticError;      
    ERRORS[NBERRORS]->errorattribute.smerr= creer_sm_erreur(et, line, name);   ;
}

void creer_sm_erreur_declaration(SemanticErrorType et, int line, char* name){
   //ERDECL[NBERRDECL++]= creer_sm_erreur(et, line, name); 

   ERRORS[NBERRORS++]->errortype=SemanticError;      
   ERRORS[NBERRORS]->errorattribute.smerr= creer_sm_erreur(et, line, name);   ;
}*/

int nombre_sm_erreurs(){
  //return  NBERRDECL + NBERRINST;
	return  NBERRSM;
}


void afficher_erreurs(){
int i;

for (i=0; i<NBERRORS; i++){
	switch (ERRORS[ i ]->errortype){
	case	SemanticError :     afficher_sm_erreur(ERRORS[i]->errorattribute.smerr->smerrort, ERRORS[i]->errorattribute.smerr->line, ERRORS[i]->errorattribute.smerr->name);break;
	case	SyntacticError: afficher_sx_erreur(ERRORS[i]->errorattribute.sxerr->sxerrort, ERRORS[i]->errorattribute.sxerr->line); break;
	}
}
}

