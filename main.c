#include "globals.h"
#include "lexer.h"
#include "parser.h"

/*\
 * pl0c -- PL/0 compiler.
 *
 * program	    =   block "." .
 * block	    =   [ "const" ident "=" (number | string) { "," ident "=" (number | string) } ";" ]
 *		            [ "var" ident { "," ident } ";" ]
 *		            { "procedure" ident ";" block ";" } statement ;
 * statement    =   [ ident ":=" expression
 *		            | "call" ident
 *		            | "begin" statement { ";" statement } "end"
 *		            | "if" condition "then" statement
 *		            | "while" condition "do" statement ] ;
 * condition	=   "odd" expression
 *		            | expression ( "=" | "#" | "<" | ">" ) expression ;
 * expression	=   [ "+" | "-" ] term { ( "+" | "-" ) term } ;
 * term		    =   factor { ( "*" | "/" ) factor } ;
 * factor	    =   ident
 *		            | number
 *		            | "(" expression ")" ;
 */

/*\
 * Main
 */

int main(int argc, char *argv[]) {
    char *startp;

    if (argc != 2) {
        (void) fputs("usage: pl0c file.pl0\n", stderr);
        exit(1);
    }

    readin(argv[1]);
    startp = raw;

    parse();

    printf("Parsing complete, %s is valid.\n", argv[1]);

    free(startp);

    return 0;
}
