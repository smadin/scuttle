#!/usr/bin/bash

# scuttle assumes it is being invoked from the root directory of the project,
# and that test source files are in test/ or the directory specified on the command line.

# TODO
# - check test dir
# - parse test source files
# - build suites of test cases
# - generate headers per suite
# - generate harness
# - generate testdir Makefile!
# - build and run harness

# syntax: bash scuttle.sh [<testdir>]
# if <testdir> not provided, assume 'test'

function scuttle() {
    function printUsage() {
        echo "Scuttle: simple C unit tests"
        echo "Usage:"
        echo "Run Scuttle from the root of your project tree. Scuttle expects to find headers in ./include and object files in ./obj."
        echo "scuttle.sh [-h] [testDir]"
        echo "----------"
        echo "-h: print this message"
        echo "testDir: subdirectory of project tree where test suites are located, or 'test' by default"
    }

    function readSuiteNames() {
        suiteFileNames=$(ls $testDir/*.c | grep test_ | grep -v _gen)
        suiteNames=$(for fn in $suiteFileNames; do basename $fn | sed -e 's/test_//' -e 's/\.c//'; done)
    }

    function extractTestNames() {
        suiteFileName="test_$suiteName.c"
        testNames=$(grep STEST_START "$testDir/$suiteFileName" | sed -e 's/STEST_START(//' -e 's/)//')
    }

    function writeSuiteHeader() {
        suiteCaps=$(echo "$suiteName" | tr '[:lower:]' '[:upper:]')
        suiteHeader="$testDir/test_$suiteName.h"
        # test suite header
        cat <<\EOF |
#ifndef _SCUTTLE_TEST_<<SUITENAME>>_H
#define _SCUTTLE_TEST_<<SUITENAME>>_H

#include "scuttle.h"

void _scuttle_suite_init_<<projname>>_<<suitename>>();
void _scuttle_suite_test_setup_<<projname>>_<<suitename>>();
void _scuttle_suite_test_teardown_<<projname>>_<<suitename>>();

EOF
        sed -e "s/<<projname>>/$projName/" -e "s/<<SUITENAME>>/$suiteCaps/" -e "s/<<suitename>>/$suiteName/" > $suiteHeader
        for testName in $testNames; do
            echo "int _scuttle_test_${testName}(char *_scuttle_msgbuf, size_t _scuttle_msgbufsz);" >> $suiteHeader
        done

        cat <<\EOF |

extern Scuttle_Test _scuttle_suite_tests_<<projname>>_<<suitename>>[];
extern Scuttle_Suite _scuttle_suite_<<projname>>_<<suitename>>;

#endif /* _SCUTTLE_TEST_<<SUITENAME>>_H */
EOF
        sed -e "s/<<projname>>/$projName/" -e "s/<<SUITENAME>>/$suiteCaps/" -e "s/<<suitename>>/$suiteName/" >> $suiteHeader
    }

    function writeSuiteData() {
        suiteData="$testDir/test_${suiteName}_gen.c"
        # test suite data
        cat <<\EOF |
#include "test_<<suitename>>.h"

Scuttle_Test _scuttle_suite_tests_<<projname>>_<<suitename>>[] = {
EOF
        sed -e "s/<<projname>>/$projName/" -e "s/<<suitename>>/$suiteName/" > $suiteData
        for testName in $testNames; do
            cat <<\EOF |
    {
        "<<testname>>",
        &_scuttle_test_<<testname>>,
    },
EOF
            sed -e "s/<<testname>>/$testName/" >> $suiteData
        done

        cat <<\EOF |
};

Scuttle_Suite _scuttle_suite_<<projname>>_<<suitename>> = {
    "<<projname>>_<<suitename>>",
    NULL, /* setup -- assigned in init */
    NULL, /* teardown -- assigned in init */
    _scuttle_suite_tests_<<projname>>_<<suitename>>,
    <<numtests>>,
};
EOF
        sed -e "s/<<projname>>/$projName/" -e "s/<<suitename>>/$suiteName/" -e "s/<<numtests>>/$numTests/" >> $suiteData
    }

    function writeHarness() {
        testHarness="$testDir/test_$projName.c"
        # test harness
        cat <<\EOF > $testHarness
#include <stdio.h>
#include <stdint.h>
EOF
        for suiteName in $suiteNames; do
            echo "#include \"test_$suiteName.h\"" >> $testHarness
        done
        cat <<\EOF >> $testHarness

int main(int argc, char **argv)
{
    Scuttle_Suite suites[] = {
EOF
        for suiteName in $suiteNames; do
            echo "        _scuttle_suite_${projName}_${suiteName}," >> $testHarness
        done
        echo "    };" >> $testHarness
        echo "    const size_t num_suites = $numSuites;" >> $testHarness
        cat <<\EOF >> $testHarness

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
EOF
    }

    function writeMakefile() {
        makefile="$testDir/Makefile"


        # Makefile
        cat <<\EOF > $makefile
CFLAGS=-g -Wall -Werror -I../../include -I../include -I.
PROJOBJS= \
EOF
        for suiteName in $suiteNames; do
            echo "	../obj/$suiteName.o \\" >> $makefile
        done
        echo "" >> $makefile

        echo "TESTOBJS= \\" >> $makefile
        for suiteName in $suiteNames; do
            echo "	obj/test_$suiteName.o obj/test_${suiteName}_gen.o \\" >> $makefile
        done
        echo "" >> $makefile

        cat <<\EOF |

.PHONY: test dirs clean

test: dirs bin/test_<<projname>>
	bin/test_<<projname>> > log/test_<<projname>>.log

dirs:
	mkdir -p obj bin log

obj/%.o: %.c
	gcc $(CFLAGS) -o $@ -c $<

bin/test_<<projname>>: obj/test_<<projname>>.o $(TESTOBJS) $(PROJOBJS)
EOF
        sed -e "s/<<projname>>/$projName/g" >> $makefile

        cat <<\EOF |
	gcc $(CFLAGS) -o $@ $^

clean:
	rm -rf *.h *_gen.c test_<<projname>>.c obj bin
EOF
        sed -e "s/<<projname>>/$projName/g" >> $makefile
    }

    if [[ -z "$1" ]]; then # no param
        if [[ ! -d ./test ]]; then # no test dir
            echo "Scuttle: ERROR: default test directory ./test not found."
            printUsage
            return 0
        else
            testDir="test"
        fi
    elif [[ "$1" =~ ^-(-)?[hH?] ]]; then # help
        printUsage
        return 0
    elif [[ ! -d "./$1" ]]; then # specified test dir doesn't exist
        echo "Scuttle: ERROR: specified test directory ./$1 not found."
        printUsage
        return 0
    else # param specifies test dir
        testDir="$1"
    fi

    if [[ ! -d "./include" || ! -d "./obj" ]]; then
        echo "Scuttle: ERROR: ./include and ./obj directories must be present"
        printUsage
        return 0
    fi

    projName=$(basename "$PWD")

    if [[ -f "$testDir/test_$projName.c" ]]; then
        rm $testDir/test_$projName.c
    fi

    readSuiteNames
    numSuites=$(echo $suiteNames | wc -w)

    for suiteName in $suiteNames; do
        extractTestNames
        numTests=$(echo $testNames | wc -w)

        writeSuiteHeader
        writeSuiteData
    done

    writeHarness
    writeMakefile

    return 0
}

scuttle "$1"
