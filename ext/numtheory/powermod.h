#ifndef NUMTHEORY_POWERMOD_H
#define NUMTHEORY_POWERMOD_H

BDIGIT int_powermod_montgomery_small(VALUE, VALUE, BDIGIT);
VALUE int_powermod_sliding_window(VALUE, VALUE, VALUE);
VALUE int_powermod_sliding_window_br(VALUE, VALUE, VALUE);
VALUE int_powermod_sliding_window_mont(VALUE, VALUE, VALUE);

#endif
