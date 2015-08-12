#include<stdlib.h>
#include<stdio.h>

unsigned long count = 0;
extern unsigned long cutoff;

void shuffle(unsigned int *t, unsigned int n);

int main(int argc, char **argv) {
    unsigned int n = atoi(argv[1]);
    unsigned int p = 1;
    if(argc > 2) cutoff = atoi(argv[2]);
    if(argc > 3) p = atoi(argv[3]);
    unsigned int *t = malloc(n*sizeof(int));
    for(unsigned int i = 0; i < n; i ++)
         t[i] = i;

    for(unsigned int i = 0; i < p; i ++) shuffle(t, n);
    printf("%f\n", ((double) count) / p);

    return 0;
}
