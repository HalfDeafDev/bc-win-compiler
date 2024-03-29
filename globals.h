#include <sys\stat.h>
#include <sys\types.h>
#include <share.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "TOKENS.h"

extern char *raw;
extern size_t line;
   
extern int type;

void error(const char *, ...);
void readin(char *);
