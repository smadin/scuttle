#!/usr/bin/bash

# Scuttle generates unit test scaffolding, letting the developer write only the test suites themselves.
# Dependencies are minimal: this script, scuttle.h, bash, GNU make, and a modern C compiler.
# See README.md for more information.

shopt -s extglob

SCUTTLE_SCRIPT_VER="1.0.0"

function scuttle() {

    # checkVer finds the version of the Scuttle header and checks it against this script.
    # it also determines whether the Scuttle header is installed system-wide or only locally.
    function checkVer() {
        unset extraInc
        unset verMismatch
        if [[ -f "/usr/include/scuttle.h" || -f "/usr/local/include/scuttle.h" ]]; then
            scuttleInclude="#include <scuttle.h>"
        else
            scuttleInclude="#include \"scuttle.h\""
            scincLoc=$(dirname $(find . -name scuttle.h))
            extraInc="-I$scincLoc"
        fi
        checkVerTmpDir=$(mktemp -d)
        checkVerTmpFile="$checkVerTmpDir/checkver.c"
        cat <<\EOF |
<<scuttleinc>>
#include <stdio.h>

int main(int argc, char **argv)
{
    printf("%s\n", SCUTTLE_VERSION_STRING);
    return 0;
}
EOF
        sed -e "s/<<scuttleinc>>/$scuttleInclude/" > $checkVerTmpFile
        cc $extraInc -o $checkVerTmpDir/checkver $checkVerTmpFile
        scuttleHeaderVer=$($checkVerTmpDir/checkver)
        if [[ "$scuttleHeaderVer" != "$SCUTTLE_SCRIPT_VER" ]]; then
            verMismatch="scuttle version mismatch"
        fi

        rm -rf $checkVerTmpDir
    }

    function printVerMismatch()
    {
        echo "scuttle.h version is $scuttleHeaderVer but scuttle.sh version is $SCUTTLE_SCRIPT_VER"
    }

    function parseOpts() {
        unset doHelp
        unset noObjs
        unset testDir

        # -h or -? prints usage and exits
        # -o suppresses check for ./obj
        # any other arg is assumed to override the test dir name
        # more than one other arg is an error
        for arg in "$@"; do
            case "$arg" in
                -h | -\?)   doHelp="true"
                    ;;
                -o)         noObjs="true"
                    ;;
                *)          if [[ -z "$testDir" ]]; then
                                testDir="$arg"
                            else
                                doHelp="true"
                            fi
                    ;;
            esac
        done
    }

    function printUsage() {
        echo "Scuttle - simple C unit tests"
        echo ""
        echo "Run Scuttle from the root of your project tree. Scuttle expects to find headers in ./include and object files in ./obj."
        echo "Scuttle generates headers, source files, and a Makefile in the test directory."
        echo "bash scuttle.sh [-h] [-o] [testDir]"
        echo "----------"
        echo "-h: print this message"
        echo "-o: don't check for ./obj subdirectory (when tests don't require compiled objects)"
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

    # writes out the header file test_<suite>.h for a test suite source file test_<suite>.c
    function writeSuiteHeader() {
        suiteCaps=$(echo "$suiteName" | tr '[:lower:]' '[:upper:]')
        suiteHeader="$testDir/test_$suiteName.h"
        # test suite header
        cat <<\EOF |
#ifndef _SCUTTLE_TEST_<<SUITENAME>>_H
#define _SCUTTLE_TEST_<<SUITENAME>>_H

<<scuttleinc>>

void _scuttle_suite_init_<<projname>>_<<suitename>>();
void _scuttle_suite_test_setup_<<projname>>_<<suitename>>();
void _scuttle_suite_test_teardown_<<projname>>_<<suitename>>();

EOF
        sed -e "s/<<scuttleinc>>/$scuttleInclude/" -e "s/<<projname>>/$projName/" -e "s/<<SUITENAME>>/$suiteCaps/" -e "s/<<suitename>>/$suiteName/" > $suiteHeader
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

    # writes out the generated data file test_<suite>_gen.c file for a given test suite source file test_<suite>.c
    # the *_gen.c files define the arrays of test cases the harness will iterate through.
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

    # writes out the test harness that drives the testing process, calls all the tests suite-by-suite, and outputs the results.
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
    (void)argc;
    (void)argv;

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

    printf("*** %lu / %lu suites passed.\n", (suitect - suite_failct), suitect);
    return 0;
}
EOF
    }

    # writes out the Makefile that builds the test harness binary.
    function writeMakefile() {
        makefile="$testDir/Makefile"
        # Makefile
        cat <<\EOF > $makefile
ifneq (,$(shell which clang))
CC=clang
else ifneq (,$(shell which gcc))
CC=gcc
endif

EOF
        echo "CFLAGS=-g -Wall -Wextra -Werror -std=c99 -I../include -I. $extraInc" >> $makefile
        if [[ "$noObjs" != "true" ]]; then
            cat <<\EOF >> $makefile
PROJOBJS= \
EOF
            for suiteName in $suiteNames; do
                echo "	../obj/$suiteName.o \\" >> $makefile
            done
            echo "" >> $makefile
        fi

        echo "TESTOBJS= \\" >> $makefile
        for suiteName in $suiteNames; do
            echo "	obj/test_$suiteName.o obj/test_${suiteName}_gen.o \\" >> $makefile
        done
        echo "" >> $makefile

        if [[ "$noObjs" == "true" ]]; then
            echo "OBJS=\$(TESTOBJS)" >> $makefile
        else
            echo "OBJS=\$(TESTOBJS) \$(PROJOBJS)" >> $makefile
        fi

        cat <<\EOF |

.PHONY: test dirs clean

test: dirs bin/test_<<projname>>
	bin/test_<<projname>> > log/test_<<projname>>.log

dirs:
	mkdir -p obj bin log

obj/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

bin/test_<<projname>>: obj/test_<<projname>>.o $(OBJS)
EOF
        sed -e "s/<<projname>>/$projName/g" >> $makefile

        cat <<\EOF |
	$(CC) -o $@ $^

clean:
	rm -rf *.h *_gen.c test_<<projname>>.c obj bin
EOF
        sed -e "s/<<projname>>/$projName/g" >> $makefile
    }

    checkVer
    [[ -n "$verMismatch" ]] && printVerMismatch && return 0

    echo "This is Scuttle, version $SCUTTLE_SCRIPT_VER"

    parseOpts "$@"
    [[ "$doHelp" == "true" ]] && printUsage && return 0
    [[ -z "$testDir" ]] && echo "test directory not specified, using default ./test" && testDir="./test"

    if [[ ! -d "$testDir" ]]; then # no test dir
        echo "Scuttle: ERROR: test directory $testDir not found."
        printUsage
        return 0
    fi

    if [[ ! -d "./include" || ! ( "$noObjs" == "true" || -d "./obj" ) ]]; then
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

scuttle "$@"
