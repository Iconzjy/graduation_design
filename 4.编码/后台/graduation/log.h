#ifndef LOG_H
#define LOG_H

#include <stdlib.h>


#if true
    #define DEBUGMODEL
#endif

#ifdef DEBUGMODEL
#define LOG(X) \
    do { \
        printf("%s\n",X); \
    } while(0)
#endif


#endif // LOG_H
