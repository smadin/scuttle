#include "bar.h"
#include "test_bar.h"
#include <stdio.h>

SSUITE_INIT(example_bar)
    printf("example_bar suite init\n");
SSUITE_READY

STEST_SETUP
    printf("example_bar test setup\n");
STEST_SETUP_END

STEST_TEARDOWN
    printf("example_bar test teardown\n");
STEST_TEARDOWN_END

STEST_START(bar_returns_beef)
    int i = bar();
    SASSERT_EQ(0xBEEF, i)
STEST_END

STEST_START(bar_doesnt_return_dead)
    int i = bar();
    SREFUTE(i == 0xDEAD)
STEST_END
