#include "inithttpcallbackfuction.h"

InitHttpCallbackFuction::InitHttpCallbackFuction(zjy::ReqTables<UserMessageManage> &reqMap)
{
    this->_reqtable.insert(std::make_pair("/api/login",this->_handlerLogin));
    this->_reqtable.insert(std::make_pair("/api/picName",this->_handlerRepSendAllPicName));
    this->_reqtable.insert(std::make_pair("/api/picsDate",this->_handlerRepSendallpicData));
    this->_reqtable.insert(std::make_pair("/api/deletePic",this->_handlerRepDeletepic));
    this->_reqtable.insert(std::make_pair("/api/uploadPic",this->_handlerRepUploadpic));
    this->_reqtable.insert(std::make_pair("/api/downloadPic",this->_handlerRepDownloadpic));
    reqMap=this->_reqtable;
}
namespace http{
//X: content-type    L:message length   content:what message you want to send to html
    std::string header(std::string X, std::string L){
        std::string header{"HTTP/1.1 200 OK\r\nContent-Type: "+X+"\nCache-Control: no-cache\nContent-Length: "+L+"\nAccess-Control-Allow-Origin: *\n\n"};
        return header;
    }
    std::string sendMsg(std::string X, std::string content){
        std::string all{};
        all += header(X, std::to_string(content.length()));
        all += content;
        all += "\n";
        return all;
    }
}
bool InitHttpCallbackFuction::_handlerLogin(http_message *msg, mg_connection *c, zjy::OnResCallback callback, UserMessageManage umm)
{
    char name[20],pwd[20];
    mg_get_http_var(&msg->query_string, "name", name, sizeof(name));
    mg_get_http_var(&msg->query_string, "pwd", pwd, sizeof(pwd));
    if(umm.login(std::string(name), std::string(pwd))){
        std::string type{"text/plain"};
        std::string msg{http::sendMsg(type,"0")};
        mg_printf(c,"%s", msg.c_str());
        return true;
    } else {
        std::string type{"text/plain"};
        std::string msg{http::sendMsg(type,"1")};
        mg_printf(c,"%s", msg.c_str());
        LOG("[Error]: login failed");
        return false;
    }
}

bool InitHttpCallbackFuction::_handlerRepSendAllPicName(http_message *msg, mg_connection *c, zjy::OnResCallback callback, UserMessageManage umm)
{
    std::string sendMsg;
    char para[100];
    mg_get_http_var(&msg->query_string,"username",para,sizeof(para));
    LOG(para);

    mg_printf(c, "HTTP/1.1 200 OK\r\n"
              "Content-Type: application/json\n"
              //"Content-Type: text/plain\n"
             // "Content-Type: image/jpeg\n"
              "Cache-Control: no-cache\n"
              "Content-Length: %lu\n"
              "Access-Control-Allow-Origin: *\n\n"
              "%s\n", sendMsg.size(), sendMsg.c_str());
}

bool InitHttpCallbackFuction::_handlerRepSendallpicData(http_message *msg, mg_connection *c, zjy::OnResCallback callback, UserMessageManage umm)
{

}

bool InitHttpCallbackFuction::_handlerRepDeletepic(http_message *msg, mg_connection *c, zjy::OnResCallback callback, UserMessageManage umm)
{

}

bool InitHttpCallbackFuction::_handlerRepUploadpic(http_message *msg, mg_connection *c, zjy::OnResCallback callback, UserMessageManage umm)
{

}

bool InitHttpCallbackFuction::_handlerRepDownloadpic(http_message *msg, mg_connection *c, zjy::OnResCallback callback, UserMessageManage umm)
{

}

void InitHttpCallbackFuction::_login(mg_connection *c, const string &msg)
{

}

void InitHttpCallbackFuction::_repSendAllPicName(mg_connection *c, const string &msg)
{

}

void InitHttpCallbackFuction::_repSendallpicData(mg_connection *c, const string &msg)
{

}

void InitHttpCallbackFuction::_repDeletepic(mg_connection *c, const string &msg)
{

}

void InitHttpCallbackFuction::_repUploadpic(mg_connection *c, const string &msg)
{

}

void InitHttpCallbackFuction::_repDownloadpic(mg_connection *c, const string &msg)
{

}
