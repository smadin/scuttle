#include "test_assertstreq.h"
#include <stdio.h>

SSUITE_INIT(scuttle_assertstreq)
SSUITE_READY

STEST_SETUP
STEST_SETUP_END

STEST_TEARDOWN
STEST_TEARDOWN_END

STEST_START(assertstreq_passes)
    const char *foo = "foo";
    const char *foo2 = "foo";
    const char *fn = NULL;
    const char *fn2 = NULL;
    SASSERT_STREQ(foo, foo)
    SASSERT_STREQ(foo, foo2)
    SASSERT_STREQ(fn, fn2)
STEST_END

int stest_assertstreq_fails_helper(char *_scuttle_msgbuf, size_t _scuttle_msgbufsz)
{
    const char *a = "foo", *b = "bar", *n = NULL;
    SASSERT_STREQ(a, b)
    SASSERT_STREQ(n, a)
    return 0;
}

STEST_START(assertstreq_fails)
    int r = stest_assertstreq_fails_helper(_scuttle_msgbuf, _scuttle_msgbufsz);
    int c = strncmp("Assertion failure: expected a (\"foo\") to equal b (\"bar\")", _scuttle_msgbuf, _scuttle_msgbufsz);
    SASSERT(r == 1)
    SASSERT(c == 0)
STEST_END

STEST_START(refutestreq_passes)
    const char *s1 = "foo", *s2 = "bar";
    const char *n = NULL;
    SREFUTE_STREQ(s1, s2)
    SREFUTE_STREQ(s1, n)
    SREFUTE_STREQ(n, s2)
STEST_END

int stest_refutestreq_fails_helper(char *_scuttle_msgbuf, size_t _scuttle_msgbufsz)
{
    const char *s1 = "foo", *s2 = "foo";
    SREFUTE_STREQ(s1, s2)
    return 0;
}

STEST_START(refutestreq_fails)
    int r = stest_refutestreq_fails_helper(_scuttle_msgbuf, _scuttle_msgbufsz);
    int c = strncmp("Assertion failure: expected s1 and s2 not to be the same pointer", _scuttle_msgbuf, _scuttle_msgbufsz);
    SASSERT(r == 1)
    SASSERT(c == 0)
STEST_END
