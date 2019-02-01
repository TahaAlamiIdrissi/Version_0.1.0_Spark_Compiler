#include "ast.h"
#include "tab_symb.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define debug false



// précondition : a<> NULL and est_feuille(a) == false
AST arbre_gauche(AST a){
  return a->noeud.op.expression_gauche;
}

// précondition : a<> NULL and est_feuille(a) == false
AST arbre_droit(AST a){
  return a->noeud.op.expression_droite;
}

// précondition : a<> NULL and est_feuille(a) == false
Type_Op top(AST a){
  return a->noeud.op.top;
}

// précondition : a<> NULL
Type  type(AST a){
return a->typename;
}

// précondition : a<> NULL 
boolean est_feuille(AST a){
  return (a->typeexp != OP) ;
}

AST creer_feuille_booleen(boolean b){
  if (debug) printf("creer_feuille_boolean()\n");
  AST result = (AST) malloc (sizeof(struct Exp));
  result->typeexp=BOOL;
  result->noeud.bool = b;
  result->typename = Boolean;

  if (debug) printf("out of creer_feuille_boolean()\n");
  return result;
}

AST creer_feuille_nombre(double n, Type type){
  if (debug) printf("creer_feuille_nombre()\n");
  AST result = (AST) malloc (sizeof(struct Exp));
  result->typeexp=NB;
  result->noeud.nombre = n;
  result->typename = type;

  if (debug) printf("out creer_feuille_nombre()\n");

  return result;
}

AST creer_feuille_idf(char *idf, Type type){
  if (debug) printf("creer_feuille_idf()\n");

  AST result = (AST) malloc (sizeof(struct Exp));
  result->typeexp=_IDF;
  result->typename = type; // Bool ou Int
  result->noeud.idf = (char*) malloc (sizeof(char)*(strlen(idf)+1));
  strcpy(result->noeud.idf, idf);

  if (debug) printf("out of creer_feuille_idf()\n");

  return result;
}

AST creer_noeud_operation(char op, AST arbre_g, AST arbre_d, Type type, int line){ //la ligne permet de tracer les probl�mes d'op�ration (exemple division par z�ro)
  if (debug) printf("creer_noeud_operation()\n");

  AST result= (AST) malloc (sizeof(struct Exp));
  result->typeexp=OP;
  result->typename = type;
  result->noeud.op.top = ((op=='+')?plus:((op=='-')?moins:((op=='*')?mult:_div)));
  result->noeud.op.expression_gauche = arbre_g;
  result->noeud.op.expression_droite = arbre_d;
  result->noeud.op.line = line;

  if (debug) printf("out of creer_noeud_operation()\n");

  return result;
}

// affichage par parcours infixé de l'arbre abstrait
// précondition : a<> NULL 
void afficher_infixe_arbre (AST ast){
  // if (est_feuille(ast)){
  switch(ast->typeexp) {
  case BOOL : printf(" %s",(ast->noeud.bool==true)?"true":"false"); break;
  case NB : printf(" %lf",ast->noeud.nombre); break;
  case _IDF :  printf(" %s",ast->noeud.idf); break;
  case OP :
    printf("gauche [ "); 	afficher_infixe_arbre(arbre_gauche(ast)); printf(" ]");
    switch(ast->noeud.op.top){
    case plus : printf(" + "); break;
    case moins : printf(" - "); break;
    case mult : printf(" * "); break;
    case _div : printf(" / "); break;
    }
        printf("droit [ "); afficher_infixe_arbre(arbre_droit(ast)); printf(" ]");
    break;
  }
}

// affichage par parcours postfixé de l'arbre abstrait
// précondition : a<> NULL 
void afficher_postfixe_arbre (AST ast){
  // if (est_feuille(ast)){
  switch(ast->typeexp) {
  case BOOL : printf(" %s",(ast->noeud.bool==true)?"true":"false"); break;
  case NB : printf(" %lf",ast->noeud.nombre); break;
  case _IDF :  printf(" %s",ast->noeud.idf); break;
  case OP :
    afficher_postfixe_arbre(arbre_gauche(ast));
    afficher_postfixe_arbre(arbre_droit(ast));
    switch(ast->noeud.op.top){
    case plus : printf(" + "); break;
    case moins : printf(" - "); break;
    case mult : printf(" * "); break;
    case _div : printf(" / "); break;
    }
    break;
  }
}

// évalation par parcours postfixé de l'arbre abstrait
// précondition : ast<> NULL 
/*double evaluer (AST ast){
  double valg, vald;
  //if (est_feuille(ast)){
  switch(ast->typeexp) {
  case NB : return ast->noeud.nombre; break;
  case _IDF : if (debug) printf("idf == %s de valeur %lf", ast->noeud.idf, value(ast->noeud.idf));
    return value(ast->noeud.idf); break;
  case OP :
    valg = evaluer(arbre_gauche(ast));
    vald = evaluer(arbre_droit(ast));
    switch(ast->noeud.op.top){
    case plus : if (debug) printf("%lf + %lf == %lf\n",valg, vald, valg + vald);return valg + vald; break;
    case moins : if (debug) printf("%lf - %lf == %lf\n",valg, vald, valg - vald);return valg - vald; break;
    case mult : if (debug) printf("%lf * %lf == %lf\n",valg, vald, valg * vald);return valg * vald; break;
    case _div : if (debug) printf("%lf / %lf == %lf\n",valg, vald, valg / vald);return valg / vald; break;
    }
    break;
  }
}*/

// génère le pseudo-code relatif à l'AST
// précondition ast <> NULL

