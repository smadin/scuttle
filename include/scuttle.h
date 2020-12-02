#ifndef _SCUTTLE_H
#define _SCUTTLE_H

#include <stdlib.h>
#include <string.h>

#define _SCUTTLE_STR(x) #x
#define _SCUTTLE_XSTR(x) _SCUTTLE_STR(x)

#define _SCUTTLE_VER_MAJ 1
#define _SCUTTLE_VER_MIN 0
#define _SCUTTLE_VER_PAT 0
#define SCUTTLE_VERSION_STRING ("" _SCUTTLE_XSTR(_SCUTTLE_VER_MAJ) "." _SCUTTLE_XSTR(_SCUTTLE_VER_MIN) "." _SCUTTLE_XSTR(_SCUTTLE_VER_PAT))
#define SCUTTLE_VERSION_INT (_SCUTTLE_VER_PAT + 1000 * (_SCUTTLE_VER_MIN + 100 * _SCUTTLE_VER_MAJ))

#define SASSERT(x) \
    if (!(x)) {\
        snprintf(_scuttle_msgbuf, _scuttle_msgbufsz, "Assertion failure: expected " #x " to be true");\
        return 1;\
    }

#define SREFUTE(x) \
    if (x) {\
        snprintf(_scuttle_msgbuf, _scuttle_msgbufsz, "Assertion failure: expected " #x " to be false");\
        return 1;\
    }

#define SASSERT_NULL(x) \
    if (x != NULL) {\
        snprintf(_scuttle_msgbuf, _scuttle_msgbufsz, "Assertion failure: expected " #x " (" _SCUTTLE_XSTR(x) ") to be NULL");\
        return 1;\
    }

#define SREFUTE_NULL(x) \
    if (x == NULL) {\
        snprintf(_scuttle_msgbuf, _scuttle_msgbufsz, "Assertion failure: expected " #x " not to be NULL");\
        return 1;\
    }

#define SASSERT_EQ(x, y) \
    if (x != y) {\
        snprintf(_scuttle_msgbuf, _scuttle_msgbufsz, "Assertion failure: expected " #x " (" _SCUTTLE_XSTR(x) ") to equal " #y " (" _SCUTTLE_XSTR(y) ")");\
        return 1;\
    }

#define SREFUTE_EQ(x, y) \
    if (x == y) {\
        snprintf(_scuttle_msgbuf, _scuttle_msgbufsz, "Assertion failure: expected " #x " (" _SCUTTLE_XSTR(x) ") not to equal " #y " (" _SCUTTLE_XSTR(y) ")");\
        return 1;\
    }

#define SASSERT_STREQ(x, y) \
    {\
        if (x != y) {\
            if (x == NULL) {\
                snprintf(_scuttle_msgbuf, _scuttle_msgbufsz, "Assertion failure: " #x " is NULL and " #y " (\"%s\") is non-NULL", y);\
                return 1;\
            } else if (y == NULL) {\
                snprintf(_scuttle_msgbuf, _scuttle_msgbufsz, "Assertion failure: " #x " (\"%s\") is non-NULL and " #y " is NULL", x);\
                return 1;\
            } else {\
                size_t _sassert_streq_xlen = strlen(x);\
                size_t _sassert_streq_ylen = strlen(y);\
                if ((_sassert_streq_xlen != _sassert_streq_ylen) ||\
                    (strncmp(x, y, _sassert_streq_xlen) != 0)) {\
                    snprintf(_scuttle_msgbuf, _scuttle_msgbufsz, "Assertion failure: expected " #x " (\"%s\") to equal " #y " (\"%s\")", x, y);\
                    return 1;\
                }\
            }\
        }\
    }

#define SREFUTE_STREQ(x, y) \
    {\
        if (x == y) {\
            snprintf(_scuttle_msgbuf, _scuttle_msgbufsz, "Assertion failure: expected " #x " and " #y " not to be the same pointer");\
            return 1;\
        }\
        if (x != NULL && y != NULL) {\
            size_t _srefute_streq_xlen = strlen(x);\
            size_t _srefute_streq_ylen = strlen(y);\
            if ((_srefute_streq_xlen == _srefute_streq_ylen) &&\
                (strncmp(x, y, _srefute_streq_xlen) == 0)) {\
                snprintf(_scuttle_msgbuf, _scuttle_msgbufsz, "Assertion failure: expected " #x "(\"%s\") not to equal " #y " (\"%s\")", x, y);\
                return 1;\
            }\
        }\
    }

#define STEST_START(x) \
int _scuttle_test_##x(char *_scuttle_msgbuf, size_t _scuttle_msgbufsz)\
{\
    strncpy(_scuttle_testname, __func__, _scuttle_testname_sz);


#define STEST_END\
    return 0;\
}

#endif /* _SCUTTLE_H */