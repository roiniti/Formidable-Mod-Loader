#ifndef SYMBOL_FINDER
#define SYMBOL_FINDER

#define MAX_NAME_LEN 256
#include <unistd.h>
struct mm {
    char name[MAX_NAME_LEN];
    unsigned long start, end;
};

extern int find_name(pid_t pid, const char *name,const  char *libn, unsigned long *addr);
extern int find_libbase(pid_t pid, const char *libn, unsigned long *addr);
 int load_memmap(pid_t pid, struct mm* mm, int* nmmp);
#endif