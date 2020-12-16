#include "test_assert.h"
#include <stdio.h>

SSUITE_INIT(scuttle_assert)
SSUITE_READY

STEST_SETUP
STEST_SETUP_END

STEST_TEARDOWN
STEST_TEARDOWN_END

STEST_START(assert_true_passes)
    SASSERT(1 == 1)
STEST_END

int stest_assert_false_fails_helper(char *_scuttle_msgbuf, const size_t _scuttle_msgbufsz)
{
    SASSERT(1 == 2)
    return 0;
}

STEST_START(assert_false_fails)
    int r = stest_assert_false_fails_helper(_scuttle_msgbuf, _scuttle_msgbufsz);
    int c = strncmp("Assertion failure: expected 1 == 2 to be true", _scuttle_msgbuf, _scuttle_msgbufsz);
    SASSERT(r == 1)
    SASSERT(c == 0)
STEST_END

STEST_START(refute_false_passes)
    SREFUTE(1 == 2)
STEST_END

int stest_refute_true_fails_helper(char *_scuttle_msgbuf, const size_t _scuttle_msgbufsz)
{
    SREFUTE(1 == 1)
    return 0;
}

STEST_START(refute_true_fails)
    int r = stest_refute_true_fails_helper(_scuttle_msgbuf, _scuttle_msgbufsz);
    int c = strncmp("Assertion failure: expected 1 == 1 to be false", _scuttle_msgbuf, _scuttle_msgbufsz);
    SASSERT(r == 1)
    SASSERT(c == 0)
STEST_END