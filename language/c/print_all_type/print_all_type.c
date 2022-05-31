#include "stdio.h"

typedef char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef long s64_;
typedef unsigned long u64_;
typedef long long s64;
typedef unsigned long long u64;

void main() {
	int nData1 = 337;
	int nData2 = -777;
	double dData = 3.141259;
	double dData2 = 314125912345;
	double dData3 = 0.00002;
	char cData = 'X';
	char szData[5] = "ABC";

	int *pnData1 = &nData1;

	printf("부호있는 10진수 정수:%%d %d\n", nData1);
	printf("부호있는 10진수 정수:%%d %d\n", nData2);
	printf("부호없는 10진수 정수:%%u %u\n", nData1);
	printf("부호없는 16진수 정수:%%x %x\n", nData1);
	printf("부호없는 16진수 정수:%%X %X\n", nData1);
	printf("부호없는 08진수 정수:%%o %o\n", nData1);
	printf(" 16진수 포인터값:%%p %p\n", szData);
	printf(" 문자열 출력:%%s %s\n", szData);
	printf(" 문자 출력:%%c %c\n", cData);
	printf(" 문자 출력:%%C %C\n", cData);
	printf(" double값 소수 출력:%%f %f\n", dData);
	printf(" double값 지수 출력:%%e %e\n", dData);
	printf(" double값 지수 출력:%%E %E\n", dData);
	printf(" 값에 따른 f/e 출력:%%g %g\n", dData);
	printf(" 값에 따른 f/e 출력:%%g %g\n", dData2);
	printf(" 값에 따른 f/e 출력:%%G %G\n", dData2);
	printf(" 값에 따른 f/e 출력:%%G %G\n", dData3);

	printf("=============================\n");

	printf("s8 = %zu bytes\n", sizeof(s8));
	printf("u8 = %zu bytes\n", sizeof(u8));
	printf("s16 = %zu bytes\n", sizeof(s16));
	printf("u16 = %zu bytes\n", sizeof(u16));
	printf("s32 = %zu bytes\n", sizeof(s32));
	printf("u32 = %lu bytes\n", sizeof(u32));
	printf("s64_ = %zu bytes\n", sizeof(s64_));
	printf("u64_ = %zu bytes\n", sizeof(u64_));
	printf("s64 = %zu bytes\n", sizeof(s64));
	printf("u64 = %zu bytes\n", sizeof(u64));

	printf("=============================\n");

	printf("u*pnData adrress :  %p \n", *pnData1);
	printf("u*pnData adrress :  %llx \n", *pnData1);
}


