#include"random.c"
#include<immintrin.h>
#include<string.h>

#define load(x) _mm_loadu_ps((float *) (x))
#define perm(x, y) _mm_permutevar_ps((__m128) (x), (__m128i) (y))
#define maskstore(x, y, z) \
    _mm_maskstore_ps((float *) (x), (__m128i) (y), (__m128) (z))
#define popcnt _mm_popcnt_u64

unsigned int *split(unsigned int *start, unsigned int *end) {
    __v4si perm1 = {303239696, 1803315264, 3166732288, 3221225472};
    __v4si perm2 = {0, 1077952576, 2483065856, 3918790656};
    __v4si mask1 = {1073741823, 54476799, 197439, 3};
    __v4si mask2 = {858993459, 252645135, 16711935, 65535};
    unsigned int *mid = start;

    struct random r = {0, 0};

    while(start <= end - 4) {
        int p = random_bits(&r, 4);

        __m128 a = perm(load(start), perm1 >> 2*p);
        __m128 b = perm(load(mid), perm2 >> 2*p);
        maskstore(start, mask2 << 2*p, b);
        maskstore(mid, mask1 << 2*p, a);
        start += 4;
        mid += popcnt(p);
    }

    while(start < end) {
        if(flip(&r)) swap(start, mid ++);
        start ++;
    }

    return mid;
}
