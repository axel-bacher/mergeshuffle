// Routines to get random data (bits and integers).
// this file should be directly included; we don't do separate compilation to
// enable inlining of functions.

#ifndef RANDOM_C
#define RANDOM_C

#ifdef COUNT
extern unsigned long count;
#endif

// get a random 64-bit register
// Uses the "rdrand" instruction giving hardware randomness
// documentation: https://software.intel.com/en-us/articles/intel-digital-random-number-generator-drng-software-implementation-guide
static inline unsigned long rand64() {
    unsigned long r;
    __asm__ __volatile__("0:\n\t" "rdrand %0\n\t" "jnc 0b": "=r" (r) :: "cc");
    return r;
}

// structure containing random bits
// the c least significant bits of x should contain fresh random data

struct random {
    unsigned long x;
    int c;
};

// mark as used the first b bits of r->x
// they should be shifted out after use (r->x >>= b)

static inline void consume_bits(struct random *r, int b) {
    #ifdef COUNT
    count += b;
    #endif

    r->c -= b;
    if(r->c < 0) {
        r->x = rand64();
        r->c = 64 - b;
    }
}

static inline unsigned long random_bits(struct random *r, unsigned int i) {
    consume_bits(r, i);
    int y = r->x & ((1UL << i) - 1);
    r->x >>= i;
    return y;
}

static inline int flip(struct random *r) {
    return random_bits(r, 1);
}


// (From "Optimal Discrete Uniform Generation from Coin Flips,
// and Application" by J. Lumbroso - http://arxiv.org/abs/1304.1916 )
//
// get a random integer between 0 and n-1

static inline unsigned long random_int(struct random *r, unsigned long n) {
    unsigned long v = 1;
    unsigned long d = 0;
    while(1) {
        d += d + flip(r); 
        v += v;

        if(v >= n) {
            if(d < n) return d;
            v -= n;
            d -= n;
        }
    }
}

static inline void swap(unsigned int *a, unsigned int *b) {
    unsigned int x = *a;
    unsigned int y = *b;
    *a = y;
    *b = x;
}

#endif
