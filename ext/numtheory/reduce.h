#ifndef NUMTHEORY_REDUCE_H
#define NUMTHEORY_REDUCE_H

VALUE rb_big_barrett_reduce(VALUE,VALUE,VALUE,int);
VALUE rb_big_barrett_mu(VALUE);

BDIGIT montgomery_setup(BDIGIT);
VALUE rb_big_montgomery_reduce(VALUE,VALUE,VALUE,int);
BDIGIT small_montgomery_reduce(BDIGIT_DBL, BDIGIT, BDIGIT);

#endif
