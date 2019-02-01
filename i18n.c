// @auteur Karim Baïna, ENSIAS, Décembre 2018

#include "i18n.h"

Langue LANGUE_COURANTE = English; /* default language */

void set_LANGUE_COURANTE(Langue l){
	LANGUE_COURANTE = l;
}

Langue get_LANGUE_COURANTE(){
	return LANGUE_COURANTE ;
}

