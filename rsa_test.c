#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "rsa.h"


void modTest() {
    assert(mod(1, 2) == 1);
    assert(mod(5, 2) == 1);
    assert(mod(ULLONG_MAX-1, ULLONG_MAX) == ULLONG_MAX-1);
    assert(mod(ULLONG_MAX, ULLONG_MAX-1) == 1);
    assert(mod(ULLONG_MAX, 2) == 1);
    assert(mod(345800, 3600) == 345800 % 3600);
}

void modAddTest() {
    assert(ModAdd(1, 2, '+', 4) == 3);
    assert(ModAdd(3, 4, '+', 2) == 1);
    assert(ModAdd(3, 4, '-', 5) == 4);
    assert(ModAdd(ULLONG_MAX-1, 1, '+', ULLONG_MAX) == 0);
    assert(ModAdd(ULLONG_MAX, ULLONG_MAX-2, '+', ULLONG_MAX) == ULLONG_MAX-2);
    assert(ModAdd(ULLONG_MAX-1, ULLONG_MAX-1, '+', ULLONG_MAX) == ULLONG_MAX-2);
}

void modMulTest() {
    /* test case[i] format: {x, y, n, answer} */
    llint test_case[][4] = {
        {3, 5, 15, 0}, {3, 6, 15, 3}, {11, 11, 100, 21},
        {ULL_MAX_BIT, 2, ULLONG_MAX, ModAdd(ULL_MAX_BIT, ULL_MAX_BIT, '+', ULLONG_MAX)},
        {ULLONG_MAX-1, 3, ULLONG_MAX,
        ModAdd(
            ModAdd(
                ULLONG_MAX-1, ULLONG_MAX-1, '+', ULLONG_MAX
            ), 
            ULLONG_MAX-1, '+', ULLONG_MAX
        )},
        {ULL_MAX_BIT >> 1, 4, ULLONG_MAX, 1}
    };
    int len = sizeof(test_case) / sizeof(llint) / 4;

    for(int i = 0; i < len; ++i) {
        int res = ModMul(test_case[i][0], test_case[i][1], test_case[i][2]);
        assert(res == test_case[i][3] || !printf("case number %d is wrong\n", i));
    }
}

void modPowTest() {
    /* test case[i] format: {base, exp, n, answer} */
    llint test_case[][4] = {
        {4, 2, 10, 6}, {5, 3, 120, 5}
    };
    int len = sizeof(test_case) / sizeof(llint) / 4;

    for(int i = 0; i < len; ++i) {
        int res = ModPow(test_case[i][0], test_case[i][1], test_case[i][2]);
        assert(res == test_case[i][3] || !printf("case number %d is wrong\n", i));
    }
}

void RNGTest() {
    uint seed = time(NULL);

    InitWELLRNG512a(&seed);
    for(int i = 0; i < 50; ++i) {
        printf("%lf\n", WELLRNG512a());
    }
}

void isPrimeTest() {
    llint repeat = 8;
    /* test case format: testNum, anser */
    llint test_case[][2] = {{2, 1}, {3, 1}, {5, 1}, {11, 1}, {19, 1}, {26, 0}};

    int len = sizeof(test_case) / sizeof(llint) / 2;

    for(int i = 0; i < len; ++i) {
        assert(IsPrime(test_case[i][0], repeat) == test_case[i][1]);
    }
}

#ifdef TEST

int main() {
    modTest();
    modAddTest();
    modMulTest();
    modPowTest();
    isPrimeTest();
    printf("Test complete");
}

#endif