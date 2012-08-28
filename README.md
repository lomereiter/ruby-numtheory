# ruby-numtheory

The library is written as C extension and aims to provide fast implementations of common number-theoretical algorithms.

[![Build Status](https://secure.travis-ci.org/lomereiter/ruby-numtheory.png)](http://travis-ci.org/lomereiter/ruby-numtheory)

## Currently implemented

* powermod (uses sliding window exponentiation)
* miller-rabin test for primality
* standard BPSW primality test
* eratosthenes sieve
* factorization (via trial division)
* factorial (uses PrimeSwing algorithm)
* sigma (σ)
* moebius (μ)
* eulerphi (φ)
* primepi (π)
* fibonacci
* modular inverse
* extended gcd
* primitive pythagorean triple generator
* integer square root
* square root in Z/pZ (for prime p)
