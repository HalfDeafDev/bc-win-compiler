@echo Compiling Full Program...
@echo Compiling Globals...
gcc -c globals.c globals.h TOKENS.h
gcc -c lexer.c lexer.h -lglobals
gcc -c main.c -llexer -lglobals
gcc -o runme main.o lexer.o globals.o
del *.o
del *.gch
