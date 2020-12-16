#include "test_assertnull.h"
#include "scuttle.h"
#include <stdio.h>

SSUITE_INIT(scuttle_assertnull)
SSUITE_READY

STEST_SETUP
STEST_SETUP_END

STEST_TEARDOWN
STEST_TEARDOWN_END

STEST_START(assertnull_passes)
    char *foo = NULL;
    SASSERT_NULL(foo)
STEST_END

int stest_assertnull_fails_helper(char *_scuttle_msgbuf, const size_t _scuttle_msgbufsz)
{
    char *foo = "asdf";
    SASSERT_NULL(foo)
    return 0;
}

STEST_START(assertnull_fails)
    int r = stest_assertnull_fails_helper(_scuttle_msgbuf, _scuttle_msgbufsz);
    int c = strncmp("Assertion failure: expected foo (foo) to be NULL", _scuttle_msgbuf, _scuttle_msgbufsz);
    SASSERT(r == 1)
    SASSERT(c == 0)
STEST_END

STEST_START(refutenull_passes)
    char *foo = "asdf";
    SREFUTE_NULL(foo)
STEST_END

int stest_refutenull_fails_helper(char *_scuttle_msgbuf, const size_t _scuttle_msgbufsz)
{
    char *foo = NULL;
    SREFUTE_NULL(foo)
    return 0;
}

STEST_START(refutenull_fails)
    int r = stest_refutenull_fails_helper(_scuttle_msgbuf, _scuttle_msgbufsz);
    int c = strncmp("Assertion failure: expected foo not to be NULL", _scuttle_msgbuf, _scuttle_msgbufsz);
    SASSERT(r == 1)
    SASSERT(c == 0)
STEST_END
