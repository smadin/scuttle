#include "foo.h"
#include "test_foo.h"
#include <stdio.h>

SSUITE_INIT(example_foo)
    printf("example_foo suite init\n");
SSUITE_READY

STEST_SETUP
    printf("example_foo test setup\n");
STEST_SETUP_END

STEST_TEARDOWN
    printf("example_foo test teardown\n");
STEST_TEARDOWN_END

STEST_START(foo_returns_fortytwo)
    int i = foo();
    SASSERT(i == 42)
    SREFUTE(i != 42)
    SASSERT_EQ(i, 42)
STEST_END

STEST_START(foo_doesnt_return_sixtynine)
    int i = foo();
    SREFUTE(i == 69)
STEST_END

STEST_START(foostr_sets_buf)
    char buf[10];
    int i = foostr(buf, 10);
    SASSERT(i)
    const char *expected = "foo";
    SASSERT_STREQ(buf, expected)
STEST_END

STEST_START(foostr_doesnt_set_buf)
    char buf[2];
    int i = foostr(buf, 2);
    SREFUTE(i)
    i = foostr(NULL, 10);
    SREFUTE(i)
STEST_END