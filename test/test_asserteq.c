#include "test_asserteq.h"
#include "scuttle.h"
#include <stdio.h>

SSUITE_INIT(scuttle_asserteq)
SSUITE_READY

STEST_SETUP
STEST_SETUP_END

STEST_TEARDOWN
STEST_TEARDOWN_END

STEST_START(asserteq_passes)
    SASSERT_EQ(1, 1)
STEST_END

int stest_asserteq_fails_helper(char *_scuttle_msgbuf, size_t _scuttle_msgbufsz)
{
    int a = 1, b = 2;
    SASSERT_EQ(a, b)
    return 0;
}

STEST_START(asserteq_fails)
    int r = stest_asserteq_fails_helper(_scuttle_msgbuf, _scuttle_msgbufsz);
    int c = strncmp("Assertion failure: expected a (1.00) to equal b (2.00)", _scuttle_msgbuf, _scuttle_msgbufsz);
    SASSERT(r == 1)
    SASSERT(c == 0)
STEST_END

STEST_START(refuteeq_passes)
    int a = 1, b = 2;
    SREFUTE_EQ(a, b)
STEST_END

int refute_equals_fails_helper(char *_scuttle_msgbuf, size_t _scuttle_msgbufsz)
{
    int a = 1;
    float b = 1.0;
    SREFUTE_EQ(a, b)
    return 0;
}

STEST_START(refuteeq_fails)
    int r = refute_equals_fails_helper(_scuttle_msgbuf, _scuttle_msgbufsz);
    int c = strncmp("Assertion failure: expected a (1.00) not to equal b (1.00)", _scuttle_msgbuf, _scuttle_msgbufsz);
    SASSERT(r == 1)
    SASSERT(c == 0)
STEST_END
