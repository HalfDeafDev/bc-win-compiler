#include "globals.h"
#include "parser.h"

static void program(void);
static void block(void);
static void statement(void);
static void condition(void);
static void expression(void);
static void term(void);
static void factor(void);

static void next(void) {
#if DEBUG
    printf("On line: %d\n", line);
#endif
    type = lex();
    ++raw;
}

static void expect(int match){
    if (match != type)
        error("syntax error\nToken: %s\nExpected Char: %c\nExpected Int: %d\nReceived Char: %c\nReceived Int: %d\n",
                token, match, match, type, type);

#if DEBUG
    printf("Expected Char: %c\nReceived Char: %c\n",
            match, match);
#endif
    next();
}

static void expectOr(int count, ...){
    va_list args;
    va_start(args, count);

    for (int i = 0; i < count; ++i) {
        if (type == va_arg(args, int)){
                next();
                goto exit;
        }
    }

    error("syntax error");
exit:
}

void parse(void) {
    program();
}

void program(void) {
    next();
    block();
    expect(TOK_DOT);

    if (type != 0)
        error("extra tokens at end of file");
}

void block(void) {
   if (type == TOK_CONST) {
        expect(TOK_CONST);
        expect(TOK_IDENT);
        expect(TOK_EQUAL);
        expectOr(2, TOK_NUMBER, TOK_STRING);
        
        while (type == TOK_COMMA) {
            expect(TOK_COMMA);
            expect(TOK_IDENT);
            expect(TOK_EQUAL); 
            expectOr(2, TOK_NUMBER, TOK_STRING); 
        }

        expect(TOK_SEMICOLON);
   }
   
   if (type == TOK_VAR) {
        expect(TOK_VAR);
        expect(TOK_IDENT);
        
        while (type == TOK_COMMA) {
            expect(TOK_COMMA);
            expect(TOK_IDENT);
        }

        expect(TOK_SEMICOLON);
   }

   while (type == TOK_PROCEDURE) {
        expect(TOK_PROCEDURE);
        expect(TOK_IDENT);
        expect(TOK_SEMICOLON);
        block();
        expect(TOK_SEMICOLON);
   }

   statement();
}

static void statement(void){
    switch (type) {
    case TOK_IDENT:
        expect(TOK_IDENT);
        expect(TOK_ASSIGN);
        expression();
        break;
    case TOK_CALL:
        expect(TOK_CALL);
        expect(TOK_IDENT);
        break;
    case TOK_BEGIN:
        expect(TOK_BEGIN);
        statement();
        while (type == TOK_SEMICOLON) {
            expect(TOK_SEMICOLON);
            statement();
        }
        expect(TOK_END);
        break;
    case TOK_IF:
        expect(TOK_IF);
        condition();
        expect(TOK_THEN);
        statement();
        break;
    case TOK_WHILE:
        expect(TOK_WHILE);
        condition();
        expect(TOK_DO);
        statement();
        break;
    }
}

static void condition(void) {
    if (type == TOK_ODD) {
        expect(TOK_ODD);
        expression();
    } else {
        expression();
        switch (type) {
        case TOK_EQUAL:
        case TOK_HASH:
        case TOK_GREATERTHAN:
            next();
            break;
        case TOK_LESSTHAN:
            next();
            if (type == TOK_GREATERTHAN)
                next();
            break;
        }
        expression();
    }
}

static void expression(void) {
    if (type == TOK_PLUS || type == TOK_MINUS) {
        expectOr(2, TOK_PLUS, TOK_MINUS);
    }
    term();
    while (type == TOK_PLUS || type == TOK_MINUS) {
        expectOr(2, TOK_PLUS, TOK_MINUS);
        term();
    }
}

static void term(void) {
    factor();
    while (type == TOK_MULTIPLY || type == TOK_DIVIDE) {
        expectOr(2, TOK_MULTIPLY, TOK_DIVIDE);
        factor();
    }
}

static void factor(void) {
    switch (type) {
    case TOK_IDENT:
        expect(TOK_IDENT);
        break;
    case TOK_NUMBER:
        expect(TOK_NUMBER);
        break;
    case TOK_LPAREN:
        expect(TOK_LPAREN);
        expression();
        expect(TOK_RPAREN);
        break;
    }
}
