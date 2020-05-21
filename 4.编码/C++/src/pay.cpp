#include "pay.h"
std::string Pay::getAlipay(std::string user)
{
    int cln_sock;
    cln_sock=socket(PF_INET,SOCK_STREAM,0);
    if(-1==cln_sock){
        LOG::ErrorLog(__FILE__,__LINE__, "sock() error");
        return nullptr;
    }
    struct sockaddr_in serv_addr;
    memset(&serv_addr,0,sizeof(serv_addr));
    char* addr="127.0.0.1";
    char* port="7998";
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr(addr);
    serv_addr.sin_port=htons(atoi(port));

    if(-1==connect(cln_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))){
        LOG::ErrorLog(__FILE__,__LINE__, "connect() error");
        close(cln_sock);
        return nullptr;
    }
    std::list<std::string> ls;
    ls.push_back(std::to_string(Pay::TYPE::GETURL));
    ls.push_back(user);
    std::string message{JSON::stringListToJson(ls)};
    std::string rtnAlipay{};
    size_t len=message.size();
    if(!write(cln_sock,message.c_str(),len)){
        LOG::ErrorLog(__FILE__,__LINE__,"something error");
        close(cln_sock);
        return nullptr;
    }
    char r[1024];
    memset(r,0,1024);
    while(ssize_t l = read(cln_sock,r,1024)){
        rtnAlipay.append(r);
    }
    close(cln_sock);
    return rtnAlipay;
}

std::string Pay::parseURL(std::string url)
{
    url = "www.zjyxxn.cn"+url;
    int cln_sock;
    cln_sock=socket(PF_INET,SOCK_STREAM,0);
    if(-1==cln_sock){
        LOG::ErrorLog(__FILE__,__LINE__, "sock() error");
        return nullptr;
    }
    struct sockaddr_in serv_addr;
    memset(&serv_addr,0,sizeof(serv_addr));
    char* addr="127.0.0.1";
    char* port="7998";
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr(addr);
    serv_addr.sin_port=htons(atoi(port));

    if(-1==connect(cln_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))){
        LOG::ErrorLog(__FILE__,__LINE__, "connect() error");
        close(cln_sock);
        return nullptr;
    }
    std::list<std::string> ls;
    ls.push_back(std::to_string(Pay::TYPE::PARSEURL));
    ls.push_back(url);
    std::string message{JSON::stringListToJson(ls)};
    std::string isPay{};
    size_t len=message.size();
    if(!write(cln_sock,message.c_str(),len)){
        LOG::ErrorLog(__FILE__,__LINE__,"something error");
        close(cln_sock);
        return nullptr;
    }
    char r[1024];
    memset(r,0,1024);
    while(ssize_t l = read(cln_sock,r,1024)){
        isPay.append(r);
    }
    close(cln_sock);
    LOG::testLog(isPay);
    return isPay;
}
