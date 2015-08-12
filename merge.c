#include"random.c"

void merge(unsigned int *t, unsigned int m, unsigned int n) {
    unsigned int *u = t;
    unsigned int *v = t + m;
    unsigned int *w = t + n;

    struct random r = {0, 0};

    while(1) {
        if(flip(&r)) {
            if(v == w) break;
            swap(u, v ++);
        } else
            if(u == v) break;
        u ++;
    }

    while(u < w) {
        unsigned int i = random_int(&r, u - t + 1);
        swap(t + i, u ++);
    }
}
