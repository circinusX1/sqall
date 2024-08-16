#ifndef SOMELIB_H
#define SOMELIB_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

    int function_in_lib(const char* p);

#ifdef __cplusplus
};
#endif

#endif // SOMELIB_H
