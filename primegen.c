#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <openssl/bn.h>


int size;

// returns x^((p-1)/2) mod p example: 174^110 mod 221 = "220" = p - 1
BIGNUM * modxf(BIGNUM *t, BIGNUM *r, BIGNUM *rand, BN_CTX *ctx) {
	BIGNUM *l = BN_new(); BN_one(l);
	BIGNUM *ans = BN_new();
	BIGNUM *res = BN_new(); BN_one(res);
	BIGNUM *a = BN_new();
	int oddflag;
	int i = 0;
	int d;
	char *dec = BN_bn2dec(t);
	d = atoi(dec); // [exp]
	BN_one(ans);
	BN_sqr(r, r, ctx); // r = r^2
	for (i ; i < (d/2) ; i++) {
		BN_mul(ans, res, r, ctx); // ans = res * r
		BN_mod(res, ans, rand, ctx); // res = ans % rand
	}
	l = BN_dup(res);
return l;
}

int primetest(BIGNUM *rand, BN_CTX *ctx) {
	BIGNUM *r = BN_new(); // random value 0 < x < rand [base]
	BIGNUM *temp = BN_new(); // temp BIGNUM for calculating exponent
	BIGNUM *t = BN_new(); // BIGNUM exponent [exponent]
	BIGNUM *rem = BN_new(); // remainder for division function
	BIGNUM *one = BN_new(); BN_one(one); // BIGNUM = 1
	BIGNUM *two = BN_new(); BN_add(two, one, one); // BIGNUM = 2
	BN_sub(temp, rand, one); // possible temp = rand - 1 
	BN_div(t, rem, temp, two, ctx); // t = temp / 2
	BIGNUM *rminusone = BN_dup(rand); BN_sub(rminusone, rminusone, one);
	// BN_cmp tests for t < newly generated random to be base
	BN_rand(r, size, 0, 1);
	while(BN_cmp(t, r) != -1) {
		// Generates new rand to be tested as base
		BN_rand(r, size, 0, 1);
	}
	// modxf returns BIGNUM corresponding to r^((rand-1)/2) mod p
	BIGNUM *result = BN_new(); result = BN_dup(modxf(t, r, rand, ctx));
	if (BN_cmp(result, rminusone) == 0 || BN_cmp(result, one) == 0) {
		return 1; // either r is a really good liar or rand is prime
	} else {
		return 0; // rand is not prime
	}	
}

int main(int argc, char *argv[])
{
	char num[6];
	time_t start = time(NULL);
	printf("Start: %s", ctime(&start));
	BN_CTX *ctx = BN_CTX_new();
	if (argc == 1) {
		if (fgets(num, 5, stdin) != NULL) {
			sscanf(num, "%d", &size);
		} else {
			printf("%s\n", "Error handling stdin");
		}
	} else if (argc == 2) {
		size = atoi(argv[1]);
	} else {
		printf("%s", "Invalid argument.\nUsage: $>primegen [number]");
	}
//	size = atoi(argv[1]);
	while (1) {
		BIGNUM *rand = BN_new(); // random BIGNUM to test: rand [modulus]
		BN_rand(rand, size, 0, 1); // 1 in BN_rand for 4th arg makes the random number odd
		int i = 0;
		int truecount = 0; // count to keep track of test successes
		for (i;i<10;i++) {
			if (primetest(rand, ctx)) {
				truecount++;
			}
		}
		if (truecount == 10) {
			if (BN_is_prime(rand, 10, NULL, ctx, NULL)) {
				printf("The number below is prime\n");
				printf("%s\n", BN_bn2dec(rand));
				BN_CTX_free(ctx);
				time_t end = time(NULL);
				printf("End: %s", ctime(&end));
				printf("Total time to perform entire program: %.2lf seconds\n", difftime(end,start));
				exit(1); // successful termination of the program. Generated prime WOOO!!!
			} else {
				// your really unlucky if it gets here. this means 
				// it passed the Miller-Rabin test 8 times and still isnt prime
			}
		}
	}


        BN_CTX_free(ctx);
        return 0;
}
