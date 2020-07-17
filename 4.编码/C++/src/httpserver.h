#include "headers.h"
#include "imagebasetool.h"
#ifndef HTTPSERVER_H
#define HTTPSERVER_H


class HttpServer
{
    using ReqHandler = zjy::ReqHandler<UserManageInterface>;
public:
    HttpServer();
    ~HttpServer();
    void init(const std::string &port);
    bool start();
    bool close();
    void addHandler(const std::string &url, ReqHandler req_handler);
    void removeHandler(const std::string & url);
    static void addSession(mg_connection* conn);
    static std::unordered_map<mg_connection*,std::string> format_websocket_session;
    static std::unordered_set<mg_connection*> s_websocket_session_set;
    static std::string s_web_dir;//web root
    static mg_serve_http_opts s_server_option;//web server opetion
    static std::unordered_map<std::string, ReqHandler> s_handler_map;
    static DBPool* _db;
    static std::unordered_map<std::string, UserManageInterface*> s_session;

private:
    static void OnHttpWebsocketEvent(mg_connection *connection, int event_type, void *event_data);
    static void HandleHttpEvent(mg_connection *connection, http_message *http_req);
    static int isWebSocket(const mg_connection* connection);
    static void HandleWebsocketMessage(mg_connection* connection, int event_type, websocket_message* ws_msg, std::string filename);
    static void SendWebsocketMsg(mg_connection* connection, std::string msg,bool setClose);
    static void SendWebsocketMsg_binary(mg_connection* connection, std::string msg,bool setClose);
    static void BroadcastWebsocketMsg(std::string msg);
    /*
     * @std::pair<mg_connection*,std::string> the fist value means which client connected,
     *  second velue storage the client's image type
    */

    std::string m_port;    // 端口
    mg_mgr m_mgr;          // 连接管理器
    static int num;

public:
    //this para is used to test
    //static UserManageInterface* _umm;

};

#endif // HTTPSERVER_H
