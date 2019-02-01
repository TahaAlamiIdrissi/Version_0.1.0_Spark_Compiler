%{
// @auteur Karim Baïna, ENSIAS, Décembre 2010 updated Décembre 2018
#include "interpreteur.h"
#include <string.h>
#define debug false
%}
%option noyywrap
%option yylineno
%%
\"[^"]*\"	      {if (debug) printf ("CSTRING\n"); set_string_attributes( yytext ); return CSTRINGOP;}
"add"		 	{if (debug) printf ("ADD\n"); return ADDOP;}
"idiv" 			{if (debug) printf ("IDIV\n"); return IDIVOP;}
"ddiv" 			{if (debug) printf ("DDIV\n"); return DDIVOP;}
"dupl" 			{if (debug) printf ("DUPL\n"); return DUPLOP;}
"jmp" 			{if (debug) printf ("JMP\n"); return JMPOP;}
"jne" 			{if (debug) printf ("JNE\n"); return JNEOP;}
"jg" 			{if (debug) printf ("JG\n"); return JGOP;}
"jeq" 			{if (debug) printf ("JEQ\n"); return JEQOP;}
"load" 			{if (debug) printf ("LOAD\n"); return LOADOP;}
"mult" 			{if (debug) printf ("MULT\n"); return MULTOP ;}
"printi"			{if (debug) printf ("PRINT\n"); return PRINTIDFOP;}
"prints"			{if (debug) printf ("PRINT\n"); return PRINTSTROP;}
"push" 			{if (debug) printf ("PUSH\n"); return PUSHOP;}
"sub" 			{if (debug) printf ("SUB\n"); return SUBOP;}
"store"			{if (debug) printf ("STORE\n"); return STOREOP;}
"swap" 			{if (debug) printf ("SWAP\n"); return SWAPOP;}
"begin:"		{
			if (debug) printf ("BEGIN LABEL\n"); 
			set_label_attributes("begin\n", yylineno);
			return BEGINOP;
			}
"end:"			{
			if (debug) printf ("END LABEL\n"); 
			set_label_attributes("end\n", yylineno);
			return ENDOP;
			}
[a-zA-Z_][a-zA-Z0-9_]*":" {
			if (debug) printf ("LABEL\n"); 
			int i = strlen(yytext);
			char * label_name = (char*) malloc(i);
			i--;
			label_name[i]='\0';
			while(--i >= 0) label_name[i] = yytext[i]; // le yytext sans le ':'
			set_label_attributes(label_name, yylineno);
			return LABELOP;
			}
[a-zA-Z_][a-zA-Z0-9_]* 	{
			if (debug) printf ("IDF(%s)\n",yytext);
			set_idf_attributes(yytext, yylineno);
			return IDFOP;
			}
[0-9]+[.][0-9]+		{
			if (debug) printf ("NUMBER(%s)\n",yytext);
			set_number_attributes( atof(yytext) );
			return DNUMBEROP;
			}
[0-9]+			{
			if (debug) printf ("NUMBER(%s)\n",yytext);
			set_number_attributes((double)atoi(yytext));
			return INUMBEROP;
			}
[R][E][M][^\n]*[\n]     /* commentaires */
[ \t\n]		        /* caractère vide */
.			{set_token_attributes(yylineno);}
%%
