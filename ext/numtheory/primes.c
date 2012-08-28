#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "primes.h"

unsigned long *numtheory_is_prime; // n = 2k+1 -> k-th bit
unsigned long *numtheory_primes; 

unsigned long
init_sieve(unsigned long max_n)
{
    size_t is_p_sz = max_n / (2 * ULONG_SZ) + 1;
    numtheory_is_prime = malloc(is_p_sz * ULONG_SZ);
    memset(numtheory_is_prime, 0xFFFFFFFF, is_p_sz * ULONG_SZ);
    unset_bit(numtheory_is_prime, 1);
    numtheory_primes = calloc((long)((double)max_n/(log((double)max_n)-4.0)+1), ULONG_SZ);

    unsigned long j;
    unsigned long p = 0;
    numtheory_primes[p++] = 2;
    numtheory_primes[p++] = 3;

    unsigned LONG_LONG i1, i2, s;
    unsigned long max_n_sqrt = (unsigned long)(sqrt((double)max_n));

    for (i1 = 5, i2 = 7; ; i1 += 6, i2 += 6)
    {
        if (i1 > max_n_sqrt) break;
        if (is_set(numtheory_is_prime, i1))
        {
            s = i1 << 1;
            for (j = i1 * i1; j <= max_n; j += s)
                unset_bit(numtheory_is_prime, j);
            numtheory_primes[p++] = i1;
        }
        if (i2 > max_n_sqrt) break;
        if (is_set(numtheory_is_prime, i2))
        {
            s = i2 << 1;
            for (j = i2 * i2; j <= max_n; j += s)
                unset_bit(numtheory_is_prime, j);
            numtheory_primes[p++] = i2;
        }
    }
    for ( ; ; i1 += 6, i2 += 6)
    {
        if (i1 > max_n) break;
        if (is_set(numtheory_is_prime, i1))
            numtheory_primes[p++] = i1;
        if (i2 > max_n) break;
        if (is_set(numtheory_is_prime, i2))
            numtheory_primes[p++] = i2;
    }
    numtheory_primes = realloc(numtheory_primes, p * ULONG_SZ);
    return p;
}
