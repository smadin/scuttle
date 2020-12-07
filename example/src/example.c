#include "example.h"
#include "foo.h"
#include "bar.h"
#include <stdio.h>

int main(int argc, char **argv)
{
    printf("SCUTTLE example\n");
    if (scuttle_example())
    {
        fprintf(stderr, "something went wrong\n");
    }
    printf("example done\n");
    return 0;
}

int scuttle_example()
{
    printf("foo() returns %d\n", foo());
    printf("bar() returns %#010x\n", bar());
    return 0;
}