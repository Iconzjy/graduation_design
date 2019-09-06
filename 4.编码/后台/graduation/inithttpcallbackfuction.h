/*******************************************************************
  这个类是用来定义http请求的回调函数的，就是说是：哪一个url对应的解决函数是哪个
***********************************************************************/

#ifndef INITHTTPCALLBACKFUCTION_H
#define INITHTTPCALLBACKFUCTION_H


#include "headers.h"
#include "UserMessageManage.h"


/**********************
 * 2019/9/5
*这个设计有问题，handler这个注册函数就应该在main.cpp或其他类中才注册，不应该是
*在初始化回调函数这个类中注册，同时，这个类全部的函数应该是静态函数，直接给默认的构造和折构函数，
*这次在这里，先把功能实现了，后面有时间的话，需要把这里面的handler这些函数益处，这个类只保留回调函数的实现
***********************/
class InitHttpCallbackFuction
{
    using ReqHandler=zjy::ReqHandler<UserMessageManage>;
public:
    InitHttpCallbackFuction() = delete;//no used
    InitHttpCallbackFuction(zjy::ReqTables<UserMessageManage>& reqMap);


//regist function
private:
    static bool _handlerLogin(http_message *msg, mg_connection* c, zjy::OnResCallback callback, UserMessageManage umm);
    //return all pictures' name,send json data type to html
    static bool _handlerRepSendAllPicName(http_message *msg, mg_connection* c, zjy::OnResCallback callback, UserMessageManage umm);
    //return all pictures' data one by one, send binary data type to html
    static bool _handlerRepSendallpicData(http_message *msg, mg_connection* c, zjy::OnResCallback callback, UserMessageManage umm);
    //delete picture
    static bool _handlerRepDeletepic(http_message *msg, mg_connection* c, zjy::OnResCallback callback, UserMessageManage umm);
    //storage pic,if user upload pic ,call this function
    static bool _handlerRepUploadpic(http_message *msg, mg_connection* c, zjy::OnResCallback callback, UserMessageManage umm);
    //download picture from server
    static bool _handlerRepDownloadpic(http_message *msg, mg_connection* c, zjy::OnResCallback callback, UserMessageManage umm);

private:
    //login
    static void _login(mg_connection *c, const string &msg);
    //return all pictures' name,send json data type to html
    static void _repSendAllPicName(mg_connection *c, const string &msg);
    //return all pictures' data one by one, send binary data type to html
    static void _repSendallpicData(mg_connection *c, const string &msg);
    //delete picture
    static void _repDeletepic(mg_connection *c, const string &msg);
    //storage pic,if user upload pic ,call this function
    static void _repUploadpic(mg_connection *c, const string &msg);
    //download picture from server
    static void _repDownloadpic(mg_connection *c, const string &msg);
private:
    std::unordered_map<std::string, ReqHandler> _reqtable;
};

#endif // INITHTTPCALLBACKFUCTION_H
