#include "tab_symb.h"
#include "pseudocode.h"
#include "analyser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define debug false
#ifndef NULL
#define NULL ((void *) 0)
#endif

void afficher_pseudo_instruction(struct pseudoinstruction pi, boolean optimisationMode){

  // l'instruction est affichee (normalement) si mode normal
  // par contre, en mode optimisation, l'instruction ne sera affichée que si ell a été visitée par le simulateur

  if ( (optimisationMode == false) || ((optimisationMode == true) && (pi.visitee == true)) )  {

	  switch(pi.codop){
		  case ADD:    printf("ADD\n");  break;
		  case IDIV:   printf("IDIV\n"); break;
		  case DDIV:   printf("DDIV\n"); break;
		  case DUPL:   printf("DUPL\n"); break;
		  case LABEL:  printf("%s:\n",pi.param.label_name); break;
		  case LOAD:   printf("LOAD "); printf("%s\n",pi.param.var); break;
		  case _MULT:  printf("MULT\n"); break;
		  case POP:    printf("POP\n");  break;
		  case PUSH:   printf("PUSH "); 
			switch(pi.param._const.etype){
					case DBL:printf("%lf\n",dvalue(pi.param._const)); break; 
					case STR:printf("%s\n",svalue(pi.param._const)); break;
			}
				break;
		  case SUB:    printf("SUB\n");  break;
		  case STORE:  printf("STORE "); printf("%s\n",pi.param.var); break;
		  case SWAP:   printf("SWAP\n"); break;
		  case PRNTI:   printf("PRINTI\n");break;
		  case PRNTS:   printf("PRINTS ");printf("%s\n",svalue(pi.param._const));break; 
		  case JNE:    printf("JNE "); printf("%s\n",pi.param.label_name);break; 
		  case JG:    printf("JG "); printf("%s\n",pi.param.label_name);break; 
		  case JMP:    printf("JMP "); printf("%s\n",pi.param.label_name);break; 
		  case JEQ:    printf("JEQ "); printf("%s\n",pi.param.label_name);break; 
		  case DATA: if ((optimisationMode == false) || ((optimisationMode == true) && (pi.param.nv.utilisee == true))) { 
				// la date est affichee normalement en mode normal
				// par contre en mode optimisation, la data ne sera affichee que si elle est utilisée par les instructions de programme
				printf("%s ",pi.param.nv.name);
				switch(pi.param.nv.value.etype){
					case DBL:	printf("%lf\n",dvalue(pi.param.nv.value)); break;
					case STR:	if (strcmp(svalue(pi.param.nv.value),"")==0) {printf("\"");printf("\"\n");}
							else printf("%s\n",svalue(pi.param.nv.value));
							break;
				}

			      } break; 
  	}
   }
}

// precondition pc <> NULL
void  afficher_pseudo_code(pseudocode  pc){
   boolean optimisationMode = false;
   if (pc != NULL){
    afficher_pseudo_instruction(pc->first, optimisationMode);    
    afficher_pseudo_code(pc->next);
  }
}

// precondition pc <> NULL
// afiche un pseudocode optimise (que les pseudoinstructions visitees)
void afficher_pseudo_code_optimise(pseudocode pc){
   boolean optimisationMode = true;
   if (pc != NULL){
    afficher_pseudo_instruction(pc->first, optimisationMode);    
    afficher_pseudo_code_optimise(pc->next);
  }
}

// precondition pc1 <> NULL et pc2 <> NULL
void inserer_code_en_queue(pseudocode  pc1, pseudocode pc2){
  
  if (debug) { 
    printf("inserer_code_en_queue(X, Y)\n");
    printf("X= ");afficher_pseudo_code(pc1);
    printf("Y= ");afficher_pseudo_code(pc2);
  }

  if (pc1->next == NULL) {
    pc1->next = pc2;
  }else{
    pseudocode pc = pc1;
    while(pc->next != NULL) {
      pc = pc->next;
    }
    
    pc->next = pc2;
  }
  if (debug) { 
    printf("X.Y=");afficher_pseudo_code(pc1);
    printf("\n");
    printf("out inserer_code_en_queue()\n");
  }
}

// declare que l'instruction est visitee
// l'optimisateur ne générera que les instructions visitees
void marquer_instruction_visitee(struct pseudoinstruction * ppi){
	ppi->visitee = true;
}



// génère le pseudo-code relatif à l'AST
// précondition ast <> NULL
pseudocode generer_pseudo_code_ast(AST ast){
  pseudocode  pc = (pseudocode)malloc(sizeof (struct pseudocodenode));
  pseudocode valg, vald,pcswap;

  switch(ast->typeexp) {
  case NB :
    pc->first.codop = PUSH;
    setdvalue(&(pc->first.param._const), ast->noeud.nombre);
    break;

  case _IDF :
    pc->first.codop = LOAD;
    pc->first.param.var = ast->noeud.idf;
    break;

  case OP :   
    	    valg = generer_pseudo_code_ast(arbre_gauche(ast));
	    vald = generer_pseudo_code_ast(arbre_droit(ast));   
    pc->first.line = ast->noeud.op.line; // la ligne permet de tracer les problèmes d'opération (exemple division par zéro)
    switch(ast->noeud.op.top){
    case plus :// if (debug) printf("%lf + %lf == %lf\n",valg, vald, valg + vald);
      pc->first.codop = ADD; break;
      pc->next = NULL;

    case moins : // if (debug) printf("%lf - %lf == %lf\n",valg, vald, valg - vald);
      pc->first.codop = SUB; // opération non commutative
      pc->next = NULL;
      if ( (leftAssociativity == true) || (optimisationMode == false)) {  
	      pcswap = (pseudocode)malloc(sizeof (struct pseudocodenode));
	      pcswap->first.codop = SWAP;
	      pcswap->next = pc;
	      pc = pcswap;
      }
      break;

    case mult : // if (debug) printf("%lf * %lf == %lf\n",valg, vald, valg * vald);
      pc->first.codop = _MULT;
      pc->next = NULL;
      break;

    case _div : // if (debug) printf("%lf - %lf == %lf\n",valg, vald, valg - vald);
      if (ast->typename == Float){
	      // division réelle
	      pc->first.codop = DDIV; // opération non commutative
      }else{
	      // division entière
	      pc->first.codop = IDIV; // opération non commutative
      }
      pc->next = NULL;
      if ( (leftAssociativity == true) || (optimisationMode == false)) {  // the swap is not necessary when D-G-R right-left-root generation (right associativity with optimising stack)
	      pcswap = (pseudocode)malloc(sizeof (struct pseudocodenode));
	      pcswap->first.codop = SWAP;
	      pcswap->next = pc;
	      pc = pcswap;
      }
      break;
    }
   
    if ( (leftAssociativity == true) || (optimisationMode == false)) { // G-D-R left-right-root when left associativity or (non optimising stack mode with all kinds of associativities)
	    inserer_code_en_queue(valg, vald);
	    inserer_code_en_queue(vald, pc);        
	    pc = valg;

    } else  {      // D-G-R right-left-root when right associativity with optimising stack
	    inserer_code_en_queue(vald, valg);
	    inserer_code_en_queue(valg, pc);            
	    pc = vald;
    }

    break;
  }
 
  return pc;
}

pseudocode generer_pseudo_code_inst(instvalueType instattribute){
  static int label_index = 0;
  pseudocode  pc, pc1,pc2,pc3,pc31,pc4,pc5,pc6,pc7, pc8, pc9, pc10, pc11, pc12;
  pseudocode  rexpcode;
  char * label_name;
  char *label_num;

  switch(instattribute.typeinst){
  case Case :

    if (debug) printf("Case");
    pc = NULL;

    label_num = myitoa ( label_index++ );

    // if value of X, exist among switch value then jump to lvi and exit else jump to default
    int i = 0;
    int value;
    pseudocode  valuelabelcode, previousvaluelabelcode= NULL, firstvaluelabelcode=NULL;
    char ** labelname = (char**) malloc ((sizeof (char *)) * instattribute.node.casenode.nbwhens);


    for (i=0; i < instattribute.node.casenode.nbwhens; i++){
	// chargement de la valeur de l'index du switch
	value = instattribute.node.casenode.whens[ i ].value;
	pc1 = (pseudocode)malloc(sizeof (struct pseudocodenode)); 
	if (pc == NULL) pc = pc1; // on se souvient de la tête de la liste à retourner à la fin
	pc1->first.codop = LOAD;
	pc1->first.param.var = name(instattribute.node.casenode.rangvar);
	pc1->next = NULL;

	pc2 = (pseudocode)malloc(sizeof (struct pseudocodenode));
	pc2->first.codop = PUSH;
	setdvalue(&(pc2->first.param._const) , value);

	pc2->next = NULL;
	pc1->next = pc2 ;

 	pc3 = (pseudocode)malloc(sizeof (struct pseudocodenode));    
    	pc3->first.codop = JEQ;
        labelname[ i ] = (char*) malloc(7+strlen(label_num)+strlen(myitoa(value)));
    	strcpy( labelname[ i ], "case");
    	strcat( labelname[ i ], "_");
    	strcat( labelname[ i ], label_num);
    	strcat( labelname[ i ], "_");
    	strcat( labelname[ i ], myitoa(value));
	pc3->first.param.label_name = labelname[ i ];
 
	pc3->next = NULL;
	pc2->next = pc3;
	if (previousvaluelabelcode != NULL) inserer_code_en_queue(previousvaluelabelcode,pc1);
  
	previousvaluelabelcode = pc1;
    }

	pc4 = (pseudocode)malloc(sizeof (struct pseudocodenode));    
    	pc4->first.codop = JMP; // 
        pc4->first.param.label_name = (char*) malloc(8+strlen(label_num));
    	strcpy( pc4->first.param.label_name, "when others");
    	strcat( pc4->first.param.label_name, label_num);
	
    	pc4->next = NULL;

	inserer_code_en_queue(pc,pc4);

    // generate liste des label lvi (foreach value vi)
    valuelabelcode, previousvaluelabelcode= NULL, firstvaluelabelcode=NULL;

    for (i=0; i < instattribute.node.casenode.nbwhens; i++){
	value = instattribute.node.casenode.whens[ i ].value;
        valuelabelcode = (pseudocode)malloc(sizeof (struct pseudocodenode));    
	if (firstvaluelabelcode == NULL) firstvaluelabelcode = valuelabelcode; //se rappeler de la tête de la liste
    	valuelabelcode->first.codop = LABEL;
    	valuelabelcode->first.param.label_name = labelname[ i ];
	valuelabelcode->next = generer_pseudo_code_list_inst(instattribute.node.casenode.whens[ i ].whenbodylinst );
	
	pc5 = (pseudocode)malloc(sizeof (struct pseudocodenode));    
    	pc5->first.codop = JMP; // 
        pc5->first.param.label_name = (char*) malloc(9+strlen(label_num));
        strcpy( pc5->first.param.label_name, "end case");
        strcat( pc5->first.param.label_name, label_num);

	inserer_code_en_queue(valuelabelcode, pc5);

	if (previousvaluelabelcode != NULL) inserer_code_en_queue(previousvaluelabelcode,valuelabelcode);
	previousvaluelabelcode = valuelabelcode;
    }

	inserer_code_en_queue(pc4,firstvaluelabelcode);

    // label default
    pc6 = (pseudocode)malloc(sizeof (struct pseudocodenode));
    pc6->first.codop = LABEL;
    pc6->first.param.label_name = pc4->first.param.label_name;
    pc6->next = generer_pseudo_code_list_inst(instattribute.node.casenode.defaultbodylinst);

		inserer_code_en_queue(firstvaluelabelcode, pc6);

    // endswitch
    pc7 = (pseudocode)malloc(sizeof (struct pseudocodenode));
    pc7->first.codop = LABEL;
    pc7->first.param.label_name = pc5->first.param.label_name;

    pc7->next = NULL;

		inserer_code_en_queue(pc6, pc7);

  break;

  case PrintIdf :
    if (debug) printf("PrintIdf");
    // NOP
    pc = (pseudocode)malloc(sizeof (struct pseudocodenode));
    pc->first.codop = LOAD;
    pc->first.param.var = name(instattribute.node.printnode.rangvar);
    rexpcode = (pseudocode)malloc(sizeof (struct pseudocodenode));
    rexpcode->first.codop = PRNTI;
    rexpcode->next = NULL;
    pc->next = rexpcode;
    break;

  case PrintString :
    if (debug) printf("PrintStr");
    // NOP
    pc = (pseudocode)malloc(sizeof (struct pseudocodenode));
    pc->first.codop = PRNTS;
    setsvalue(&(pc->first.param._const), instattribute.node.printnode.str);
    pc->next = NULL;
    break;
    
  case AssignArith :
    if (debug) printf("AssignArith");
    rexpcode = generer_pseudo_code_ast(instattribute.node.assignnode.right);
    if (debug) afficher_pseudo_code(rexpcode);
    pc = (pseudocode)malloc(sizeof (struct pseudocodenode));
    pc->first.codop = STORE;
    pc->first.param.var = name(instattribute.node.assignnode.rangvar);
    pc->next = NULL;
    inserer_code_en_queue(rexpcode, pc);
    pc = rexpcode;
    if (debug) afficher_pseudo_code(pc);
    break;

  case AssignBool :
    if (debug) printf("AssignBool");
    pc = (pseudocode)malloc(sizeof (struct pseudocodenode));
    pc->first.codop = PUSH;
    setdvalue(&(pc->first.param._const) , (double) (((instattribute.node.assignnode.right)->noeud).bool));

    pc1 = (pseudocode)malloc(sizeof (struct pseudocodenode));  
    pc1->first.codop = STORE;
    pc1->first.param.var = name(instattribute.node.assignnode.rangvar);
    pc1->next = NULL;

    pc->next = pc1;
   
    if (debug) afficher_pseudo_code(pc);
    break;
    
  case IfThenArith :
    if (debug) printf("If");
    
    pc = generer_pseudo_code_ast(instattribute.node.ifnode.right);

    pc1 = (pseudocode)malloc(sizeof (struct pseudocodenode));
    pc1->first.codop = LOAD;
    pc1->first.param.var = name(instattribute.node.ifnode.rangvar);
    pc1->next = NULL;

    pc2 = (pseudocode)malloc(sizeof (struct pseudocodenode));    
    pc2->first.codop = JNE;
    label_num=myitoa(label_index++);
    pc2->first.param.label_name = (char*) malloc(6+strlen(label_num));
    strcpy( pc2->first.param.label_name, "endif");
    strcat( pc2->first.param.label_name, label_num);

    if (debug)    printf("label == %s", pc2->first.param.label_name);
    pc2->next = NULL;

    pc3 = generer_pseudo_code_list_inst(instattribute.node.ifnode.thenlinst );

    // label endif
    pc4 = (pseudocode)malloc(sizeof (struct pseudocodenode));
    pc4->first.codop = LABEL;
    pc4->first.param.label_name = pc2->first.param.label_name;
    pc4->next = NULL;

    inserer_code_en_queue(pc3, pc4);
    pc2->next = pc3;
    pc1->next = pc2;
    inserer_code_en_queue(pc, pc1);

    if (debug)   {printf("debut-- ");afficher_pseudo_code(pc);printf(" --fin");}
    break;

  case IfThenElseArith :
    if (debug) printf("If");
    
    pc = generer_pseudo_code_ast(instattribute.node.ifnode.right);

    pc1 = (pseudocode)malloc(sizeof (struct pseudocodenode));
    pc1->first.codop = LOAD;
    pc1->first.param.var = name(instattribute.node.ifnode.rangvar);
    pc1->next = NULL;

    pc2 = (pseudocode)malloc(sizeof (struct pseudocodenode));    
    pc2->first.codop = JNE;
    label_num=myitoa(label_index++);
    pc2->first.param.label_name = (char*) malloc(6+strlen(label_num));
    strcpy( pc2->first.param.label_name, "else");
    strcat( pc2->first.param.label_name, label_num);
    if (debug)    printf("label == %s", pc2->first.param.label_name);
    pc2->next = NULL;

    pc3 = generer_pseudo_code_list_inst(instattribute.node.ifnode.thenlinst );

    pc31 = (pseudocode)malloc(sizeof (struct pseudocodenode));    
    pc31->first.codop = JMP;
    pc31->first.param.label_name = (char*) malloc(6+strlen(label_num));
    strcpy( pc31->first.param.label_name, "endif");
    strcat( pc31->first.param.label_name, label_num);

    pc4 = (pseudocode)malloc(sizeof (struct pseudocodenode));
    pc4->first.codop = LABEL;
    pc4->first.param.label_name = pc2->first.param.label_name;
    pc4->next = NULL;

    pc31->next = pc4;

    pc5 = generer_pseudo_code_list_inst(instattribute.node.ifnode.elselinst );   
    pc4->next = pc5;
   
    pc6 = (pseudocode)malloc(sizeof (struct pseudocodenode));
    pc6->first.codop = LABEL;
    pc6->first.param.label_name = (char*) malloc(strlen( pc31->first.param.label_name)+1);
    strcpy( pc6->first.param.label_name,  pc31->first.param.label_name);
    pc6->next = NULL;

    inserer_code_en_queue(pc5, pc6);
    inserer_code_en_queue(pc3, pc31);
    pc2->next = pc3;
    pc1->next = pc2;
    inserer_code_en_queue(pc, pc1);

    if (debug) {printf("debut-- ");afficher_pseudo_code(pc);printf(" --fin");}

    break;
   //If then elsif then else end if ----------------------------- 
    case IfThenElsifElseArith :

          if (debug) printf("If");
    
    pc = generer_pseudo_code_ast(instattribute.node.ifnode.right);

    pc1 = (pseudocode)malloc(sizeof (struct pseudocodenode)); 
    pc1->first.codop = LOAD;
    pc1->first.param.var = name(instattribute.node.ifnode.rangvar);
    pc1->next = NULL;
      inserer_code_en_queue(pc, pc1);

    pc2 = (pseudocode)malloc(sizeof (struct pseudocodenode));    
    pc2->first.codop = JEQ;
    label_num=myitoa(label_index++);
    pc2->first.param.label_name = (char*) malloc(6+strlen(label_num));
    strcpy( pc2->first.param.label_name, "then");
    strcat( pc2->first.param.label_name, label_num);
    if (debug)    printf("label == %s", pc2->first.param.label_name);
    pc2->next = NULL;
    pc1->next=pc2;

    for(i=0;i<instattribute.node.ifnode.nbelseif;i++){
      pc3=generer_pseudo_code_ast(instattribute.node.ifnode.listelsif[i].right);

      pc4=(pseudocode)malloc(sizeof (struct pseudocodenode));
      pc4->first.codop=LOAD;
      pc4->first.param.var=name(instattribute.node.ifnode.listelsif[i].rangvar);
      pc4->next=NULL;

      inserer_code_en_queue(pc3,pc4);

      pc5=(pseudocode)malloc(sizeof (struct pseudocodenode));
      pc5->first.codop=JEQ;
      label_num=myitoa(label_index++);
      labelname[ i ] = (char*) malloc(12+strlen(label_num));
    	strcpy( labelname[ i ], "elsif");
    	strcat( labelname[ i ], "_");
    	strcat( labelname[ i ], label_num);
    	strcat( labelname[ i ], "_");
    	strcat( labelname[ i ], "then");
      pc5->first.param.label_name = labelname[ i ];
      if (debug)    printf("label == %s", pc5->first.param.label_name);
      pc5->next = NULL;
      pc4->next=pc5;
    }

    pc6=(pseudocode)malloc(sizeof(struct pseudocodenode));
    pc6->first.codop=JMP;
    pc6->first.param.label_name = (char*) malloc(6+strlen(label_num));
    	strcpy( pc6->first.param.label_name, "else_");
    	strcat( pc6->first.param.label_name, label_num);

  //traitement des labels ..
  
  for(i=0 ; i<instattribute.node.ifnode.nbelseif ; i++){

      valuelabelcode = (pseudocode)malloc(sizeof (struct pseudocodenode));  

	if (firstvaluelabelcode == NULL)
      firstvaluelabelcode = valuelabelcode; //se rappeler de la tête de la liste
    	
      valuelabelcode->first.codop = LABEL;
    	valuelabelcode->first.param.label_name = labelname[ i ];
	    valuelabelcode->next = generer_pseudo_code_list_inst(instattribute.node.ifnode.listelsif[ i ].elsiflinst);

      pc7=(pseudocode)malloc(sizeof(struct pseudocodenode));
      pc7->first.codop=JMP;
      pc7->first.param.label_name=(char*)malloc(7+strlen(label_num));
      strcpy( pc7->first.param.label_name, "endif_");
      strcat( pc7->first.param.label_name, label_num);
      inserer_code_en_queue(valuelabelcode,pc7);

      if (previousvaluelabelcode != NULL) 
        inserer_code_en_queue(previousvaluelabelcode,valuelabelcode);

      	previousvaluelabelcode = valuelabelcode;
  }
  pc6->next = firstvaluelabelcode;

  pc8 = (pseudocode)malloc(sizeof(struct pseudocodenode));
  pc8->first.codop=LABEL;
  pc8->first.param.label_name = pc6->first.param.label_name;
  pc8->next=generer_pseudo_code_list_inst(instattribute.node.ifnode.elselinst);

    inserer_code_en_queue(firstvaluelabelcode,pc8);
  pc9 = (pseudocode)malloc(sizeof (struct pseudocodenode));
  pc9->first.codop=LABEL;
  pc9->first.param.label_name = pc7->first.param.label_name;
  pc9->next = NULL;

  inserer_code_en_queue(pc8,pc9);
//------------------------------------------------------------------------------------------------------------------------------------
        break;
    case While :
    // le début de loop
    pc = (pseudocode)malloc(sizeof (struct pseudocodenode));    
    pc->first.codop = LABEL;
    label_num=myitoa(label_index++);    
    pc->first.param.label_name = (char*) malloc(7+strlen(label_num));
    strcpy( pc->first.param.label_name, "while_");
    strcat( pc->first.param.label_name, label_num);

    // test si i = ast right 
    pc1 = generer_pseudo_code_ast(instattribute.node.loopnode.right);
    pc->next = pc1;

    //load idf
    pc2 = (pseudocode)malloc(sizeof (struct pseudocodenode));  
    pc2->first.codop = LOAD;
    pc2->first.param.var = name(instattribute.node.loopnode.rangvar);
    inserer_code_en_queue(pc1, pc2);

    //le jmp if not equal
    pc3 = (pseudocode)malloc(sizeof (struct pseudocodenode));    
    pc3->first.codop = JNE;
    label_num=myitoa(label_index++);
    pc3->first.param.label_name = (char*) malloc(9+strlen(label_num));
    strcpy( pc3->first.param.label_name, "end loop");
    strcat( pc3->first.param.label_name, label_num);
    if (debug)    printf("label == %s", pc2->first.param.label_name);
    pc2->next = pc3;

    // le corps du loop apres exit when
    pc4 = (pseudocode)malloc(sizeof (struct pseudocodenode));
    pc4 = generer_pseudo_code_list_inst(instattribute.node.loopnode.listinstapresexit);
    pc3->next = pc4;

    // l'itération (le jmp au début)
    pc5 = (pseudocode)malloc(sizeof (struct pseudocodenode));    
    pc5->first.codop = JMP;
    pc5->first.param.label_name = (char*) malloc(strlen( pc->first.param.label_name) + 1);
    strcpy( pc5->first.param.label_name, pc->first.param.label_name);
    inserer_code_en_queue(pc4, pc5);

    // la fin de loop
    pc6 = (pseudocode)malloc(sizeof (struct pseudocodenode));    
    pc6->first.codop = LABEL;
    pc6->first.param.label_name = (char*) malloc(strlen( pc3->first.param.label_name)+1);
    strcpy( pc6->first.param.label_name,  pc3->first.param.label_name);
    pc6->next = NULL;

    pc5->next = pc6;

    break;
    case IfThenElsifArith : 
         if (debug) printf("If");
    
    pc = generer_pseudo_code_ast(instattribute.node.ifnode.right);
    
    pc1=(pseudocode)malloc(sizeof(struct pseudocodenode));
    pc1->first.codop=LOAD;
    pc1->first.param.var=name(instattribute.node.ifnode.rangvar);
    pc1->next=NULL;

    inserer_code_en_queue(pc,pc1);

    pc2 = (pseudocode)malloc(sizeof (struct pseudocodenode));    
    pc2->first.codop = JEQ;
    label_num=myitoa(label_index++);
    pc2->first.param.label_name = (char*) malloc(6+strlen(label_num));
    strcpy( pc2->first.param.label_name, "then");
    strcat( pc2->first.param.label_name, label_num);
    if (debug)    printf("label == %s", pc2->first.param.label_name);
    pc2->next = NULL;

    pc1->next=pc2;

    //debut du traitement des elsifs
    //pseudocode  valuelabelcode, previousvaluelabelcode= NULL, firstvaluelabelcode=NULL;
    char ** labelname = (char**) malloc ((sizeof (char *)) * instattribute.node.ifnode.nbelseif);
    pseudocode pcNew=NULL;
    for (i=0; i < instattribute.node.ifnode.nbelseif; i++){

      pc3 = generer_pseudo_code_ast(instattribute.node.ifnode.listelsif[i].right);
      if (pcNew == NULL) pcNew = pc3; // on se souvient de la tête de la liste à retourner à la fin
      pc4->first.codop = LOAD;
      pc4->first.param.var = name(instattribute.node.ifnode.rangvar);
      inserer_code_en_queue(pc3, pc4)

      pc5 = (pseudocode)malloc(sizeof (struct pseudocodenode));    
    	pc5->first.codop = JEQ;
      labelname[ i ] = (char*) malloc(12+strlen(label_num));
    	strcpy( labelname[ i ], "elsif");
    	strcat( labelname[ i ], "_");
    	strcat( labelname[ i ], label_num);
    	strcat( labelname[ i ], "_");
    	strcat( labelname[ i ], "then");
      pc5->first.param.label_name = labelname[ i ];
      pc5->next = NULL;

      pc4->next = pc5;

      if (previousvaluelabelcode != NULL) inserer_code_en_queue(previousvaluelabelcode,pc3);
      previousvaluelabelcode = pc3;
    } 
    // THEN listinst
    pc6 = (pseudocode)malloc(sizeof (struct pseudocodenode));
    pc6->first.codop = LABEL;
    pc6->first.param.label_name = pc2->first.param.label_name;
    pc6->next = generer_pseudo_code_list_inst(instattribute.node.ifnode.thenlinst);
	  inserer_code_en_queue(pcNew,pc6);

    valuelabelcode, previousvaluelabelcode= NULL, firstvaluelabelcode=NULL;

    for (i=0; i < instattribute.node.ifnode.nbelseif; i++){

      valuelabelcode = (pseudocode)malloc(sizeof (struct pseudocodenode));    
	    if (firstvaluelabelcode == NULL) firstvaluelabelcode = valuelabelcode; //se rappeler de la tête de la liste
    	valuelabelcode->first.codop = LABEL;
    	valuelabelcode->first.param.label_name = labelname[ i ];
	    valuelabelcode->next = generer_pseudo_code_list_inst(instattribute.node.ifnode.listelsif[ i ].elsiflinst);
	
	    pc7 = (pseudocode)malloc(sizeof (struct pseudocodenode));    
    	pc7->first.codop = JMP; // 
      pc7->first.param.label_name = (char*) malloc(7+strlen(label_num));
      strcpy( pc7->first.param.label_name, "end if");
      strcat( pc7->first.param.label_name, label_num);

	    inserer_code_en_queue(valuelabelcode, pc7);

      if (previousvaluelabelcode != NULL) inserer_code_en_queue(previousvaluelabelcode,valuelabelcode);
      previousvaluelabelcode = valuelabelcode;
    }

	  inserer_code_en_queue(pc6,firstvaluelabelcode);

    pc8 = (pseudocode)malloc(sizeof (struct pseudocodenode));
    pc8->first.codop = LABEL;
    pc8->first.param.label_name = (char*) malloc(7+strlen(label_num));
    strcpy( pc7->first.param.label_name, "end if");
    strcat( pc7->first.param.label_name, label_num);
    pc8->next = NULL;

		inserer_code_en_queue(firstvaluelabelcode, pc8);
    break;
    case For:
    // affectation i = borneinf    
    pc = (pseudocode)malloc(sizeof (struct pseudocodenode));
    pc->first.codop = PUSH;
    setdvalue(&(pc->first.param._const) , (double) (instattribute.node.fornode.borneinf));

    pc1 = (pseudocode)malloc(sizeof (struct pseudocodenode));  
    pc1->first.codop = STORE;
    pc1->first.param.var = name(instattribute.node.fornode.rangvar);

    pc->next = pc1;

    // le début du for
    pc2 = (pseudocode)malloc(sizeof (struct pseudocodenode));    
    pc2->first.codop = LABEL;
    label_num=myitoa(label_index++);    
    pc2->first.param.label_name = (char*) malloc(4+strlen(label_num));
    strcpy( pc2->first.param.label_name, "for");
    strcat( pc2->first.param.label_name, label_num);

    pc1->next = pc2;

    // test si i > bornesup
    pc3 = (pseudocode)malloc(sizeof (struct pseudocodenode));  
    pc3->first.codop = PUSH;
    setdvalue( &(pc3->first.param._const), (double) (instattribute.node.fornode.bornesup));

    pc2->next = pc3;

    pc4 = (pseudocode)malloc(sizeof (struct pseudocodenode));  
    pc4->first.codop = LOAD;
    pc4->first.param.var = name(instattribute.node.fornode.rangvar);
    
    pc3->next = pc4;

    pc5 = (pseudocode)malloc(sizeof (struct pseudocodenode));    
    pc5->first.codop = JG;
    //label_num=myitoa(label_index++);   
    pc5->first.param.label_name = (char*) malloc(7+strlen(label_num));
    strcpy( pc5->first.param.label_name, "endfor");
    strcat( pc5->first.param.label_name, label_num);

    pc4->next = pc5;

    // le corps du for
    pc6 = generer_pseudo_code_list_inst(instattribute.node.fornode.forbodylinst);

    pc5->next = pc6;

    // l'incrémentation de l'indice

    pc7 = (pseudocode)malloc(sizeof (struct pseudocodenode));  
    pc7->first.codop = PUSH;
    setdvalue(&(pc7->first.param._const) , (double) 1);

    inserer_code_en_queue(pc6, pc7);

    pc8 = (pseudocode)malloc(sizeof (struct pseudocodenode));  
    pc8->first.codop = LOAD;
    pc8->first.param.var = name(instattribute.node.fornode.rangvar);
    
    pc7->next = pc8;

    pc9 = (pseudocode)malloc(sizeof (struct pseudocodenode));  
    pc9->first.codop = ADD;
    
    pc8->next = pc9;

    pc10 = (pseudocode)malloc(sizeof (struct pseudocodenode));  
    pc10->first.codop = STORE;
    pc10->first.param.var = name(instattribute.node.fornode.rangvar);

    pc9->next = pc10;

    // l'itération (le jmp au début)
    pc11 = (pseudocode)malloc(sizeof (struct pseudocodenode));    
    pc11->first.codop = JMP;
    pc11->first.param.label_name = (char*) malloc(strlen( pc2->first.param.label_name) + 1);
    strcpy( pc11->first.param.label_name, pc2->first.param.label_name);

    pc10->next = pc11;

    // la fin du for
    pc12 = (pseudocode)malloc(sizeof (struct pseudocodenode));    
    pc12->first.codop = LABEL;
    pc12->first.param.label_name = (char*) malloc(strlen( pc5->first.param.label_name)+1);
    strcpy( pc12->first.param.label_name,  pc5->first.param.label_name);
    pc12->next = NULL;

    pc11->next = pc12;

    break;

    case LoopNoExitWhen:

    // le début de loop
    pc = (pseudocode)malloc(sizeof (struct pseudocodenode));    
    pc->first.codop = LABEL;
    label_num=myitoa(label_index++);    
    pc->first.param.label_name = (char*) malloc(5+strlen(label_num));
    strcpy( pc->first.param.label_name, "loop");
    strcat( pc->first.param.label_name, label_num);

    // le corps du loop
    pc1 = (pseudocode)malloc(sizeof (struct pseudocodenode));
    pc1 = generer_pseudo_code_list_inst(instattribute.node.loopnode.listinstavantexit);
    pc->next = pc1;

    // l'itération (le jmp au début)
    pc2 = (pseudocode)malloc(sizeof (struct pseudocodenode));    
    pc2->first.codop = JMP;
    pc2->first.param.label_name = (char*) malloc(strlen( pc->first.param.label_name) + 1);
    strcpy( pc2->first.param.label_name, pc->first.param.label_name);
    inserer_code_en_queue(pc1, pc2);

    // la fin de loop
    pc3 = (pseudocode)malloc(sizeof (struct pseudocodenode));    
    pc3->first.codop = LABEL;
    pc3->first.param.label_name = (char*) malloc(9+strlen(label_num));
    strcpy( pc3->first.param.label_name, "end loop");
    strcat( pc3->first.param.label_name, label_num);
    pc3->next = NULL;
    pc2->next = pc3;

    break;

    case LoopEwBegin:

    // le début de loop
    pc = (pseudocode)malloc(sizeof (struct pseudocodenode));    
    pc->first.codop = LABEL;
    label_num=myitoa(label_index++);    
    pc->first.param.label_name = (char*) malloc(5+strlen(label_num));
    strcpy( pc->first.param.label_name, "loop");
    strcat( pc->first.param.label_name, label_num);

    // test si i = ast right 
    pc1 = generer_pseudo_code_ast(instattribute.node.loopnode.right);
    pc->next = pc1;

    //load idf
    pc2 = (pseudocode)malloc(sizeof (struct pseudocodenode));  
    pc2->first.codop = LOAD;
    pc2->first.param.var = name(instattribute.node.loopnode.rangvar);
    inserer_code_en_queue(pc1, pc2);

    //le jmp if not equal
    pc3 = (pseudocode)malloc(sizeof (struct pseudocodenode));    
    pc3->first.codop = JNE;
    label_num=myitoa(label_index++);
    pc3->first.param.label_name = (char*) malloc(9+strlen(label_num));
    strcpy( pc3->first.param.label_name, "end loop");
    strcat( pc3->first.param.label_name, label_num);
    if (debug)    printf("label == %s", pc2->first.param.label_name);
    pc2->next = pc3;

    // le corps du loop apres exit when
    pc4 = (pseudocode)malloc(sizeof (struct pseudocodenode));
    pc4 = generer_pseudo_code_list_inst(instattribute.node.loopnode.listinstapresexit);
    pc3->next = pc4;

    // l'itération (le jmp au début)
    pc5 = (pseudocode)malloc(sizeof (struct pseudocodenode));    
    pc5->first.codop = JMP;
    pc5->first.param.label_name = (char*) malloc(strlen( pc->first.param.label_name) + 1);
    strcpy( pc5->first.param.label_name, pc->first.param.label_name);
    inserer_code_en_queue(pc4, pc5);

    // la fin de loop
    pc6 = (pseudocode)malloc(sizeof (struct pseudocodenode));    
    pc6->first.codop = LABEL;
    pc6->first.param.label_name = (char*) malloc(strlen( pc3->first.param.label_name)+1);
    strcpy( pc6->first.param.label_name,  pc3->first.param.label_name);
    pc6->next = NULL;

    pc5->next = pc6;

    break;

    case LoopEwMiddle:

    // le début du loop
    pc = (pseudocode)malloc(sizeof (struct pseudocodenode));    
    pc->first.codop = LABEL;
    label_num=myitoa(label_index++);    
    pc->first.param.label_name = (char*) malloc(5+strlen(label_num));
    strcpy( pc->first.param.label_name, "loop");
    strcat( pc->first.param.label_name, label_num);

    // le corps du loop avant exit when
    pc1 = (pseudocode)malloc(sizeof (struct pseudocodenode));
    pc1 = generer_pseudo_code_list_inst(instattribute.node.loopnode.listinstavantexit);
    pc->next = pc1;

    // test si i = ast right 
    pc2 = generer_pseudo_code_ast(instattribute.node.loopnode.right);
    inserer_code_en_queue(pc1, pc2);

    //load idf
    pc3 = (pseudocode)malloc(sizeof (struct pseudocodenode));  
    pc3->first.codop = LOAD;
    pc3->first.param.var = name(instattribute.node.loopnode.rangvar);
    inserer_code_en_queue(pc2, pc3);

    //le jmp if not equal
    pc4 = (pseudocode)malloc(sizeof (struct pseudocodenode));    
    pc4->first.codop = JNE;
    label_num=myitoa(label_index++);
    pc4->first.param.label_name = (char*) malloc(9+strlen(label_num));
    strcpy( pc4->first.param.label_name, "end loop");
    strcat( pc4->first.param.label_name, label_num);
    if (debug)    printf("label == %s", pc2->first.param.label_name);
    pc3->next = pc4;

    // le corps du loop apres exit when
    pc5 = (pseudocode)malloc(sizeof (struct pseudocodenode));
    pc5 = generer_pseudo_code_list_inst(instattribute.node.loopnode.listinstapresexit);
    pc4->next = pc5;

    // l'itération (le jmp au début)
    pc6 = (pseudocode)malloc(sizeof (struct pseudocodenode));    
    pc6->first.codop = JMP;
    pc6->first.param.label_name = (char*) malloc(strlen( pc->first.param.label_name) + 1);
    strcpy( pc6->first.param.label_name, pc->first.param.label_name);
    inserer_code_en_queue(pc5, pc6);

    // la fin de loop
    pc7 = (pseudocode)malloc(sizeof (struct pseudocodenode));    
    pc7->first.codop = LABEL;
    pc7->first.param.label_name = (char*) malloc(strlen( pc4->first.param.label_name)+1);
    strcpy( pc7->first.param.label_name,  pc4->first.param.label_name);
    pc7->next = NULL;

    pc6->next = pc7;

    break;

    case LoopEwEnd:

    // le début du loop
    pc = (pseudocode)malloc(sizeof (struct pseudocodenode));    
    pc->first.codop = LABEL;
    label_num=myitoa(label_index++);    
    pc->first.param.label_name = (char*) malloc(5+strlen(label_num));
    strcpy( pc->first.param.label_name, "loop");
    strcat( pc->first.param.label_name, label_num);

    // le corps du loop avant exit when
    pc1 = (pseudocode)malloc(sizeof (struct pseudocodenode));
    pc1 = generer_pseudo_code_list_inst(instattribute.node.loopnode.listinstavantexit);
    pc->next = pc1;

    // test si i = ast right 
    pc2 = generer_pseudo_code_ast(instattribute.node.loopnode.right);
    inserer_code_en_queue(pc1, pc2);

    //load idf
    pc3 = (pseudocode)malloc(sizeof (struct pseudocodenode));  
    pc3->first.codop = LOAD;
    pc3->first.param.var = name(instattribute.node.loopnode.rangvar);
    inserer_code_en_queue(pc2, pc3);

    //le jmp if not equal
    pc4 = (pseudocode)malloc(sizeof (struct pseudocodenode));    
    pc4->first.codop = JNE;
    label_num=myitoa(label_index++);
    pc4->first.param.label_name = (char*) malloc(9+strlen(label_num));
    strcpy( pc4->first.param.label_name, "end loop");
    strcat( pc4->first.param.label_name, label_num);
    if (debug)    printf("label == %s", pc2->first.param.label_name);
    pc3->next = pc4;

    // l'itération (le jmp au début)
    pc5 = (pseudocode)malloc(sizeof (struct pseudocodenode));    
    pc5->first.codop = JMP;
    pc5->first.param.label_name = (char*) malloc(strlen( pc->first.param.label_name) + 1);
    strcpy( pc5->first.param.label_name, pc->first.param.label_name);
    pc4->next = pc5;

    // la fin de loop
    pc6 = (pseudocode)malloc(sizeof (struct pseudocodenode));    
    pc6->first.codop = LABEL;
    pc6->first.param.label_name = (char*) malloc(strlen( pc4->first.param.label_name)+1);
    strcpy( pc6->first.param.label_name,  pc4->first.param.label_name);
    pc6->next = NULL;

    pc5->next = pc6;

    break;

    
   
  }
  
  return pc;
}

pseudocode generer_pseudo_code_list_inst(listinstvalueType * plistinstattribute){
  pseudocode  pc1=NULL, pc2=NULL;
   if (debug) printf("in generer_pseudo_code_list_inst( )");
 if (plistinstattribute != NULL){
   pc1 = generer_pseudo_code_inst(plistinstattribute->first);    
   pc2 = generer_pseudo_code_list_inst(plistinstattribute->next);
   inserer_code_en_queue(pc1, pc2);
   if (debug) afficher_pseudo_code(pc1);
  }
   if (debug) printf("out generer_pseudo_code_list_inst( )");
 return pc1;
}

pseudocode generer_pseudo_code(listinstvalueType * plistinstattribute){
	int i=0;
	pseudocode pcresult;
	pseudocode tete = NULL;
	//générer le code pour les DATA
	if (debug) printf("Generer le code pour les %d DATA...\n", nombre_variables());
	for(i = nombre_variables() - 1; i >= 0; i--){
		if (debug) printf("%d ieme malloc.....\n",i);
		pcresult = (pseudocode) malloc(sizeof(struct pseudocodenode));
		if (debug) printf("DATA..\n");
		pcresult->first.codop = DATA;
		if (debug) {printf("malloc..pour....\n"); printf("%s\n", name(i));}
		pcresult->first.param.nv.name = (char*) malloc(strlen(name(i))+1);
		if (debug) printf("strcpy..\n");
		strcpy(pcresult->first.param.nv.name, name(i));
		pcresult->first.param.nv.value = valinit(i);
		if ((debug) && (pcresult->first.param.nv.value.etype == DBL)) {printf("valinit == ..\n"); printf("%lf\n", dvalue(valinit(i)));}
		if ((debug) && (pcresult->first.param.nv.value.etype == STR)) {printf("valinit == ..\n"); printf("%s\n", svalue(valinit(i)));}				
		if (debug) printf("tete..\n");
		if (debug) {printf("utilisation == ..\n"); printf("%s\n", (est_utilisee(i)==true)?"true":"false");}
		pcresult->first.param.nv.utilisee = est_utilisee(i); // nécessaire pour l'optimisateur
		pcresult->next = tete;
		if (debug) printf("pcresult..\n");
		tete = pcresult;

	}

	// générer le begin:
	if (debug) printf("Generer le code pour le label begin...\n");
	pseudocode pcbegin = (pseudocode) malloc(sizeof(struct pseudocodenode));
	pcbegin->first.codop = LABEL;
	pcbegin->first.param.label_name = (char*) malloc(6);
	strcpy(pcbegin->first.param.label_name, "begin");
	pcbegin->next = NULL;

	//générer le code pour les INSTRUCTUTIONS
	if (debug) printf("Generer le code pour les instructions...\n");
	pseudocode pcbody = generer_pseudo_code_list_inst(plistinstattribute);

	// lier le label begin en début du body
	if (debug) printf("lier le label begin en début du body...\n");
	inserer_code_en_queue(pcbegin, pcbody);

	// générer le label end:
	if (debug) printf("générer le label end:...\n");
	pseudocode pcend = (pseudocode) malloc(sizeof(struct pseudocodenode));
	pcend->first.codop = LABEL;
	pcend->first.param.label_name = (char*) malloc(4);
	strcpy(pcend->first.param.label_name, "end");
	pcend->next = NULL;

	// lier le label end en fin du body
	if (debug) printf("lier le label end en fin du body...\n");
	inserer_code_en_queue(pcbody,pcend);

	// lier le tout begin-insts-end en fin des data
	if (debug) printf("lier le tout begin-insts-end en fin des data...\n");
	inserer_code_en_queue(pcresult,pcbegin);

	return pcresult;
}
