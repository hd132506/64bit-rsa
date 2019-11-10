/* ***************************************************************************** */
/*
  1. miniRSA헤더 파일
    헤더파일에는 miniRSA관련 인터페이스 및 WELL Random number generator 코드가 포함되어 있습니다.
    안전한 난수 생성을 위한 WELL Random number generator를 사용합니다.
    WELL Random number generator의 관련된 내용은 발표 논문 및 인터넷에서 쉽게 찾을 수 있으며,
    소스코드 또한 발표논문에서 쉽게 얻을 수 있습니다. 혹시 관심있는 학생은 찾아보길 바랍니다.
    
    사용 방법은 다음과 같습니다.
    mini RSA 알고리즘 구현 중 WELLRNG512a()를 통해서 난수를 생성합니다.
    또한 InitWELLRNG512a(uint *seed) 함수를 통해 난수 생성 알고리즘의 시드를 설정합니다.
*/
/* Copyright:      Francois Panneton and Pierre L'Ecuyer, University of Montreal */
/*                 Makoto Matsumoto, Hiroshima University                        */
/* Notice:         This code can be used freely for personal, academic,          */
/*                 or non-commercial purposes. For commercial purposes,          */
/*                 please contact P. L'Ecuyer at: lecuyer@iro.UMontreal.ca       */
/* ***************************************************************************** */

// WELL Random number generator 관련 매크로
#define W	32
#define R	16
#define P	0
#define M1	13
#define M2	9
#define M3	5

#define MAT0POS(t,v)	(v^(v>>t))
#define MAT0NEG(t,v)	(v^(v<<(-(t))))

#define MAT3NEG(t,v)	(v<<(-(t)))
#define MAT4NEG(t,b,v)	(v^((v<<(-(t)))&b))

#define V0		STATE[state_i]
#define VM1		STATE[(state_i+M1) & 0x0000000fU]
#define VM2		STATE[(state_i+M2) & 0x0000000fU]
#define VM3		STATE[(state_i+M3) & 0x0000000fU]
#define VRm1	STATE[(state_i+15) & 0x0000000fU]
#define VRm2	STATE[(state_i+14) & 0x0000000fU]
#define newV0	STATE[(state_i+15) & 0x0000000fU]
#define newV1	STATE[state_i]
#define newVRm1	STATE[(state_i+14) & 0x0000000fU]

#define FACT	2.32830643653869628906e-10

#define RND_MAX	0xffffffff
#define RND_MIN	0x0fffffff
#define FALSE	0
#define TRUE	1

#define ULL_MAX_BIT 0x8000000000000000

/* Decides what to compile (rsa_test.c/rsa.c)*/
// #define TEST

// mini RSA 관련 타입
typedef unsigned char bool;
typedef unsigned char byte;
typedef unsigned int  uint;
typedef unsigned long long int llint;

// WELL Random number generator 관련 전역 변수
static unsigned int state_i = 0;
static unsigned int STATE[R];
static unsigned int z0, z1, z2;

// WELL Random number generator 관련 함수
void InitWELLRNG512a(uint *init);
double WELLRNG512a(void);

llint mod(llint a, llint n);
llint quotient(llint a, llint n);
llint randomWithRange(llint from, llint to);

// mini RSA 관련 인터페이스(구현해야 합니다.)
llint ModAdd(llint a, llint b, byte op, llint n);
llint ModMul(llint x, llint y, llint n);
llint ModPow(llint base, llint exp, llint n);
bool IsPrime(llint testNum, llint repeat);
llint GCD(llint a, llint b);
llint ModInv(llint a, llint m);
void miniRSAKeygen(llint *p, llint *q, llint *e, llint *d, llint *n);
llint miniRSA(llint data, llint key, llint n);

