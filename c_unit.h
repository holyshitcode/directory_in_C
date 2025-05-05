#ifndef C_UNIT_H
#define C_UNIT_H

#include <setjmp.h>

typedef enum Boolean {
    IsNot, null
} boolean;

extern jmp_buf jump;

int assertEqualString(char *input, char *expected, boolean boolean);
int assertEqualInt(int input, int expected, boolean boolean);
int assertEqualCharLen(char *input, char *expected, boolean boolean);
int assertEqualIntArray(int input[], int inputLen, int expected[], int expectedLen, boolean boolean);
int assertEqualDoubleArray(double input[], double inputLen, double expected[], double expectedLen, boolean boolean);
int assertEqualLongArray(long input[], long inputLen, long expected[], long expectedLen, boolean boolean);
int assertCharContaining(char *input, char expected, boolean boolean);
int assertStrContainingStr(char *input, char *expected, boolean boolean);
int assertIntArrayContainingInt(int input[], int inputLen, int expected, boolean boolean);
int assertDoubleArrayContainingDouble(double input[], int inputLen, double expected, boolean boolean);
int assertLongArrayContainingLong(long input[], int inputLen, long expected, boolean boolean);
int assertMallocIsNotNull(void *input, boolean boolean);
void try(void);

#endif