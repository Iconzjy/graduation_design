#include <future>
#include "httpserver.h"
#include "dealhttpmsg.hpp"

// 初始化HttpServer静态类成员
mg_serve_http_opts HttpServer::s_server_option;
std::string HttpServer::s_web_dir = "./web";
int HttpServer::num=0;
bool DBPool::_isCreateDBPool = false;
DBPool *a = nullptr;
DBPool* HttpServer::_db = DBPool::createDBPool();
std::unordered_map<std::string, zjy::ReqHandler<UserManageInterface>> HttpServer::s_handler_map;
std::unordered_set<mg_connection*> HttpServer::s_websocket_session_set;
std::unordered_map<mg_connection*,std::string> HttpServer::format_websocket_session;
std::unordered_map<std::string, UserManageInterface*> HttpServer::s_session;
//UserManageInterface* HttpServer::_umm = new UserManageInterface(HttpServer::_db);
static LOG exitLog=LOG();

using namespace std;
using namespace cv;
typedef unsigned char byte;

//用于检查用户是否长时间不在线
void examine();

int main(int argc, char *argv[])
{
    std::string port = "7999";
    auto http_server = std::shared_ptr<HttpServer>(new HttpServer);
    http_server->init(port);
    // add handler
    http_server->addHandler("/api/login", DealHttpMsg::login<UserManageInterface>);
    http_server->addHandler("/api/regist", DealHttpMsg::regist<UserManageInterface>);
    http_server->addHandler("/api/getPersonalInfo",DealHttpMsg::repSendPersonalInfo<UserManageInterface>);
    http_server->addHandler("/api/modifypersonmsg", DealHttpMsg::modifyPersonMsg<UserManageInterface>);
    http_server->addHandler("/api/logoff", DealHttpMsg::logoff<UserManageInterface>);
    http_server->addHandler("/api/isvip", DealHttpMsg::repSendVIPLevel<UserManageInterface>);

    http_server->addHandler("/api/geturl", DealHttpMsg::repGetURL<UserManageInterface>);
    http_server->addHandler("/api/sync", DealHttpMsg::repParseOrder<UserManageInterface>);

    http_server->addHandler("/api/repsendpicdata", DealHttpMsg::repSendpicData<UserManageInterface>);
    http_server->addHandler("/api/repsendpicnamebytype", DealHttpMsg::repSendPicNameByType<UserManageInterface>);
    http_server->addHandler("/api/repsendpicnamebycustom", DealHttpMsg::repSendPicNameByCustom<UserManageInterface>);

    http_server->addHandler("/api/repdownloadpic", DealHttpMsg::repDownloadpic<UserManageInterface>);

    http_server->addHandler("/api/repmovepictorecycle", DealHttpMsg::repMovePicToRecycle<UserManageInterface>);
    http_server->addHandler("/api/reprecoverpictorecycle", DealHttpMsg::repRecoverPicToRecycle<UserManageInterface>);
    http_server->addHandler("/api/repdeletepicindisk", DealHttpMsg::repDeletepicInDisk<UserManageInterface>);
    http_server->addHandler("/api/repdeleteallinrecycle", DealHttpMsg::repDeleteAllInRecycle<UserManageInterface>);
    http_server->addHandler("/api/userclosebrower",DealHttpMsg::closeBrower<UserManageInterface>);
    http_server->addHandler("/api/repdeletealluserpicindisk", DealHttpMsg::repDeleteAllUserpicInDisk<UserManageInterface>);
    http_server->start();

    try {
        auto f=std::async(std::launch::async,examine);
        f.get();
    } catch (...) {

    }
    return 0;
}
void examine()
{/*
    while (true) {
        try {
            for(auto session:HttpServer::s_session){
                UserManageInterface* umm=session.second;
                if(umm->mtm->getTimer_hour()>1.0){
                    umm->~UserManageInterface();
                }
            }
            this_thread::sleep_for(std::chrono::minutes(1));

        } catch (const exception& e) {
            std::cerr<<"EXCEPTION,thread id:"<<this_thread::get_id()<<": "<<e.what()<<std::endl;
        }
    }
*/
}
