clear
flex -i analyser.l
gcc -c lex.yy.c
gcc -c analyser.c
gcc -c tab_symb.c
gcc -c error.c
gcc -o parser lex.yy.o analyser.o tab_symb.o error.o
./parser <Examples/decl_if
