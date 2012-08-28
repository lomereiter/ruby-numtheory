#include "numtheory.h"

#include "primes.h"
#include "queue.h"
#include "reduce.h"
#include "numtheory_macros.c"
#include "constants.h"

#ifdef DEBUG
#include <stdio.h>
#include <ruby/intern.h>
#endif

static VALUE mNumTheory;
static ID primality_tests_ID;

static unsigned long PRIMES_UPPER_LIMIT = 3141592;
static unsigned long NUM_OF_PRIMES;

const VALUE zero = INT2FIX(0);
const VALUE one = INT2FIX(1);
const VALUE two = INT2FIX(2);
const VALUE three = INT2FIX(3);
const VALUE four = INT2FIX(4);
const VALUE five = INT2FIX(5);
const VALUE six = INT2FIX(6);
const VALUE seven = INT2FIX(7);
const VALUE eight = INT2FIX(8);

/*
 *  The <code>NumTheory</code> module contains basic number-theoretical
 *  functions. 
 */

void 
Init_numtheory()
{
#if 0
    VALUE rb_cInteger = rb_define_class("Integer", rb_cObject);
    VALUE rb_cFixnum = rb_define_class("Fixnum", rb_cObject);
#endif
    VALUE rb_mNumTheory = rb_define_module("NumTheory");
    mNumTheory = rb_mNumTheory;

    id_rand = rb_intern("rand");
    id_mod = rb_intern("modulo");
    id_div = rb_intern("div");
    id_divmod = rb_intern("divmod");
    id_gcd = rb_intern("gcd");
    id_lcm = rb_intern("lcm");
    id_pow = rb_intern("**");
    id_mul = rb_intern("*");

    primality_tests_ID = rb_intern("PrimalityTests");
    /* PrimalityTests: the number of rounds in Miller-Rabin test for primality. */
    rb_define_const(rb_mNumTheory, "PrimalityTests", INT2FIX(20));

    rb_define_module_function(rb_mNumTheory, "powermod", 
            numtheory_powermod, -1);
    rb_define_module_function(rb_mNumTheory, "fibonacci",
            numtheory_fibonacci, -1);
    rb_define_module_function(rb_mNumTheory, "precompute_primes_upto",
            numtheory_precompute_primes_upto, 1);
    rb_define_module_function(rb_mNumTheory, "prime",
            numtheory_prime, 1);
    rb_define_module_function(rb_mNumTheory, "primepi",
            numtheory_primepi, 1);
    rb_define_module_function(rb_mNumTheory, "sigma",
            numtheory_sigma, -1);
    rb_define_module_function(rb_mNumTheory, "eulerphi",
            numtheory_eulerphi, 1);
    rb_define_module_function(rb_mNumTheory, "moebius",
            numtheory_moebius, 1);
    rb_define_module_function(rb_mNumTheory, "product",
            numtheory_product, 1);

    rb_define_module_function(rb_mNumTheory, "triples",
            numtheory_pythagorean_triples, 1);

    rb_define_method(rb_cInteger, "powermod",
            numtheory_int_powermod, 2);
    rb_define_method(rb_cInteger, "prime?",
            numtheory_prime_p, 0);
    rb_define_method(rb_cInteger, "MR_pseudoprime?",
            numtheory_miller_rabin_pseudoprime_p, 0);
    rb_define_method(rb_cInteger, "BPSW_pseudoprime?",
            numtheory_bpsw_pseudoprime_p, 0);
    rb_define_method(rb_cInteger, "nextprime",
            numtheory_nextprime, 0);
    rb_define_method(rb_cInteger, "prime_division",
            numtheory_prime_division, 0);
    rb_define_method(rb_cFixnum, "factorial",
            numtheory_factorial_primeswing, 0);
    rb_define_method(rb_cInteger, "primes_upto",
            numtheory_primes_upto, 1);
    rb_define_method(rb_cInteger, "extended_gcd",
            numtheory_extended_gcd, 1);
    rb_define_method(rb_cInteger, "inverse",
            numtheory_modular_inverse, 1);
    rb_define_method(rb_cInteger, "znorder",
            numtheory_multiplicative_order, 1);
    rb_define_method(rb_cInteger, "popcount",
            numtheory_popcount, 0);
    rb_define_method(rb_cInteger, "bitlength",
            numtheory_bitlength, 0);
    rb_define_method(rb_cInteger, "jacobi",
            numtheory_jacobi, 1);
    rb_define_method(rb_cInteger, "sqrt_mod",
            numtheory_sqrtmod, 1);
    rb_define_method(rb_cInteger, "isqrt",
            numtheory_isqrt, 0);
    rb_define_method(rb_cInteger, "square?",
            numtheory_perfect_square_p, 0);

    NUM_OF_PRIMES = init_sieve(PRIMES_UPPER_LIMIT);
    /* PRECOMPUTED_PRIMES: the number of precomputed primes. */
    rb_define_const(rb_mNumTheory, "PRECOMPUTED_PRIMES", 
            INT2NUM(NUM_OF_PRIMES));
    
}

/*
 *  call-seq:
 *    NumTheory.precompute_primes_upto(n)
 *
 *  Precomputes primes up to the <i>n</i> via Eratosthenes' sieve method.
 *
 *    NumTheory.precompute_primes_upto(20000000)
 */
VALUE 
numtheory_precompute_primes_upto(VALUE obj, VALUE n)
{
    if (FIXNUM_P(n))
    {
        unsigned long t = FIX2LONG(n);
        if (t < PRIMES_UPPER_LIMIT || FIX2LONG(n) < 0)
        {
            rb_warn("new size should be more than old one; prime table is unchanged");
            return Qnil;
        }
        PRIMES_UPPER_LIMIT = FIX2LONG(n);

        free(numtheory_is_prime);
        free(numtheory_primes);

        NUM_OF_PRIMES = init_sieve(PRIMES_UPPER_LIMIT);
        rb_const_set(mNumTheory, rb_intern("PRECOMPUTED_PRIMES"),
                INT2NUM(NUM_OF_PRIMES));
    }
    else
    {
        rb_raise(rb_eArgError, "The number is too big!");
    }
    return Qnil;
}

/*
 *  call-seq:
 *    NumTheory.powermod(base, power, [modulo, one]) -> obj
 *
 *  Returns (<code>base ** power) % modulo</code> if modulo is not nil.
 *  Otherwise returns <code>base ** power</code>. Parameter <i>one</i> 
 *  can be used to provide the identity of the multiplicative group containing 
 *  <i>base</i>. 
 * 
 *    NumTheory.powermod(2, 20)                             #=> 1048576
 *    NumTheory.powermod(2, 20, 1000)                       #=> 576
 *    
 *    require 'matrix'
 *    NumTheory.powermod(Matrix[[1,1],[1,0]], 20, nil, 
 *                       Matrix.identity(2))                
 *                              #=> Matrix[[10946, 6765], [6765, 4181]]
 */
VALUE
numtheory_powermod(int argc, VALUE *argv)
{
    VALUE base, pow, modulo, one;
    rb_scan_args(argc, argv, "22", &base, &pow, &modulo, &one);

    ID id_mul = rb_intern("*");
    ID id_mod = rb_intern("%");

    VALUE result = NIL_P(one) ? INT2BIG(1) : one;
    FOR_BITS(pow, result = rb_funcall(result, id_mul, 1, result),
                  {},
                  result = rb_funcall(result, id_mul, 1, base),
                  if (!NIL_P(modulo))
                  {
                      result = rb_funcall(result, id_mod, 1, modulo);
                  });
    return result;
}

static VALUE int_powermod_sliding_window(VALUE,VALUE,VALUE);
static VALUE int_powermod_sliding_window_br(VALUE,VALUE,VALUE);

/*
 *  call-seq:
 *    integer.powermod(power, modulo)         -> integer
 *
 *  Returns (+self+ ** <i>power</i>) % <i>modulo</i>.
 *  Both <i>power</i> and <i>modulo</i> must be integer.
 *
 *    172.powermod(238, 239) #=> 1
 */
VALUE
numtheory_int_powermod(VALUE b, VALUE p, VALUE m){
    if (!INTEGER_P(p))
    {
        rb_raise(rb_eArgError, "power must be integer");
    }
    if (!INTEGER_P(m))
    {
        rb_raise(rb_eTypeError, "modulo must be integer");
    }
    if (ZERO_P(p))
    {
        return one;
    }
    if (NEGATIVE_P(p))
    {
        b = numtheory_modular_inverse(b, m);
        p = ABS(p);
    } 
    else 
    {
        b = MOD(b, m);
    }
    const int SLIDING_WINDOW_THRESHOLD = 4;
    const int BARRETT_REDUCE_THRESHOLD = 4;

    int use_barrett = 0,
        use_sliding_window = 0;

    if (!FIXNUM_P(p) && RBIGNUM_LEN(p) >= SLIDING_WINDOW_THRESHOLD)
        use_sliding_window = 1;

    if (!FIXNUM_P(m) && RBIGNUM_LEN(m) >= BARRETT_REDUCE_THRESHOLD)
        use_barrett = 1;

    if (use_barrett)
        return int_powermod_sliding_window_br(b, p, m);

    if (use_sliding_window)
        return int_powermod_sliding_window(b, p, m);

    VALUE result = one;
    FOR_BITS(p, result = MUL(result, result),
                {},
                result = MUL(result, b),
                result = MOD(result, m));
    return result;
}

static int tail_zeros(VALUE n)
{
    if (FIXNUM_P(n))
    {
        long t = FIX2LONG(n);
        long s = 0;
        while (!(t & 1))
            t >>= 1, ++s;
        return s;
    }
    else
    {
        BDIGIT* digit = RBIGNUM_DIGITS(n);
        unsigned int L = RBIGNUM_LEN(n);
        unsigned int i, s, z;
        unsigned LONG_LONG j;
        const unsigned LONG_LONG MAX = 1ULL << (SIZEOF_BDIGITS * 8 - 1);
        for (i = s = 0, z = 1; i < L; ++i, ++digit)
        {
            for (j = 1; j <= MAX; j <<= 1)
                if ((*digit & j) || j == MAX)
                {
                    z = 0;
                    break;
                }
                else
                {
                    ++s;
                }
            if (z == 0) break;
        }
        return s;
    }
}

/*
 *  call-seq:
 *    integer.MR_pseudoprime? -> true or false
 *
 *  Returns <code>true</code> if +self+ is pseudoprime
 *  accordingly to Miller-Rabin test with <i>NumTheory::PrimalityTests</i>
 *  rounds. Otherwise returns <code>false</code>.
 */
VALUE 
numtheory_miller_rabin_pseudoprime_p(VALUE n)
{
    long PRIMALITY_TESTS = FIX2LONG(rb_const_get(mNumTheory, primality_tests_ID));
    
    if (FIXNUM_P(n))
    {
        long t = FIX2LONG(n);
        if (t < 0) t = -t;
        if (t == 2) return Qtrue;
        if (!(t & 1)) return Qfalse;
        if (t < 2) return Qfalse;
    }

    VALUE num;
    if (NEGATIVE_P(n))
    {
        num = ABS(n);
    }
    else
    {
        num = n; // keeps just a reference instead of clone
    }

    if (EVEN_P(num))
    {
        return Qfalse;
    }

    VALUE num_minus_one = SUB(num, one);
    VALUE d = CLONE(num_minus_one);
    VALUE num_minus_four = SUB(num, four);

    int s = tail_zeros(d);
    d = RIGHT_SHIFT(d, s);

    int i,j;
    for (i = 0; i < PRIMALITY_TESTS; ++i)
    {

        VALUE a = rb_funcall(rb_cRandom, 
                id_rand,
                1, 
                num_minus_four);
        a = ADD(a, two);

        VALUE x = numtheory_int_powermod(a, d, num);
        if (EQL(x, one) ||
            EQL(x, num_minus_one))
        {
            continue;
        }
        for(j = 0; j < s; ++j)
        {
            x = MOD(MUL(x, x), num);
            if (EQL(x, one))
            {
                return Qfalse;
            }
            if (EQL(x, num_minus_one))
            {
                break;
            }
        }
        if (!EQL(x, num_minus_one))
        {
            return Qfalse;
        }
    }
    return Qtrue;
}

inline static int
prime_p(unsigned LONG_LONG t)
{
    return (t & 1) ? t == 3 ? 1 : 
                       ((t % 6 == 1) || (t % 6 == 5)) ?
                           t < PRIMES_UPPER_LIMIT ? 
                               is_set(numtheory_is_prime, (unsigned long)t) ? 1 : 0 :
                               numtheory_bpsw_pseudoprime_p(ULL2NUM(t)) :
                       0
                   : t == 2 ? 1 : 0;
}

/*
 *  call-seq:
 *    integer.prime?  -> true or false
 *
 *  Returns exact result for values which are precomputed, and result
 *  of standard Baillie-Pomerance-Selfridge-Wagstaff test for larger values.
 *
 *    29.prime?      #=> true
 */
VALUE
numtheory_prime_p(VALUE n) 
{
    unsigned long t;
    if (FIXNUM_P(n) && (t = abs(FIX2LONG(n)), t < PRIMES_UPPER_LIMIT))
    {
#ifdef DEBUG
        fprintf(stderr, "[prime_p] looking up in the precomputed table...\n");
#endif
        return prime_p(t) ? Qtrue : Qfalse;
    }
    else
    {
#ifdef DEBUG
        fprintf(stderr, "[prime_p] calling BPSW primality test...\n");
#endif
        return numtheory_bpsw_pseudoprime_p(n);
    }
}

/*
 *  call-seq:
 *    integer.nextprime -> integer
 *
 *  Returns the least (pseudo)prime > +self+. 
 *
 *    (10**28).nextprime #=> 10000000000000000000000000331
 */
VALUE
numtheory_nextprime(VALUE n)
{
    VALUE p = n;
    for(;;)
    {
        p = ADD(p, one);
        if (numtheory_prime_p(p))
            return p;
    }
}

inline static void raise_not_enough_exception()
{
    rb_raise(rb_eArgError, "Not enough precomputed primes");
}

inline static unsigned long
nth_prime(long n)
{
    return numtheory_primes[n-1];
}

/*
 *  call-seq:
 *    NumTheory.prime(integer)  -> integer
 *
 *  Returns prime with corresponding index. 
 *  The prime 2 is assumed to have index 1.
 *
 *    NumTheory.prime 1             #=> 2
 *    NumTheory.prime 1000          #=> 7919
 */
VALUE
numtheory_prime(VALUE obj, VALUE i)
{
    long t;
    if (FIXNUM_P(i))
    {
        t = FIX2LONG(i);
        if (t < 1)
        {
            rb_raise(rb_eArgError, "the argument must be positive");
        }
        if ((unsigned long)t > NUM_OF_PRIMES)
        {
            raise_not_enough_exception();
        }
        return ULONG2NUM(nth_prime((unsigned long)t));
    }
    raise_not_enough_exception();
    return Qnil;
}

long
primepi(unsigned long t)
{
    if (t < 2) return 0;
    
    unsigned long l, m, r;
    l = 0;
    r = NUM_OF_PRIMES - 1;

    if (numtheory_primes[r] <= t)
    {
        return r + 1;
    }

    while (l < r-1)
    {
        m = (l + r) >> 1;
        if (numtheory_primes[m] > t)
            r = m;
        else if (numtheory_primes[m] < t)
            l = m;
        else return m + 1;
    }
    return r;
}

/*
 *  call-seq:
 *    NumTheory.primepi(integer) -> integer
 *
 *  Returns the number of primes less or equal than a given value.
 *
 *    NumTheory.primepi 10          #=> 4
 *    NumTheory.primepi -10         #=> 0
 */
VALUE
numtheory_primepi(VALUE obj, VALUE n)
{
    if (NEGATIVE_P(n))
    {
        return zero;
    }
    unsigned long t = TO_ULONG(n);
    if (t <= PRIMES_UPPER_LIMIT)
    {
        return ULONG2NUM(primepi(t));
    }
    raise_not_enough_exception();
    return Qnil;
}

/*
 *  call-seq:
 *    NumTheory.fibonacci(n, [modulo]) -> integer
 *
 *  Returns <i>n</i>-th fibonacci number (<code>% modulo</code> 
 *  if the <i>modulo</i> is given).
 *  0-th fibonacci number is assumed to be zero.
 *
 *    NumTheory.fibonacci 10                    #=> 55
 *    NumTheory.fibonacci 10**30, 1234567890    #=> 862134135
 */
VALUE
numtheory_fibonacci(int argc, VALUE* argv)
{
    VALUE n, modulo;
    rb_scan_args(argc, argv, "11", &n, &modulo);
  
    if (FIXNUM_P(n) && FIX2LONG(n) < 2)
    {
        return n;
    }
    
    if (!INTEGER_P(n) || NEGATIVE_P(n))
    {
        rb_raise(rb_eArgError, "n must be nonnegative integer");
    }
    if (!NIL_P(modulo) && !INTEGER_P(modulo))
    {
        rb_raise(rb_eTypeError, "modulo must be integer");
    }

    VALUE a, b, c, old_a, old_b_sq, old_c;
    a = c = one;
    b = zero;

    n = SUB(n, one);

    FOR_BITS(n, { old_a = a; old_c = c;
                  old_b_sq = MUL(b, b);
                  
                  a = ADD(MUL(a, a), old_b_sq);
                  c = ADD(MUL(c, c), old_b_sq);
                  b = MUL(b, ADD(old_a, old_c));
                },
                {
                },
                { c = b;
                  b = a;
                  a = ADD(b, c);
                },
                if (!NIL_P(modulo))
                {
                  a = MOD(a, modulo);
                  b = MOD(b, modulo);
                  c = MOD(c, modulo);
                });
    return a;
}

/*
 *  call-seq:
 *    NumTheory.sigma(n, [pow=1])  -> integer
 *
 *  Returns sum of <i>pow</i>-th powers of 
 *  divisors of <code>|n|</code>. <i>pow</i> must be
 *  nonnegative integer.
 *
 *    NumTheory.sigma 1234567890, 0        #=> 48 
 *    NumTheory.sigma 1234567890, 1        #=> 3211610688
 */
VALUE
numtheory_sigma(int argc, VALUE* argv)
{
    VALUE n, pow;
    rb_scan_args(argc, argv, "11", &n, &pow);
  
    if (!INTEGER_P(n))
    {
        rb_raise(rb_eTypeError, "n must be integer");
    }
    n = ABS(n);

    if (NIL_P(pow))
    {
        pow = one;
    } 
    else if (!INTEGER_P(pow))
    {
        rb_raise(rb_eTypeError, "power must be integer");
    }
    if (NEGATIVE_P(pow))
    {
        rb_raise(rb_eArgError, "power must be >= 0");
    }
    if (ZERO_P(n))
    {
        rb_raise(rb_eArgError, "n can't be zero");
    }
    VALUE result = one;
    VALUE tmp1, tmp2;
    int i;
    FOR_PRIME_FACTORS(n, p, d,
            { tmp1 = one;
              tmp2 = rb_big_pow(TO_BIGNUM(p), pow);
              for (i = FIX2LONG(d); i; --i)
              {
                  tmp1 = MUL(tmp1, tmp2);
                  tmp1 = ADD(tmp1, one);
              }
              result = MUL(result, tmp1);
            });
    return result;
}

/*
 *  call-seq:
 *    NumTheory.eulerphi(n)   -> integer
 *
 *  Returns the number of positive integers less than <code>|n|</code>
 *  and coprime with <code>n</code>.
 *
 *    NumTheory.eulerphi(1234567890)        #=> 329040288
 */
VALUE
numtheory_eulerphi(VALUE obj, VALUE n)
{
    if (!INTEGER_P(n))
    {
        rb_raise(rb_eTypeError, "n must be integer");
    }
    VALUE num = ABS(n);
    if (ZERO_P(num))
    {
        rb_raise(rb_eArgError, "the number must be nonzero");
    }
    VALUE result = CLONE(num);
    FOR_PRIME_FACTORS(num, p, d, 
            {
                result = MUL(result, SUB(p, one));
                result = DIV(result, p);
            });
    return result;
}

/*
 *  call-seq:
 *    integer.prime_division    ->  ary
 *
 *  Returns factorization of the number in the form of
 *  array of pairs <code>[prime, power]</code>.
 *
 *    1234567890.prime_division   
 *          #=> [[2, 1], [3, 2], [5, 1], [3607, 1], [3803, 1]]
 *    (-987654321).prime_division 
 *          #=> [[-1, 1], [3, 2], [17, 2], [379721, 1]]
 */
VALUE
numtheory_prime_division(VALUE n)
{
    VALUE pd[2];
    VALUE pds = rb_ary_new();
    if (NEGATIVE_P(n))
    {
        pd[0] = INT2FIX(-1); pd[1] = one;
        rb_ary_push(pds, rb_ary_new4(2,pd));
    }
    VALUE num = ABS(n);
    if (ZERO_P(n))
    {
        pd[0] = zero; pd[1] = one;
        rb_ary_push(pds, rb_ary_new4(2,pd));
    }
    else
    {
        FOR_PRIME_FACTORS(num, p, d,
            {
                pd[0] = p; pd[1] = d;
                rb_ary_push(pds, rb_ary_new4(2,pd));
            });
    }
    return pds;
}

/*
 *  call-seq:
 *    NumTheory.moebius(integer)  ->  -1, 0 or 1
 *
 *  Returns moebius function of argument.
 *
 *    NumTheory.moebius(4)      #=> 0
 *    NumTheory.moebius(35)     #=> 1
 */
VALUE
numtheory_moebius(VALUE obj, VALUE n)
{
    if (!INTEGER_P(n))
    {
        rb_raise(rb_eTypeError, "n must be integer");
    }
    VALUE num = ABS(n);
    if (ZERO_P(num))
    {
        rb_raise(rb_eArgError, "the number must be nonzero");
    }
    char result = 1;
    FOR_PRIME_FACTORS(num, p, d,
            {
                if (FIX2LONG(d) > 1)
                {
                    return zero;
                }
                else
                {
                    result *= -1;
                }
            });
    return INT2FIX(result);
}

static VALUE
product(VALUE* beg, VALUE* end)
{
    long len = end - beg;
    if (!len)
    {
        return one;
    }
    if (len < 4)
    {
        VALUE t = *(--end);
        while (beg != end)
            t = MUL(t, *(--end));
        return t;
    }
    long half_len = len >> 1;
    VALUE* mid= beg + half_len;
    return MUL(product(beg, mid),
               product(mid, end));
}

static VALUE
product_of_ulongs(unsigned long* beg, unsigned long* end)
{
    long len = end - beg;
    unsigned long tmp;
    if (len < 4)
    {
        tmp = *(--end);
        VALUE t = ULONG2NUM(tmp);
        while (beg != end)
            tmp = *(--end),
            t = MUL(t, ULONG2NUM(tmp));
        return t;
    }
    long half_len = len >> 1;
    unsigned long* mid = beg + half_len;
    return MUL(product_of_ulongs(beg, mid),
               product_of_ulongs(mid, end));
}

static VALUE
primorial(unsigned long min, unsigned long max)
{
    unsigned long *l;
    long m_pi = primepi(min);
    if (m_pi == 0)
    {
        m_pi = 1;
    }
    l = numtheory_primes + m_pi - 1;
    if (nth_prime(m_pi) < min)
    {
        ++m_pi;
        ++l;
    }
    return product_of_ulongs(l, numtheory_primes + primepi(max));
}

/*
 *  call-seq:
 *    NumTheory.product(ary)  -> integer
 *
 *  Returns product of integer values stored in <i>ary</i>.
 *  Returns 1 if <i>ary</i> is empty.
 *
 *    NumTheory.product([1,2,3,4,5,6])    #=> 720
 */
VALUE
numtheory_product(VALUE obj, VALUE arr)
{
    if (!RB_TYPE_P(arr, T_ARRAY))
    {
        rb_raise(rb_eTypeError, "the argument must be an array");
    }
    return product(RARRAY_PTR(arr), RARRAY_PTR(arr) + RARRAY_LEN(arr));
}

/*
 *  call-seq:
 *    integer.primes_upto(limit) {|i| block } -> self
 *    integer.primes_upto(limit)              -> enumerator
 *
 *  Yields primes in interval from +self+ to <i>limit</i> inclusively.
 *
 *    2.primes_upto(17).to_a   #=> [2, 3, 5, 7, 11, 13, 17]
 */
VALUE
numtheory_primes_upto(VALUE min, VALUE _max)
{
    VALUE max = _max;
    if (!INTEGER_P(max))
    {
        if (TYPE(max) == T_FLOAT)
        {
           max = rb_to_int(max); 
        }
        else
        {
            rb_raise(rb_eTypeError, "upper bound must be a number");
        }
    }
    if (NEGATIVE_P(SUB(max,min)))
    {
       rb_raise(rb_eArgError, "upper bound must be more than lower");
    }
    if (NEGATIVE_P(SUB(max, two)))
    {
       return rb_ary_new2(0);
    }
    if (NEGATIVE_P(SUB(min, two)))
    {
       min = two;
    }
    unsigned long m = TO_ULONG(min);
    unsigned long M = TO_ULONG(max);
    if (M > PRIMES_UPPER_LIMIT)
    {
       raise_not_enough_exception();
// TODO: allow for bignums
    }

    RETURN_ENUMERATOR(min, 1, &max);

    long m_pi = primepi(m);
    long M_pi = primepi(M);

    unsigned long* p_c = numtheory_primes + m_pi - 1;
    if (*p_c < m)
       ++m_pi, ++p_c;
       
    for ( ; m_pi <= M_pi; ++m_pi)
    {
       rb_yield(ULONG2NUM(*p_c));
       ++p_c;
    }
    return min;
}

static VALUE 
odd_swing(long n)
{
    if (n < 65)
    {
        return ULL2NUM(numtheory_small_odd_swings[n]);
    }
    unsigned long root_n = (int)(floor(sqrt((double)n))+0.5);
    unsigned long n3 = n / 3;
    long pi_n3 = primepi(n3);
    long count = 0;
    long q;
    VALUE* prime_list = malloc(sizeof(VALUE) * pi_n3);
    unsigned long* ptr = numtheory_primes + 1; // initially points to 3
    unsigned long prime;
    unsigned long prod;
    for ( ; *ptr <= root_n; ++ptr)
    {
        prod = 1;
        q = n;
        prime = *ptr;
        while (q /= prime)
            if (q & 1)
                prod *= prime;

        if (prod > 1)
            prime_list[count++] = INT2FIX(prod);
    }
    for ( ; *ptr <= n3; ++ptr)
    {
        prime = *ptr;
        if ((n / prime) & 1)
            prime_list[count++] = INT2FIX(prime);
    }
    VALUE r = product(prime_list, prime_list + count);
    free(prime_list);
    return MUL(r, primorial(n/2 + 1, n));
}


static VALUE
rec_factorial(long n)
{
    if (n < 2) return one;
    VALUE n2fac = rec_factorial(n >> 1);
    n2fac = MUL(n2fac, n2fac);
    return MUL(n2fac, odd_swing(n));
}

/*
 *  call-seq:
 *    integer.factorial    -> integer
 *
 *  Returns factorial of the number.
 *
 *    50.factorial.to_s(31)  #=> "283sp352ltrc9csg1398f0dt7dg067g15ikr4b6spf70"
 */
VALUE
numtheory_factorial_primeswing(VALUE n)
{
    long t;
    if ((t = FIX2LONG(n)) >= 10000000)
    {
        rb_raise(rb_eArgError, "the argument is too big!");
    }
    if (t < 0)
    {
        rb_raise(rb_eArgError, "factorial of negative number is undefined");
    }
    if (t <= 20)
    {
        return ULL2NUM(numtheory_small_factorials[t]);
    }
    if ((unsigned long)t > PRIMES_UPPER_LIMIT)
    {
        raise_not_enough_exception();
    }
    long shift = 0;
    long q = t;
    while (q > 0)
    {
        q >>= 1;
        shift += q;
    }
    return rb_big_lshift(TO_BIGNUM(rec_factorial(t)), LONG2NUM(shift));
}

struct Egcd {
    VALUE d, a, b;
};

static struct Egcd
extended_gcd(VALUE x, VALUE y)
{
    struct Egcd egcd_old, egcd_new;
    if (ZERO_P(y))
    {
        egcd_new.d = x;
        egcd_new.a = one;
        egcd_new.b = zero;
        return egcd_new;
    }
    VALUE divmod = DIVMOD(x, y);
    egcd_old = extended_gcd(y, rb_ary_entry(divmod, 1));
    egcd_new.d = egcd_old.d;
    egcd_new.a = egcd_old.b;
    egcd_new.b = SUB(egcd_old.a, MUL(rb_ary_entry(divmod, 0),
                                     egcd_old.b));
    return egcd_new;
}

/*
 *  call-seq:
 *    Integer.extended_gcd(y)   -> Integer
 *
 *  Returns array <code>[d, [a, b]]</code> where
 *  <i>d</i> is +self+.gcd(<i>y</i>) and <code>a*self + b*y = d</code>
 *
 *    0.extended_gcd(0)           #=> [0, [0, 0]]
 *    239.extended_gcd(932)       #=> [1, [39, -10]]
 */
VALUE
numtheory_extended_gcd(VALUE x, VALUE y)
{
    if (!INTEGER_P(y))
    {
        rb_raise(rb_eTypeError, "not an integer");
    }
    struct Egcd egcd;
    if (ZERO_P(x) && ZERO_P(y))
    {
        egcd.d = egcd.a = egcd.b = zero;
    }
    else
    {
        egcd = extended_gcd(x, y);
    }
    VALUE ary = rb_ary_new2(2);
    rb_ary_store(ary, 0, egcd.d);
    VALUE coeffs = rb_ary_new2(2);
    rb_ary_store(coeffs, 0, egcd.a);
    rb_ary_store(coeffs, 1, egcd.b);
    rb_ary_store(ary, 1, coeffs);
    return ary;
}

/*
 *  call-seq:
 *    integer.inverse(m)  -> integer
 *
 *  Returns modular inverse modulo <i>m</i> of +self+.
 *
 *    1234.inverse(12345)   #=> 9874
 */
VALUE
numtheory_modular_inverse(VALUE x, VALUE y)
{
    struct Egcd egcd = extended_gcd(x, y);
    if (egcd.d != one)
    {
        rb_raise(rb_eArgError, "modular inverse doesn't exist");
    }
    VALUE result = egcd.a;
    if (NEGATIVE_P(result))
    {
        result = ADD(result, y);
    }
    return result;
}

/*
 *  call-seq:
 *    integer.znorder(modulo)  -> integer
 *
 *  Returns minimal positive integer <i>n</i> such that
 *  <code>+self+.powermod(n, modulo) == 1</code>
 *
 *    20.znorder(11)  => 5
 */
VALUE
numtheory_multiplicative_order(VALUE a, VALUE m)
{
    if (rb_funcall(a, id_gcd, 1, m) != one)
    {
        rb_raise(rb_eArgError, "not in (Z/nZ)*");
    }
    VALUE result = one;
    VALUE pd, phi, o, x;
    FOR_PRIME_FACTORS(ABS(m), p, d,
    {
        o = one;
        pd = POW(p, SUB(d, o));
        phi = MUL(SUB(p, o), pd);
        pd = MUL(pd, p);
        FOR_PRIME_FACTORS(phi, q, e,
        {
            x = numtheory_int_powermod(a, 
                                       DIV(phi, POW(q, e)),
                                       pd);
            while (x != one)
            {
                o = MUL(o, q);
                x = numtheory_int_powermod(x, q, pd);
            }
        });
        result = rb_funcall(result, id_lcm, 1, o);
    });
    return result;
}

static inline VALUE* triple_new(VALUE v1, VALUE v2, VALUE v3)
{
    VALUE* v = malloc(sizeof(VALUE)*3);
    v[0] = v1; v[1] = v2; v[2] = v3;
    return v;
}

#define TRIPLE_SUM(triple) \
    (ADD(triple[0], ADD(triple[1], triple[2])))


/*
 *  call-seq:
 *    NumTheory.triples(max_perimeter) -> enumerator
 *   
 *  Yields primitive pythagorean triples with perimeter less
 *  or equal to <i>max_perimeter</i>.
 */
VALUE 
numtheory_pythagorean_triples(VALUE obj, VALUE max_l)
{
    RETURN_ENUMERATOR(obj, 1, &max_l);

    Queue* q = queue_new();

    VALUE* triple;

    queue_push(q, triple_new(three, four, five));

    VALUE a, b, c, a2, b2, c2, c3;
    VALUE t1,t2,t3;

    while (q -> first)
    {
        triple = queue_pop(q);
        if (MORE(TRIPLE_SUM(triple), max_l))
        {
            free(triple);
            continue;
        }

        a = triple[0]; b = triple[1]; c = triple[2];
        rb_yield_values2(3, triple);

        // uses ternary pythagorean tree
        a2 = DOUBLED(a);
        b2 = DOUBLED(b);
        c2 = DOUBLED(c);
        c3 = ADD(c, c2);

        t1 = ADD(a,c2); t2 = ADD(a2,c2); t3 = ADD(a2,c3);

        queue_push(q, triple_new(SUB(t1, b2),
                                 SUB(t2, b),
                                 SUB(t3, b2)));

        queue_push(q, triple_new(SUB(ADD(b2, c2), a),
                                 SUB(ADD(b, c2), a2),
                                 SUB(ADD(b2, c3), a2)));

        queue_push(q, triple_new(ADD(t1, b2),
                                 ADD(t2, b),
                                 ADD(t3, b2)));
        free(triple);
    }
    free(q);
    return Qnil;
}

/*
 *  call-seq:
 *    integer.popcount -> integer
 *  
 *  Returns the number of set bits.
 */
VALUE
numtheory_popcount(VALUE n)
{
    if (NEGATIVE_P(n))
    {
        rb_raise(rb_eArgError, "popcount is undefined for negative numbers");
    }
    unsigned long k = 0;
    FOR_BITS(n, {}, {}, { ++k; }, {});
    return ULONG2NUM(k);
}

int
long_bitlength(unsigned long v)
{   
    /* taken from http://graphics.stanford.edu/~seander/bithacks.html */

#if SIZEOF_LONG == 8
    const unsigned long b[] = {0x2, 0xC, 0xF0, 0xFF00, 0xFFFF0000, 0xFFFFFFFF00000000ULL};
    const unsigned long S[] = {1, 2, 4, 8, 16, 32};
    int i;
    register unsigned long r = 0;
    for (i = 5; i >= 0; --i)
#else
    const unsigned long b[] = {0x2, 0xC, 0xF0, 0xFF00, 0xFFFF0000 };
    const unsigned long S[] = {1, 2, 4, 8, 16};
    int i;
    register unsigned long r = 0;
    for (i = 4; i >= 0; --i)
#endif
    {
      if (v & b[i])
      {
        v >>= S[i];
        r |= S[i];
      } 
    }
    return ++r;
}

/*
 *  call-seq:
 *    integer.bitlength -> integer
 *
 *  Returns the length in bits. Defined for positive integers only.
 *  0 is assumed to have length 1.
 */
VALUE
numtheory_bitlength(VALUE n)
{
    if (NEGATIVE_P(n))
    {
        rb_raise(rb_eArgError, "n must be positive");
    }
    long bit_len;
    if (FIXNUM_P(n))
    {
        return INT2FIX(long_bitlength(FIX2LONG(n)));
    }
    else
    {
        long len = RBIGNUM_LEN(n);
        bit_len = (len - 1) * SIZEOF_BDIGITS * 8;
        return INT2FIX(bit_len + long_bitlength(RBIGNUM_DIGITS(n)[len - 1]));
    }
}

static int
ull_jacobi(LONG_LONG a, unsigned LONG_LONG n)
{
    /* it's assumed that n is odd and > 0 */
    int t = 1;
    LONG_LONG tmp;
    while (a) {
        if (a < 0) {
            a = -a; if ((n & 3) == 3) t = -t;
        }
        switch (n & 7) {
            case 1: case 7:
                while (!(a&1)) a >>= 1; break;
            case 3: case 5:
                while (!(a&1)) a >>= 1, t = -t; break;
        }
        tmp = a; a = n; n = tmp; 
        if ((a & 3) == 3 && (n & 3) == 3) t = -t;
        a %= n; 
        if (a > (LONG_LONG)(n >> 1)) a -= n;
    }
    return n == 1 ? t : 0;
}

/*
 *  call-seq:
 *    integer.jacobi(n) -> -1, 0 or 1
 *  
 *  Returns jacobi symbol (a|n) where n must be odd positive
 */
VALUE
numtheory_jacobi(VALUE a, VALUE n)
{
    if (EVEN_P(n) || NEGATIVE_P(n))
    {
        rb_raise(rb_eArgError, "n must be odd positive");
    }
    int t = 1, z;
    VALUE tmp;

    a = MOD(a, n); 
    if (MORE(a, HALF(n))) {
        a = SUB(a, n);
    }

    while (!ZERO_P(a))
    {
        if (FIXNUM_P(n) && FIXNUM_P(a)) 
        {
            return INT2FIX(t * ull_jacobi(FIX2LONG(a), FIX2LONG(n)));
        }
        if (RBIGNUM_LEN(n) <= 2) // unsigned LONG_LONG is used 
        {
            return INT2FIX(t * ull_jacobi(FIXNUM_P(a) ? FIX2LONG(a) : rb_big2ll(a), 
                                          rb_big2ull(n)));
        }
        if (NEGATIVE_P(a))
        {
            NEGATE(a);
            if (MOD_4(n) == 3)
                t = -t;
        }
        z = tail_zeros(a);
        a = RIGHT_SHIFT(a, z);
        if (z & 1)
        {
            switch (MOD_8(n))
            {
                case 3:
                case 5:
                    t = -t;
                    break;
            }
        }
        tmp = a;
        a = n;
        n = tmp; /* n > 0 */
        if (MOD_4(a) == 3 && MOD_4(n) == 3)
            t = -t;
        a = MOD(a, n); 
        if (MORE(a, HALF(n))) {
            a = SUB(a, n);
        }
    }
    if (n == one)
    {
        return INT2FIX(t);
    }
    else
    {
        return zero;
    }
}

/*
 *  call-seq:
 *    a.sqrt_mod(p) -> integer or nil
 *
 *  If (a|p) = 1 returns the square root of a in Z/pZ
 *  from the interval [0, p/2]. Otherwise returns +nil+.
 */
VALUE
numtheory_sqrtmod(VALUE a, VALUE p)
{
    if (!INTEGER_P(p))
    {
        rb_raise(rb_eArgError, "the parameters must be integer");
    }
    else if (!numtheory_prime_p(p))
    {
        rb_raise(rb_eArgError, "p must be prime");
    }
    p = ABS(p);
    if (p == two)
    {
        return EVEN_P(a) ? zero : one;
    }
    if (numtheory_jacobi(a, p) != one)
    {
        return Qnil;
    }

    VALUE t = SUB(p, one);
    VALUE p_minus_one = CLONE(t);

    int e = tail_zeros(t);
    VALUE r;

    t = RIGHT_SHIFT(t, e);

    if (e == 1)
    {
        r = numtheory_int_powermod(a, ADD(HALF(t), one), p);
    }
    else if (e == 2)
    {
        if (numtheory_int_powermod(a, t, p) == one)
        {
            r = numtheory_int_powermod(a, ADD(HALF(t), one), p);
        }
        else
        {
            a = DOUBLED(a);
            r = MOD(MUL(a, numtheory_int_powermod(DOUBLED(a), HALF(t), p)), p);
        }
    }
    else 
    {
        // Tonelli-Shanks algorithm
        // implemented as described in "Handbook of Applied Cryptography"
        int b = 14;
        while(1)
        {
            if (numtheory_jacobi(INT2FIX(b), p) == INT2FIX(-1))
                break;
            ++b;
        }

        VALUE a_inv = numtheory_modular_inverse(a, p);
        VALUE D = numtheory_int_powermod(INT2FIX(b), t, p);
        r = numtheory_int_powermod(a, ADD(HALF(t), one), p);

        int i;
        VALUE d;
        VALUE power_of_two = rb_big_lshift(TO_BIGNUM(one), INT2FIX(e - 2));
        for (i = 1; i < e; ++i)
        {
            d = numtheory_int_powermod(MUL(MUL(r, r), a_inv),
                                       power_of_two, p);
            power_of_two = HALF(power_of_two);
            if (EQL(d, p_minus_one))
            {
                r = MOD(MUL(r, D), p);
            }
            D = MOD(MUL(D, D), p);
        }
    }
    if (MORE(r, HALF(p)))
    {
        r = SUB(p, r);
    }
    return r;
}

/*
 *  call-seq:
 *    integer.isqrt -> integer
 *
 *  Returns the square root integer part.
 */
VALUE
numtheory_isqrt(VALUE n)
{
    if (NEGATIVE_P(n))
    {
        rb_raise(rb_eArgError, "can't take square root from negative number");
    }
    if (ZERO_P(n))
    {
        return zero;
    }
    VALUE x, y;
    if (FIXNUM_P(n))
    {
        x = n;
    }
    else
    {
        x = rb_big_lshift(TO_BIGNUM(one), ADD(HALF(numtheory_bitlength(n)), one));
    }
    for ( ; ; )
    {
        y = HALF(ADD(x, DIV(n, x)));
        if (LESS(y, x))
            x = y;
        else
            break;
    }
    return x;
}

static int MOD_255(VALUE n)
{
    if (FIXNUM_P(n))
    {
        return FIX2LONG(n) % 255;
    }
    unsigned LONG_LONG m = 0;
    BDIGIT* digit = RBIGNUM_DIGITS(n);
    int i;
    for (i = 0; i < RBIGNUM_LEN(n); ++i, ++digit)
    {
        m += *digit & 0xFF;
        m += (*digit & 0xFF00) >> 8;
        m += (*digit & 0xFF0000) >> 16;
        m += (*digit & 0xFF000000UL) >> 24;
    }
    return m % 255;
}

/*
 *  call-seq:
 *    integer.square? -> true or false
 *
 *  Checks whether the integer is a perfect square or not.
 */
VALUE
numtheory_perfect_square_p(VALUE n)
{
    if (NEGATIVE_P(n))
    {
        return Qfalse;
    }

    if ( !issquare_mod256[ (FIXNUM_P(n) ? FIX2ULONG(n) : RBIGNUM_DIGITS(n)[0]) & 0xFF ]
            ||
         !issquare_mod255[ MOD_255(n) ])
    {
        // only 44 residues mod 256 are squares, and only 54 residues mod 255.
        // 44/256 * 54/255 = 0.0364 so in 96.4% of cases there's no need to 
        //   check anything else
        return Qfalse;
    }

    VALUE r = numtheory_isqrt(n);
    return EQL(MUL(r, r), n) ? Qtrue : Qfalse;
}

static VALUE
lucas_pseudoprime_p(VALUE n, VALUE a, VALUE b)
{
    // for internal use only

    // the details about the algorithm can be found in the book 
    //     "Prime numbers. A computational perspective."

    VALUE d = SUB(MUL(a, a), MUL(b, four));
    // it's assumed here that d is not a full square and gcd(2abd, n) = 1

    VALUE A = MOD(SUB(MUL(MUL(a, a), 
                          numtheory_modular_inverse(b, n)), 
                      two), 
                  n);

    VALUE m = HALF(SUB(n, numtheory_jacobi(d, n)));

    VALUE u = two;
    VALUE v = CLONE(A);
    FOR_BITS(m, {},
                {
                    v = MOD(SUB(MUL(u, v), A), n);
                    u = MOD(SUB(MUL(u, u), two), n);
                },
                {
                    u = MOD(SUB(MUL(u, v), A), n);
                    v = MOD(SUB(MUL(v, v), two), n);
                },
                {});
    if (EQL( MOD(MUL(A, u), n),
             MOD(MUL(two, v), n)))
    {
        return Qtrue;
    }
    return Qfalse;
}
 
static VALUE power_of_2_mod(VALUE p, VALUE m){
    // Used internally in base-2 Miller-Rabin test.
    // Uses left shift instead of multiplication.
    VALUE result = one;
    FOR_BITS(p, result = MUL(result, result),
                {},
                result = DOUBLED(result),
                result = MOD(result, m));
    return result;
}
   
static VALUE miller_rabin_base2_pseudoprime_p(VALUE n)
{
    /* for internal use only */
#ifdef DEBUG
    fprintf(stderr, "[miller-rabin base 2] start test for %s...\n", TO_CSTRING(n));
#endif
    VALUE num = rb_big_clone(TO_BIGNUM(n));
    VALUE num_minus_one = SUB(num, one);
    VALUE d = rb_big_clone(TO_BIGNUM(num_minus_one));

    int s = tail_zeros(d);
#ifdef DEBUG
    fprintf(stderr, "[miller-rabin base 2] p-1 =  %s...\n", TO_CSTRING(d));
    fprintf(stderr, "[miller-rabin base 2] tail zeros: %d\n", s);
#endif
    d = RIGHT_SHIFT(d, s); 

    VALUE x = power_of_2_mod(d, num);
#ifdef DEBUG
    fprintf(stderr, "[miller-rabin base 2] 2**d = %s modulo %s\n", TO_CSTRING(x), TO_CSTRING(num));
#endif
    if (EQL(x, one) || EQL(x, num_minus_one)) return Qtrue;

    while (s--) {
        x = MOD(MUL(x, x), num);
#ifdef DEBUG
    fprintf(stderr, "[miller-rabin base 2] x = %s modulo %s\n", TO_CSTRING(x), TO_CSTRING(num));
#endif
        if (EQL(x, one)) return Qfalse;
        if (EQL(x, num_minus_one)) break;
    }
    if (!EQL(x, num_minus_one)) {
        return Qfalse;
    }
    return Qtrue;
}

/*
 *  call-seq:
 *    integer.BPSW_pseudoprime? -> true or false
 *
 *  Standard BPSW primality test.
 */
VALUE
numtheory_bpsw_pseudoprime_p(VALUE n)
{
    VALUE num = ABS(n);

    if (EQL(num, two))
    {
        return Qtrue;
    }
    if (LESS(num, two) || EVEN_P(num) || 
                          (numtheory_perfect_square_p(num) == Qtrue))
    {
#ifdef DEBUG
        fprintf(stderr, "[bpsw_pseudoprime_p] the number is less than 2, even or a perfect square\n");
#endif
        return Qfalse;
    }

    // Miller-Rabin test with base 2
    if (miller_rabin_base2_pseudoprime_p(num) == Qfalse)
    {
#ifdef DEBUG
        fprintf(stderr, "[bpsw_pseudoprime_p] Miller-Rabin test with base 2 returned false\n");
#endif
        return Qfalse;
    }

    // standard Lucas-Selfridge test. Based on the description from
    //     http://www.e-maxx.ru/algo/bpsw
    int d_abs, d_sign, d;
    for (d_abs = 5, d_sign = 1; ; d_abs += 2, d_sign = -d_sign)
    {
        d = d_abs * d_sign;
        VALUE G = rb_funcall(num, id_gcd, 1, INT2FIX(d));
        if (MORE(G, one) && LESS(G, num))
        {
            return Qfalse;
        }
        if (numtheory_jacobi(INT2FIX(d), num) == INT2FIX(-1))
        {
            break;
        }
    }
   
#ifdef DEBUG
    fprintf(stderr, "[bpsw_pseudoprime_p] calling Lucas primality test...\n");
#endif
    return lucas_pseudoprime_p(num, one, RIGHT_SHIFT(SUB(one, INT2FIX(d)), 2));
}

DEFINE_POWERMOD_SLIDING_WINDOW(int_powermod_sliding_window,
                               {}, 
                               MOD)

DEFINE_POWERMOD_SLIDING_WINDOW(int_powermod_sliding_window_br,
                               VALUE mu = rb_big_barrett_mu(m),
                               BARRETT_MOD)
