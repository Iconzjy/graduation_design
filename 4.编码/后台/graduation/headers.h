#ifndef HEADERS_H
#define HEADERS_H


#include <time.h>
#include <iostream>
#include <string>
#include <string.h>
#include <byteswap.h>
#include <utility>
#include <functional>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <mysql/mysql.h>
#include <vector>
#include <list>
#include <iterator>
#include <opencv2/opencv.hpp>


#include "mongoose.h"
#include "log.h"
#include "User.h"
#include "ImagePath.h"

namespace zjy {

    /***************************
     * define in log.h
    #if true
        #define DEBUGMODEL
    #endif
    ****************************/


    //this function used to deal with http request
    using OnResCallback = std::function<void (mg_connection *c, string *msg)>;

    //add UserMessageManage class as T
    template <typename T>
    using ReqHandler = std::function<bool (http_message *msg, mg_connection* c, OnResCallback,T)>;

    //this function is used to storage the request mapping table(url and callback function)
    template <typename T>
    using ReqTables=std::unordered_map<std::string, zjy::ReqHandler<T>>;

    namespace sql {
    const std::string host{"139.199.180.227"};
    const int port{3306};
    const std::string user{"root"};
    const std::string db{"graduation"};
    const std::string pwd{"2427072749"};
    }

    //here is path where storage picture
    namespace picRoot {
    const std::string path{"/home/"};
    }
#ifndef HTTP
#define HTTP
//    namespace http{
//    //X: content-type    L:message length   content:what message you want to send to html
//        std::string header(std::string X, std::string L){
//            std::string header{"HTTP/1.1 200 OK\r\nContent-Type: "+X+"\nCache-Control: no-cache\nContent-Length: "+L+"\nAccess-Control-Allow-Origin: *\n\n"};
//            return header;
//        }
//        std::string sendMsg(std::string X, std::string content){
//            std::string all{};
//            all += header(X, std::to_string(content.length()));
//            all += content;
//            all += "\n";
//            return all;
//        }
//    }
#endif
}

#endif // HEADERS_H
