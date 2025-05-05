#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include "c_unit.h"

jmp_buf jump;

int assertEqualString(char *input, char *expected, boolean boolean) {
    if (boolean == IsNot) {
        if (strcmp(input, expected) == 0) {
            printf("❌ FAIL: Expected \"%s\" but got \"%s\"\n", expected, input);
            return 0;
        }
        printf("✅ PASS: \"%s\" != \"%s\"\n", input, expected);
        return 1;
    }
    if (strcmp(input, expected) == 0) {
        printf("✅ PASS: \"%s\" == \"%s\"\n", input, expected);
        return 1;
    } else {
        printf("❌ FAIL: Expected \"%s\" but got \"%s\"\n", expected, input);
        return 0;
    }
}

int assertEqualInt(int input, int expected, boolean boolean) {
    if (boolean == IsNot) {
        if (input == expected) {
            printf("❌ FAIL: Expected %d to be different, but got %d\n", expected, input);
            return 0;
        }
        printf("✅ PASS: %d != %d\n", input, expected);
        return 1;
    }
    if (input != expected) {
        printf("❌ FAIL: Expected %d but got %d\n", expected, input);
        return 0;
    }
    printf("✅ PASS: %d == %d\n", input, expected);
    return 1;
}

int assertEqualCharLen(char *input, char *expected, boolean boolean) {
    if (input == NULL || expected == NULL) {
        if (boolean == IsNot) {
            printf("✅ PASS: One of the inputs is NULL as expected\n");
            return 1;
        }
        printf("❌ FAIL: One of the inputs is NULL\n");
        return 0;
    }

    int inputLen = strlen(input);
    int expectedLen = strlen(expected);

    if (boolean == IsNot) {
        if (inputLen == expectedLen) {
            printf("❌ FAIL: Expected different lengths but both are %d\n", inputLen);
            return 0;
        }
        printf("✅ PASS: Length %d != %d\n", inputLen, expectedLen);
        return 1;
    }

    if (inputLen != expectedLen) {
        printf("❌ FAIL: Expected length %d but got %d\n", expectedLen, inputLen);
        return 0;
    }

    printf("✅ PASS: Length %d == %d\n", inputLen, expectedLen);
    return 1;
}

int assertEqualIntArray(int input[], int inputLen, int expected[], int expectedLen, boolean boolean) {
    if (input == NULL || expected == NULL) {
        printf("❌ FAIL: One of the input arrays is NULL\n");
        return 0;
    }

    if (inputLen != expectedLen) {
        printf("❌ FAIL: Expected length %d but got %d\n", expectedLen, inputLen);
        return 0;
    }

    for (int i = 0; i < inputLen; i++) {
        if (input[i] != expected[i]) {
            printf("❌ FAIL: Arrays differ at index %d (expected %d, got %d)\n", i, expected[i], input[i]);
            return 0;
        }
    }

    if (boolean == IsNot) {
        printf("❌ FAIL: Expected arrays to be different, but they are identical\n");
        return 0;
    }

    printf("✅ PASS: Arrays are identical (length %d)\n", inputLen);
    return 1;
}

int assertEqualDoubleArray(double input[], double inputLen, double expected[], double expectedLen, boolean boolean) {
    if (input == NULL || expected == NULL) {
        printf("❌ FAIL: One of the input arrays is NULL\n");
        return 0;
    }

    if (inputLen != expectedLen) {
        printf("❌ FAIL: Expected length %f but got %f\n", expectedLen, inputLen);
        return 0;
    }

    for (int i = 0; i < inputLen; i++) {
        if (input[i] != expected[i]) {
            printf("❌ FAIL: Arrays differ at index %d (expected %f, got %f)\n", i, expected[i], input[i]);
            return 0;
        }
    }

    if (boolean == IsNot) {
        printf("❌ FAIL: Expected arrays to be different, but they are identical\n");
        return 0;
    }

    printf("✅ PASS: Arrays are identical (length %f)\n", inputLen);
    return 1;
}

int assertEqualLongArray(long input[], long inputLen, long expected[], long expectedLen, boolean boolean) {
    if (input == NULL || expected == NULL) {
        printf("❌ FAIL: One of the input arrays is NULL\n");
        return 0;
    }

    if (inputLen != expectedLen) {
        printf("❌ FAIL: Expected length %ld but got %ld\n", expectedLen, inputLen);
        return 0;
    }

    for (int i = 0; i < inputLen; i++) {
        if (input[i] != expected[i]) {
            printf("❌ FAIL: Arrays differ at index %d (expected %ld, got %ld)\n", i, expected[i], input[i]);
            return 0;
        }
    }

    if (boolean == IsNot) {
        printf("❌ FAIL: Expected arrays to be different, but they are identical\n");
        return 0;
    }

    printf("✅ PASS: Arrays are identical (length %ld)\n", inputLen);
    return 1;
}

int assertCharContaining(char *input, char expected, boolean boolean) {
    if (input == NULL) {
        printf("❌ FAIL: Input string is NULL\n");
        return 0;
    }

    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] == expected) {
            if (boolean == IsNot) {
                printf("❌ FAIL: Expected %c to NOT be in the string, but found at index %d\n", expected, i);
                return 0;
            }
            printf("✅ PASS: Expected %c is in index %d\n", expected, i);
            return 1;
        }
    }

    if (boolean == IsNot) {
        printf("✅ PASS: Expected %c is not in the string\n", expected);
        return 1;
    }

    printf("❌ FAIL: Expected char %c doesn't exist in the string\n", expected);
    return 0;
}

int assertStrContainingStr(char *input, char *expected, boolean boolean) {
    if (input == NULL || expected == NULL) {
        printf("❌ FAIL: One of the inputs is NULL\n");
        return 0;
    }

    if (strstr(input, expected)) {
        if (boolean == IsNot) {
            printf("❌ FAIL: Expected string '%s' found, but should not exist\n", expected);
            return 0;
        }
        printf("✅ PASS: Expected string '%s' found\n", expected);
        return 1;
    }

    if (boolean == IsNot) {
        printf("✅ PASS: Expected string '%s' does not exist in input\n", expected);
        return 1;
    }

    printf("❌ FAIL: Expected string '%s' not found\n", expected);
    return 0;
}

int assertIntArrayContainingInt(int input[], int inputLen, int expected, boolean boolean) {
    for (int i = 0; i < inputLen; i++) {
        if (input[i] == expected) {
            if (boolean == IsNot) {
                printf("❌ FAIL: Expected Int %d found at index %d, but it should not exist\n", expected, i);
                return 0;
            }
            printf("✅ PASS: Expected Int %d found at index %d\n", expected, i);
            return 1;
        }
    }

    if (boolean == IsNot) {
        printf("✅ PASS: Expected Int %d is not in array\n", expected);
        return 1;
    }

    printf("❌ FAIL: Expected %d not in Array\n", expected);
    return 0;
}

int assertDoubleArrayContainingDouble(double input[], int inputLen, double expected, boolean boolean) {
    for (int i = 0; i < inputLen; i++) {
        if (input[i] == expected) {
            if (boolean == IsNot) {
                printf("❌ FAIL: Expected %f found at index %d, but it should not exist\n", expected, i);
                return 0;
            }
            printf("✅ PASS: Expected %f found at index %d\n", expected, i);
            return 1;
        }
    }

    if (boolean == IsNot) {
        printf("✅ PASS: Expected %f is not in array\n", expected);
        return 1;
    }

    printf("❌ FAIL: Expected %f not in Array\n", expected);
    return 0;
}

int assertLongArrayContainingLong(long input[], int inputLen, long expected, boolean boolean) {
    for (int i = 0; i < inputLen; i++) {
        if (input[i] == expected) {
            if (boolean == IsNot) {
                printf("❌ FAIL: Expected %ld found at index %d, but it should not exist\n", expected, i);
                return 0;
            }
            printf("✅ PASS: Expected %ld found at index %d\n", expected, i);
            return 1;
        }
    }

    if (boolean == IsNot) {
        printf("✅ PASS: Expected %ld is not in array\n", expected);
        return 1;
    }

    printf("❌ FAIL: Expected %ld not in Array\n", expected);
    return 0;
}

int assertMallocIsNotNull(void *input, boolean boolean) {
    if (input == NULL) {
        if (boolean == IsNot) {
            printf("✅ PASS: Expected Location is NULL (as expected)\n");
            return 1;
        }
        printf("❌ FAIL: Memory Allocation has failed (NULL pointer)\n");
        return 0;
    }
    if (boolean == IsNot) {
        printf("✅ PASS: Expected Location is not NULL (as expected)\n");
        return 1;
    }
    printf("✅ PASS: Expected Location %p exists\n", input);
    return 1;
}

void try() {
    printf("🚨 에러 발생!\n");
    longjmp(jump, 1);
}