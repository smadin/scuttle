#ifndef _SCUTTLE_EXAMPLE_FOO_H
#define _SCUTTLE_EXAMPLE_FOO_H

#include <stddef.h>

/* foo() returns 42 */
int foo();

/* foostr() puts "foo" in buf.
 * returns 1 on success, 0 if buf is NULL or bufsz is <= 3
 */
int foostr(char *buf, size_t bufsz);

#endif /* _SCUTTLE_EXAMPLE_FOO_H */