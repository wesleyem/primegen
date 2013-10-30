# Prime Number Generator

2^n bit prime number generator using the miller-rabin primality test.

#### Dependencies
	$> sudo apt-get install libssl-dev

#### Usage
	$> gcc primegen.c -o primegen -lcrypto
	$> ./primegen 10
	Start: Mon Oct 28 21:17:29 2013
	The number below is prime
	788153
	End: Mon Oct 28 21:17:34 2013
	Total time to perform entire program: 5.00 seconds