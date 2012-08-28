#ifndef NUMTHEORY_H
#define NUMTHEORY_H 

#include <ruby.h>
#include <math.h>

#ifndef HAVE_RUBY_VERSION_H
#include <version.h>
#else
#include <ruby/version.h>
#endif

#if RUBY_VERSION_MINOR == 8
#include <intern.h>
#include <defines.h>
#endif

#ifdef DEBUG
#include <stdio.h>
inline static char* TO_CSTRING(VALUE x) {
    VALUE v = rb_String(x);
    return StringValueCStr(v);
}
#endif

ID id_rand, id_mod, id_divmod, id_div,
   id_gcd, id_lcm, id_pow, id_mul;

extern const VALUE zero,one,two,three,four,five,six,seven,eight;

VALUE numtheory_powermod(int argc, VALUE *argv);
VALUE numtheory_miller_rabin_pseudoprime_p(VALUE n);
VALUE numtheory_nextprime(VALUE n);
VALUE numtheory_precompute_primes_upto(VALUE obj, VALUE n);
VALUE numtheory_prime(VALUE obj, VALUE i);
VALUE numtheory_primepi(VALUE obj, VALUE n);
VALUE numtheory_fibonacci(int argc, VALUE *argv);
VALUE numtheory_sigma(int argc, VALUE *argv);
VALUE numtheory_eulerphi(VALUE obj, VALUE n);
VALUE numtheory_moebius(VALUE obj, VALUE n);
VALUE numtheory_prime_division(VALUE n);
VALUE numtheory_product(VALUE obj, VALUE arr);
VALUE numtheory_primes_upto(VALUE min, VALUE max);
VALUE numtheory_factorial_primeswing(VALUE n);
VALUE numtheory_extended_gcd(VALUE x, VALUE y);
VALUE numtheory_multiplicative_order(VALUE a, VALUE m);
VALUE numtheory_pythagorean_triples(VALUE obj, VALUE max_l);
VALUE numtheory_popcount(VALUE n);
VALUE numtheory_bitlength(VALUE n);
VALUE numtheory_sqrtmod(VALUE a, VALUE p);
VALUE numtheory_isqrt(VALUE n);
VALUE numtheory_perfect_square_p(VALUE n);
VALUE numtheory_bpsw_pseudoprime_p(VALUE n);
VALUE numtheory_int_powermod(VALUE b, VALUE p, VALUE m);

VALUE numtheory_prime_p(VALUE n);
VALUE numtheory_modular_inverse(VALUE x, VALUE y);
VALUE numtheory_jacobi(VALUE a, VALUE n);

#define BITSPERDIG  (SIZEOF_BDIGITS * CHAR_BIT)
#define BIGRAD  ((BDIGIT_DBL)1 << BITSPERDIG)
#define BIGLO(x) ((BDIGIT)((x) & (BIGRAD - 1)))

inline static VALUE INT2BIG(long x) {
    return rb_int2big(x);
}

inline static VALUE INTVALUE2BIG(VALUE x) {
#ifdef DEBUG
    fprintf(stderr, "[intvalue2big] initial value = %s\n", TO_CSTRING(x));
    fprintf(stderr, "[intvalue2big] after conversion to long: %ld\n", FIX2LONG(x));
    fprintf(stderr, "[intvalue2big] after conversion to BIGNUM: %s\n", TO_CSTRING(rb_int2big(FIX2LONG(x))));
#endif
    return rb_int2big(FIX2LONG(x));
}

inline static VALUE TO_BIGNUM(VALUE x) {
    return FIXNUM_P(x) ? INTVALUE2BIG(x) : x;
}

#if RUBY_VERSION_MINOR == 8
static ID id_cmp, id_equal;

inline static VALUE rb_big_cmp(VALUE x, VALUE y) {
    return rb_funcall(x, id_cmp, 1, y);
}

inline static int rb_bigzero_p(VALUE x) {
    return rb_funcall(x, id_equal, 1, zero) == Qtrue;
}

inline static VALUE rb_big_eq(VALUE x, VALUE y) {
    return rb_funcall(x, id_equal, 1, y);
}

inline static VALUE rb_big_div(VALUE x, VALUE y) {
    return rb_funcall(x, id_div, 1, y);
}
#undef RBIGNUM_DIGITS
#define RBIGNUM_DIGITS(obj) ((BDIGIT*)(RBIGNUM(obj)->digits))

#endif

inline static unsigned long TO_ULONG(VALUE x) {
    if (!FIXNUM_P(x))
    { 
      if (rb_big_cmp(x, ULONG2NUM(ULONG_MAX)) != one) 
        return rb_big2ulong(x);
      else 
        rb_raise(rb_eNotImpError, "Not implemented for numbers >= 2**32");
    } 
    return FIX2ULONG(x);
}

inline static int EVEN_P(VALUE x) {
    return FIXNUM_P(x) ? !(FIX2ULONG(x)&1) : !(RBIGNUM_DIGITS(x)[0]&1);
}

inline static int ZERO_P(VALUE x) {
    return FIXNUM_P(x) ? FIX2LONG(x) == 0 : rb_bigzero_p(x); 
}

inline static VALUE ADD(VALUE x, VALUE y) {
    return FIXNUM_P(x) ? FIXNUM_P(y) ? 
                   LONG2NUM(FIX2LONG(x) + FIX2LONG(y)) : 
                   rb_big_plus(y, x) : 
           rb_big_plus(x, y);
}

inline static VALUE SUB(VALUE x, VALUE y) {
    return (FIXNUM_P(x) && FIXNUM_P(y)) ? 
                   LONG2NUM(FIX2LONG(x) - FIX2LONG(y)) : 
           rb_big_minus(TO_BIGNUM(x), y);
}

#define NEGATE(x) do { if (FIXNUM_P(x)) x = INT2FIX(-FIX2LONG(x)); \
                       else RBIGNUM_SET_SIGN(x, !RBIGNUM_SIGN(x)); } \
while(0);

inline static VALUE MUL(VALUE x, VALUE y) { 
    return rb_funcall(x, id_mul, 1, y);
}

inline static VALUE POW(VALUE b, VALUE p) {
    return rb_funcall(b, id_pow, 1, p);
}

inline static VALUE DIV(VALUE x, VALUE y) {
    return rb_funcall(x, id_div, 1, y);
}

inline static VALUE DIVMOD(VALUE x, VALUE y) {
    return rb_funcall(x, id_divmod, 1, y);
}

inline static VALUE MOD(VALUE x, VALUE y) {
    return rb_funcall(x, id_mod, 1, y);
}

inline static int MOD_4(VALUE x) {
    return FIXNUM_P(x) ? FIX2LONG(x) & 3 :
        RBIGNUM_SIGN(x) ? RBIGNUM_DIGITS(x)[0] & 3 :
                      4 - (RBIGNUM_DIGITS(x)[0] & 3);
}

inline static int MOD_8(VALUE x) {
    return FIXNUM_P(x) ? FIX2LONG(x) & 7 :
        RBIGNUM_SIGN(x) ? RBIGNUM_DIGITS(x)[0] & 7 :
                      8 - (RBIGNUM_DIGITS(x)[0] & 7);
}

inline static int EQL(VALUE x, VALUE y) {
    return (FIXNUM_P(x) && FIXNUM_P(y)) ? 
                   x == y : 
           rb_big_eq(TO_BIGNUM(x), y) == Qtrue;
}

inline static int MORE(VALUE x, VALUE y) {
    return (FIXNUM_P(x) && FIXNUM_P(y)) ?
                   FIX2LONG(x) > FIX2LONG(y) :
           rb_big_cmp(TO_BIGNUM(x), y) == one;
}

inline static int LESS(VALUE x, VALUE y) {
    return (FIXNUM_P(x) && FIXNUM_P(y)) ?
                   FIX2LONG(x) < FIX2LONG(y) :
           rb_big_cmp(TO_BIGNUM(x), y) == INT2FIX(-1);
}

inline static int MORE_EQL(VALUE x, VALUE y) {
    return (FIXNUM_P(x) && FIXNUM_P(y)) ?
                   FIX2LONG(x) >= FIX2LONG(y) :
           rb_big_cmp(TO_BIGNUM(x), y) != INT2FIX(-1);
}

inline static VALUE ABS(VALUE x) {
    if (FIXNUM_P(x)) {
#ifdef DEBUG
        fprintf(stderr, "[abs] x = %s\n", TO_CSTRING(x));
        fprintf(stderr, "[abs] FIX2LONG(x) = %ld\n", FIX2LONG(x));
        fprintf(stderr, "[abs] result = %s\n", TO_CSTRING(LONG2FIX(labs(FIX2LONG(x)))));
#endif
        return LONG2FIX(labs(FIX2LONG(x)));
    }
    VALUE v = rb_big_clone(x);
    RBIGNUM_SET_SIGN(v, 1);
    return v;
}

inline static int NEGATIVE_P(VALUE x) {
    return FIXNUM_P(x) ? FIX2LONG(x) < 0 : RBIGNUM_NEGATIVE_P(x);
}

inline static VALUE CLONE(VALUE x) {
    return FIXNUM_P(x) ? x : rb_big_clone(x);
}

inline static int INTEGER_P(VALUE x) {
    return (TYPE(x) == T_FIXNUM) || (TYPE(x) == T_BIGNUM);
}

inline static VALUE DOUBLED(VALUE x) {
    return FIXNUM_P(x) ? INT2FIX(FIX2LONG(x) << 1) :
                         rb_big_lshift(x, one);
}

inline static VALUE HALF(VALUE x) {
    return FIXNUM_P(x) ? INT2FIX(FIX2LONG(x) >> 1) :
            rb_big_rshift(x, one);
}

inline static VALUE RIGHT_SHIFT(VALUE x, int s) {
    return FIXNUM_P(x) ? INT2FIX(FIX2LONG(x) >> s) :
            rb_big_rshift(x, INT2FIX(s));
}

#ifdef min
#undef min
#endif
inline static int min(int a, int b) { return a < b ? a : b; }


#endif
