#ifndef PSEUDO_CODE_H
#define PSEUDO_CODE_H

#include "cfg.h"

// @auteur Karim Baïna, ENSIAS, Décembre 2010 updated in December 2018

//typedef enum {false, true} boolean;

typedef enum {DATA, ADD, IDIV, DDIV, DUPL, JMP, JNE, JG, JEQ, LABEL, LOAD, _MULT, POP, PRNTI, PRNTS, PUSH, SUB, STORE, SWAP} CODOP; // MULT existe déja comme token

typedef struct pseudocodenode * pseudocode;

struct namevalue {
	char * name;
	Element value; // double or string
	boolean utilisee; // nécessaire pour l'optimisateur de code
};

typedef union {
  char * var; // eg. LOAD X
  Element _const; // eg. print "....." ou PUSH 6 ou PUSH "......." (prevu dans le pseudocode mais pas encore dans le générateur (pour X = "...." print X))
  char * label_name; // eg. JMP endif:
  struct namevalue nv; // stockage de la data
} Param;


struct pseudoinstruction{
  CODOP codop;
  Param param;
  int line;   //la ligne permet de tracer les problèmes d'opération (exemple division par zéro)
  boolean visitee; // necessaire pour l'optimisation
};

struct pseudocodenode{
  struct pseudoinstruction first;
  struct pseudocodenode * next;
};

// precondition pc1 <> NULL et pc2 <> NULL
// insere pc2 en queue de pc1
void inserer_code_en_queue(pseudocode pc1, pseudocode pc2);

// affiche une pseudo insctruction selon le mode de visite (interpretationMode=0, optimisationMode=1)
void afficher_pseudo_instruction(struct pseudoinstruction pi , boolean pseudocodeVisitMode);


// precondition pc <> NULL
// afiche un pseudocode optimise (que les pseudoinstructions visitees)
void afficher_pseudo_code_optimise(pseudocode pc);


// precondition pc <> NULL
// afiche un pseudocode
void afficher_pseudo_code(pseudocode pc);


// declare que l'instruction est visitee
// l'optimisateur ne générera que les instructions visitees
void marquer_instruction_visitee(struct pseudoinstruction * ppi);

pseudocode generer_pseudo_code_ast(AST ast);

pseudocode generer_pseudo_code_inst(instvalueType instattribute);
     
pseudocode generer_pseudo_code_list_inst(listinstvalueType * plistinstattribute);

pseudocode generer_pseudo_code(listinstvalueType * plistinstattribute);

#endif
