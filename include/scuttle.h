#ifndef _SCUTTLE_H
#define _SCUTTLE_H

#define __STDC_WANT_IEC_60559_BFP_EXT__
#include <stdlib.h>
#include <string.h>

#define STR(x) #x
#define XSTR(x) STR(x)
#define MIN(x,y) (x <= y ? x : y)

#define SASSERT(x) \
    if (!(x)) {\
        snprintf(msgbuf, bufsz, "Assertion failure: expected " #x " to be true");\
        return 1;\
    }

#define SREFUTE(x) \
    if (x) {\
        snprintf(msgbuf, bufsz, "Assertion failure: expected " #x " to be false");\
        return 1;\
    }

#define SASSERT_NULL(x) \
    if (x != NULL) {\
        snprintf(msgbuf, bufsz, "Assertion failure: expected " #x " (" XSTR(x) ") to be NULL");\
        return 1;\
    }

#define SREFUTE_NULL(x) \
    if (x == NULL) {\
        snprintf(msgbuf, bufsz, "Assertion failure: expected " #x " not to be NULL");\
        return 1;\
    }

#define SASSERT_EQ(x, y) \
    if (x != y) {\
        snprintf(msgbuf, bufsz, "Assertion failure: expected " #x " (" XSTR(x) ") to equal " #y " (" XSTR(y) ")");\
        return 1;\
    }

#define SREFUTE_EQ(x, y) \
    if (x == y) {\
        snprintf(msgbuf, bufsz, "Assertion failure: expected " #x " (" XSTR(x) ") not to equal " #y " (" XSTR(y) ")");\
        return 1;\
    }

#define SASSERT_STREQ(x, y) \
    {\
        if (x != y) {\
            if (x == NULL) {\
                snprintf(msgbuf, bufsz, "Assertion failure: " #x " is NULL and " #y " (\"%s\") is non-NULL", y);\
                return 1;\
            } else if (y == NULL) {\
                snprintf(msgbuf, bufsz, "Assertion failure: " #x " (\"%s\") is non-NULL and " #y " is NULL", x);\
                return 1;\
            } else {\
                size_t __sassert_streq_xlen = strlen(x);\
                size_t __sassert_streq_ylen = strlen(y);\
                if ((__sassert_streq_xlen != __sassert_streq_ylen) ||\
                    (strncmp(x, y, __sassert_streq_xlen) != 0)) {\
                    snprintf(msgbuf, bufsz, "Assertion failure: expected " #x " (\"%s\") to equal " #y " (\"%s\")", x, y);\
                    return 1;\
                }\
            }\
        }\
    }

#define SREFUTE_STREQ(x, y) \
    {\
        if (x == y) {\
            snprintf(msgbuf, bufsz, "Assertion failure: expected " #x " and " #y " not to be the same pointer");\
            return 1;\
        }\
        if (x != NULL && y != NULL) {\
            size_t __srefute_streq_xlen = strlen(x);\
            size_t __srefute_streq_ylen = strlen(y);\
            if ((__srefute_streq_xlen == __srefute_streq_ylen) &&\
                (strncmp(x, y, __srefute_streq_xlen) == 0)) {\
                snprintf(msgbuf, bufsz, "Assertion failure: expected " #x "(\"%s\") not to equal " #y " (\"%s\")", x, y);\
                return 1;\
            }\
        }\
    }

#define STEST_START(x) \
int _scuttle_test_##x(char *msgbuf, size_t bufsz)\
{\
    strncpy(testname, __func__, testname_sz);


#define STEST_END\
    return 0;\
}

#endif /* _SCUTTLE_H */