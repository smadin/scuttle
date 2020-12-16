#include "foo.h"
#include <string.h>

int foo()
{
    return 42;
}

int foostr(char *buf, size_t bufsz)
{
    if (!buf || bufsz < 4)
    {
        return 0;
    }
    strncpy(buf, "foo", bufsz);
    return 1;
}