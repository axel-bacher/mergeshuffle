#include<stdlib.h>
#include<stdio.h>

extern unsigned long cutoff;

void shuffle(unsigned int *t, unsigned int n);

int main(int argc, char **argv) {
    unsigned int n = atoi(argv[1]);

    unsigned int *t = malloc(n*sizeof(int));
    for(unsigned int i = 0; i < n; i ++)
         t[i] = i;

    shuffle(t, n);

    for(unsigned int i = 0; i < n; i ++) printf("%u\n", t[i]);

    return 0;
}
