#include"fisher_yates.c"
#include"merge_vec.c"

void merge(unsigned int *t, unsigned int m, unsigned int n);
extern unsigned long cutoff;

void shuffle(unsigned int *t, unsigned int n) {
    unsigned int c = 0;
    while((n >> c) > cutoff) c ++;
    unsigned int q = 1 << c;
    unsigned long nn = n;

    #pragma omp parallel for
    for(unsigned int i = 0; i < q; i ++) {
        unsigned long j = nn * i >> c;
        unsigned long k = nn * (i+1) >> c;
        fisher_yates(t + j, k - j);
    }

    for(unsigned int p = 1; p < q; p += p) {
        #pragma omp parallel for
        for(unsigned int i = 0; i < q; i += 2*p) {
            unsigned long j = nn * i >> c;
            unsigned long k = nn * (i + p) >> c;
            unsigned long l = nn * (i + 2*p) >> c;
            merge(t + j, k - j, l - j);
        }
    }
}
