#ifndef INTERPRETEUR_H
#define INTERPRETEUR_H

#include "analyser.h"

// @auteur Karim Baïna, ENSIAS, Décembre 2010, mise à jour Décembre 2012, puis Décembre 2018

//typedef enum {false=0, true=1} boolean;

typedef enum {
ADDOP = 10000,
IDIVOP = 10001,
DDIVOP = 10002,
DUPLOP = 10003,
JMPOP = 10004,
JNEOP = 10005,
JGOP = 10006,
JEQOP=10007,
LABELOP = 10008,
LOADOP = 10009,
MULTOP = 100010,
PRINTIDFOP = 10011,
PRINTSTROP = 10012,
PUSHOP = 10013,
SUBOP = 10014,
STOREOP = 10015,
SWAPOP = 10016,
IDFOP = 10017,
DNUMBEROP = 10018,
INUMBEROP = 10019,
BEGINOP = 10020,
ENDOP = 10021,
CSTRINGOP = 10022
//,ERRORLEX = 10021
} typetokenOp;


void set_idf_attributes(char *name, int line);
void set_number_attributes(double value);
void set_label_attributes(char *name, int line);
void set_string_attributes(char * s);
void set_token_attributes(int line);


#endif
