interpreteurlex : interpreteur.h interpreteur.l
	flex -i interpreteur.l
	mv lex.yy.c interpreteur.yy.c
	gcc -c interpreteur.yy.c

interpreteur : interpreteur.h vm.h pseudocode.h tab_symb.h i18n.h cfg.h interpreteur.c
	gcc -c interpreteur.c

interpreteurout : interpreteurlex interpreteur tab_Symb pseudocode pile vm error i18n ast
	gcc -o sparki error.o i18n.o tab_symb.o pseudocode.o pile.o vm.o ast.o cfg.o interpreteur.yy.o interpreteur.o

lex : analyser.h i18n.h analyser.l
	flex -i analyser.l
	gcc -c lex.yy.c

cfg : pseudocode.h ast.h cfg.h tab_symb.h cfg.c
	gcc -c cfg.c

pile : pile.h cfg.h pile.c
	gcc -c pile.c

vm : vm.h pseudocode.h pile.h cfg.h error.h
	gcc -c vm.c

error : error.h i18n.h error.c
	gcc -c error.c

tab_Symb : tab_symb.h cfg.h tab_symb.c
	gcc -c tab_symb.c

ast : ast.h ast.c pseudocode.h
	gcc -c ast.c

pseudocode : pseudocode.h pseudocode.c
	gcc -c pseudocode.c

i18n : i18n.h i18n.c
	gcc -c i18n.c

analyser : cfg.h error.h analyser.h pseudocode.h vm.h tab_symb.h i18n.h analyser.c
	gcc -c analyser.c

analyserout :  lex cfg error tab_Symb analyser ast 
	gcc -o sparkc cfg.o ast.o tab_symb.o lex.yy.o analyser.o

all : analyserout interpreteurout

clean :
	rm *.o
	rm *.yy.c
	rm sparkc sparki 
	rm *~
	rm *.exe
