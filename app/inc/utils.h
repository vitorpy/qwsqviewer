#ifndef UTIL_H
#define UTIL_H

#include <cstdio>

#if defined(QT_DEBUG)
#define CHECK(x) do { \
    bool k = x; \
    if (!k) \
        fprintf(stderr, "FAILED: %s on %s:%d\n", #x, __FILE__, __LINE__); \
    } while (0);

#define DEBUG_PRINT(...) fprintf(stderr, __VA_ARGS__);
#else
#define CHECK(x)

#define DEBUG_PRINT(...)
#endif

#if defined(MAIN_OBJ)
#define DECORATION
#else
#define DECORATION extern
#endif

/*DECORATION Restaurante _global;*/

#endif // UTIL_H
