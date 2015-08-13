#include"fisher_yates.c"
#include"split_vec.c"

extern unsigned long cutoff;

void shuffle(unsigned int *t, unsigned int n) {
    if(n <= cutoff) {
        fisher_yates(t, n);
    } else {
        unsigned int *start = t;
        unsigned int *end = start + n;
        unsigned int *mid = split(start, end);
        shuffle(start, mid - start);
        shuffle(mid, end - mid);
    }
}
