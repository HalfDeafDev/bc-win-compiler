#include "globals.h"

char *raw;
int type;
size_t line = 1;
char *token;

void error(const char *fmt, ...) {
    va_list ap;
    
    (void) fprintf(stderr, "error: %lu: ", line);

    va_start(ap, fmt);
    (void) vfprintf(stderr, fmt, ap);
    va_end(ap);

    (void) fputc('\n', stderr);

    exit(1);
}

void readin(char *file) {
    int fd;
    struct _stat st;

    /*\
     *  Finds the last occurence of character in a null-terminated byte string.
     *  Searching for `\0` will looking for the terminating null character
     */
    if (strrchr(file, '.') == NULL)
        error("file must end in '.pl0'");

    /*\
     * The sign of the result is the sign of the difference
     *
     * Negative = lhs appears before rhs lexicographically
     * Zero = Equal
     * Postive = lhs appears after rhs in lexicographical order
     *
     * Double negation turns any 1's or -1's (not equals) into 0's then 1's
     */

    if (!!strcmp(strrchr(file, '.'), ".pl0"))
        error("file must end in '.pl0'");

    _set_fmode(_O_BINARY);
    _sopen_s(&fd, file, _O_RDONLY, _SH_DENYNO, _S_IREAD);

    if (fd == 1)
        error("couldn't open %s", file);

    /*\
     * Not sure if this is going to work, searching only brings up unix man pages
     *
     * From StackOverflow, this should return 0 if successful and fills the provided buffer (&st)
     * with the file size in the form of bytes.
     */
    if (_fstat(fd, &st) != 0)
        error("couldn't get file size");

    if ((raw = malloc(st.st_size + 1)) == NULL)
        error("malloc failed");

    int file_size = _read(fd, raw, st.st_size);
    if (file_size != st.st_size)
        error("couldn't read %s, %d != %d", file, file_size, st.st_size);

    raw[st.st_size] = '\0';

    (void) _close(fd);
}
