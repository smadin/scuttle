#include "test_scuttle.h"
#include "scuttle.h"
#include <stdio.h>

static char testname[256];
static size_t testname_sz = 256;

int main(int argc, char **argv)
{
    char msgbuf[1024];
    size_t msgbufsz = 1024;
    int failct = 0;

    printf("TEST_COUNT is %u\n", TEST_COUNT);

    for (int i = 0; i < TEST_COUNT; i++)
    {
        int result = (*(tests[i]))(msgbuf, msgbufsz);

        switch(result)
        {
            case 1:
                fprintf(stderr, "Test %s failed:\n\t%s\n", testname, msgbuf);
                failct++;
                break;
            case 0:
            default:
                fprintf(stderr, "Test %s passed\n", testname);
                break;
        }
        msgbuf[0] = '\0';
    }

    printf("*** RESULT:\n\t%d tests\t%d failures\t%0.2f%% pass rate\n", TEST_COUNT, failct, 100.0f * ((float)(TEST_COUNT - failct) / (float)TEST_COUNT));
}

int assert_true_passes(char *msgbuf, size_t bufsz)
{
    strncpy(testname, __func__, testname_sz);
    SASSERT(1 == 1)
    return 0;
}

int assert_false_fails_helper(char *msgbuf, size_t bufsz)
{
    SASSERT(1 == 2)
    return 0;
}

int assert_false_fails(char *msgbuf, size_t bufsz)
{
    strncpy(testname, __func__, testname_sz);
    int r = assert_false_fails_helper(msgbuf, bufsz);
    fprintf(stderr, "\t%s inner result:\n\t\t%s\n", __func__, msgbuf);
    SASSERT(r == 1)
    return 0;
}

int refute_false_passes(char *msgbuf, size_t bufsz)
{
    strncpy(testname, __func__, testname_sz);
    SREFUTE(1 == 2)
    return 0;
}

int refute_true_fails_helper(char *msgbuf, size_t bufsz)
{
    SREFUTE(1 == 1)
    return 0;
}

int refute_true_fails(char *msgbuf, size_t bufsz)
{
    strncpy(testname, __func__, testname_sz);
    int r = refute_true_fails_helper(msgbuf, bufsz);
    fprintf(stderr, "\t%s inner result:\n\t\t%s\n", __func__, msgbuf);
    SASSERT(r == 1)
    return 0;
}

int assert_null_passes(char *msgbuf, size_t bufsz)
{
    strncpy(testname, __func__, testname_sz);
    void *n = NULL;
    SASSERT_NULL(n)
    return 0;
}

int assert_null_fails_helper(char *msgbuf, size_t bufsz)
{
    char *s = "foo";
    SASSERT_NULL(s)
    return 0;
}

int assert_null_fails(char *msgbuf, size_t bufsz)
{
    strncpy(testname, __func__, testname_sz);
    int r = assert_null_fails_helper(msgbuf, bufsz);
    fprintf(stderr, "\t%s inner result:\n\t\t%s\n", __func__, msgbuf);
    SASSERT(r == 1)
    return 0;
}

int refute_null_passes(char *msgbuf, size_t bufsz)
{
    strncpy(testname, __func__, testname_sz);
    char *s = "foo";
    SREFUTE_NULL(s)
    return 0;
}

int refute_null_fails_helper(char *msgbuf, size_t bufsz)
{
    char *s = NULL;
    SREFUTE_NULL(s)
    return 0;
}

int refute_null_fails(char *msgbuf, size_t bufsz)
{
    strncpy(testname, __func__, testname_sz);
    int r = refute_null_fails_helper(msgbuf, bufsz);
    fprintf(stderr, "\t%s inner result:\n\t\t%s\n", __func__, msgbuf);
    SASSERT(r == 1)
    return 0;
}

int assert_equals_passes(char *msgbuf, size_t bufsz)
{
    strncpy(testname, __func__, testname_sz);
    SASSERT_EQ(1, 1)
    return 0;
}

int assert_equals_fails_helper(char *msgbuf, size_t bufsz)
{
    int a = 1, b = 2;
    SASSERT_EQ(a, b)
    return 0;
}

int assert_equals_fails(char *msgbuf, size_t bufsz)
{
    strncpy(testname, __func__, testname_sz);
    int r = assert_equals_fails_helper(msgbuf, bufsz);
    fprintf(stderr, "\t%s inner result:\n\t\t%s\n", __func__, msgbuf);
    SASSERT(r == 1)
    return 0;
}

int refute_equals_passes(char *msgbuf, size_t bufsz)
{
    strncpy(testname, __func__, testname_sz);
    int a = 1, b = 2;
    SREFUTE_EQ(a, b)
    return 0;
}

int refute_equals_fails_helper(char *msgbuf, size_t bufsz)
{
    int a = 1;
    float b = 1.0;
    SREFUTE_EQ(a, b)
    return 0;
}

int refute_equals_fails(char *msgbuf, size_t bufsz)
{
    strncpy(testname, __func__, testname_sz);
    int r = refute_equals_fails_helper(msgbuf, bufsz);
    fprintf(stderr, "\t%s inner result:\n\t\t%s\n", __func__, msgbuf);
    SASSERT(r == 1)
    return 0;
}

int assert_streq_passes(char *msgbuf, size_t bufsz)
{
    strncpy(testname, __func__, testname_sz);
    const char *foo = "foo";
    const char *foo2 = "foo";
    const char *fn = NULL;
    const char *fn2 = NULL;
    SASSERT_STREQ(foo, foo)
    SASSERT_STREQ(foo, foo2)
    SASSERT_STREQ(fn, fn2)
    return 0;
}

int assert_streq_fails_helper(char *msgbuf, size_t bufsz)
{
    const char *a = "foo", *b = "bar", *n = NULL;
    SASSERT_STREQ(a, b)
    SASSERT_STREQ(n, a)
    return 0;
}

int assert_streq_fails(char *msgbuf, size_t bufsz)
{
    strncpy(testname, __func__, testname_sz);
    int r = assert_streq_fails_helper(msgbuf, bufsz);
    fprintf(stderr, "\t%s inner result:\n\t\t%s\n", __func__, msgbuf);
    SASSERT(r == 1)
    return 0;
}

int refute_streq_passes(char *msgbuf, size_t bufsz)
{
    strncpy(testname, __func__, testname_sz);
    const char *s1 = "foo", *s2 = "bar";
    const char *n = NULL;
    SREFUTE_STREQ(s1, s2)
    SREFUTE_STREQ(s1, n)
    SREFUTE_STREQ(n, s2)
    return 0;
}

int refute_streq_fails_helper(char *msgbuf, size_t bufsz)
{
    const char *s1 = "foo", *s2 = "foo";
    SREFUTE_STREQ(s1, s2)
    return 0;
}

int refute_streq_fails(char *msgbuf, size_t bufsz)
{
    strncpy(testname, __func__, testname_sz);
    int r = refute_streq_fails_helper(msgbuf, bufsz);
    fprintf(stderr, "\t%s inner result:\n\t\t%s\n", __func__, msgbuf);
    SASSERT(r == 1)
    return 0;
}

STEST_START(assert_true)
    SASSERT(1)
STEST_END