#include <stdio.h>
#include <stdint.h>
#include "test_example_foo.h"
// #include "test_example_bar.h"

char _scuttle_testname[256];
const size_t _scuttle_testname_sz = 256;

int main(int argc, char **argv)
{
    Scuttle_Suite suites[] = {
        _scuttle_suite_example_foo,
        // _scuttle_suite_example_bar,
    };
    const size_t num_suites = 1;

    char test_msgbuf[256];
    const size_t test_msgbufsz = 256;

    size_t suitect = 0, suite_failct = 0;

    for (size_t i = 0; i < num_suites; i++)
    {
        Scuttle_Suite *suitep = suites + i;
        printf("Test suite %s:\n", suitep->name);
        suitect++;
        size_t failct = 0;
        uint8_t suite_fail = 0;
        for (size_t j = 0; j < suitep->num_tests; j++)
        {
            Scuttle_Test *testp = suitep->tests + j;
            short result = (*(testp->test_fn))(test_msgbuf, test_msgbufsz);
            if (result)
            {
                fprintf(stderr, "Test %s:%s failed:\n\t%s\n", suitep->name, testp->name, test_msgbuf);
                failct++;
                if (!suite_fail)
                {
                    suite_fail = 1;
                    suite_failct++;
                }
            }
            test_msgbuf[0] = '\0';
        }

        printf("*** Suite %s: %lu / %lu tests passed.\n", suite_fail ? "failed" : "passed", (suitep->num_tests - failct), suitep->num_tests);
    }

    printf("*** %lu / %lu suites passed\n", (suitect - suite_failct), suitect);
    return 0;
}