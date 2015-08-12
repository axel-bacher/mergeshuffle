#include"random.c"

unsigned int *split(unsigned int *start, unsigned int *end) {
    unsigned int *mid = start;
    struct random r = {0, 0};
    while(start < end) {
        if(flip(&r)) {
            swap(start, mid ++);
        }
        start ++;
    }
    return mid;
}
