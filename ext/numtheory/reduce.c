#include <string.h>
#include "numtheory.h"

#define MULADD(i, j) \
    __asm__ volatile(                   \
          "movl %6, %%eax \n\t"     \
          "mull %7        \n\t"     \
          "addl %%eax, %0 \n\t"     \
          "adcl %%edx, %1 \n\t"     \
          "adcl $0, %2    \n\t" :   \
          "=m"(c0), "=m"(c1), "=m"(c2) :    \
          "m"(c0), "m"(c1), "m"(c2), "m"(i), "m"(j) :     \
          "%eax","%edx", "%cc");

#define BITSPERDIG (SIZEOF_BDIGITS * CHAR_BIT)
#define BIGRAD ((BDIGIT_DBL)1 << BITSPERDIG)
#define BIGLO(x) ((BDIGIT)((x) & (BIGRAD - 1)))

#define RBIGNUM_SET_LEN(b,l) \
        ((RBASIC(b)->flags & RBIGNUM_EMBED_FLAG) ? \
              (void)(RBASIC(b)->flags = \
                        (RBASIC(b)->flags & ~RBIGNUM_EMBED_LEN_MASK) | \
                        ((l) << RBIGNUM_EMBED_LEN_SHIFT)) : \
              (void)(RBIGNUM(b)->as.heap.len = (l)))

inline static VALUE rshift_digits_inplace(VALUE n, int k)
{
    if (FIXNUM_P(n)) {
        return INT2FIX(0);
    }

    BDIGIT* ds = RBIGNUM_DIGITS(n);
    int len = RBIGNUM_LEN(n);
    int i;
    if (len > k)
    {
        memmove(ds, ds + k, SIZEOF_BDIGITS * (len - k));
        RBIGNUM_SET_LEN(n, len - k);
        return n;
    } 
    else
    {
        RBIGNUM_SET_LEN(n, 0);
        return n;
    }
}

inline static VALUE power_of_two(int p)
{
    return rb_big_lshift(rb_int2big(1), INT2FIX(p));
}

static VALUE 
bigmul_low_digits(VALUE x, VALUE y, int digits)
{
    int xn = RBIGNUM_LEN(x);
    if (FIXNUM_P(y)) y = rb_int2big(FIX2LONG(y));
    int yn = RBIGNUM_LEN(y);

    int all = 0;
    if (digits > xn + yn) 
    {
        digits = xn + yn;
        all = 1;
    }
    VALUE z = rb_big_new(digits, 1);

    BDIGIT* xds = RBIGNUM_DIGITS(x);
    BDIGIT* yds = RBIGNUM_DIGITS(y);
    BDIGIT* zds = RBIGNUM_DIGITS(z);

    BDIGIT c0,c1,c2; // carry
    int i, j, k, beg, end;
    for (k = c0 = c1 = c2 = 0; k < digits; k++)
    {
        beg = k >= yn - 1 ? k - yn + 1 : 0; 
        end = k >= xn     ? xn - 1     : k;
        for (i = beg, j = k - beg; i <= end; ++i, --j)
            MULADD(xds[i], yds[j]);
        zds[k] = c0;
        c0 = c1;
        c1 = c2;
        c2 = 0;
    }
    return rb_big_norm(z);
}
/*
VALUE bigmul_low_digits_old(VALUE x, VALUE y, int digits)
{
    int xn = RBIGNUM_LEN(x);
    if (FIXNUM_P(y)) y = rb_int2big(FIX2LONG(y));
    int yn = RBIGNUM_LEN(y);
    VALUE z = rb_big_new(digits, 1);
    BDIGIT* xds = RBIGNUM_DIGITS(x);
    BDIGIT* yds = RBIGNUM_DIGITS(y);
    BDIGIT* zds = RBIGNUM_DIGITS(z);

    int i, j, k;
    for (i = 0; i < digits; zds[i++] = 0);
    BDIGIT_DBL dd, carry;
    for (i = 0; i < xn; ++i)
    {
        dd = xds[i];
        carry = 0;
        k = digits - i;
        if (k > yn) k = yn;
        for (j = 0; j < k; ++j)
        {
            carry += dd * yds[j] + zds[i + j];
            zds[i + j] = BIGLO(carry);
            carry >>= BITSPERDIG;
        }
        if (carry && (i + j) < digits)
            zds[i + j] = carry;
    }
    return rb_big_norm(z);
} */

static VALUE 
bigmul_high_digits(VALUE x, VALUE y, int dontcare, int x_right_shift)
{
    int xn = RBIGNUM_LEN(x);
    if (FIXNUM_P(y)) y = rb_int2big(FIX2LONG(y));
    int yn = RBIGNUM_LEN(y);


    BDIGIT* xds = RBIGNUM_DIGITS(x);
    BDIGIT* yds = RBIGNUM_DIGITS(y);

    /* in order to avoid rb_big_clone call,
       let's virtually "shift" x instead of actual shifting */
    if (x_right_shift >= xn) { 
        return rb_int2big(0);
    } else {
        xds += x_right_shift;
        xn -= x_right_shift;
    }

    int digits = xn + yn;
    int i, j, k;
    VALUE z = rb_big_new(digits, 1);
    BDIGIT* zds = RBIGNUM_DIGITS(z);
    for (i = 0; i < digits; zds[i++] = 0);

    int beg, end;

    BDIGIT c0, c1, c2;
    for (k = dontcare, c0 = c1 = c2 = 0; k < digits; k++)
    {
        beg = k >= yn - 1 ? k - yn + 1 : 0; 
        end = k >= xn     ? xn - 1     : k;
        for (i = beg, j = k - beg; i <= end; ++i, --j)
            MULADD(xds[i], yds[j]);
        zds[k] = c0;
        c0 = c1;
        c1 = c2;
        c2 = 0;
    }
    return z;
}

/*
VALUE bigmul_high_digits_old(VALUE x, VALUE y, int dontcare)
{
    int xn = RBIGNUM_LEN(x);
    if (FIXNUM_P(y)) y = rb_int2big(FIX2LONG(y));
    int yn = RBIGNUM_LEN(y);
    VALUE z = rb_big_new(xn + yn, 1);
    BDIGIT* xds = RBIGNUM_DIGITS(x);
    BDIGIT* yds = RBIGNUM_DIGITS(y);
    BDIGIT* zds = RBIGNUM_DIGITS(z);

    int i, j, k;
    for (i = 0; i < xn + yn; zds[i++] = 0);
    BDIGIT_DBL dd, carry;
    for (i = 0; i < xn; ++i)
    {
        dd = xds[i];
        carry = 0;
        k = dontcare - i;
        if (k < 0) k = 0;
        for (j = k; j < yn; ++j)
        {
            carry += dd * yds[j] + zds[i + j];
            zds[i + j] = BIGLO(carry);
            carry >>= BITSPERDIG;
        }
        if (i+j >= dontcare)
            zds[i+j] = carry;
    }
    return z;
}*/


VALUE rb_big_barrett_mu(VALUE m)
{
    long len = RBIGNUM_LEN(m);
    return rb_big_div(power_of_two(2 * len * BITSPERDIG), m);
}

VALUE rb_big_barrett_reduce(VALUE x, VALUE m, VALUE mu, int inplace)
{
    if (!NEGATIVE_P(x) && MORE(m, x))
    {
        return x;
    }
    unsigned long len = RBIGNUM_LEN(m); /* >= 1 */
    if (FIXNUM_P(x))
    {
        return FIX2LONG(x) >= 0 ? x : rb_big_plus(m, x);
    }

    VALUE   q = bigmul_high_digits(x, mu, len - 1, len - 1);
    /* q is not normed */
    q = rshift_digits_inplace(q, len + 1);
    /* [x/m] - 3 <= q <= [x/m] */
    q = bigmul_low_digits(q, m, len + 1);
    /* q is normed */

    VALUE a = inplace ? x : rb_big_clone(x);
    if (RBIGNUM_LEN(a) > (long)len + 1)
    {
        RBIGNUM_SET_LEN(a, len + 1);
    }

    a = rb_big_minus(a, q);
    if (NEGATIVE_P(a))
        a = ADD(a, power_of_two((len + 1) * BITSPERDIG));
    /* a = (x - q * m) mod (base ** (len + 1)) */

    while (!MORE(m, a)) /* that is, m <= a */
        a = SUB(a, m);
    return rb_big_norm(a);
}
