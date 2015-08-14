#include"fisher_yates.c"
#include"split_vec.c"
#include<pthread.h>
#include<stdio.h>

extern unsigned long cutoff;

struct array {
    unsigned int *start;
    unsigned int *end;
};

void *shuffle_thread(void *arg) {
    struct array *a = (struct array *) arg;
    unsigned int *start = a->start;
    unsigned int *end = a->end;

    if(end - start <= cutoff) {
        fisher_yates(start, end - start);
    } else {
        unsigned int *mid = split(start, end);
        a->end = mid;
        struct array a2 = {mid, end};
        pthread_t thread;
        pthread_create(&thread, NULL, shuffle_thread, (void *) &a2);
        shuffle_thread((void *) a);
        pthread_join(thread, NULL);
    }

    return NULL;
}

void shuffle(unsigned int *t, unsigned int n) {
    struct array a = {t, t + n};
    shuffle_thread((void *) &a);
}
