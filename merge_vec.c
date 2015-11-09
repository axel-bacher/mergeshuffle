#include"random.c"
#include<immintrin.h>

#define load(x) _mm_loadu_ps((float *) (x))
#define perm(x, y) _mm_permutevar_ps((__m128) (x), (__m128i) (y))
#define maskstore(x, y, z) \
    _mm_maskstore_ps((float *) (x), (__m128i) (y), (__m128) (z))
#define sll(x, y) _mm_sll_epi32((__m128i) (x), (__m128i) (y))
#define srl(x, y) _mm_srl_epi32((__m128i) (x), (__m128i) (y))
#define popcnt _mm_popcnt_u64

void merge(unsigned int *t, unsigned int m, unsigned int n) {
    __v4si perm1 = {303239696, 1803315264, 3166732288, 3221225472};
    __v4si perm2 = {0, 1077952576, 2483065856, 3918790656};
    __v4si mask1 = {1073741823, 54476799, 197439, 3};
    __v4si mask2 = {858993459, 252645135, 16711935, 65535};
    unsigned int *u = t;
    unsigned int *v = t + m;
    unsigned int *w = t + n;

    struct random r = {0, 0};

    // treat elements four at a time

    while(1) {
        // draw four random bits
        consume_bits(&r, 4);
        int p = r.x & 15;

        // 4 elements are drawn, popcnt(p) of which come from the second half
        unsigned int *uu = u + 4;
        unsigned int *vv = v + popcnt(p);

        // if proceeding would bring us too far, undo and stop
        if(uu > v || vv > w) {
            r.c += 4;
            break;
        }

        // permute the elements using the selected control words
        __v4si pp = {2*p, 2*p, 2*p, 2*p};
        __m128 a = perm(load(u), srl(perm1, pp));
        __m128 b = perm(load(v), srl(perm2, pp));
        maskstore(u, sll(mask2, pp), b);
        maskstore(v, sll(mask1, pp), a);
        u = uu; v = vv;
        r.x >>= 4;
    }

    // manage elements one at a time

    while(1) {
        if(flip(&r)) {
            if(v == w) break;
            swap(u ++, v ++);
        } else {
            if(u == v) break;
            u ++;
        }
    }

    // use Fisher-Yates to finish

    while(u < w) {
        unsigned int i = random_int(&r, u - t + 1);
        swap(t + i, u ++);
    }
}
