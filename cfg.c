#include "cfg.h"
#include "tab_symb.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define debug false

instvalueType* creer_instruction_printIdf(int rangvar){
        if (debug) printf("creer_instruction_printIdf()\n");

	instvalueType * printinstattribute = (instvalueType *) malloc (sizeof(instvalueType));

	printinstattribute->typeinst = PrintIdf;

	printinstattribute->node.printnode.rangvar = rangvar;
	
	if (debug) printf("out of creer_instruction_printIdf()\n");

	return printinstattribute;

}

instvalueType* creer_instruction_printString(char * s){
        if (debug) printf("creer_instruction_printString()\n");

	instvalueType * printinstattribute = (instvalueType *) malloc (sizeof(instvalueType));

	printinstattribute->typeinst = PrintString;

	printinstattribute->node.printnode.str = (char *) malloc ((strlen(s)+1)*sizeof(char));

	strcpy(printinstattribute->node.printnode.str, s);
	
	if (debug) printf("out of creer_instruction_printString()\n");

	return printinstattribute;
}

instvalueType* creer_instruction_affectation(int rangvar, AST * past){
        if (debug) 	printf("creer_instruction_affectation()\n");

	instvalueType * pinstattribute = (instvalueType *) malloc (sizeof(instvalueType));

	pinstattribute->typeinst = (type(*past)==Boolean)?AssignBool:AssignArith;
	pinstattribute->node.assignnode.rangvar = rangvar;
	pinstattribute->node.assignnode.right = * past;

        if (debug) printf("out of creer_instruction_affectation()\n");

	return pinstattribute;
}

instvalueType* creer_instruction_if_then(int rangvar, AST * past,listinstvalueType * plistthen){

  if (debug) printf("creer_instruction_if_then()\n");

	instvalueType * pinstattribute = (instvalueType *) malloc (sizeof(instvalueType));

	pinstattribute->typeinst = IfThenArith;
	pinstattribute->node.ifnode.rangvar = rangvar;
	pinstattribute->node.ifnode.right = * past;
	pinstattribute->node.ifnode.thenlinst = plistthen;
        if (debug) printf("out of creer_instruction_if_then()\n");	

	return pinstattribute;
}


instvalueType* creer_instruction_if_then_else(int rangvar, AST * past, listinstvalueType * plistthen, listinstvalueType * plistelse){

        if (debug) printf("creer_instruction_if_then_else()\n");

	instvalueType * pinstattribute = (instvalueType *) malloc (sizeof(instvalueType));

	pinstattribute->typeinst = IfThenElseArith;
	pinstattribute->node.ifnode.rangvar = rangvar;
	pinstattribute->node.ifnode.right = * past;
	pinstattribute->node.ifnode.thenlinst = plistthen;
	pinstattribute->node.ifnode.elselinst = plistelse;

        if (debug) printf("out of creer_instruction_if_then_else()\n");	

	return pinstattribute;
} 
instvalueType* creer_instruction_if_then_elsif(int rangvar, AST * past,int nbelsif, listinstvalueType * plistthen, listinstvalueType * plistelsif){

  if (debug) printf("creer_instruction_if_then_elsif()\n");

	instvalueType * pinstattribute = (instvalueType *) malloc (sizeof(instvalueType));
      
      pinstattribute->typeinst = IfThenElsifArith;
      pinstattribute->node.ifnode.rangvar = rangvar;
      pinstattribute->node.ifnode.right = * past;
      pinstattribute->node.ifnode.thenlinst = plistthen;
      pinstattribute->node.ifnode.listelsif->elsiflinst = plistelsif;
      pinstattribute->node.ifnode.nbelseif=nbelsif;

        if (debug) printf("out of creer_instruction_if_then_elsif()\n");	

  return pinstattribute;
}

instvalueType* creer_instruction_if_then_elsif_else(int rangvar, AST * past,int nbelsif,listinstvalueType * plistelse, listinstvalueType * plistthen, listinstvalueType * plistelsif){
  if (debug) printf("creer_instruction_if_then_elsif_else()\n");

	instvalueType * pinstattribute = (instvalueType *) malloc (sizeof(instvalueType));
      
      pinstattribute->typeinst = IfThenElsifElseArith;
      pinstattribute->node.ifnode.rangvar = rangvar;
      pinstattribute->node.ifnode.right = * past;
      pinstattribute->node.ifnode.thenlinst = plistthen;
      pinstattribute->node.ifnode.elselinst=plistelse;
      pinstattribute->node.ifnode.listelsif->elsiflinst = plistelsif;
      pinstattribute->node.ifnode.nbelseif=nbelsif;

        if (debug) printf("out of creer_instruction_if_then_elsif_else()\n");	

  return pinstattribute;
}

extern instvalueType* creer_instruction_while(int rangvar, AST * past, listinstvalueType * plistwhile){
 
       if (debug) printf("creer_instruction_while()\n");
  
  instvalueType * pinstattribute = (instvalueType *) malloc (sizeof(instvalueType));
	pinstattribute->typeinst =While;
  pinstattribute->node.whilenode.rangvar=rangvar;
  pinstattribute->node.whilenode.whilebodylinst=plistwhile;
  pinstattribute->node.whilenode.right = * past;

        if (debug) printf("out of creer_instruction_while()\n");	

	return pinstattribute;
}

instvalueType* creer_instruction_for(int rangvar, int borneinf, int bornesup, listinstvalueType *plistfor){

        if (debug) printf("creer_instruction_for()\n");

	instvalueType * pinstattribute = (instvalueType *) malloc (sizeof(instvalueType));

	pinstattribute->typeinst = For;
	pinstattribute->node.fornode.rangvar = rangvar;
	pinstattribute->node.fornode.borneinf = borneinf;
	pinstattribute->node.fornode.bornesup = bornesup;
	pinstattribute->node.fornode.forbodylinst = plistfor;

        if (debug) printf("out of creer_instruction_for()\n");	

	return pinstattribute;
}

instvalueType* creer_instruction_case(int rangvar, int nbwhens, whenvaluelinstType *whens, listinstvalueType *plistcasedefaultbody){        
  
  if (debug) printf("creer_instruction_case()\n");

      instvalueType * pinstattribute = (instvalueType *) malloc (sizeof(instvalueType));

      pinstattribute->typeinst = Case;
      pinstattribute->node.casenode.rangvar = rangvar;
      pinstattribute->node.casenode.nbwhens = nbwhens;
      pinstattribute->node.casenode.whens = whens;
      pinstattribute->node.casenode.defaultbodylinst = plistcasedefaultbody;
  if (debug) printf("out of creer_instruction_case()\n");	

	return pinstattribute;
}

instvalueType* creer_instruction_loopnoexitwhen(listinstvalueType * plistavantexit){

        if (debug) printf("creer_instruction_loopnoexitwhen()\n");

  instvalueType * pinstattribute = (instvalueType *) malloc (sizeof(instvalueType));

  pinstattribute->typeinst = LoopNoExitWhen;
  pinstattribute->node.loopnode.listinstavantexit = plistavantexit;

        if (debug) printf("out of creer_instruction_loopnoexitwhen()\n"); 

  return pinstattribute;
}

instvalueType* creer_instruction_loopewbegin(int rangvar, AST * past, listinstvalueType * plistapresexit){

        if (debug) printf("creer_instruction_loopewbegin()\n");

  instvalueType * pinstattribute = (instvalueType *) malloc (sizeof(instvalueType));

  pinstattribute->typeinst = LoopEwBegin;
  pinstattribute->node.loopnode.rangvar = rangvar;
  pinstattribute->node.loopnode.right = * past;
  pinstattribute->node.loopnode.listinstapresexit = plistapresexit;

        if (debug) printf("out of creer_instruction_loopewbegin()\n"); 

  return pinstattribute;
}

instvalueType* creer_instruction_loopewmiddle(int rangvar, AST * past, listinstvalueType * plistavantexit, listinstvalueType * plistapresexit){

        if (debug) printf("creer_instruction_loopewmiddle()\n");

  instvalueType * pinstattribute = (instvalueType *) malloc (sizeof(instvalueType));

  pinstattribute->typeinst = LoopEwMiddle;
  pinstattribute->node.loopnode.rangvar = rangvar;
  pinstattribute->node.loopnode.right = * past;
  pinstattribute->node.loopnode.listinstavantexit = plistavantexit;
  pinstattribute->node.loopnode.listinstapresexit = plistapresexit;

        if (debug) printf("out of creer_instruction_loopewmiddle()\n"); 

  return pinstattribute;
}

instvalueType* creer_instruction_loopewend(int rangvar, AST * past, listinstvalueType * plistavantexit){

        if (debug) printf("creer_instruction_loopewend()\n");

  instvalueType * pinstattribute = (instvalueType *) malloc (sizeof(instvalueType));

  pinstattribute->typeinst = LoopEwEnd;
  pinstattribute->node.loopnode.rangvar = rangvar;
  pinstattribute->node.loopnode.right = * past;
  pinstattribute->node.loopnode.listinstavantexit = plistavantexit;

        if (debug) printf("out of creer_instruction_loopewend()\n"); 

  return pinstattribute;
}

/*
void interpreter_inst(instvalueType instattribute){
  double rexp;
  switch(instattribute.typeinst){
  case PrintIdf :
    if (debug) printf("Print");
    if (typevar(instattribute.node.printnode.rangvar) == Bool){
      printf("%s\n", ((valinit(instattribute.node.printnode.rangvar)==false)?"false":"true"));
    }else{
      printf("%lf\n", valinit(instattribute.node.printnode.rangvar));
    }
    break;
    
  case AssignArith :
   
    if (debug) printf("AssignArith");
    rexp=evaluer(instattribute.node.assignnode.right);
    if (debug) printf("rexp==%f\n",rexp);
    set_valinit(instattribute.node.assignnode.rangvar, rexp);
    if (debug) {
      printf("lexp==%f\n",valinit(instattribute.node.assignnode.rangvar));
      afficherTS();}
    break;
    
  case AssignBool :
    if (debug) printf("AssignBool");
   
    set_valinit(instattribute.node.assignnode.rangvar, instattribute.node.assignnode.right0);
   
    break;
    
  case IfThenArith :
    if (debug) printf("If");
    if ( valinit(instattribute.node.ifnode.rangvar) == evaluer(instattribute.node.ifnode.right) ){
      interpreter_list_inst( instattribute.node.ifnode.thenlinst );
    }
    break;

 case IfThenElseArith :
    if (debug) printf("If");
    if ( valinit(instattribute.node.ifnode.rangvar) == evaluer(instattribute.node.ifnode.right) ){
      interpreter_list_inst( instattribute.node.ifnode.thenlinst );
    }else{
      interpreter_list_inst( instattribute.node.ifnode.elselinst );
    }
    break;
  }
}

void interpreter_list_inst(listinstvalueType * plistinstattribute){
  if (debug) printf("here");
  if  (plistinstattribute != NULL){
    interpreter_inst(plistinstattribute->first);
    interpreter_list_inst(plistinstattribute->next);
  } 
}

*/

void afficher_inst(instvalueType instattribute){
  if (debug) printf("afficher_inst()\n");
  int i;

  switch(instattribute.typeinst){

  case PrintIdf :
    printf("PrintIdf "); printf("%s ;\n", name(instattribute.node.printnode.rangvar));
    break;

  case PrintString :
    printf("PrintString "); printf("%s ;\n", instattribute.node.printnode.str);
    break;
        
  case AssignArith :
    printf("AssignArith ");
    printf("%s", name(instattribute.node.assignnode.rangvar));
    afficher_infixe_arbre(instattribute.node.assignnode.right);
    printf(";\n");
    break;

  case AssignBool :
    printf("AssignBool ");
    printf("%s", name(instattribute.node.assignnode.rangvar));
    afficher_infixe_arbre( instattribute.node.assignnode.right );
    // printf(" := %s",(instattribute.node.assignnode.right0==true)?"true":"false");
    printf(";\n");
    break;
    
  case IfThenArith :
    printf("If ");
    printf("%s ==",name(instattribute.node.ifnode.rangvar));
    // printf("%lf )", instattribute.node.ifnode.right);
    afficher_infixe_arbre( instattribute.node.ifnode.right );
    printf(" \n Then ");
    afficher_list_inst( instattribute.node.ifnode.thenlinst );
    printf(" endIf;\n");
    break;

  case IfThenElseArith :
    printf("If ");
    printf(" %s == ",name(instattribute.node.ifnode.rangvar));
    // printf("%lf )", instattribute.node.ifnode.right);
    afficher_infixe_arbre( instattribute.node.ifnode.right );
    printf(" \n Then ");
    afficher_list_inst( instattribute.node.ifnode.thenlinst );
    printf(" Else ");
    afficher_list_inst( instattribute.node.ifnode.elselinst );
    printf(" endIf;\n");
    break;
  case IfThenElsifArith :
    printf("If ");
    printf(" %s == ",name(instattribute.node.ifnode.rangvar));
    // printf("%lf )", instattribute.node.ifnode.right);
    afficher_infixe_arbre( instattribute.node.ifnode.right );
    printf(" \n Then ");
    afficher_list_inst( instattribute.node.ifnode.thenlinst );
    i = 0;
      while (i < instattribute.node.ifnode.nbelseif ) {
        printf("\nelsif ");
        printf(" %s == ",name(instattribute.node.ifnode.listelsif[i].rangvar));
        afficher_infixe_arbre( instattribute.node.ifnode.listelsif[i].right );
        printf(" \n Then ");
        afficher_list_inst( instattribute.node.ifnode.listelsif[i].elsiflinst );
      i++;
    }
    printf("\n end if;");
  break;
  case IfThenElsifElseArith:
    printf("If ");
    printf(" %s == ",name(instattribute.node.ifnode.rangvar));
    // printf("%lf )", instattribute.node.ifnode.right);
    afficher_infixe_arbre( instattribute.node.ifnode.right );
    printf(" \n Then ");
    afficher_list_inst( instattribute.node.ifnode.thenlinst );
    i = 0;
      while (i < instattribute.node.ifnode.nbelseif ) {
        printf("\nelsif ");
        printf(" %s == ",name(instattribute.node.ifnode.listelsif[i].rangvar));
        afficher_infixe_arbre( instattribute.node.ifnode.listelsif[i].right );
        printf(" \n Then ");
        afficher_list_inst( instattribute.node.ifnode.listelsif[i].elsiflinst );
      i++;
    }
    printf(" Else ");
    afficher_list_inst( instattribute.node.ifnode.elselinst );
    printf("\n end if;");
  break;
  case For :
    printf("For ");
    printf("( %s = ",name(instattribute.node.fornode.rangvar));
    printf(" %d to ",instattribute.node.fornode.borneinf );
    printf(" %d ) ",instattribute.node.fornode.bornesup );

    afficher_list_inst( instattribute.node.fornode.forbodylinst );
    printf(" endFor;\n");
    break;

  case Case :
    printf("Case ");
    printf(" %s  ",name(instattribute.node.casenode.rangvar));
    int i = 0;
  //    printf(" \n----> on a  %d when + when others\n",instattribute.node.casenode.nbwhens );
      while (i < instattribute.node.casenode.nbwhens ) {
      printf("\n when %d => ",instattribute.node.casenode.whens[i].value);
      afficher_list_inst( instattribute.node.casenode.whens[i].whenbodylinst );
      i++;
    }

    printf("\nwhen others : ");
    afficher_list_inst( instattribute.node.casenode.defaultbodylinst );

    printf(" end case;\n");
    break;
    
    case While :// While loop without label 
    printf("While ");
      printf(" %s ==",name(instattribute.node.whilenode.rangvar));
      // printf("%lf )", instattribute.node.ifnode.right);
      afficher_infixe_arbre( instattribute.node.whilenode.right );
      printf(" \n Loop ");
      afficher_list_inst( instattribute.node.whilenode.whilebodylinst );
      printf(" end loop;\n");
      break;

  case LoopNoExitWhen :
   printf("loop\n");
   afficher_list_inst(instattribute.node.loopnode.listinstavantexit);
   printf(" end loop\n");
   break;

  case LoopEwBegin :
   printf("loop\n");
   printf("exit when %d", instattribute.node.loopnode.rangvar);
   afficher_infixe_arbre( instattribute.node.loopnode.right);
   afficher_list_inst( instattribute.node.loopnode.listinstapresexit );
   printf(" end loop\n");
   break;


  case LoopEwMiddle :
   printf("loop\n");
   afficher_list_inst(instattribute.node.loopnode.listinstavantexit);
   printf("exit when %d", instattribute.node.loopnode.rangvar);
   afficher_infixe_arbre( instattribute.node.loopnode.right);
   afficher_list_inst( instattribute.node.loopnode.listinstapresexit );
   printf(" end loop\n");
   break;

  case LoopEwEnd :
   printf("loop\n");
   afficher_list_inst(instattribute.node.loopnode.listinstavantexit);
   printf("exit when %d", instattribute.node.loopnode.rangvar);
   afficher_infixe_arbre( instattribute.node.loopnode.right);
   printf(" end loop\n");
   break;
  }

  if (debug) printf("out of afficher_inst()\n");
}

void afficher_list_inst(listinstvalueType * plistinstattribute){
  if (debug) printf("afficher_list_inst()\n");

  if (plistinstattribute != NULL){
   
    afficher_inst(plistinstattribute->first);    
    afficher_list_inst(plistinstattribute->next);
  }else if (debug) printf("Bugg list instruction NULL !!!!!!!!!!!!!!!!!!!!!!!\n");

  if (debug) printf("out of afficher_list_inst()\n");

}

void inserer_inst_en_tete(listinstvalueType ** pplistinstattribute, instvalueType instattribute){
//int i=0; for (i=0;i<100;i++)printf("IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII\n");

  if (debug) {printf("inserer_inst_en_tete( "); afficher_list_inst(*pplistinstattribute); afficher_inst(instattribute); printf(" )");}

  listinstvalueType * liste = (listinstvalueType *) malloc(sizeof(listinstvalueType));
//printf("MALLOCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC");
  liste->first = instattribute;
  liste->next = *pplistinstattribute;

  *pplistinstattribute = liste;

  if (debug) printf("out of inserer_inst_en_tete()\n");
}

void inserer_inst_en_queue(listinstvalueType * plistinstattribute, instvalueType instattribute){
  if (debug) printf ("debut inserer_inst_en_queue()");

  listinstvalueType * liste = (listinstvalueType *) malloc(sizeof(listinstvalueType));
  liste->first = instattribute;
  liste->next = NULL;

  if (plistinstattribute->next == NULL) {
    plistinstattribute->next = liste;
  }else{
    listinstvalueType * pliste = plistinstattribute;
    
    while(pliste->next != NULL) {
      pliste = pliste->next;
    }
    
    pliste->next = liste;
  }
  if (debug) printf ("debut inserer_inst_en_queue()");
}



// pré-requis : e.etype == DBL;
double dvalue(Element e){
	return e.evalue.dvalue;
}

// pré-requis : e.etype == STR;
char * svalue(Element e){
	return e.evalue.svalue;
}

void setdvalue(Element *e, double d){
	e->evalue.dvalue = d;
	e->etype = DBL;
}

void setsvalue(Element *e, char * s){
	if (debug) printf("setsvalue()\n");
	if ((debug) && (s == NULL)) printf("s NULL!!!!!!!\n"); 
	e->evalue.svalue = (char *)malloc((strlen(s)+1)*sizeof(char));
	strcpy(e->evalue.svalue, s);
	e->etype = STR;
	if (debug) printf("out of setsvalue()\n");
}
