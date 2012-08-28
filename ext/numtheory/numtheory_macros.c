typedef int by_ISO_C_standard_translation_unit_should_not_be_empty;

#define FOR_PRIME_FACTORS(n, p, d, action) do { \
    int __d; \
    BDIGIT __i; \
    unsigned LONG_LONG __t; \
    VALUE __num = (FIXNUM_P(n) ? n : rb_big_clone(n)); \
    if (!FIXNUM_P(__num)) \
    { \
      if (rb_big_cmp(__num, ULL2NUM(ULLONG_MAX)) != INT2FIX(1)) \
      { \
        __t = rb_big2ull(__num); \
      } \
      else \
      { \
        rb_raise(rb_eNotImpError, "Not implemented for numbers >= 2**64"); \
      } \
    } \
    else \
    { \
      __t = FIX2LONG(__num); \
    } \
 \
    unsigned LONG_LONG __p; \
    VALUE p, d; \
    if (prime_p(__t)) \
    { \
      d = INT2FIX(1); \
      p = ULL2NUM(__t); \
      action; \
      __t = 1; \
      break; \
    } \
    for (__i = 0; (__i < NUM_OF_PRIMES) && (__t > 1); ++__i) \
    { \
      __p = numtheory_primes[__i]; \
      if (__p * __p > __t) \
      { \
        d = INT2FIX(1); \
        __p = __t; \
        p = ULL2NUM(__p); \
        action; \
        __t = 1; \
        break; \
      } \
      if (__t % __p == 0) \
      { \
        __d = 0; \
        do { \
            __t /= __p; \
            ++__d; \
        } while (!(__t % __p)); \
        p = ULL2NUM(__p); \
        d = INT2FIX(__d); \
        action; \
                \
        if (prime_p(__t)) \
        { \
          d = INT2FIX(1); \
          p = ULL2NUM(__t); \
          action; \
          __t = 1; \
          break; \
        } \
      } \
    } \
    if (__t != 1) \
    { \
      p = ULL2NUM(__t); \
      if (numtheory_prime_p(p) == Qtrue) \
      { \
          d = INT2FIX(1); \
          action; \
      } \
      else \
      { \
          raise_not_enough_exception(); \
      } \
    } \
} while(0)

#define FOR_BITS(n, before_action, if_zero_action, if_one_action, final_action) do { \
    if (FIXNUM_P(n)) { \
        long _pow = FIX2LONG(n); \
        long _half_p = _pow >> 1; \
        long _j = 1; \
        for( ; _j <= _half_p; _j <<= 1 ); \
        for( ; _j ; _j >>= 1) { \
            before_action; \
            if (_pow & _j) do { if_one_action; } while(0); \
            else if_zero_action; \
            final_action; \
        } \
    } \
    else { \
        BDIGIT* _digits = RBIGNUM_DIGITS(n); \
        long _digits_length = RBIGNUM_LEN(n); \
        unsigned LONG_LONG _j; \
        BDIGIT* _digit = _digits + _digits_length - 1; \
        BDIGIT _half_fst_digit = (*_digit) >> 1; \
        for (_j = 1; _j <= _half_fst_digit; _j <<= 1); \
        for (; _digit >= _digits; --_digit) { \
            for (; _j > 0; _j >>= 1){ \
                before_action; \
                if (*_digit & _j) do { if_one_action; } while(0); \
                else if_zero_action; \
                final_action; \
            } \
            _j = 1ULL << (SIZEOF_BDIGITS * 8 - 1); \
        } \
    } \
} while(0)

#define DEFINE_POWERMOD_SLIDING_WINDOW(FUNC_NAME, REDUCE_PROLOGUE, REDUCE_METHOD) \
static VALUE \
FUNC_NAME(VALUE b, VALUE p, VALUE m) {\
    VALUE result = one; \
    enum { squaring, collecting } state; \
    \
    state = collecting; \
    int len, k; \
    int zeros = 0; \
    int pow = 0; \
    \
    /* adjusting window size */ \
    if (FIXNUM_P(p)) p = INTVALUE2BIG(p); \
    len = RBIGNUM_LEN(p); \
    if      (len  <  8) k = 4; \
    else if (len  < 20) k = 5; \
    else if (len  < 48) k = 6; \
    else if (len < 120) k = 7; \
    else k = 8; \
    \
    len = 0; \
    \
    REDUCE_PROLOGUE; \
    \
    b = MOD(b, m); /* necessary because for Barrett reduction to work \
                      b must be less than m^2 */    \
    \
    VALUE b_squared = REDUCE_METHOD(MUL(b, b), m); \
    VALUE *powers = ALLOCA_N(VALUE, 1<<(k-1)); \
    powers[0] = b; \
    \
    int i; \
    /* precomputation of odd powers; \
       powers[n] = (b ** (2n + 1)) % m        */ \
    for (i = 1; i < (1<<(k-1)); ++i) \
        powers[i] = REDUCE_METHOD(MUL(powers[i-1], b_squared), m); \
    \
    FOR_BITS(p, {}, \
                { /* if the bit is zero */ \
                    if (state == collecting) { \
                        ++ len; \
                        ++ zeros; \
                        pow <<= 1; \
                        if (len == k) { \
                            pow >>= zeros; \
    \
                            for (i = len; i > zeros; i--) \
                                result = REDUCE_METHOD(MUL(result, result), m); \
                            result = REDUCE_METHOD(MUL(result, powers[pow>>1]), m); \
                            while (zeros--) \
                                result = REDUCE_METHOD(MUL(result, result), m); \
    \
                            state = squaring; pow = len = zeros = 0; \
                        } \
                    } else { \
                        result = REDUCE_METHOD(MUL(result, result), m); \
                    } \
                }, \
                { /* the bit is one */ \
                    if (state == collecting) { \
                        ++ len; \
                        zeros = 0; \
                        pow = (pow << 1) + 1; \
                        if (len == k) { \
                            while (len--) \
                                result = REDUCE_METHOD(MUL(result, result), m); \
                            result = REDUCE_METHOD(MUL(result, powers[pow>>1]), m); \
                            state = squaring; \
                            pow = len = 0; \
                        } \
                    } else { \
                        state = collecting; \
                        pow = 1; \
                        len = 1; \
                        zeros = 0; \
                    } \
                }, {}); \
    if (len > 0) { \
        pow >>= zeros; \
    \
        for (i = len; i > zeros; i--) \
            result = REDUCE_METHOD(MUL(result, result), m); \
        result = REDUCE_METHOD(MUL(result, powers[pow>>1]), m); \
        while (zeros--) \
            result = REDUCE_METHOD(MUL(result, result), m); \
    } \
    return result; \
}

#define BARRETT_MOD(x, m) rb_big_barrett_reduce(x, m, mu, 1)
