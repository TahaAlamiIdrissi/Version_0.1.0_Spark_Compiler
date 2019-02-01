#include "analyser.h"
#include <stdio.h>
#include <string.h>
#include "tab_symb.h"
//#include "cfg.h"
#include "error.h"
//#include "pseudocode.h"
//#include "vm.h"
//#include "i18n.h"

#include <stdlib.h>

#define Malloc(type) (type *) malloc((strlen(type) + 1) * sizeof(type));
#define debug false

typetoken token;

boolean follow_token = false;

constvalueType constattribute;
typevalueType typeattribute;
instvalueType instattribute;
listinstvalueType listinstattribute;
tokenvalueType tokenattribute;
stringvalueType stringattribute;
AST * astattribute;

int rangvar;
boolean semanticerror = false;

int main(int argc, char **argv) {
    astattribute = (AST *) malloc(sizeof(AST));
    token = _read_token();
    if (_prog()) {
        printf("0 syntactic errors\n");
        if (nombre_sm_erreurs() == 0){
            if (debug) {if (semanticerror == true)  printf("BUGG 0 semantic error BUT semanticerror == true !!\n");}
            if (debug) printf("0 erreurs sémantiques\n");
            if (debug) afficherTS();
        }
        else{
            if (debug) afficherTS();
            //printf("%d erreurs sémantiques\n", nombre_sm_erreurs());
            afficher_sm_erreurs();
        }
    } else {
        if (nombre_sx_erreurs() == 0) creer_sx_erreur(NONCATEGORIZED, tokenattribute.line);
        afficher_erreurs();
        if (debug) afficherTS();
    }
    //print_semantic_errors();
    return 0;
}

typetoken _read_token() {
    if (follow_token) {
        follow_token = false;
        return token;
    } else {
        token = (typetoken) yylex();
        return token;
    }
}

//PROG : procedure idf is LIST_DECL beg LIST_INST end idf ;
boolean _prog(){
    boolean result = false;
    if (debug) printf("prog()\n");

    if (token == PROCEDURE){
        token = _read_token();
        if (token == IDF){
            token = _read_token();
            if (token == IS){
                token = _read_token();
                if (_list_decl()){
                    token = _read_token();
                    if (token == BEG){
                        token = _read_token();
                        if (_list_inst()){
                            token = _read_token();
                            if (token == END){
                                token = _read_token();
                                if (token == IDF){
                                    token = _read_token();
                                    if (token == SEMI_COLON){
                                        result = true;
                                    }else{
                                        creer_sx_erreur(SEMICOLONEXPECTED, tokenattribute.line);
                                        result = false ;
                                    }
                                }else{
                                    creer_sx_erreur(IDFEXPECTED, tokenattribute.line);
                                    result = false ;
                                }
                            }else{
                                creer_sx_erreur(ENDEXPECTED, tokenattribute.line);
                                result = false ;
                            }
                        }else{
                            result = false ;
                        }
                    }else{
                        creer_sx_erreur(BEGINEXPECTED, tokenattribute.line);
                        result = false ;
                    }
                }else{
                    result = false ;
                }
            }else{
                creer_sx_erreur(ISEXPECTED, tokenattribute.line);
                result = false ;
            }
        }else{
            creer_sx_erreur(IDFEXPECTED, tokenattribute.line);
            result = false ;
        }
    }else{
        creer_sx_erreur(PROCEDUREEXPECTED, tokenattribute.line);
        result = false ;
    }

    if (debug) printf("out of prog()\n");
    return result ;
}


//DECL : idf LIST_IDF : MODIFIER TYPE ADD_ASSIGNMENT ;
boolean _decl(){
    boolean result = false;
    if (debug) printf("decl()\n");

    if (token == IDF){
        if (debug) printf("VAR{%s}",varattribute[NBRMAXIDF].name);
        token = _read_token();
        if (_list_idf()){
            token = _read_token();
            if (token == COLON){
                token = _read_token();
                if (_modifier()){
                    token = _read_token();
                    if (_type()){
                        token = _read_token();
                        if (_add_assignment()){
                            token = _read_token();
                            if (token == SEMI_COLON){
                                for(int i = 0; i<NBRMAXIDF; i++){
                                    if(inListIdf(varattribute[i].name) || inTS(varattribute[i].name, &rangvar)){
                                        //erreur sem deja declarée
                                    }else{
                                        varvalueType newvar;
                                        newvar.nbdecl = 1;
                                        newvar.name = (char *)malloc(sizeof(char)*strlen(varattribute[i].name)+1);
                                        strcpy(newvar.name, varattribute[i].name);
                                        if (debug) printf("VAR{%s}-->NEW{%s}",varattribute[i].name, newvar.name);
                                        newvar.line = varattribute[i].line;
                                        newvar.initialisation = varattribute[i].initialisation; // l'initialisation est marquée par decl_aux dans varattribute
                                        newvar.utilisation = false; //toute variable est par défaut non utilisee
                                        newvar.typevar = typeattribute.typename;
                                        switch(newvar.typevar){
                                            case String : setsvalue(&(newvar.valinit) , ((varattribute[i].initialisation == true)?stringattribute.value:""/*sm error string not ini*/)); break;
                                            default : setdvalue(&(newvar.valinit) , ((varattribute[i].initialisation == true)?constattribute.valinit:0.0)); break;
                                        }
                                        ajouter_nouvelle_variable_a_TS(newvar);
                                    }
                                    //verification de sm error de badlydeclared
                                }
                                result = true ;
                            }else{
                                creer_sx_erreur(SEMICOLONEXPECTED, tokenattribute.line);
                                result = false ;
                            }
                        }else{
                            result = false ;
                        }
                    }else{
                        creer_sx_erreur(TYPEEXPECTED, tokenattribute.line);
                        result = false ;
                    }
                }else{
                    result = false ;
                }
            }else{
                creer_sx_erreur(COLONEXPECTED, tokenattribute.line);
                result = false ;
            }
        }else{
            result = false ;
        }
    }else{
        result = false ;
    }

    if (debug) printf("out of decl()\n");
    return result ;
}


//MODIFIER : constant | epsilon
// Follows (character|string|integer|float|boolean|natural|positive)
// First (constant)
boolean _modifier(){
    boolean result = false;
    if (debug) printf("modifier()\n");

    if (token == CHARACTER){
        follow_token = true;
        result = true ;
    } 
    else if (token == STRING){
        follow_token = true;
        result = true ;
    }
    else if (token == INTEGER){
        follow_token = true;
        result = true ;
    }
    else if (token == FLOAT){
        follow_token = true;
        result = true ;
    }
    else if (token == BOOLEAN){
        follow_token = true;
        result = true ;
    }
    else if (token == NATURAL){
        follow_token = true;
        result = true ;
    }
    else if (token == POSITIVE){
        follow_token = true;
        result = true ;
    }
    else if (token == CONSTANT){
        result = true ;
    }
    else {
        result = false ;
    }

    if (debug) printf("out of modifier()\n");
    return result ;
}

//LIST_IDF : , idf LIST_IDF | epsilon
// Follows (':')
// first (',')
boolean _list_idf(){
    boolean result = false;
    if (debug) printf("list_idf()\n");

    if (token == COLON){
        follow_token = true;
        result = true ;
    } 
    else if (token == COMMA){
        token =_read_token();
        if (token == IDF){
            if (debug) printf("VAR{%s}",varattribute[NBRMAXIDF].name);
            token = _read_token();
            if (_list_idf()){
                result = true ;
            }else{
                result = false ;
            }
        }else{
            creer_sx_erreur(IDFEXPECTED, tokenattribute.line);
            result = false ;
        }
    }
    else {
        result = false ;
    }

    if (debug) printf("out of list_idf()\n");
    return result ;
}

//ADD_ASSIGNMENT : ASSIGNMENT | epsilon
// Follows (';')
// First (':=')
boolean _add_assignment(){
    boolean result = false ;
    if (debug) printf("add_assignment()\n");

    if (token == SEMI_COLON){
        for(int i = 0; i<NBRMAXIDF; i++){
            varattribute[i].initialisation = false;
        }
        follow_token = true;
        result = true ;
    }
    else if (_assignment()){
        result = true ;
    }
    else {
        result = false ;
    }

    if (debug) printf("out of add_assignment()\n");
    return result ;
}


//TYPE : character | String | float | integer | boolean | natural | positive
boolean _type(){
    boolean result = false ;
    if (debug) printf("type()\n");

    if (token == CHARACTER){
        typeattribute.typename = Character;
        result = true ;
    } 
    else if (token == STRING){
        typeattribute.typename = String;
        result = true ;
    }
    else if (token == INTEGER){
        typeattribute.typename = Integer;
        result = true ;
    }
    else if (token == FLOAT){
        typeattribute.typename = Float;
        result = true ;
    }
    else if (token == BOOLEAN){
        typeattribute.typename = Boolean;
        result = true ;
    }
    else if (token == NATURAL){
        typeattribute.typename = NATURAL;
        result = true ;
    }
    else if (token == POSITIVE){
        typeattribute.typename = Positive;
        result = true ;
    }
    else {
        result = false ;
    }

    if (debug) printf("out of type()\n");
    return result ;
}


//LIST_DECL : DECL LIST_DECL | epsilon
// Follows ('beg')
// First ('idf')
boolean _list_decl(){
    boolean result = false;
    if (debug) printf("list_decl()\n");

    if (token == BEG){
        follow_token = true;
        result = true ;
    }else if (_decl()){
        token =_read_token();
        if (_list_decl()){
            result = true ;
        }else{
            result = false ;
        }
    }else {
        result = false ;
    }
    
    if (debug) printf("out of list_decl()\n");
    return result ;
}

//LOOP: LOOP_TYPE 'end' 'loop' ';'
// First (while, for, loop)
boolean _loop() {
    boolean result = false ;
    if (debug) printf("loop()\n");

    if(_loop_type()){
    token = _read_token();
        if (token == END){ 
        token = _read_token();
            if (token == LOOP){
                token = _read_token();
                if(token == SEMI_COLON){
                    result = true;
                }
                else{
                    creer_sx_erreur(SEMICOLONEXPECTED, tokenattribute.line);
                    result = false;
                }
            }
            else{
                creer_sx_erreur(LOOPEXPECTED, tokenattribute.line);
                result = false;
            }
        }
        else{
            creer_sx_erreur(ENDEXPECTED, tokenattribute.line);
            result = false;
        }
    }
    else{
        result = false;
    }

    if (debug) printf("out of loop()\n");
    return result;
}

//LOOP_TYPE: WHILE_LOOP | FOR_LOOP | OTHER_LOOP
boolean _loop_type() {
    boolean result = false ;
    if (debug) printf("loop_type()\n");

    if (_while_loop()){
        result = true ;
    }
    else if (_for_loop()){
        result = true ;
    }
     else if (_other_loop()){
        result = true ;
    }
    else{
        result = false;
    }

    if (debug) printf("out of loop_type()\n");
    return result;
}

//WHILE_LOOP: 'while' GLOBAL_COND LOOP_STATEMENT
// First (while)
boolean _while_loop(){
    boolean result = false ;
    if (debug) printf("while_loop()\n");
    if (token == WHILE){
        token = _read_token();
        if(_global_cond()){
        token = _read_token();
            if (_loop_statement()){
                result = true;
            }
            else{
                result = false ;
            }
        }
        else{
            creer_sx_erreur(CONDITIONEXPECTED, tokenattribute.line);
            result = false ;
        }
    }
    else{
        result = false ;
    }
    if (debug) printf("out of while_loop()\n");
    return result ;
}

//FOR_LOOP: 'for' idf in INDEX_TYPE LOOP_STATEMENT
// First(for)
boolean _for_loop(){
    boolean result = false ;
    if (debug) printf("for_loop()\n");
    if (token == FOR){
        token = _read_token();
        if (token == IDF){
            token = _read_token();
            if (token == IN){
                token = _read_token();
                if (_index_type()){
                    token = _read_token();
                    if(_loop_statement()){
                        result = true ;
                    }
                    else{
                        result = false ;
                    }
                }
                else{
                    creer_sx_erreur(INDEXTYPEEXPECTED, tokenattribute.line);
                    result = false ;
                }
            }
            else{
                creer_sx_erreur(INEXPECTED, tokenattribute.line);
                result = false ;
            }
        }else{
            creer_sx_erreur(IDFEXPECTED, tokenattribute.line);
            result = false ;
        }
    }
    else{
        result = false ;
    }
    if (debug) printf("out of for_loop()\n");
    return result;
}

//INDEX_TYPE: integer range inumber .. inumber | inumber .. inumber | idf ' range
boolean _index_type(){
    boolean result = false ;
    if (debug) printf("index_type()\n");
    if (token == INTEGER){
        token = _read_token();
        if (token == RANGE){
            token = _read_token();
            if (token == INUMBER){
                token = _read_token();
                if (token == INTERVAL){
                    token = _read_token();
                    if(token == INUMBER){
                        result = true ;
                    }
                    else{
                        result = false ;
                    }
                }
                else{
                    result = false ;
                }
            }
            else{
                result = false ;
            }
        }
        else{
            result = false ;
        }
    }
    else if (token == INUMBER){
        token = _read_token();
        if (token == INTERVAL){
            token = _read_token();
            if(token == INUMBER){
                result = true ;
            }
            else{
                result = false ;
            }
        }
        else{
            result = false ;
        }
    }
    else  if (token == IDF){
        token = _read_token();
        if (token == APOSTROPHE){
            token = _read_token();
            if(token == RANGE){
                result = true ;
            }
            else{
                result = false ;
            }
        }
        else{
            result = false ;
        }
    }
    else {
        result = false ;
    }
    if (debug) printf("out of index_type()\n");
    return result ;
}

//OTHER_LOOP: 'loop' EXTRA_STATEMENT LOOP_CONDITION
// First('loop')
boolean _other_loop(){
    boolean result = false ;
    if (debug) printf("other_loop()\n");
    if (token == LOOP){
        token = _read_token();
        if(_extra_statement()){
            token = _read_token();
            if(_loop_condition()){
                result = true ;
            }
            else{
                result = false ;
            }
        }
        else{
            result = false ;
        }
    }else{
        result = false ;
    }
    if (debug) printf("out of other_loop()\n");
    return result;
}

//LOOP_CONDITION :  exit when GLOBAL_COND ';' EXTRA_STATEMENT | epsilon
//Follows (end)
//First (exit)
boolean _loop_condition(){
    boolean result = false ;
    if (debug) printf("loop_condition()\n");
    if (token == END){
        follow_token = true;
        result = true;
    }
    else if(token == EXIT){
        token = _read_token(); 
        if(token == WHEN){
        token = _read_token();
            if(_global_cond()){
                token = _read_token();
                if(token == SEMI_COLON){
                    token = _read_token();
                    if(_extra_statement()){
                        result = true ;
                    }
                    else {
                        result = false ;
                    }
                }else {
                    creer_sx_erreur(SEMICOLONEXPECTED, tokenattribute.line);
                    result = false ;
                }
            }else {
                creer_sx_erreur(CONDITIONEXPECTED, tokenattribute.line);
                result = false ;
            }
        }else {
            creer_sx_erreur(WHENEXPECTED, tokenattribute.line);
            result = false ;
        }
    }
    else{
        result = false ;
    }
    if (debug) printf("out of loop_condition()\n");
    return result ;
}

//EXTRA_STATEMENT: LIST_INST | epsilon
// Follows (end, exit)
// First (while, for, loop, if, case, idf, put, put_line)
boolean _extra_statement(){
    boolean result = false ;
    if (debug) printf("extra_statement()\n");
    if (token == END){
        follow_token = true;
        result = true;
    }
    if (token == EXIT){
        follow_token = true;
        result = true;
    }
    else if (_list_inst()){
        result = true;
    }
    else{
        result = false;
    }
    if (debug) printf("out of extra_statement()\n");
    return result;
}   
//LOOP_STATEMENT: loop LIST_INST
boolean _loop_statement(){
    boolean result = false;
    if (debug) printf("loop_statement()\n");
    if (token == LOOP){
        token = _read_token();
        if (_list_inst()){
            result = true;
        }
        else{
            result = false ;
        }
    }
    else{
        result = false ;
    }
    if (debug) printf("out of loop_statement()\n");
    return result;
}


// ADDSUB : MULTDIV ADDSUBAUX

boolean _addsub(AST *past){
    boolean result;
   if (debug) printf("addsub()\n");

   if (leftAssociativity == true){
       
        // reçoit un arbre [past] en paramètre avec un arbre droit NULL
        // lui installe l'arbre reçu de Multidiv comme arbre droit
        // le repasse à ADDSUBaux en paramètre

        AST *past1 = (AST *) malloc(sizeof(AST));
        AST *past2 = (AST *) malloc(sizeof(AST));

        (*past1) = (AST) malloc (sizeof(struct Exp));
            // past a un arbre droit NULL
        if (_multdiv(past1)){
            token = _read_token();
            
            if ((*past)->noeud.op.expression_gauche == NULL) (*past) = *past1; // initialisation par le première feuille gauche.
            else (*past)->noeud.op.expression_droite = *past1; 
            if (_addsubaux(past) == true){ 
                    if ((arbre_droit(*past) != NULL) && (arbre_gauche(*past) != NULL)) {
                        if (type(arbre_gauche(*past)) == type(arbre_droit(*past))){ // Int/Int ou Double/Double
                            (*past)->typename = type(arbre_gauche(*past));
                        }else{ 
                            /* (*past)->typename = Double;  printf("Sem Err ");*/
                            }
                    }else {(*past) = *past1;} // ??

                result = true;
            }else result = false;
        }else {
            result = false;
        }
    }else{
        // leftAssociativity == false
        // produit un arbre dégénéré droit (associativité droite aulieu de gauche)  
        AST *past1 = (AST *) malloc(sizeof(AST));
        AST *past2 = (AST *) malloc(sizeof(AST));

        if (_multdiv(past1)){
            token = _read_token();
            if (_addsubaux(past2) == true){
                if ( (*past1 != NULL) && (*past2 != NULL) ){
                    char op = ((top(*past2)==plus)?'+':((top(*past2)==moins)?'-':((top(*past2)==mult)?'*':'/')));
                    if (debug) printf("--------->type(*past1) ==%s\n",(type(*past1)==Integer)?"Int":(type(*past1)==Float)?"Double":"Bool");
                    if (debug) printf("--------->type(arbre_droit(*past2)) ==%s\n",(type(arbre_droit(*past2))==Integer)?"Int":(type(arbre_droit(*past2))==Float)?"Double":"Bool");
                    
                        if ( (type(*past1) == type(arbre_droit(*past2))) ){ //Int/Int ou Double/Double
                            *past = creer_noeud_operation(op, *past1, arbre_droit(*past2), type(*past1), tokenattribute.line); // 1ere inférence de type d'arbre arithmétique
                        } else{ 
                            *past = creer_noeud_operation(op, *past1, arbre_droit(*past2), Float, tokenattribute.line); // casting implicite Int[+-*/]Double ou Double[+-*/]Int ==> Double
                        }       
                    //}
                }else *past = *past1;
                result = true;
            }else result = false;
        }else result = false;
    }
   if (debug) printf("out of addsub()\n");
   return result;
}

// ADDSUBAUX : + MULTDIV ADDSUBAUX | - MULTDIV ADDSUBAUX | epsilon
// NULLABLE(ADDSUBAUX) = true
// Follows(')', then, 'loop', ';')
// First(ADDSUBAUX) = { '-' , '+' }
boolean _addsubaux(AST *past){
    boolean result;
   if (debug) printf("addsubaux()\n");

   if (leftAssociativity == true){
    // reçoit un arbre [past] en paramètre
    // crée un nouvel arbre [newpast] dont il place la racine [+ ou -], place [past] à sa gauche avec un arbre droit NULL
    // le repasse à ADDSUB en paramètre

    if ( (token == SEMI_COLON) || (token == PCLOSE) || (token == THEN) || (token == LOOP)){
        follow_token = true;
        result = true;
    }else if (token == PLUS) {
            token = _read_token();
            *past = creer_noeud_operation('+', *past, NULL, type(*past), tokenattribute.line); // 2eme inférence de type d'arbre arithmétique
            if (_addsub(past) == true){
                result = true;
            }else result = false;
    } else if (token == MINUS) {
            token = _read_token();
            *past = creer_noeud_operation('-', *past, NULL, type(*past), tokenattribute.line); // 2eme inférence de type d'arbre arithmétique
            if (_addsub(past) == true){
                result = true;
            }else result = false;
    } else {result = false; /* creer_sx_erreur(AddorsubExpected, tokenattribute.line); */}
    // NE PAS GERER LES ERREURS DES FOLLOWS  (ici (token == PVIRG) || (token == PCLOSE) ) DANS LES NULLABLES CAR DECONNECTES DE LEUR CONTEXTE D'APPEL (LE FOLLOW ET l'UNION DES POSSIBILITES ==> MESSAGES ERREURS TROP VAGUES)) MAIS LES GERER DANS LES APPELANTS DES NULLABLES PLUS CONTEXTUALISES !!

   }else{ //(leftAssociativity == false){
    // produit un arbre dégénéré droit (associativité droite aulieu de gauche)
    *past = NULL;

    AST *past1 = (AST *) malloc(sizeof(AST));

    if ( (token == SEMI_COLON) || (token == PCLOSE) || (token == THEN) || (token == LOOP) ){
        follow_token = true;
        result = true;
    }else if (token == PLUS) {
            token = _read_token();
            if (_addsub(past1) == true){
                if (*past1 != NULL)
                    *past = creer_noeud_operation('+', NULL, *past1, type(*past1), tokenattribute.line); // 2eme inférence de type d'arbre arithmétique
                result = true;
            }else result = false;
    } else if (token == MINUS) {
            token = _read_token();
            if (_addsub(past1) == true){
                if (*past1 != NULL)
                    *past = creer_noeud_operation('-', NULL, *past1, type(*past1), tokenattribute.line); //3eme inférence de type d'arbre arithmétique
                result = true;
            }else result = false;
    } else result = false;
   }
 if (debug) printf("out of addsubaux()\n");
 return result;
}

// MULTDIV : AUX MULTDIVAUX
boolean _multdiv(AST *past){
    boolean result;
   if (debug) printf("multdiv()\n");

   if (leftAssociativity == true){
    // reçoit un arbre [past] en paramètre avec un arbre droit NULL
    // lui installe l'arbre reçu de Aux comme arbre droit
    // le repasse à Multdivaux en paramètre

    AST *past1 = (AST *) malloc(sizeof(AST));
    AST *past2 = (AST *) malloc(sizeof(AST));

        // NEW 3 : past a un arbre droit NULL
    (*past1) = (AST) malloc (sizeof(struct Exp));

    if (_aux(past1)){
        token = _read_token();

        if ((*past)->noeud.op.expression_gauche == NULL) (*past) = *past1; // initialisation par le première feuille gauche.
        else (*past)->noeud.op.expression_droite = *past1;

        if (_multdivaux(past) == true){
            //printf("before if");
                if ((arbre_droit(*past) != NULL) && (arbre_gauche(*past) != NULL)) {
                    //printf("after if");
                    if (type(arbre_gauche(*past)) == type(arbre_droit(*past))){ // Int/Int ou Double/Double
                        (*past)->typename = type(arbre_gauche(*past));
                    }else {
                        //(*past)->typename = Double;
                        printf("Semantic Error different type ");
                        if (debug) {printf("<multdivICI>\n");afficher_infixe_arbre(*past);printf("\n");}
                    }
                }else {
                    //printf("before past ");
                    (*past) = *past1;
                    //printf("after past ");
                } // ??
            result = true;
        }else result = false;
    }else result = false;
    /*
            aux()
            out of aux()
            multdivaux()
                multdiv()
                    aux()
                    out of aux()
                    multdivaux()
                    out of multdivaux()
                out of multdiv()
            out of multdivaux()

    */

    }else{// (leftAssociativity == false){
    // produit un arbre dégénéré droit (associativité droite aulieu de gauche)
    *past = NULL;

    AST *past1 = (AST *) malloc(sizeof(AST));
    AST *past2 = (AST *) malloc(sizeof(AST));

    if (_aux(past1)){
        token = _read_token();
        if (_multdivaux(past2) == true){
            if ( (*past1 != NULL) && (*past2 != NULL) ){
                if (debug) printf("( (*past1 != NULL) && (*past2 != NULL) )\n");
                char op = ((top(*past2)==plus)?'+':((top(*past2)==moins)?'-':((top(*past2)==mult)?'*':'/')));
                if (type(*past1) == type(arbre_droit(*past2))){ //Int/Int ou Double/Double
                    *past = creer_noeud_operation(op, *past1, arbre_droit(*past2), type(*past1), tokenattribute.line); // 4eme inférence de type
                } else {
                    *past = creer_noeud_operation(op, *past1, arbre_droit(*past2), Float, tokenattribute.line); // casting implicite Int[+-*/]Double ou Double[+-*/]Int ==> Double
                }
            }else {
                *past = *past1;
            }
            result = true;
        }else result = false;
    }else result = false;
    }
    if (debug) printf("out of multdiv()\n");
    return result;
}

// MULTDIVAUX : * AUX MULTDIVAUX | / AUX MULTDIVAUX | epsilon
// NULLABLE(MULTDIVAUX) = true
// Follows('-', '+', ')', then, 'loop', ';')
// First(MULTDIVAUX) = { '*', '/' }
boolean _multdivaux(AST *past){
    boolean result;
    if (debug) printf("multdivaux()\n");

   if (leftAssociativity == true){
    // reçoit un arbre [past] en paramètre
    // crée un nouvel arbre [newpast] dont il place la racine [/ ou *], place [past] à sa gauche avec un arbre droit NULL
    // le repasse à MultDiv en paramètre
    
    if ( (token == PLUS) || (token == MINUS) || (token == SEMI_COLON) || (token == PCLOSE) ){
        follow_token = true;
        result = true;
    }else if (token == MULT) {
            token = _read_token();
            *past = creer_noeud_operation('*', *past, NULL, type(*past), tokenattribute.line); // NEW 7 : 2eme inférence de type d'arbre arithmétique
            if (_multdiv(past)){
                result = true;
            }else result = false;
    } else if (token == DIV) {
            token = _read_token();
            *past = creer_noeud_operation('/', *past, NULL, type(*past), tokenattribute.line); // NEW 8 : 2eme inférence de type d'arbre arithmétique
            if (_multdiv(past)){
                result = true;          
            }else result = false;
    } else {result = false;/* creer_sx_erreur(MultordivExpected, tokenattribute.line);*/} // NE PAS GERER LES ERREURS DES FOLLOWS  (ici (token == PLUS) || (token == MINUS) || (token == PVIRG) || (token == PCLOSE) ) DANS LES NULLABLES CAR DECONNECTES DE LEUR CONTEXTE D'APPEL (LE FOLLOW ET l'UNION DES POSSIBILITES ==> MESSAGES ERREURS TROP VAGUES)) creer_sx_erreur(MultordivorplusorminusorpvirgorpcloseExpected, yylineno);

   }else{ //(leftAssociativity == false){   
    // produit un arbre dégénéré droit (associativité droite aulieu de gauche)
    *past = NULL;

    AST *past1 = (AST *) malloc(sizeof(AST));
    
    if ( (token == PLUS) || (token == MINUS) || (token == SEMI_COLON) || (token == PCLOSE) ){
        follow_token = true;
        result = true;
    }else if (token == MULT) {
            token = _read_token();
            if (_multdiv(past1)){
                if ( (*past1 != NULL) )
                    *past = creer_noeud_operation('*', NULL, *past1, type(*past1), tokenattribute.line); // 5eme inférence de type d'arbre arithmétique
                result = true;
            }else result = false;
    } else if (token == DIV) {
            token = _read_token();
            if (_multdiv(past1)){
                if ( (*past1 != NULL) )
                    *past = creer_noeud_operation('/', NULL, *past1, type(*past1), tokenattribute.line); //6eme inférence de type d'arbre arithmétique
                result = true;          
            }else result = false;
    } else result = false;
   }

   if (debug) printf("out of multdivaux()\n");
   return result;
}

//AUX : idf | inumber | fnumber | ( ADDSUB )
boolean _aux(AST *past){
    boolean result;


    if (debug) printf("aux()\n");

    if (leftAssociativity == false){ *past = NULL; }

    if  (token == IDF) {
        //les test pour sm error
         if (est_definie(rangvar) == false){
            *past = creer_feuille_idf(name(rangvar), typevar(rangvar)); // On ne peut pas donc avoir un AST de type Bool (meme si l'IDF est un Bool)
        }else{
            // l'IDF est Int ou Double et est initialisee 
            *past = creer_feuille_idf(name(rangvar), typevar(rangvar)); // On ne peut pas donc avoir un AST de type Bool (meme si l'IDF est un Bool)
            //utiliser( rangvar ); //sert à l'optimisateur
        }
        result = true;
    } else if (token == INUMBER) {
        *past = creer_feuille_nombre(constattribute.valinit, Integer);
        result = true;
    } else if (token == FNUMBER) {
        *past = creer_feuille_nombre(constattribute.valinit, Float);
        result = true;
    }  else if (token == POPEN) {
        token = _read_token();
        if (_addsub(past)){
            token = _read_token();
            if (token == PCLOSE){
                result = true;
            }else {
                result = false;
                //creer_sx_erreur(PcloseExpected, tokenattribute.line);
            }
        }else result = false;
    } else  {result = false;
    creer_sx_erreur(VALUEEXPECTED, tokenattribute.line);}


    if (debug) printf("out of aux()\n");
    return result;
}

//CASE_STATEMENT : case idf is CASE_BODY 'when_others' '=>' LIST_INST end case ;
// First (case)
boolean _case_statement(){
    boolean result;
    if (debug) printf("case_statement()\n");

    if(token == CASE){
        token = _read_token();
        if(token == IDF){
            token = _read_token();
            if(token == IS){
                token = _read_token();
                if(_case_body()){
                    token = _read_token();
                    if(token == WHEN_OTHERS){
                        token = _read_token();
                        if(token == ARROW){
                            token = _read_token();
                            if(_list_inst()){
                                token = _read_token();
                                if(token == END){
                                    token = _read_token();
                                    if(token == CASE){
                                        result = true;
                                    }else{
                                        creer_sx_erreur(CASEEXPECTED, tokenattribute.line);
                                        result = false;
                                    }
                                }else{
                                    creer_sx_erreur(ENDEXPECTED, tokenattribute.line);
                                    result = false;
                                }
                            }else{
                                result = false;
                            }
                        }else{
                            creer_sx_erreur(ARROWEXPECTED, tokenattribute.line);
                            result = false;
                        }
                        
                    }else{
                        creer_sx_erreur(WHENOTHERSEXPECTED, tokenattribute.line);
                        result = false;
                    }
                }else{
                    result = false;
                }
            }else{
                creer_sx_erreur(ISEXPECTED, tokenattribute.line);
                result = false;
            }
        }else{
            creer_sx_erreur(IDFEXPECTED, tokenattribute.line);
            result = false;
        }
    }else{
        result = false;
    }

    if (debug) printf("out of case_statement()\n");
    return result;
}

//CASE_BODY :  when VALUE '=>' LIST_INST CASE_BODY | epsilon
// Follows ('when_others')
boolean _case_body(){
    boolean result;
    if (debug) printf("case_statement()\n");

    if(token == WHEN_OTHERS){
        follow_token = true;
        result = true;
    }else if (token == WHEN){
        token = _read_token();
        if(_value()){
            token = _read_token();
            if(token == ARROW){
                token = _read_token();
                if(_list_inst()){
                    token = _read_token();
                    if(_case_body()){
                        result = true;
                    }else{
                        result = false;
                    }
                    
                }else{
                    result = false;
                }
            }else{
                creer_sx_erreur(ARROWEXPECTED, tokenattribute.line);
                result = false;
            }
        }else{
            result = false;
        }
    }else{
        result = false;
    }

    if (debug) printf("out of case_statement()\n");
    return result;
}
//CONDITION_BOOL : true | false
boolean _condition_bool(){
    boolean result;
    if (debug) printf("condition_bool()\n");

    if(token == TRUE){
        result = true;
    }else if(token == FALSE){
        result = false;
    }else{
        result = false;
    }

    if (debug) printf("out of condition_bool()\n");
    return result;
}
//CONDITION_ARITH : VALUE LOGICAL_OPERATOR VALUE

boolean _condition_arith(){
    boolean result;
    if (debug) printf("condition_arith()\n");

    if(_value()){
        token = _read_token();
        if(_operator_arith()){
            token = _read_token();
            if(_value()){
                result = true;
            }else{
                result = false;
            }
        }else{
            result = false;
        }
    }else{
        result = false;
    }

    if (debug) printf("out of condition_arith()\n");
    return result;
}
//OPERATOR_ARITH : '=' | '/=' | '<' | '>' | '<=' | '>='
boolean _operator_arith(){
    boolean result;
    if (debug) printf("operator_arith()\n");

    if(token == EQUAL){
        result = true;
    }else if (token == DIFFERENT){
        result = true;
    }else if (token == LESS){
        result = true;
    }else if (token == GREATER){
        result = true;
    }else if (token == LESS_OR_EQ){
        result = true;
    }else if (token == GREATER_OR_EQ){
        result = true;
    }else{
        result = false;
    }

    if (debug) printf("out of operator_arith()\n");
    return result;
}
//COND : CONDITION_BOOL | CONDITION_ARITH
boolean _cond(){
    boolean result;
    if (debug) printf("cond()\n");

    if(_condition_bool()){
        result = true;
    }else if (_condition_arith()){
        result = true;
    }else{
        result = false;
    }

    if (debug) printf("cond()\n");
    return result;
}
//LIST_COND : COND LIST_COND_AUX
boolean _list_cond(){
    boolean result;
    if (debug) printf("list_cond()\n");

    if(_cond()){
        result = true;
        token = _read_token();
        if (_list_cond_aux()){
            result = true;
        }else{
        result = false;
        }
    }

    if (debug) printf("out of list_cond()\n");
    return result;
}

//LIST_COND_AUX : OR_LIST_COND_AUX | AND_LIST_COND_AUX | XOR_LIST_COND_AUX
boolean _list_cond_aux(){
    boolean result;
    if (debug) printf("list_cond_aux()\n");

    if(_or_list_cond_aux()){
        result = true;   
    }else if (_xor_global_cond_aux()){
        result = true;
    }else if (_and_global_cond_aux()){
        result = true;
    }else{
        result = false;
    }

    if (debug) printf("out of list_cond_aux()\n");
    return result;
}
//OR_LIST_COND_AUX : 'or' COND OR_LIST_COND_AUX | epsilon
// Follows ( ')', then, 'loop', ';' )
boolean _or_list_cond_aux(){
    boolean result;
    if (debug) printf("or_list_cond_aux()\n");

    if(token == PCLOSE){
        follow_token = true;
        result = true;
    }else if(token == THEN){
        follow_token = true;
        result = true;
    }else if(token == LOOP){
        follow_token = true;
        result = true;
    }else if(token == SEMI_COLON){
        follow_token = true;
        result = true;
    }else if (token == OR){
        token = _read_token();
        if(_cond()){
            token = _read_token();
            if(_or_list_cond_aux()){
                result = true;
            }else{
                result = false;
            }
        }else{
            result = false;
        }
    }else{
        result = false;
    }

    if (debug) printf("out of or_list_cond_aux()\n");
    return result;
}
//AND_LIST_COND_AUX : 'and' COND AND_LIST_COND_AUX | epsilon
// Follows ( ')', then, 'loop', ';' )
boolean _and_list_cond_aux(){
    boolean result;
    if (debug) printf("and_list_cond_aux()\n");

    if(token == PCLOSE){
        follow_token = true;
        result = true;
    }else if(token == THEN){
        follow_token = true;
        result = true;
    }else if(token == LOOP){
        follow_token = true;
        result = true;
    }else if(token == SEMI_COLON){
        follow_token = true;
        result = true;
    }else if (token == AND){
        token = _read_token();
        if(_cond()){
            token = _read_token();
            if(_and_list_cond_aux()){
                result = true;
            }else{
                result = false;
            }
        }else{
            result = false;
        }
    }else{
        result = false;
    }

    if (debug) printf("out of and_list_cond_aux()\n");
    return result;
}
//XOR_LIST_COND_AUX : 'xor' COND XOR_LIST_COND_AUX | epsilon
// Follows ( ')', then, 'loop', ';' )
boolean _xor_list_cond_aux(){
    boolean result;
    if (debug) printf("xor_list_cond_aux()\n");

    if(token == PCLOSE){
        follow_token = true;
        result = true;
    }else if(token == THEN){
        follow_token = true;
        result = true;
    }else if(token == LOOP){
        follow_token = true;
        result = true;
    }else if(token == SEMI_COLON){
        follow_token = true;
        result = true;
    }else if (token == XOR){
        token = _read_token();
        if(_cond()){
            token = _read_token();
            if(_xor_list_cond_aux()){
                result = true;
            }else{
                result = false;
            }
        }else{
            result = false;
        }
    }else{
        result = false;
    }

    if (debug) printf("out of xor_list_cond_aux()\n");
    return result;
}
//GLOBAL_COND : '(' LIST_COND ')' GLOBAL_COND_AUX | LIST_COND
boolean _global_cond(){
    boolean result;
    if (debug) printf("global_cond()\n");

    if(token == POPEN){
        token = _read_token();
        if(_list_cond()){
            token = _read_token();
            if(token == PCLOSE){
                token = _read_token();
                if(_global_cond_aux()){
                    result = true;
                }else{
                    result = false;
                }
            }else{
                result = false;
            }
        }else{
            result = false;
        }
    }else if (_list_cond()){
        result = true;
    }else{
        result = false;
    }

    if (debug) printf("out of global_cond()\n");
    return result;
}
//GLOBAL_COND_AUX : OR_GLOBAL_COND_AUX | AND_GLOBAL_COND_AUX | XOR_GLOBAL_COND_AUX
boolean _global_cond_aux(){
    boolean result;
    if (debug) printf("global_cond_aux()\n");

    if(_or_list_cond_aux()){
        result = true;
    }else if(_and_list_cond_aux()){
        result = true;
    }else if(_xor_list_cond_aux()){
        result = true;
    }else{
        result = false;
    }

    if (debug) printf("out of global_cond_aux()\n");
    return result;
}
//OR_GLOBAL_COND_AUX : 'or' '(' LIST_COND ')' OR_GLOBAL_COND_AUX | epsilon
// Follows (then, 'loop', ';')
boolean _or_global_cond_aux(){
    boolean result = false ;
    if ( token == THEN ){
        return true ;
    }
    else if ( token == LOOP ){
        return true ;
    }
    else if ( token == SEMI_COLON ){
        return true ;
    }
    else if(token == OR){
            token = _read_token();
            if(token == POPEN){
                token = _read_token();
                if(_list_cond()){
                    token = _read_token();
                    if(token == PCLOSE){
                        token = _read_token();
                        if(_or_global_cond_aux()){
                            result = true ;
                        }
                        else {
                            result = false ;
                        }
                    }
                    else {
                        result = false ;
                    }
                }else {
                    result = false ;
                }
            }else {
                result = false ;
            }
        }
    else {
        result = false ;
    }
}


//AND_GLOBAL_COND_AUX : 'and' '(' LIST_COND ')' AND_GLOBAL_COND_AUX | epsilon
// Follows (then, 'loop', ';')
boolean _and_global_cond_aux(){
    boolean result = false ;
    if ( token == THEN ){
        return true ;
    }
    else if ( token == LOOP ){
        return true ;
    }
    else if ( token == SEMI_COLON ){
        return true ;
    }
    else if(token == AND){
            token = _read_token();
            if(token == POPEN){
                token = _read_token();
                if(_list_cond()){
                    token = _read_token();
                    if(token == PCLOSE){
                        token = _read_token();
                        if(_and_global_cond_aux()){
                            result = true ;
                        }
                        else {
                            result = false ;
                        }
                    }
                    else {
                        result = false ;
                    }
                }else {
                    result = false ;
                }
            }else {
                result = false ;
            }
        }
    else {
        result = false ;
    }
}

//XOR_GLOBAL_COND_AUX : 'xor' '(' LIST_COND ')' XOR_GLOBAL_COND_AUX | epsilon
// Follows (then, 'loop', ';')
boolean _xor_global_cond_aux(){
    boolean result = false ;
    if ( token == THEN ){
        return true ;
    }
    else if ( token == LOOP ){
        return true ;
    }
    else if ( token == SEMI_COLON ){
        return true ;
    }
    else if(token == XOR){
            token = _read_token();
            if(token == POPEN){
                token = _read_token();
                if(_list_cond()){
                    token = _read_token();
                    if(token == PCLOSE){
                        token = _read_token();
                        if(_xor_global_cond_aux()){
                            result = true ;
                        }
                        else {
                            result = false ;
                        }
                    }
                    else {
                        result = false ;
                    }
                }else {
                    result = false ;
                }
            }else {
                result = false ;
            }
        }
    else {
        result = false ;
    }
}
/*  LIST_INST : INST LIST_INST_AUX
 */
boolean _list_inst(){
    boolean result ;
    if (debug) printf("list_inst()\n");

    if(_inst()){
        token = _read_token();
        if(_list_inst_aux()){
            result = true;
        }else{
            result = false;
        }
    }else {
        result = false;
    }

    if (debug) printf("out of list_inst()\n");
    return result;
}
/*  LIST_INST_AUX : LIST_INST | epsilon
    Follows ('end')
    First (while, for, loop, if, case, idf, put, put_line)
 */
boolean _list_inst_aux(){
    boolean result ;
    if (debug) printf("list_inst_aux()\n");

    if( token == END ){
        follow_token = true;
        result = true;
    }else if (_list_inst()) {
        result = true;
    }else {
        result = false;
    }
    if (debug) printf("out of list_inst_aux()\n");
    return result;
}

//INST : LOOP | IF_STATEMENT | CASE_STATEMENT | ASSIGNMENT_STATEMENT | PUT_STATEMENT
boolean _inst(){
    boolean result;
    if(debug) printf("inst()\n");

    if(_loop()){
        result = true;
    }else if (_if_statement()) {
        result = true;
    }else if(_case_statement()){
        result = true;
    }else if (_assignment_statement()) {
        result = true;
    }else if (_put_statement()) {
        result = true;
    }else{
        result = false;
    }
    if (debug) printf("out of list_inst_aux()\n");
    return result;
}
/* PUT_STATEMENT : 'put' '(' VALUE ')' ';' 
                | 'put_line' '(' VALUE ')' ';'
 */
boolean _put_statement() {
    boolean result;
    if(debug) printf("put_statement()\n");

    if( token == PUT ){
        token = _read_token();
        if( token ==  POPEN){
            token = _read_token();
            if(_value()){
                token = _read_token();
                if( token == PCLOSE){
                    token = _read_token();
                    if( token == SEMI_COLON){
                        result = true;
                    }else{
                        creer_sx_erreur(SEMICOLONEXPECTED, tokenattribute.line);
                        result = false;
                    }
                }else{
                    creer_sx_erreur(PCLOSEEXPECTED, tokenattribute.line);
                    result = false;
                }
            }else {
                result = false;
            }
        }else{
            creer_sx_erreur(POPENEXPECTED, tokenattribute.line);
            result = false;
        }
    }else if( token == PUT_LINE ){
        token = _read_token();
        if( token ==  POPEN){
            token = _read_token();
            if(_value()){
                token = _read_token();
                if( token == PCLOSE){
                    token = _read_token();
                    if( token == SEMI_COLON){
                        result = true;
                    }else{
                        result = false;
                    }
                }else{
                    result = false;
                }
            }else {
                result = false;
            }
        }else{
            result = false;
        }
    }else{
        result = false;
    }
    if (debug) printf("out of put_statement()\n");
    return result;
}

/* VALUE : inumber 
        | fnumber 
        | char 
        | str 
        | true | false 
        | idf
 */
boolean _value(){
    boolean result;
    if(debug) printf("value()\n");

    if( token == INUMBER ){
        result = true;
    }else if ( token == FNUMBER ){
        result = true;
    }else if ( token == CHAR ){
        result = true;
    }else if ( token == CSTR ){
        result = true;
    }else if ( token == TRUE){
        result = true;
    }else if (token == FALSE){
        result = true;
    }else if (token == IDF){
        result = true;
    }else{
        result = false;
    }

    if (debug) printf("out of value()\n");
    return result;
}

//ASSIGNMENT_STATEMENT : idf ASSIGNMENT ;
boolean _assignment_statement(){
    boolean result;
    if(debug) printf("assignment_statement()\n");

    if( token == IDF ){
        token = _read_token();
        if( _assignment()){
            result = true;
        }else {
            result = false;
        }
    }else {
        result = false;
    }
    if (debug) printf("out of assignment_statement()\n");
    return result;
}

//ASSIGNMENT : ':=' EXPRESSION
boolean _assignment(){
    boolean result;
    if(debug) printf("assignement()\n");

    if( token == AFFECTATION){
        token = _read_token();
        if(_expression()){
            for(int i = 0; i<NBRMAXIDF; i++){
            varattribute[i].initialisation = true;
            }
            result = true;
        }else{
            result = false;
        }
    }else{
        creer_sx_erreur(AFFECTATIONEXPECTED, tokenattribute.line);
        result = false;
    }
    if (debug) printf("out of assignment()\n");
    return result;
}
//EXPRESSION : char | cstr | true | false | ADDSUB 
boolean _expression(){
    boolean result;
    if(debug) printf("_expression()\n");

    if( token == CHAR ){
        constattribute.typename = Character;
        result = true;
    }else if( token == CSTR ){
        constattribute.typename = String;
        result = true;
    }else if( token == TRUE ){
        constattribute.typename = Boolean;
        result = true;
    }else if( token == FALSE ){
        constattribute.typename = Boolean;
        result = true;
    }else if(_addsub(astattribute)){
        constattribute.typename = (*astattribute)->typename;
        constattribute.valinit = evaluer(*astattribute);
        result = true;
    }else{
        result = false;
    }
    if (debug) printf("out of expression()\n");
    return result;
}
/* IF_STATEMENT : 
        if GLOBAL_COND then LIST_INST ELSE_STATEMENT end if ;
 */
boolean _if_statement(){
    boolean result;
    if(debug) printf("if_statement()");

    if( token == IF ){
        token = _read_token();
        if( _global_cond()){
            token = _read_token();
            if( token == THEN ){
                token = _read_token();
                if(_list_inst()){
                    token = _read_token();
                    if(_else_statement()){
                        token = _read_token();
                        if( token == END ){
                            token = _read_token();
                            if( token == IF ){
                                token = _read_token();
                                if( token == SEMI_COLON){
                                    result = true;
                                }else{
                                    creer_sx_erreur(SEMICOLONEXPECTED, tokenattribute.line);
                                    result = false;
                                }
                            }else{
                                creer_sx_erreur(IFEXPECTED, tokenattribute.line);
                                result = false;
                            }
                        }else{
                            creer_sx_erreur(ENDEXPECTED, tokenattribute.line);
                            result = false;
                        }
                    }else{
                        result = false;
                    }
                }else{
                    result = false;
                }
            }else{
                creer_sx_erreur(THENEXPECTED, tokenattribute.line);
                result = false;
            }
        }else{
            creer_sx_erreur(CONDITIONEXPECTED, tokenattribute.line);
            result = false;
        }
    }else{
        result = false;
    }
    if (debug) printf("out of if_statement()\n");
    return result; 
}

/* ELSE_STATEMENT : else LIST_INST 
                | elsif GLOBAL_COND then LIST_INST ELSE_STATEMENT 
                | epsilon
Follows (end)
First (else,elsif) */
boolean _else_statement(){
    boolean result;
    if(debug) printf("else_statement()");
    
    if( token == END){
        follow_token = true;
        result = true;
    }else if( token == ELSE ){
        token = _read_token();
        if( _list_inst()){
            result = true;
        }else{
            result = false;
        }
    }else if( token == ELSIF ){
        token = _read_token();
        if( _global_cond()){
            token = _read_token();
            if( token == THEN ){
                token = _read_token();
                if(_list_inst()){
                    token = _read_token();
                    if( _else_statement()){
                        result = true;
                    }else{
                        result = false;
                    }
                }else{
                    result = false;
                }
            }else{
                creer_sx_erreur(THENEXPECTED, tokenattribute.line);
                result = false;
            }
        }else{
            creer_sx_erreur(CONDITIONEXPECTED, tokenattribute.line);
            result = false;
        }
    }else{
        result = false;
    }

}

void set_idf_attributes(char *name, int line){
    if (debug) printf("[%s]", name);
    varattribute[NBRMAXIDF].name = (char *)malloc(sizeof(char) * strlen(name)+1);
    strcpy(varattribute[NBRMAXIDF].name, name);
    if (debug) printf("[%s]", varattribute[NBRMAXIDF].name);
    varattribute[NBRMAXIDF].line = line;
    NBRMAXIDF++;
}

void set_number_attributes(double value){
    constattribute.valinit = value;
}

void set_string_attributes(char * s){
    if (debug) printf("set_string_attributes()\n");
    if ((debug) && (s == NULL)) printf("s NULL!!!!!!!\n"); 

    stringattribute.value = (char *)malloc((strlen(s)+1) * sizeof(char));
    strcpy(stringattribute.value, s);
    if (debug) printf("out set_string_attributes()\n");
}

void set_Mode_Optimisation( boolean _optimisationMode ){
   optimisationMode = _optimisationMode;
}

boolean get_Mode_Optimisation( ){
    return optimisationMode;
}

void set_Left_Associativity( boolean _leftAssociativity ){
    leftAssociativity = _leftAssociativity;
}

boolean get_Left_Associativity( ){
    return leftAssociativity;
}


void set_token_attributes(int line){
    tokenattribute.line = line;
}