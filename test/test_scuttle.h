#ifndef _TEST_SCUTTLE
#define _TEST_SCUTTLE

#include <stddef.h>

typedef int (*TestFunc)(char *msgbuf, size_t bufsz);

int assert_true_passes(char *msgbuf, size_t bufsz);
int assert_false_fails(char *msgbuf, size_t bufsz);
int refute_false_passes(char *msgbuf, size_t bufsz);
int refute_true_fails(char *msgbuf, size_t bufsz);
int assert_null_passes(char *msgbuf, size_t bufsz);
int assert_null_fails(char *msgbuf, size_t bufsz);
int refute_null_passes(char *msgbuf, size_t bufsz);
int refute_null_fails(char *msgbuf, size_t bufsz);
int assert_equals_passes(char *msgbuf, size_t bufsz);
int assert_equals_fails(char *msgbuf, size_t bufsz);
int refute_equals_passes(char *msgbuf, size_t bufsz);
int refute_equals_fails(char *msgbuf, size_t bufsz);
int assert_streq_passes(char *msgbuf, size_t bufsz);
int assert_streq_fails(char *msgbuf, size_t bufsz);
int refute_streq_passes(char *msgbuf, size_t bufsz);
int refute_streq_fails(char *msgbuf, size_t bufsz);
int _scuttle_test_assert_true(char *msgbuf, size_t bufsz);

TestFunc tests[] = {
    assert_true_passes,
    assert_false_fails,
    refute_false_passes,
    refute_true_fails,
    assert_null_passes,
    assert_null_fails,
    refute_null_passes,
    refute_null_fails,
    assert_equals_passes,
    assert_equals_fails,
    refute_equals_passes,
    refute_equals_fails,
    assert_streq_passes,
    assert_streq_fails,
    refute_streq_passes,
    refute_streq_fails,
    _scuttle_test_assert_true,
};
unsigned int TEST_COUNT = sizeof(tests) / sizeof(TestFunc);

#endif /* _TEST_SCUTTLE */