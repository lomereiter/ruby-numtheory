#ifndef NUMTHEORY_PRIMES_H
#define NUMTHEORY_PRIMES_H

#include <ruby.h>

unsigned long init_sieve(unsigned long max_n); // returns prime_pi(max_n)

#define ULONG_SZ sizeof(unsigned long)

extern unsigned long *numtheory_is_prime;
extern unsigned long *numtheory_primes;

static inline unsigned long
is_set(unsigned long* ary, unsigned long n)
{
    long k = n >> 1;
    return ary[k / ULONG_SZ] & (1 << (k & (ULONG_SZ - 1)));
}
   
static inline void 
unset_bit(unsigned long* ary, unsigned long n)
{
    long k = n >> 1;
    ary[k / ULONG_SZ] &= ~(1 << (k & (ULONG_SZ - 1)));
}

#endif
