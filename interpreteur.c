#include <stdio.h>
#include <string.h>
#include "interpreteur.h"
#include "pseudocode.h"
#include "vm.h"
#include "ast.h"
#include "tab_symb.h"
#include "i18n.h"

#define debug false

#include <stdlib.h>

// @auteur Karim Baïna, ENSIAS, Décembre 2010, Mise à jour Décembre 2011, Mise à jour Décembre 2018

/* 
 PSEUDOCODE : DATA BEGIN LISTE_INST_PSEUDOCODE END
 LISTE_INST_PSEUDOCODE : INST_PSEUDOCODE LISTE_INST_AUX_PSEUDOCODE
 LISTE_INST_AUX_PSEUDOCODE : LISTE_INST_PSEUDOCODE | epsilon
 DATA : DATA_ITEM DATA_AUX 
 DATA_AUX : DATA | epsilon
 DATA_ITEM : idfop CONST
 CONST : inumberop | dnumberopc| cstringop
 INST_PSEUDOCODE :
	ADDOP 		|
	DIVOP		|
	DUPLOP		|
	JMPOP LABELOP	|
	JNEOP LABELOP	|
	JGOP LABELOP	|
	JEQOP LABELOP	|
	LABELOP ':'	|
	LOADOP IDFOP	|
	MULTOP		|
	PRINTIDFOP		|
	PRINTSTROP		|
	PUSHOP CONST 	|
	SUBOP		|
	STOREOP	IDFOP	|
	SWAPOP
*/

boolean _pseudocode(pseudocode * ppc);

boolean _liste_inst_pc(pseudocode * ppc);
boolean _liste_inst_aux_pc(pseudocode * ppc);
boolean _inst_pc(struct pseudoinstruction ** ppi);

boolean _data(pseudocode * ppc);
boolean _data_aux(pseudocode * ppc);
boolean _data_item(struct pseudoinstruction ** ppi);
boolean _const(Element * pe);

typetokenOp _lire_token();

extern int yylex();

typetokenOp token; 
boolean follow_token = false;

varvalueType labelattribute;
varvalueType varattribute;
constvalueType constattribute;

tokenvalueType tokenattribute;
stringvalueType stringattribute;

int rangvar;
boolean semanticerror = false;

int main(int argc, char ** argv){
	pseudocode * ppc_element_fictif = (pseudocode *) malloc (sizeof(pseudocode));
	pseudocode * ppc = (pseudocode *) malloc (sizeof(pseudocode));

	*ppc_element_fictif = (pseudocode) malloc (sizeof(struct pseudocodenode)); // une sentinelle (vide) qu'il faudra enlever avant l'interprétation

	if (debug) printf("Chargement du Pseudo Code :\n");

	if (argc == 2)
		{if (strcmp(argv[1],"-v")==0) printf("%s %s\n",dictionary[MessageDeBienvenueI][get_LANGUE_COURANTE()], VERSION);}
 	else{

	token = _lire_token();
	if (_pseudocode(ppc_element_fictif) == true) {
		if (debug) ("0 erreurs syntaxiques\n");

		if (debug) afficherTS();
			
		*ppc = (*ppc_element_fictif)->next;

		if (debug) { printf("Affichage du Pseudo Code chargé :\n"); afficher_pseudo_code(*ppc);}
		// élimination de l'élément fictif (sentinelle)		

		if (debug) printf("Initialisation de la machine virtuelle :\n");
		inintialiser_machine_abstraite();

		if (debug) printf("Interprétation du pseudocode :\n");
		interpreter_pseudo_code(*ppc);

	}else {
		printf("plusieurs erreurs syntaxiques\n");
		/* printf("%d erreurs syntaxiques\n", nombre_sx_erreurs());
		afficher_sx_erreurs(); */
	}
	}
	
return 0;
}
// PSEUDOCODE : DATA BEGINOP LISTE_INST_PSEUDOCODE ENDOP
boolean _pseudocode(pseudocode * ppc){
	boolean result;
	if (debug) printf("pseudocode()\n");

	if (_data(ppc)) {

		token = _lire_token();
		if (token == BEGINOP){

			pseudocode pcbegin = (pseudocode) malloc(sizeof(struct pseudocodenode));
			pcbegin->first.codop = LABEL;
			pcbegin->first.param.label_name = (char*) malloc(6);
			strcpy(pcbegin->first.param.label_name, "begin");
			pcbegin->next = NULL;

			inserer_code_en_queue(*ppc, pcbegin);

			token = _lire_token();
			if (_liste_inst_pc(ppc)) {
				token = _lire_token();
				if (token == ENDOP){
				
					pseudocode pcend = (pseudocode) malloc(sizeof(struct pseudocodenode));
					pcend->first.codop = LABEL;
					pcend->first.param.label_name = (char*) malloc(4);
					strcpy(pcend->first.param.label_name, "end");
					pcend->next = NULL;					

					inserer_code_en_queue(*ppc,pcend);

					result = true;
				} else {
					result = false;
				}
			}else result = false;
		}else{			
			result = false;
		}
	}else result = false;
	
	if (debug) printf("out of pseudocode()\n");
	return result;
}

//  LISTE_INST_PSEUDOCODE : INST_PSEUDOCODE LISTE_INST_AUX_PSEUDOCODE
boolean _liste_inst_pc(pseudocode * ppc){
	boolean result;
	if (debug) printf("list_inst_pc()\n");

	struct pseudoinstruction ** ppi = (struct pseudoinstruction **) malloc (sizeof(struct pseudoinstruction *));
	pseudocode * ppc_aux = (pseudocode *) malloc (sizeof(pseudocode));

	*ppi = NULL;
	(*ppc_aux) = (pseudocode) malloc (sizeof(struct pseudocodenode));

	if (_inst_pc(ppi)) {
		(*ppc_aux)->first = **ppi;
		(*ppc_aux)->next = NULL;
		token = _lire_token();
		if (_liste_inst_aux_pc(ppc_aux) == true){
			inserer_code_en_queue(*ppc, *ppc_aux);
			result = true;
		}else result = false;
	}else result = false;

	if (debug) printf("out of list_inst_pc()\n");
	return result;
}

// LISTE_INST_AUX_PSEUDOCODE : LISTE_INST_PSEUDOCODE | epsilon
// NULLABLE(LISTE_INST_AUX_PSEUDOCODE) = true
// follow(LISTE_INST_AUX_PSEUDOCODE) = {ENDOP}
// first(LISTE_INST_AUX_PSEUDOCODE) = first(LISTE_INST_PSEUDOCODE)
boolean _liste_inst_aux_pc(pseudocode * ppc){
        boolean result;
	if (debug) printf("list_inst_aux_pc()\n");

	if (token == ENDOP){
		// _yyless("\tend\n");
		follow_token = true;
		result = true;
	} else if (_liste_inst_pc(ppc) == true){
		result = true;
	} else result = false;

	if (debug) printf("out of list_inst_aux_pc()\n");	
	return result;
}

/*   INST_PSEUDOCODE :
	ADDOP 		|
	IDIVOP		|
	DDIVOP          |
	DUPLOP		|
	LABELOP	        |
	MULTOP		|
	PRINTIDFOP	|
	PRINTSTROP CSTRINGOP	|
	SUBOP		|
	SWAPOP          |
	STOREOP	IDFOP	|
	JMPOP IDFOP	|
	JNEOP IDFOP	|
	JGOP IDFOP	|
	JEQOP IDFOP	|
	LOADOP IDFOP	|
	PUSHOP CONST 	
*/
boolean _inst_pc(struct pseudoinstruction ** ppi){
	boolean result;
	if (debug) printf("inst_pc()\n");

	// ppi est déjà allouée, mais pas *ppi
	(*ppi) = (struct pseudoinstruction *) malloc(sizeof (struct pseudoinstruction));

	if (token == ADDOP){		
		(*ppi)->codop = ADD;
		result = true;
	}else if (token == IDIVOP){		
		(*ppi)->codop = IDIV;
		result = true;
	}else if (token == DDIVOP){		
		(*ppi)->codop = DDIV;
		result = true;
	}else if (token == DUPLOP){		
		(*ppi)->codop = DUPL;
		result = true;
	}else if (token == LABELOP){		
		(*ppi)->codop = LABEL;
		(*ppi)->param.label_name = (char*) malloc(strlen(labelattribute.name)+1);
		strcpy((*ppi)->param.label_name, labelattribute.name);
		result = true;
	}else if (token == MULTOP){		
		(*ppi)->codop = _MULT;
		result = true;
	}else if (token == PRINTIDFOP){		
		(*ppi)->codop = PRNTI;
		result = true;
	}else if (token == PRINTSTROP){
		(*ppi)->codop = PRNTS;
		token = _lire_token();
		if (token == CSTRINGOP){
			setsvalue( &(*ppi)->param._const, stringattribute.value);
			result = true;
		}else result = false;		
	}else if (token == SUBOP){		
		(*ppi)->codop = SUB;
		result = true;
	}else if (token == SWAPOP){		
		(*ppi)->codop = SWAP;
		result = true;
        }else if (token == STOREOP){
		(*ppi)->codop = STORE;
		token = _lire_token();
		if (token == IDFOP){
			(*ppi)->param.var = (char*) malloc(sizeof(char) * strlen(varattribute.name)+1);
			strcpy((*ppi)->param.var, varattribute.name);
			result = true;
		}else result = false;
	}else if (token == LOADOP){
		(*ppi)->codop = LOAD;
		token = _lire_token();
		if (token == IDFOP){
			(*ppi)->param.var = (char*) malloc(sizeof(char) * strlen(varattribute.name)+1);
			strcpy((*ppi)->param.var, varattribute.name);
			result = true;
		}else result = false;
	}else if (token == JMPOP){
		(*ppi)->codop = JMP;
		token = _lire_token();
		if (token == IDFOP){
			(*ppi)->param.label_name = (char*) malloc(sizeof(char) * strlen(varattribute.name)+1);
			strcpy((*ppi)->param.label_name, varattribute.name);
			result = true;
		}else result = false;
	}else if (token == JNEOP){
		(*ppi)->codop = JNE;
		token = _lire_token();
		if (token == IDFOP){
			(*ppi)->param.label_name = (char*) malloc(sizeof(char) * strlen(varattribute.name)+1);
			strcpy((*ppi)->param.label_name, varattribute.name);
			result = true;
		}else result = false;
	}else if (token == JEQOP){
		(*ppi)->codop = JEQ;
		token = _lire_token();
		if (token == IDFOP){
			(*ppi)->param.label_name = (char*) malloc(sizeof(char) * strlen(varattribute.name)+1);
			strcpy((*ppi)->param.label_name, varattribute.name);
			result = true;
		}else result = false;
	}else if (token == JGOP){
		(*ppi)->codop = JG;
		token = _lire_token();
		if (token == IDFOP){
			(*ppi)->param.label_name = (char*) malloc(sizeof(char) * strlen(varattribute.name)+1);
			strcpy((*ppi)->param.label_name, varattribute.name);
			result = true;
		}else result = false;		
	}else if (token == PUSHOP){ // PUSH "......"
		Element e;
		(*ppi)->codop = PUSH;
		token = _lire_token();
		if (_const( &e )) {
			switch(e.etype){
				case DBL : setdvalue(&((*ppi)->param._const), dvalue(e)); break;
				case STR : setsvalue(&((*ppi)->param._const), svalue(e)); break;
			}
			if (debug) {printf("après PUSH--->"); afficher_pseudo_instruction(**ppi, false);}
			result = true;
		}else result = false;
	}else result = false;

	(*ppi)->visitee = false; // toute pseudoinstruction est par defaut pas encore visitee !! jusqu'au passage de l'interpréteur

	if (debug) printf("out inst_pc()\n");
return result;
}

// DATA : DATA_ITEM DATA_AUX 
boolean _data(pseudocode * ppc){
	boolean result;

	if (debug) printf("data()\n");
	struct pseudoinstruction ** ppi = (struct pseudoinstruction **) malloc(sizeof(struct pseudoinstruction *));
	pseudocode * ppc_aux = (pseudocode *) malloc (sizeof(pseudocode));

	*ppi = NULL;

	(*ppc_aux) = (pseudocode) malloc (sizeof(struct pseudocodenode));

	if (_data_item(ppi)){
		(*ppc_aux)->first = **ppi;
		(*ppc_aux)->next = NULL;
		token = _lire_token();
		if (_data_aux(ppc_aux)){
			inserer_code_en_queue(*ppc, *ppc_aux);
			result = true;
		}else result = false;
	}else result = false;

	if (debug) printf("out of data()\n");
	return result;
}

// DATA_AUX : DATA | epsilon
// NULLABLE(DATA_AUX) = true
// follow(DATA_AUX) = {beginop}
// first(DATA_AUX) = first(DATA_AUX) = {idf}
boolean _data_aux(pseudocode * ppc){
	boolean result;
	if (debug) printf("data_aux()\n");

	if ( (token == BEGINOP) ){
		follow_token = true;
		result = true;
	}else if (_data(ppc)){
		result = true;
	}else{
		/* printf("TOKE = %d\n",token);
		creer_sx_erreur(BeginExpected, tokenattribute.line); */
		result = false;
	}

	if (debug) printf("out of data_aux()\n");
	return result;
}

// DATA_ITEM : idfop CONST
boolean _data_item(struct pseudoinstruction ** ppi) {
	boolean result;
	Element e;
	// ppi  est alloué mais pas *ppi
	*ppi = (struct pseudoinstruction *) malloc(sizeof(struct pseudoinstruction));

	if (debug) printf("data_item()\n");

	if ( (token == IDFOP) ) {
		(*ppi)->codop = DATA;

		(*ppi)->param.nv.name = (char*) malloc(sizeof(char) * (strlen(varattribute.name)+1));
		strcpy((*ppi)->param.nv.name, varattribute.name);
		
		token = _lire_token();
		if (_const(&e) == true){
			switch(e.etype){
				case DBL : setdvalue(&((*ppi)->param.nv.value), dvalue(e)); break;
				case STR : setsvalue(&((*ppi)->param.nv.value), svalue(e)); break;
			}
			result = true;
		}else result = false;
	}else result = false;

	if (debug) printf("out of data_item()\n");
	return result;
}

// CONST : inumberop | dnumberop | cstringop
boolean _const(Element * pe) {
	boolean result;
	if (debug) printf("const()\n");

	if (token == INUMBEROP) {
		if (debug) printf("INUMBER =>%lf", constattribute.valinit);
		setdvalue(pe, constattribute.valinit);

		//setdvalue(&((*ppi)->param._const) , constattribute.valinit);
		result = true;	
	}else if (token == DNUMBEROP) {
		if (debug) printf("DNUMBER => %lf\n", constattribute.valinit);
		setdvalue(pe, constattribute.valinit);

		//setdvalue(&((*ppi)->param._const) , constattribute.valinit);
		result = true;
	}else if (token == CSTRINGOP) {
		//setsvalue(&((*ppi)->param._const) , stringattribute.value);
		setsvalue(pe, stringattribute.value);

		result = true;
	} else result = false;

	if (debug) printf("out of const()\n");
	return result;
}

// lit le prochain token s'il n'a pas déjà été lu par le prédicat d'un nullable
typetokenOp _lire_token(){
	if (follow_token){
		follow_token = false;
		return token;
	}else{ 
		return (typetokenOp) yylex();
	}
}

void set_label_attributes(char *name, int line){
	labelattribute.name = (char *)malloc(sizeof(char) * (strlen(name)+1));
	strcpy(labelattribute.name, name);
	labelattribute.line = line;
}

void set_idf_attributes(char *name, int line){
	if (debug) printf("set_idf_attributes(%s, %d)\n",name, line);
	varattribute.name = (char *)malloc(sizeof(char) * (strlen(name)+1));
	strcpy(varattribute.name, name);
	varattribute.line = line;
	if (debug) printf("out set_idf_attributes()\n");
}

void set_number_attributes(double value){
	if (debug) printf("set_number_attributes(%lf)\n", value);
	constattribute.valinit = value;
	if (debug) printf("out set_number_attributes()\n", value);
}

void set_string_attributes(char * s){
	stringattribute.value = (char *)malloc((strlen(s)+1) * sizeof(char));
	strcpy(stringattribute.value, s);
}

void set_token_attributes(int line){
	tokenattribute.line = line;
}
