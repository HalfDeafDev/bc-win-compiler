#include <sys\stat.h>
#include <sys\types.h>
#include <share.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "TOKENS.h"

#define DEBUG 0

extern char *raw;
extern size_t line;
   
extern int type;
extern char* token;

void error(const char *, ...);
void readin(char *);
