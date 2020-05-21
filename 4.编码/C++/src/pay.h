#include <string>
#include <arpa/inet.h> //sockaddr_in
#include <sys/socket.h>
#include <memory.h>
#include <unistd.h>   //read
#include "log.h"
#include "json.hpp"
#ifndef PAY_H
#define PAY_H


class Pay
{
public:
    static std::string getAlipay(std::string user);
    static std::string parseURL(std::string url);
private:
    enum TYPE{
        GETURL=1,
        PARSEURL=2
    };
};

#endif // PAY_H
