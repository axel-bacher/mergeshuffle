#include"random.c"

unsigned long cutoff = 0x10000;

void fisher_yates(unsigned int *t, unsigned int n) {
    struct random r = {0, 0};

    for(unsigned int i = 0; i < n; i ++) {
        unsigned int j = random_int(&r, i + 1);
        swap(t + i, t + j);
    }
}
