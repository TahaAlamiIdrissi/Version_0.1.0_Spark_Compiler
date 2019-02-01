#ifndef AST_H
#define AST_H


typedef enum {false=0, true=1} boolean;

typedef enum {
               NB=0, _IDF = 1, BOOL = 2, OP=3,
                _CALL=4

             } Type_Exp ; // IDF is also a token !!


typedef enum {
              Integer, Boolean, Character,
              String, Float, Natural, Positive

              }Type;

typedef enum {
              plus, moins, mult, _div,mod,
              puissance,_abs

              } Type_Op; // abs et div sont des mots réservés


struct Exp ; /* pré déclaration de la structure de stockage d'une expression */

typedef struct Exp * AST;

typedef struct call {
char * callfnc;

AST * effectiveparams; // liste des paramètres affectifs

} call;

typedef union {
  double nombre ;
  char *idf;
// call* acall;
  boolean bool; //cas spécial de la feuille TRUE / FALSE
  struct {
    int line;
    Type_Op top;
    AST expression_gauche ;
    AST expression_droite ;
  } op;
} ExpValueTypeNode;

typedef struct Exp {
  Type_Exp typeexp ; // NB ou IDF ou OP
  Type     typename; // Int ou Bool ou Double
  ExpValueTypeNode noeud;
}expvalueType;

// précondition : a<> NULL and est_feuille(a) == false
AST arbre_gauche(AST a);

// précondition : a<> NULL and est_feuille(a) == false
AST arbre_droit(AST a);

// précondition : a<> NULL and est_feuille(a) == false
Type_Op top(AST a);

// précondition : a<> NULL
Type type(AST a);

// précondition : a<> NULL 
boolean est_feuille(AST a);


AST creer_feuille_booleen(boolean b);

AST creer_feuille_nombre(double n, Type type);

AST creer_feuille_idf(char *idf, Type type);

AST creer_noeud_operation(char op, AST arbre_g, AST arbre_d, Type type, int line);

// affichage par parcours infixé de l'arbre abstrait
// précondition : ast<> NULL 
void afficher_infixe_arbre (AST ast);

// affichage par parcours postfixé de l'arbre abstrait
// précondition : ast<> NULL 
void afficher_postfixe_arbre (AST ast);

// évalation par parcours postfixé de l'arbre abstrait
// précondition : ast<> NULL 
//double evaluer (AST ast);

// génère le pseudo-code relatif à l'AST
// précondition ast <> NULL

//pseudocode generer_pseudo_code_ast(AST ast);


#endif 

