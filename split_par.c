#include"fisher_yates.c"

#ifdef __AVX__
#include"split_vec.c"
#else
#include"split.c"
#endif

#include<pthread.h>
#include<stdio.h>

extern unsigned long cutoff;
unsigned long cutoff2 = 0x100000;

struct array {
    unsigned int *start;
    unsigned int *end;
};

void *shuffle_thread(void *arg) {
    struct array *a = (struct array *) arg;
    unsigned int *start = a->start;
    unsigned int *end = a->end;

    if(end - start <= cutoff) {
        fisher_yates(start, end - start); // Small input, use Fisher-Yates
    } else {
        unsigned int *mid = split(start, end);
        a->end = mid;
        struct array a2 = {mid, end};
        if(end - start <= cutoff2) {
            shuffle_thread((void *) a);   // Intermediate input,
            shuffle_thread((void *) &a2); // use sequential Rao-Sandelius
        } else {
            pthread_t thread; // Large input, use parallel R-S
            pthread_create(&thread, NULL, shuffle_thread, (void *) &a2);
            shuffle_thread((void *) a);
            pthread_join(thread, NULL);
        }
    }

    return NULL;
}

void shuffle(unsigned int *t, unsigned int n) {
    struct array a = {t, t + n};
    shuffle_thread((void *) &a);
}
