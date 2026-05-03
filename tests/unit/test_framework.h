#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

/*
 * Minimal test framework — no external dependencies.
 * Each test file defines test functions and registers them via TEST_SUITE.
 */

#include <stdio.h>
#include <string.h>

static int _tests_run    = 0;
static int _tests_passed = 0;
static int _tests_failed = 0;

#define ASSERT_EQ(a, b) do { \
    _tests_run++; \
    if ((a) == (b)) { \
        _tests_passed++; \
    } else { \
        _tests_failed++; \
        fprintf(stderr, "  FAIL %s:%d: expected %ld, got %ld\n", \
                __FILE__, __LINE__, (long)(b), (long)(a)); \
    } \
} while (0)

#define ASSERT_STR_EQ(a, b) do { \
    _tests_run++; \
    if (strcmp((a), (b)) == 0) { \
        _tests_passed++; \
    } else { \
        _tests_failed++; \
        fprintf(stderr, "  FAIL %s:%d: expected \"%s\", got \"%s\"\n", \
                __FILE__, __LINE__, (b), (a)); \
    } \
} while (0)

#define ASSERT_EQ_INT(a, b)  ASSERT_EQ((long)(a), (long)(b))

#define RUN_TEST(fn) do { \
    printf("  %-40s", #fn); \
    int _before = _tests_failed; \
    fn(); \
    if (_tests_failed == _before) printf("OK\n"); \
} while (0)

#define TEST_SUMMARY() do { \
    printf("\n%d tests, %d passed, %d failed\n", \
           _tests_run, _tests_passed, _tests_failed); \
} while (0)

#define TEST_EXIT() return (_tests_failed > 0) ? 1 : 0

#endif /* TEST_FRAMEWORK_H */
