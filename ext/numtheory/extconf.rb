require 'mkmf'
$CFLAGS = "-g -O3 -funroll-all-loops -fPIC -pedantic -std=c99 -Wall -Wno-unused-but-set-variable"
create_makefile 'numtheory'
