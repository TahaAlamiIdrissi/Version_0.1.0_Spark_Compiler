#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pseudocode.h"
#include "cfg.h"
#include "tab_symb.h"
#include "error.h"
#include "vm.h"
#include "pile.h"
#include "error.h"

#define debug false

// auteur Karim Baïna, ENSIAS, Décembre 2010, mise à jour Décembre 2011, mise à jour Décembre 2018

// La machine virtuelle est définie par
// un jeu d'instruction : pseudocode
// une mémoire statique : tablesymbole (implicit dans tablesymb.c)
// une pile : pile ci-dessous

Pile * VM_STACK;

// inintialise la machine abstraite
void inintialiser_machine_abstraite(){
	VM_STACK = creer_pile();
}

// visite une pseudo insctruction  avec deux modes (interpretationMode, optimiserMode)
// cas interpretationMode : effectue l'interprétation de la pseudo instruction
// case optimiserMode : simule l'exécution et effectue l'opération de la pseudo instruction avec les vraies valeur simulées en mémoire statique et en pile, effectue le marquage de la pseudo instruction, et ne fait pas d'affichage du résultat de la pseudo instruction
void visiter_pseudo_instruction(struct pseudoinstruction *ppi, char ** next_label_name, boolean optimiserMode){
  Element op1, op2, resultat;

  // boolean optimiserMode = get_Mode_Optimisation( ); // on aurait pu récupérer la valur de la variable globale mais il aurait fallu faire un appel de focntion à chaque visite de pi ==> on l'a passe en parametre directement de la fonction actuelle

if (debug) printf("visiter_pseudo_instruction()\n");

  int* prangvar = (int*) malloc(sizeof(int)); 

  *next_label_name = NULL;
  
  if (debug) { printf("état pile avant interprétation instruction :"); afficher_pseudo_instruction( *ppi, false ); etat_pile( VM_STACK );}

  if (optimiserMode == true) marquer_instruction_visitee( ppi ); // // declare que l'instruction est visitee. l'optimisateur ne générera que les instructions visitees


  switch(ppi->codop){
    case DATA:
		if (debug) printf("DATA = \n");
		varvalueType nv;
		nv.name = (char*) malloc(sizeof(char) * strlen(ppi->param.nv.name)+1);
		if (debug) printf("------------------------------------------------------------->DATANAME = %s",ppi->param.nv.name);
		strcpy(nv.name, ppi->param.nv.name);
		switch (ppi->param.nv.value.etype){
			case DBL : nv.typevar = Double; setdvalue(&(nv.valinit), dvalue(ppi->param.nv.value)); break;
			case STR : nv.typevar = String; setsvalue(&(nv.valinit), svalue(ppi->param.nv.value)); break;
			default : if (debug) printf("-----------------BUG without ppi->param.nv.value.etype"); break;
		}

		ajouter_nouvelle_variable_a_TS(nv);		// alimenter la mémoire statique de la machine virtuelle
		break;

    case ADD: 
      		    if (debug) printf("ADD = \n");
		    if (debug) etat_pile(VM_STACK) ; 
    		    op1 = depiler(VM_STACK);
		    op2 = depiler(VM_STACK);
		    setdvalue(&resultat , dvalue(op1) + dvalue(op2)); /*resultat = op1 + op2;*/
		    empiler(VM_STACK, resultat);
		    if (debug) etat_pile(VM_STACK) ; 
		    break;

    case IDIV:      /* division entière */ 
		    if (debug) printf("IDIV = \n");
		    if (debug) etat_pile(VM_STACK) ; 
		    op1 = depiler(VM_STACK);
		    op2 = depiler(VM_STACK);
		    if (dvalue(op2) == 0) {
			creer_sm_erreur(DivisionbyZero, ppi->line, NULL); // line may be either IDIV pseudocode instruction line used by interpreter or initial / code operateur assigned by generator/oprimiser
			return ;
		    }
		    setdvalue(&resultat , (int)(dvalue(op1) / dvalue(op2))); /*resultat = (int) (op1 / op2);*/
	            empiler(VM_STACK, resultat);

                    if (debug) etat_pile(VM_STACK) ; 
                    break;

    case DDIV:      /* division réelle */
		    if (debug) printf("DDIV = \n");
		    if (debug) etat_pile(VM_STACK) ; 
		    op1 = depiler(VM_STACK);
		    op2 = depiler(VM_STACK); 
		    if (dvalue(op2) == 0) {
			creer_sm_erreur(DivisionbyZero, ppi->line, NULL); // line may be either IDIV pseudocode instruction line used by interpreter or initial / code operateur assigned by generator/oprimiser
			return ;
		    }
		    setdvalue(&resultat , (double)(dvalue(op1) / dvalue(op2))); /*resultat = (double) op1 / (double) op2;*/
                    empiler(VM_STACK, resultat);
                    if (debug) etat_pile(VM_STACK) ; 
                    break;

    case _MULT:  
		if (debug) printf("MULT = \n");
		if (debug) etat_pile(VM_STACK) ; 
		op1 = depiler(VM_STACK);
		op2 = depiler(VM_STACK);
		setdvalue(&resultat , dvalue(op1) * dvalue(op2)); /*resultat = op1 * op2;*/
		empiler(VM_STACK, resultat);
		if (debug) etat_pile(VM_STACK) ; 		
		break;

    case SUB:
		if (debug) printf("SUB = \n");
	        if (debug) etat_pile(VM_STACK) ;
	        op1 = depiler(VM_STACK);
		op2 = depiler(VM_STACK);
		setdvalue(&resultat , dvalue(op1) - dvalue(op2)); /*resultat = op1 - op2;*/
		empiler(VM_STACK, resultat);
		if (debug) etat_pile(VM_STACK) ;
		break;

    case LOAD: 
		//if (debug) {printf("Etat Memoire Statique avant interpretation(LOAD) :\n"); afficherTS();}
 
		if (inTS(ppi->param.var, prangvar) != true) {
			if (debug) {printf("BUGGGG %s n'est pas :\n", ppi->param.var); afficherTS();}
		}else{   if (debug) printf("%s est à l'indice %d :\n",  ppi->param.var, *prangvar);

			switch (typevar(*prangvar)){
			case String : setsvalue(&resultat, svalue(valinit(*prangvar)));
					if (debug) printf("LOADS = %s %s\n", ppi->param.var, svalue(valinit(*prangvar)));			
					break;
			default : setdvalue(&resultat, dvalue(valinit(*prangvar))); 
					if (debug) printf("LOADI = %s %lf\n", ppi->param.var, dvalue(valinit(*prangvar)));			
					break;

			}
		}
	
		empiler(VM_STACK, resultat);
		//if (debug) {printf("Etat Memoire Statique apres interpretation(LOAD) :\n"); afficherTS();}
		break;

    case STORE:
		if (debug) printf("STORE = \n");

		//if (debug) {printf("Etat Memoire Statique avant interpretation(STORE) :\n"); afficherTS();}
		op1 = depiler(VM_STACK); 
		if (inTS(ppi->param.var, prangvar) != true) {
			if (debug) {printf("BUGGGG %s n'est pas :\n", ppi->param.var); afficherTS();}
		} else {

			switch (typevar(*prangvar)){
			case String : set_svalinit(*prangvar, svalue(op1)) ; break;
			default : set_dvalinit(*prangvar, dvalue(op1)) ; break;
			}
		}

		//if (debug) {printf("Etat Memoire Statique apres interpretation(STORE) :\n"); afficherTS();}

		break;

    case DUPL:
		if (debug) printf("DUPL = \n");
		op1 = depiler(VM_STACK);
		empiler(VM_STACK, op1);
		empiler(VM_STACK, op1);
		break;

    case PUSH:
		if (debug) printf("PUSH = \n");

		switch (ppi->param._const.etype){
			case DBL : setdvalue(&resultat, dvalue(ppi->param._const)); break;
			case STR : setsvalue(&resultat, svalue(ppi->param._const)); break;
		}

		empiler(VM_STACK, resultat);
		break;

    case SWAP:  
		if (debug) printf("SWAP = \n");
		op1 = depiler(VM_STACK); //LAST-IN
		op2 = depiler(VM_STACK); 
		empiler(VM_STACK, op1); //FIRST-IN
		empiler(VM_STACK, op2);
		break;
    
    case JNE:   
		if (debug) printf("JNE = \n");
		op1 = depiler(VM_STACK);
		op2 = depiler(VM_STACK);
		if (dvalue(op1) != dvalue(op2)) {*next_label_name = (char*) malloc (strlen(ppi->param.label_name)+1);
		strcpy(*next_label_name, ppi->param.label_name);}
		else {;}
		break;
    case JG:   
		if (debug) printf("JG = \n");
		op1 = depiler(VM_STACK);
		op2 = depiler(VM_STACK);
		if (dvalue(op1) > dvalue(op2)) {*next_label_name = (char*) malloc (strlen(ppi->param.label_name)+1); strcpy(*next_label_name, ppi->param.label_name);}
		else {;}
		break;

    case JMP:   if (debug) printf("JMP = *%s*\n", ppi->param.label_name);
		*next_label_name = (char*) malloc (strlen(ppi->param.label_name)+1);
		strcpy(*next_label_name, ppi->param.label_name);
		break; 

    case JEQ:   
		if (debug) printf("JEQ = \n");
		op1 = depiler(VM_STACK);
		op2 = depiler(VM_STACK);
		if (dvalue(op1) == dvalue(op2)) {*next_label_name = (char*) malloc (strlen(ppi->param.label_name)+1); strcpy(*next_label_name, ppi->param.label_name);}
		else {;}
		break;


    case PRNTI:  if (debug) printf("PRNTI = \n");
		op1 = depiler(VM_STACK);
		if (optimiserMode == false) {
			switch (op1.etype){
			case DBL : if (debug) {printf("----------------->DBL!!!!!!");} printf("%lf\n", dvalue(op1)); break;
			case STR : if (strcmp(svalue(op1),"")!=0) printf("%s\n", substring(svalue(op1), 2, strlen(svalue(op1))-2) ); break;
			}
		}
		break;

    case PRNTS:  if (debug) printf("PRNTS = \n");
		if ((debug) && (svalue(ppi->param._const) == NULL)) printf("------------------------------------------------------------->ppi->param._const NULL\n");
		if (optimiserMode == false) 
		if (strcmp(svalue(ppi->param._const),"")!=0) printf("%s\n", substring(svalue(ppi->param._const), 2, strlen(svalue(ppi->param._const))-2) ); break;

    case LABEL: if (debug) printf("LABEL = \n");
		if ((debug) && (strcmp("begin",ppi->param.label_name)==0)) {
			printf("-------------------------------------->Debut Memoire Statique :\n");
			afficherTS();
			printf("-------------------------------------->Fin Memoire Statique :\n");
		}

		; break; //l'effet de bord de la pasudo-instruction LABEL est de faire brancher à la pseudo-instruction suivante

    // case POP:   IGNORE
  }
  if (debug) printf("out visiter_pseudo_instruction()\n");
  if (debug) { printf("état pile après interprétation instruction :"); etat_pile( VM_STACK );}

}

// precondition pc <> NULL
// precondition le pseudo-code se termine en un temps fini
// optimise un pseudocode en éliminant les dead paths
void optimiser_pseudo_code(pseudocode pc){
if (debug) printf("optimiser_pseudo_code()\n");
 boolean optimiserMode = true;
 visiter_pseudo_code( pc , optimiserMode ) ;
if (debug) printf("out optimiser_pseudo_code()\n");
}


// precondition pc <> NULL
// interprète un pseudocode
void interpreter_pseudo_code(pseudocode pc){
if (debug) printf("interpreter_pseudo_code()\n");
 boolean optimiserMode = false;
 visiter_pseudo_code( pc , optimiserMode ) ;
if (debug) printf("out interpreter_pseudo_code()\n");
}

// precondition pc <> NULL
// visite un pseudocode selon deux modes (non optimisedMode, optimiserMode)
// cas non optimisedMode : effectue l'interprétation du pseudo code
// case optimiserMode : simule l'exécution et effectue les opérations avec les vraies valeur simulées en mémoire statique et en pile, effectue le marquage des pseudo instrutions visitees du pseudocode, et ne fait pas d'affichage des résultats

void visiter_pseudo_code(pseudocode pc, boolean optimiserMode){
if (debug) printf("visiter_pseudo_code()\n"); 
static pseudocode debut_code = NULL;

// initialisation de la variable statique par la première ligne de pseudocode nécessaire à la recherche du branchement en arrière
if (debut_code == NULL) debut_code = pc;

 char ** next_label_name = (char **) malloc(sizeof (char*));

 if (pc != NULL){

    visiter_pseudo_instruction(&(pc->first), next_label_name, optimiserMode);    // on passe un pointeur pour pouvoir marquer que l'interpréteur visite la pseudoinstruction

    if (*next_label_name == NULL) visiter_pseudo_code(pc->next, optimiserMode); // Il n y a pas de branchement !!

    else{ // JNE, JG, ou JMP ==> effectuer un branchement (recherche en O(n) depuis le début du code peut être optimisé par un accès direct au label O(1))
	struct pseudocodenode * compteur_ordinal = debut_code; // pc->next;
	//printf("LE LABEL RECHERCHE == %s", *next_label_name);
	while ( (compteur_ordinal->first.codop != LABEL) || 
		(strcmp(compteur_ordinal->first.param.label_name, *next_label_name) != 0) ) { // (compteur_ordinal ne peut pas == NULL) s'il vient y avoir un JMP/JNE, JG dans le code (par construction)
		//afficher_pseudo_instruction(compteur_ordinal->first);   	
		compteur_ordinal = compteur_ordinal->next;
	}
        if (optimiserMode == true) marquer_instruction_visitee( &(compteur_ordinal->first) ); // marquer que le label a été visité

	// ici : compteur_ordinal->first.codop == LABEL && strcmp(compteur_ordinal->first.param.label_name, next_label_name) == 0)
	visiter_pseudo_code( compteur_ordinal->next, optimiserMode ); // branchement
    }
 }
if (debug) printf("out visiter_pseudo_code()\n");
}
