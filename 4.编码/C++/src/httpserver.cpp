#include <exception>
#include "httpserver.h"
#include "log.h"

//user upload's picture name,used to slite suffix

extern std::string uploadformat;
std::string uploadformat{};
HttpServer::HttpServer()
{
}

HttpServer::~HttpServer()
{

}

static bool route_check(http_message *http_msg, char *route_prefix)
{
    if (mg_vcmp(&http_msg->uri, route_prefix) == 0)
        return true;
    else
        return false;

    // TODO: 还可以判断 GET, POST, PUT, DELTE等方法
    //mg_vcmp(&http_msg->method, "GET");
    //mg_vcmp(&http_msg->method, "POST");
    //mg_vcmp(&http_msg->method, "PUT");
    //mg_vcmp(&http_msg->method, "DELETE");
}

void HttpServer::init(const std::string &port)
{
    m_port = port;
    s_server_option.enable_directory_listing = "yes";
    s_server_option.document_root = s_web_dir.c_str();

    //开启 CORS，本项只针对主页加载有效
    s_server_option.extra_headers = "Access-Control-Allow-Origin: *";
}

bool HttpServer::start()
{
    mg_mgr_init(&m_mgr, nullptr);
    mg_connection *connection = mg_bind(&m_mgr, m_port.c_str(), HttpServer::OnHttpWebsocketEvent);
    if (connection == nullptr)
        return false;
    // for both http and websocket
    mg_set_protocol_http_websocket(connection);

    printf("starting http server at port: %s\n", m_port.c_str());
    // loop
    while (true){
        mg_mgr_poll(&m_mgr, 500); // ms
    }
}

bool HttpServer::close()
{
    mg_mgr_free(&m_mgr);
    return true;
}

void HttpServer::addHandler(const std::string &url, HttpServer::ReqHandler req_handler)
{
    if (s_handler_map.find(url) != s_handler_map.end()){
        LOG::ErrorLog(__FILE__, __LINE__, "url exist");
        return;
    }
    s_handler_map.insert(std::make_pair(url, req_handler));
}

void HttpServer::removeHandler(const std::string &url)
{
    auto it = s_handler_map.find(url);
    if (it != s_handler_map.end())
        s_handler_map.erase(it);
}

void HttpServer::addSession(mg_connection *conn)
{
    s_websocket_session_set.insert(conn);
}
//why nummm will increase, if _db have not inited, the number will increase and never stoped
static int nummm=0;
void HttpServer::OnHttpWebsocketEvent(mg_connection *connection, int event_type, void *event_data)
{
    // distinguish http and websocket
    if (event_type == MG_EV_HTTP_REQUEST) {
        http_message *http_req = static_cast<http_message *>(event_data);
        HandleHttpEvent(connection, http_req);
    }
    else if (event_type == MG_EV_WEBSOCKET_HANDSHAKE_DONE ||
             event_type == MG_EV_WEBSOCKET_FRAME ||
             event_type == MG_EV_CLOSE) {
        websocket_message *ws_message = static_cast<struct websocket_message *>(event_data);
        HandleWebsocketMessage(connection, event_type, ws_message,"");
    }
}

void HttpServer::HandleHttpEvent(mg_connection *connection, http_message *http_req)
{
    //std::unique_ptr<UserManageInterface> umm(new UserManageInterface(_db));
    std::string req_str = std::string(http_req->message.p, http_req->message.len);
    printf("got request: %s\n", req_str.c_str());
    // 先过滤是否已注册的函数回调
    std::string url = std::string(http_req->uri.p, http_req->uri.len);
    std::string body = std::string(http_req->body.p, http_req->body.len);


//    char name[20];
//    mg_get_http_var(&http_req->query_string, "name", name, sizeof(name));
    std::string phone="";
    try {
        struct mg_str *cookie_header;
        cookie_header=mg_get_http_header(http_req, "Cookie");    //获取Cookie 整条
        if(cookie_header == nullptr)
            throw "";
        std::string getCookie = std::string(cookie_header->p,cookie_header->len);
        auto pos = getCookie.find("phone=");
        if(pos!=std::string::npos){
            pos=pos+6;
            LOG::testLog(std::to_string(getCookie.size()));
            while (pos<getCookie.size()&&getCookie.at(pos)!=';') {
                phone+=getCookie.at(pos);
                pos++;
            }
            LOG::testLog(phone);
        }
    } catch (...){
        LOG::ErrorLog(__FILE__,__LINE__,"other error");
    }

    auto _exist = HttpServer::s_session.find(phone);
    //std::unique_ptr<UserManageInterface> umm{};
    UserManageInterface* umm=nullptr;
    if(HttpServer::s_session.end() == _exist){
//        umm.reset(new UserManageInterface(_db));
        umm = new UserManageInterface(_db);
    }else{
//        umm.reset(_exist->second);
        umm = _exist->second;
        umm->mtm->setBegin();
    }
    auto it = s_handler_map.find(url);
    if (it != s_handler_map.end())
    {
        /*
         *
         * i think i need to add session in here to record whether user is connected,
         * if not, create a new uermanageinterface(new user object)
         * in this part, i need several line code to deal with session witch from http protocol
         * ,it's means how to analysis http(client's brower will send cookies)
          */

//        auto isExist = HttpServer::s_session.find(umm->getPersonInfo()->getPhone());
//        if(isExist!=HttpServer::s_session.end()){
//            umm = isExist;
//        } else {
//            umm = new UserManageInterface(_db);
//        }

        ReqHandler handle_func = it->second;
        handle_func(connection, http_req, umm/*HttpServer::_umm*/);
    }else{
        // other request
        std::string rootpage="/";
        if (route_check(http_req, const_cast<char*>(rootpage.c_str()))) // index page
            mg_serve_http(connection, http_req, s_server_option);
        else {

        }
    }


}

int HttpServer::isWebSocket(const mg_connection *connection)
{
    return connection->flags & MG_F_IS_WEBSOCKET;
}
/*websock always used to send image from client to server*/
void HttpServer::HandleWebsocketMessage(mg_connection *connection, int event_type, websocket_message *ws_msg, std::string /*filename*/)
{  
    if( event_type == MG_EV_WEBSOCKET_HANDSHAKE_DONE ){
        LOG::SuccessLog(__FILE__, __LINE__, "client websocket connected");
        char addr[32];
        mg_sock_addr_to_str(&connection->sa, addr, sizeof(addr), MG_SOCK_STRINGIFY_IP | MG_SOCK_STRINGIFY_PORT);
        LOG::SuccessLog(__FILE__, __LINE__, std::string("client addr:")+addr);
        //s_websocket_session_set.insert(connection);
    } else if (event_type == MG_EV_WEBSOCKET_FRAME) {
        mg_str received_msg = {(char*)ws_msg->data, ws_msg->size};
        //char buff[409600] = {0};
        //strncpy(buff, received_msg.p, received_msg.len); // must use strncpy, specifiy memory pointer and length
        std::string buff((char*)ws_msg->data,ws_msg->size);

        //here need to judge whether path exist
        std::string path;

        //need to get suffux from client's browser
        if(format_websocket_session.find(connection)==format_websocket_session.end()){
            format_websocket_session.insert(std::make_pair(connection,buff));
            return;
        }else{
            std::map<std::string,std::string> jsonValue=JSON::jsonToMap(format_websocket_session.find(connection)->second);
            std::string userPhone = jsonValue.find("phone")->second;
            std::string userImageName = jsonValue.find("img_name")->second;
            std::string reqType = jsonValue.find("type")->second;
            zjy::pic::picFormat format1 = ImageBaseTool::parseImageFormat(userImageName);

            /*used for case 0*/
            std::string format2;
            time_t t = time(nullptr);
            char ch[64] = {0};
            int isSaved;
            /*used for case 0 end*/

            std::string describe;//used to object detection

            switch (std::atoi(reqType.c_str())) {
            case 0://user upload personal image to photo album
//                path=zjy::pic::picRoot::path+userPhone+"/";
//                strftime(ch, sizeof(ch) - 1, "%Y%m%d%H%M%S", localtime(&t));
//                format2=userPhone+ch;
                switch (format1) {
                case zjy::pic::picFormat::jpg:
                    format2=zjy::pic::picFormatSuffix::JPG;
                    break;
                case zjy::pic::picFormat::png:
                    format2=zjy::pic::picFormatSuffix::PNG;
                    break;
                case zjy::pic::picFormat::JPEG:
                    format2=zjy::pic::picFormatSuffix::JPEG;
                    break;
                case zjy::pic::picFormat::TIFF:
                    format2=zjy::pic::picFormatSuffix::TIFF;
                    break;
                case zjy::pic::picFormat::other:
                    LOG::ErrorLog(__FILE__,__LINE__,"upload image format error");
                    break;
                }
                isSaved=UserManageInterface::saveImage((char*)ws_msg->data,ws_msg->size,userPhone,format2,describe);
//                isSaved= ImageBaseTool::saveImage((char*)ws_msg->data,path,picName,ws_msg->size);
                if(isSaved==0)
                    SendWebsocketMsg(connection, "i received your picture",false);
                else
                    SendWebsocketMsg(connection, "i have not received your picture or image exist",false);
                format_websocket_session.erase(connection);
                break;
            case 1:{//text detect(CSER)
                cv::Mat pic = ImageBaseTool::binaryToMat(buff);
                std::vector<uchar> data_encode;

                zjyObjectDetection::textDetectCSER(pic);

                cv::imencode(".jpeg",pic,data_encode);
                std::string str_enclde(data_encode.begin(),data_encode.end());
                str_enclde = ImageBaseTool::base64_encode(str_enclde.c_str(),str_enclde.length());
//                //decode
//                str_enclde = ImageBaseTool::base64_decode(str_enclde.c_str());
//                std::vector<uchar> data_decode(str_enclde.begin(),str_enclde.end());
//                cv::Mat img_decode = cv::imdecode(data_decode,cv::IMREAD_COLOR);
//                cv::imshow("123",img_decode);
//                cv::waitKey(0);
                mg_send_websocket_frame(connection, WEBSOCKET_OP_TEXT,str_enclde.c_str(), str_enclde.size());

                format_websocket_session.erase(connection);
            }break;
            case 2:{//head detect
                cv::Mat pic = ImageBaseTool::binaryToMat(buff);
                std::vector<uchar> data_encode;
                int num = zjyObjectDetection::head_detect(pic);
                cv::imencode(".jpeg",pic,data_encode);
                std::string str_encode(data_encode.begin(),data_encode.end());
                std::string numInPic{"人数:"+std::to_string(num)};
                str_encode = ImageBaseTool::base64_encode(str_encode.c_str(),str_encode.length());
                mg_send_websocket_frame(connection, WEBSOCKET_OP_TEXT,str_encode.c_str(), str_encode.size());
                mg_send_websocket_frame(connection, WEBSOCKET_OP_TEXT,numInPic.c_str(), numInPic.size());
                format_websocket_session.erase(connection);
            }break;
            case 3:{//tencent ocr
                cv::Mat pic = ImageBaseTool::binaryToMat(buff);
		std::vector<uchar> data_encode;
                cv::imencode(".jpeg",pic,data_encode);
                std::string str_encode(data_encode.begin(),data_encode.end());
                str_encode = ImageBaseTool::base64_encode(str_encode.c_str(),str_encode.length());
		std::string words=zjyObjectDetection::tencentOCR(str_encode);
		std::cout<<words<<std::endl;
                mg_send_websocket_frame(connection, WEBSOCKET_OP_TEXT,words.c_str(), words.size());
                format_websocket_session.erase(connection);
            }break;
            }

        }
    } else if (event_type == MG_EV_CLOSE) {
        if (isWebSocket(connection))
        {
            LOG::SuccessLog(__FILE__, __LINE__, "client websocket closed\n");
            // move session
            if (s_websocket_session_set.find(connection) != s_websocket_session_set.end())
                s_websocket_session_set.erase(connection);
        }
    }
}

void HttpServer::SendWebsocketMsg(mg_connection *connection, std::string msg, bool setClose=false)
{
    mg_send_websocket_frame(connection, WEBSOCKET_OP_TEXT, msg.c_str(), strlen(msg.c_str()));
    if(setClose==true){
        mg_send_websocket_frame(connection,WEBSOCKET_OP_CLOSE,nullptr,0);
    }

}

void HttpServer::SendWebsocketMsg_binary(mg_connection *connection, std::string msg, bool setClose=false)
{
    mg_send_websocket_frame(connection, WEBSOCKET_OP_BINARY, msg.c_str(), strlen(msg.c_str()));
    if(setClose==true){
        mg_send_websocket_frame(connection,WEBSOCKET_OP_CLOSE,nullptr,0);
    }

}

void HttpServer::BroadcastWebsocketMsg(std::string msg)
{
    for (mg_connection *connection : s_websocket_session_set)
        mg_send_websocket_frame(connection, WEBSOCKET_OP_TEXT, msg.c_str(), strlen(msg.c_str()));
}
