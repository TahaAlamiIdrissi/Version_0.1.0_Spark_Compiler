// @auteur Karim Baïna, ENSIAS, Décembre 2010

#include "pile.h"
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

#define debug false

Pile * creer_pile(){
	Pile * pile = (Pile *) malloc(sizeof (Pile));

	pile->taille = -1;

	return pile;
}

boolean pile_vide(Pile  *p){
	return (p->taille == -1);
}

void empiler(Pile * p, Element e){
	if (debug) {printf("empiler()\n");}

	switch (e.etype){
		case DBL : setdvalue(&(p->elements[p->taille + 1]), dvalue(e)); break;
		case STR : setsvalue(&(p->elements[p->taille + 1]), svalue(e)); break;
		default : if (debug) {printf("ni DBL ni STR");} break;
	}

	
	p->taille ++;

	if (debug) {printf("out empiler()\n");}
}

// pré-condition : pile_vide(p) = false
Element depiler(Pile * p){
	Element result;

	if (debug) {printf("depiler()\n");}

	if ((debug) && (p->taille == -1))  {printf("------BUGG------------------------>PILE VIDE!!\n");}
	switch ( p->elements[ p->taille ].etype ){
		case DBL : setdvalue(&result, dvalue(p->elements[ p->taille ])); break;
		case STR : setsvalue(&result, svalue(p->elements[ p->taille ])); break;
		default : if (debug) {printf("ni DBL ni STR");} break;
	}

	p->taille--;

	if (debug) {printf("out depiler()\n");}

	return result;
}

// affiche l'état de la pile (pour débugguer)
void etat_pile(Pile *p){
	int i = 0;
	printf(" [");
	for(i = 0; i<= p->taille; i++)	{
		switch ((p->elements[ i ]).etype){
			case DBL : printf(" %2lf ", (p->elements[ i ]).evalue.dvalue); break;
			case STR : printf(" %s ", (p->elements[ i ]).evalue.svalue); break;
			default : if (debug) {printf("ni DBL ni STR");} break;
		}
	}
	printf("]\n");
}
/*
 int main(){
	Pile *p = creer_pile();

	if (pile_vide(p) != true){
		printf("erreur 1");
	}else {
		Element a, b;
		a.etype=DBL;
		a.evalue.dvalue=1.0;
	
		b.etype=DBL;
		b.evalue.svalue=(char*)malloc(5);
		strcpy(b.evalue.svalue, "mama");

		empiler(p, a);
		Element e = depiler(p);
		
		etat_pile( p );
	}
} 
*/
