#include "util.h"

void *mmalloc(size_t size) {
    void *p = malloc(size);
    if (!p) {
        fprintf(stderr,"\nRan out of memory!\n");
        exit(1);
    }
    return p;
}

str String(char *s) {
    str p = mmalloc(strlen(s)+1);
    strcpy(p,s);
    return p;
}
