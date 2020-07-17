#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#ifndef LOG_H
#define LOG_H


#if true
    #define DEBUGMODEL
#endif


#define ZJYLOG(X) \
    do { \
        printf("%s\n",X); \
    } while(0)

class LOG
{
public:
    LOG()=default;
    ~LOG(){
	std::cerr<<errno<<std::endl;
    }
    static void SuccessLog(std::string file, int line, std::string msg);
    static void SuccessLog(std::string msg);
    static void ErrorLog(std::string file, int line, std::string msg);
    static void testLog(std::string &msg);
    static void testLog(std::string &&msg);
};


#endif // LOG_H
