#include "bar.h"
#include "scuttle.h"
#include "test_example_bar.h"

SSUITE_START(example_bar)

STEST_START(bar_return_true)
    int i = bar();
    SASSERT_EQ(0xBEEF, i)
STEST_END

STEST_START(bar_return_false)
    int i = bar();
    SREFUTE(i == 0xDEAD)
STEST_END

SSUITE_END