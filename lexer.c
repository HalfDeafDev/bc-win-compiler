#include "globals.h"

#include <ctype.h>
#include <errno.h> // Added for strtonum replacement (strtol)


/*
 * Lexer
 */

static void comment(void) {
    int ch;

    while ((ch = *raw++) != '}') {
        if (ch == '\0')
            error("unterminated comment");

        if (ch == '\n')
            ++line;
    }
}

static int ident(void) {
    char *p;
    size_t i, len;

    p = raw;
    while (isalnum(*raw) || *raw == '_')
        ++raw;
    len = raw - p;

    --raw;
    free(token);

    if ((token = malloc(len + 1)) == NULL)
        error("malloc failed");
    for (i = 0; i < len; i++)
        token[i] = *p++;
    token[i] = '\0';

    if (!strcmp(token, "const"))
        return TOK_CONST;
    else if (!strcmp(token, "var"))
        return TOK_VAR;
    else if (!strcmp(token, "procedure"))
        return TOK_PROCEDURE;
    else if (!strcmp(token, "call"))
        return TOK_CALL;
    else if (!strcmp(token, "begin"))
        return TOK_BEGIN;
    else if (!strcmp(token, "end"))
        return TOK_END;
    else if (!strcmp(token, "if"))
        return TOK_IF;
    else if (!strcmp(token, "then"))
        return TOK_THEN;
    else if (!strcmp(token, "while"))
        return TOK_WHILE;
    else if (!strcmp(token, "do"))
        return TOK_DO;
    else if (!strcmp(token, "odd"))
        return TOK_ODD;

    return TOK_IDENT;
}

static int number(void) {
    const char *errstr;
    char *p;
    size_t i, j = 0, len;

    p = raw;
    while (isdigit(*raw) || *raw == '_')
        ++raw;
    len = raw - p;

    --raw;
    free(token);

    if ((token = malloc(len + 1)) == NULL)
        error("malloc failed");

    for (i = 0; i < len; i++) {
        if (isdigit(*p))
            token[j++] = *p;
        ++p;
    }
    token[j] = '\0';

    /*
     * strtonum(char*, minval, maxval, **errstr);
     * This is for Unix, it's different for Windows
     *
    (void) strtonum(token, 0, LONG_MAX, &errstr);
    if (errstr != NULL)
        error("invalid number: %s", token);

    */

    char** end = &token+j;
    errno = 0;

    strtoll(token, end, 10);

    if (errno == ERANGE) {
        error("Could not convert '%s' to a number.");
    }

    return TOK_NUMBER;
}

static int sqString() {
    char* head = raw;

    ++raw;
    while(*raw != '\'') {
        ++raw;
    }

    size_t str_len = raw - head;

    if((token = malloc(str_len + 2)) == NULL)
        error("malloc failed.");
    
    size_t i = 0, j = 0;

    for (; i < str_len + 1; i++) {
        token[j++] = *head;
        ++head;
    }

    token[j] = '\0';

    return TOK_STRING;
}

static int dqString() {

}

static int astring() {
    free(token);

    if (*raw == '\'') {
        return sqString();
    } else if (*raw == '\"') {
        return dqString();
    }
}

int lex(void) {
again:
    /* Skip Whitespace. */
    while (*raw == ' ' || *raw == '\t' || *raw == '\n' || *raw == '\r') {
        if (*raw++ == '\n' || *raw == '\r')
            ++line;
    }

    if (isalpha(*raw) || *raw == '_')
        return ident();

    if (isdigit(*raw))
        return number();

    if (*raw == '\'' || *raw == '\"')
        return astring();

    switch (*raw) {
        case '{':
            comment();
            goto again;
        case '.':
        case '=':
        case ',':
        case ';':
        case '#':
        case '<':
        case '>':
        case '+':
        case '-':
        case '*':
        case '/':
        case '(':
        case ')':
            return (*raw);
        case ':':
            if (*++raw != '=')
                error("unknown token??: %c", *raw);

            return TOK_ASSIGN;
        case '\0':
            return 0;
        default:
            error("unknown token: (%c)[%d]",(char) *raw,(int) *raw);
    }

    return 0;
}

/*
 * Parser
 */


void tokenize(void) {
    while ((type = lex()) != 0) {
        ++raw;
        (void) fprintf(stdout, "%lu|%d\t", line, type);
        switch (type) {
            case TOK_STRING:
                (void) fprintf(stdout, "%s", token);
                break;
            case TOK_IDENT:
            case TOK_NUMBER:
            case TOK_CONST:
            case TOK_VAR:
            case TOK_PROCEDURE:
            case TOK_CALL:
            case TOK_BEGIN:
            case TOK_END:
            case TOK_IF:
            case TOK_THEN:
            case TOK_WHILE:
            case TOK_DO:
            case TOK_ODD:
                (void) fprintf(stdout, "%s", token);
                break;
            case TOK_DOT:
            case TOK_EQUAL:
            case TOK_COMMA:
            case TOK_SEMICOLON:
            case TOK_HASH:
            case TOK_LESSTHAN:
            case TOK_GREATERTHAN:
            case TOK_PLUS:
            case TOK_MINUS:
            case TOK_MULTIPLY:
            case TOK_DIVIDE:
            case TOK_LPAREN:
            case TOK_RPAREN:
                (void) fputc(type, stdout);
                break;
            case TOK_ASSIGN:
                (void) fputs(":=", stdout);
        }
        (void) fputc('\n', stdout);
    }
}
