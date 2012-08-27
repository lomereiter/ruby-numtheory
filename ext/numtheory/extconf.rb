require 'mkmf'
$CFLAGS = "-g -O3 -funroll-all-loops -fPIC -pedantic -std=c99"
create_makefile 'numtheory'
