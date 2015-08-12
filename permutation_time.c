#include<stdlib.h>
#include<stdio.h>
#include<sys/time.h>

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

    struct timeval start, end;
    gettimeofday(&start, NULL);
    for(unsigned int i = 0; i < p; i ++) shuffle(t, n);
    gettimeofday(&end, NULL);
    printf("%f\n", (end.tv_sec - start.tv_sec + ((double) (end.tv_usec - start.tv_usec)) / 1000000) / p);

    return 0;
}
