unsigned long long numtheory_small_factorials[] = 
{ 1ULL, 1ULL, 2ULL, 6ULL, 24ULL, 120ULL, 720ULL, 5040ULL, 40320ULL, 
  362880ULL, 3628800ULL, 39916800ULL, 479001600ULL, 6227020800ULL, 
  87178291200ULL, 1307674368000ULL, 20922789888000ULL, 355687428096000ULL,
  6402373705728000ULL, 121645100408832000ULL, 2432902008176640000ULL };

unsigned long long numtheory_small_odd_swings[] = 
{ 1ULL, 1ULL, 1ULL, 3ULL, 3ULL, 15ULL, 5ULL, 35ULL, 35ULL, 315ULL, 63ULL, 
  693ULL, 231ULL, 3003ULL, 429ULL, 6435ULL, 6435ULL, 109395ULL, 12155ULL, 
  230945ULL, 46189ULL, 969969ULL, 88179ULL, 2028117ULL, 676039ULL, 16900975ULL,
  1300075ULL, 35102025ULL, 5014575ULL, 145422675ULL, 9694845ULL, 300540195ULL,
  300540195ULL, 9917826435ULL, 583401555ULL, 20419054425ULL, 2268783825ULL, 
  83945001525ULL, 4418157975ULL, 172308161025ULL, 34461632205ULL, 
  1412926920405ULL, 67282234305ULL, 2893136075115ULL, 263012370465ULL, 
  11835556670925ULL, 514589420475ULL, 24185702762325ULL, 8061900920775ULL, 
  395033145117975ULL, 15801325804719ULL, 805867616040669ULL, 61989816618513ULL,
  3285460280781189ULL, 121683714103007ULL, 6692604275665385ULL, 
  956086325095055ULL, 54496920530418135ULL, 1879204156221315ULL,
  110873045217057585ULL, 7391536347803839ULL, 450883717216034179ULL, 
  14544636039226909ULL, 916312070471295267ULL, 916312070471295267ULL };

char issquare_mod256[] = { 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 };

char issquare_mod255[] = { 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
