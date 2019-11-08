/*
 * @file    rsa.c
 * @author  작성자 이름 / 학번
 * @date    작성 일자
 * @brief   mini RSA implementation code
 * @details 세부 설명
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "rsa.h"

llint p, q, e, d, n;


// 난수 생성을 위한 초기화 함수
void InitWELLRNG512a(uint *init) {
	int j;
	state_i = 0;
	for (j = 0; j < R; j++) STATE[j] = init[j];
}

// 난수 생성 함수
double WELLRNG512a(void) {
	z0	= VRm1;
	z1	= MAT0NEG(-16, V0) ^ MAT0NEG(-15, VM1);
	z2	= MAT0POS(11, VM2);
	newV1 = z1 ^ z2;
	newV0 = MAT0NEG(-2, z0) ^ MAT0NEG(-18, z1) ^ MAT3NEG(-28, z2) ^ MAT4NEG(-5, 0xda442d24U, newV1);
	state_i = (state_i + 15) & 0x0000000fU;
	return ((double) STATE[state_i]) * FACT;
}

/* 
 * Modular operation
 * which computes a mod n 
 */
llint mod(llint a, llint n) {
    llint shifter = n;

    /* 
    Subtract the multiple of n first(kind of binary search)
    a would be reduced fast in any case
    */
    while(a > (shifter << 1) && shifter < ULL_MAX_BIT) shifter <<= 1;
    while(shifter >= n && a >= n) {
        if(a >= shifter)  a -= shifter;
        shifter >>= 1;
    }
    return a;
}

/*
 * @brief     모듈러 덧셈 연산을 하는 함수.
 * @param     llint a     : 피연산자1.
 * @param     llint b     : 피연산자2.
 * @param     byte op    : +, - 연산자.
 * @param     llint n      : 모듈러 값.
 * @return    llint result : 피연산자의 덧셈에 대한 모듈러 연산 값. (a op b) mod n
 * @todo      모듈러 값과 오버플로우 상황을 고려하여 작성한다.
 */
llint ModAdd(llint a, llint b, byte op, llint n) {
    llint result;
    a = mod(a, n);
    b = (op == '+') ? mod(b, n) : n - mod(b, n);
    result = a + b;

    /* Overflow control */
    if(result < a || result < b) 
        result += 1 + mod(ULLONG_MAX, n);

    result = mod(result, n);

    return result;
}

/*
 * @brief      모듈러 곱셈 연산을 하는 함수.
 * @param      llint x       : 피연산자1.
 * @param      llint y       : 피연산자2.
 * @param      llint n       : 모듈러 값.
 * @return     llint result  : 피연산자의 곱셈에 대한 모듈러 연산 값. (a x b) mod n
 * @todo       모듈러 값과 오버플로우 상황을 고려하여 작성한다.
 */
llint ModMul(llint x, llint y, llint n) {
    llint result = 0;
    x = mod(x, n);
    y = mod(y, n);

    while(y > 0) {
        if(y & 1) /* x is odd */
            result = ModAdd(result, x, '+', n);
        
        /* Controlling overflow*/
        x = (x & ULL_MAX_BIT) ? ModAdd(x, x, '+', n) : x << 1;
        y >>= 1;
    }

    return result;
}

/*
 * @brief      모듈러 거듭제곱 연산을 하는 함수.
 * @param      llint base   : 피연산자1.
 * @param      llint exp    : 피연산자2.
 * @param      llint n      : 모듈러 값.
 * @return     llint result : 피연산자의 연산에 대한 모듈러 연산 값. (base ^ exp) mod n
 * @todo       모듈러 값과 오버플로우 상황을 고려하여 작성한다.
               'square and multiply' 알고리즘을 사용하여 작성한다.
 */
llint ModPow(llint base, llint exp, llint n) {
    /* Base condition */
    if(exp == 1) return base;

    llint result = 1, sqrt;
    if(exp & 1) result = base; /* When exp is odd */

    /* Divide and conquer(Also controlling overflow by ModMul()) */
    sqrt = ModPow(base, exp >> 1, n);
    result = ModMul(ModMul(sqrt, sqrt, n), result, n);

    return result;
}

/*
 * @brief      입력된 수가 소수인지 입력된 횟수만큼 반복하여 검증하는 함수.
 * @param      llint testNum   : 임의 생성된 홀수.
 * @param      llint repeat    : 판단함수의 반복횟수.
 * @return     llint result    : 판단 결과에 따른 TRUE, FALSE 값.
 * @todo       Miller-Rabin 소수 판별법과 같은 확률적인 방법을 사용하여,
               이론적으로 4N(99.99%) 이상 되는 값을 선택하도록 한다. 
 */
bool IsPrime(llint testNum, llint repeat) {
    bool result = 0;
    llint s = 0, d = 0; /* n-1 = 2^s*d */
    do testNum >>= 1, ++s;
    while(!(testNum & 1));
    d = testNum;

    

    return result;
}

/*
 * @brief       모듈러 역 값을 계산하는 함수.
 * @param       llint a      : 피연산자1.
 * @param       llint m      : 모듈러 값.
 * @return      llint result : 피연산자의 모듈러 역수 값.
 * @todo        확장 유클리드 알고리즘을 사용하여 작성하도록 한다.
 */
llint ModInv(llint a, llint m) {
    llint result;
    return result;
}

/*
 * @brief     RSA 키를 생성하는 함수.
 * @param     llint *p   : 소수 p.
 * @param     llint *q   : 소수 q.
 * @param     llint *e   : 공개키 값.
 * @param     llint *d   : 개인키 값.
 * @param     llint *n   : 모듈러 n 값.
 * @return    void
 * @todo      과제 안내 문서의 제한사항을 참고하여 작성한다.
 */
void miniRSAKeygen(llint *p, llint *q, llint *e, llint *d, llint *n) {
}

/*
 * @brief     RSA 암복호화를 진행하는 함수.
 * @param     llint data   : 키 값.
 * @param     llint key    : 키 값.
 * @param     llint n      : 모듈러 n 값.
 * @return    llint result : 암복호화에 결과값
 * @todo      과제 안내 문서의 제한사항을 참고하여 작성한다.
 */
llint miniRSA(llint data, llint key, llint n) {
    llint result;
    return result;
}

llint GCD(llint a, llint b) {
    llint prev_a;

    while(b != 0) {
        printf("GCD(%lld, %lld)\n", a, b);
        prev_a = a;
        a = b;
        while(prev_a >= b) prev_a -= b;
        b = prev_a;
    }
    printf("GCD(%lld, %lld)\n\n", a, b);
    return a;
}

#ifndef TEST

int main(int argc, char* argv[]) {
    byte plain_text[4] = {0x12, 0x34, 0x56, 0x78};
    llint plain_data, encrpyted_data, decrpyted_data;
    uint seed = time(NULL);

    memcpy(&plain_data, plain_text, 4);

    // 난수 생성기 시드값 설정
    seed = time(NULL);
    InitWELLRNG512a(&seed);

    // RSA 키 생성
    miniRSAKeygen(&p, &q, &e, &d, &n);
    printf("0. Key generation is Success!\n ");
    printf("p : %llu\n q : %llu\n e : %llu\n d : %llu\n N : %llu\n\n", p, q, e, d, n);

    // RSA 암호화 테스트
    encrpyted_data = miniRSA(plain_data, e, n);
    printf("1. plain text : %llu\n", plain_data);    
    printf("2. encrypted plain text : %llu\n\n", encrpyted_data);

    // RSA 복호화 테스트
    decrpyted_data = miniRSA(encrpyted_data, d, n);
    printf("3. cipher text : %llu\n", encrpyted_data);
    printf("4. Decrypted plain text : %llu\n\n", decrpyted_data);

    // 결과 출력
    printf("RSA Decryption: %s\n", (decrpyted_data == plain_data) ? "SUCCESS!" : "FAILURE!");

    return 0;
}

#endif