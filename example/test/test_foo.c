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
    SASSERT_EQ(42, i)
STEST_END

STEST_START(foo_doesnt_return_sixtynine)
    int i = foo();
    SREFUTE(i == 69)
STEST_END
