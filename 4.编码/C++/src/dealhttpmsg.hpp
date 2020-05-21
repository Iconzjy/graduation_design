#include <opencv2/opencv.hpp>
#include "zjynamespace.h"
#include "headers.h"
#include "httpserver.h"
#ifndef SENDHTTPMSG_H
#define SENDHTTPMSG_H

/****************************************
*this struct is used to deal with message from
* brower(send or receive http message)
* this are callback functions
* the 4th para in using ReqHandler = std::function<void (mg_connection*, http_message*, T*, OnResCallback<T>)>;
*/

struct DealHttpMsg
{
public:
    //response
    
    template<typename T>
    static void regist(mg_connection *c, const http_message *msg, T *umm);

    template<typename T>
    static void logoff(mg_connection *c, const http_message *msg, T *umm);

    template<typename T>
    static void login(mg_connection *c, const http_message *msg, T *umm/*, zjy::OnResCallback respond*/);

    template<typename T>
    static void closeBrower(mg_connection *c, const http_message *msg, T *umm);

    template<typename T>
    static void modifyPersonMsg(mg_connection *c, const http_message *msg, T *umm/*, zjy::OnResCallback respond*/);

    template<typename T>
    static void repSendPersonalInfo(mg_connection *c, const http_message *msg, T *umm);

    template<typename T>
    static void repSendVIPLevel(mg_connection *c, const http_message *msg, T *umm);

    //return all pictures' name,send json data type to html
    template<typename T>
    static void repSendPicNameByType(mg_connection *c, const http_message *msg, T *umm/*, zjy::OnResCallback respond*/);
    //return all pictures' name,send json data type to html
    template<typename T>
    static void repSendPicNameByCustom(mg_connection *c, const http_message *msg, T *umm/*, zjy::OnResCallback respond*/);

    //return all pictures' data, send binary data type to html(only one picture)
    template<typename T>
    static void repSendpicData(mg_connection *c, const http_message *msg, T *umm/*, zjy::OnResCallback respond*/);
    //delete picture, only change in database(mysql)
    template<typename T>
    static void repMovePicToRecycle(mg_connection *c, const http_message *msg, T *umm/*, zjy::OnResCallback respond*/);
    //recover user's picture, only change in database(mysql)
    template<typename T>
    static void repRecoverPicToRecycle(mg_connection *c, const http_message *msg, T *umm/*, zjy::OnResCallback respond*/);
    //delete picture in hard disk.
    template<typename T>
    static void repDeletepicInDisk(mg_connection *c, const http_message *msg, T *umm/*, zjy::OnResCallback respond*/);
    //delete all of user's picture in hard disk.
    template<typename T>
    static void repDeleteAllUserpicInDisk(mg_connection *c, const http_message *msg, T *umm/*, zjy::OnResCallback respond*/);
    //delete all of user's picture in recycle bin.
    template<typename T>
    static void repDeleteAllInRecycle(mg_connection *c, const http_message *msg, T *umm/*, zjy::OnResCallback respond*/);
    //storage pic,if user upload pic ,call this function
    //this function will not used, HttpServer::HandleWebsocketMessage replace it
    template<typename T>
    static void repUploadpic(mg_connection *c, const http_message *msg, T *umm/*, zjy::OnResCallback respond*/);
    //download picture from server
    template<typename T>
    static void repDownloadpic(mg_connection *c, const http_message *msg, T *umm/*, zjy::OnResCallback respond*/);

    /*AliPay*/
    //get alipay url
    template<typename T>
    static void repGetURL(mg_connection *c, const http_message *msg, T *umm/*, zjy::OnResCallback respond*/);

    //parse order's para
    template<typename T>
    static void repParseOrder(mg_connection *c, const http_message *msg, T *umm/*, zjy::OnResCallback respond*/);


    //type: content-type    Lenth:message length   content:what message you want to send to html
    static std::string header(std::string type, std::string Lenth){
        std::string header{"HTTP/1.1 200 OK\r\nContent-Type: "+type+"\nCache-Control: no-cache\nContent-Length: "+Lenth+"\n"
                      "Access-Control-Allow-Origin: https://www.zjyxxn.cn\nAccess-Control-Allow-Credentials: true\n"
                      "Access-Control-Allow-Methods:GET,POST,PUT,POST\nAccess-Control-Allow-Headers:x-requested-with,content-type\n"};
        return header;
    }
    static std::string sendMsg(std::string type, std::string &content, bool cookie = false, std::string cookieValue=""){
        std::string all{};
        all += header(type, std::to_string(content.length()));
        if (cookie == true) {
            all += "Set-Cookie:phone=" + cookieValue + "; domain=www.zjyxxn.cn; path=/\n";
        }
        all += "\n";
        all += content;
        all += "\r\n";
        return all;
    }
    static std::string sendMsg(std::string type, std::string &&content, bool cookie = false, std::string cookieValue=""){
        std::string all{};
        all += header(type, std::to_string(content.length()));
        if (cookie == true) {
            all += "Set-Cookie: phone=" + cookieValue + "; domain=www.zjyxxn.cn; path=/\n";//domain=http:://localhost:443;
        }
        all += "\n";
        all += content;
        all += "\r\n";
        return all;
    }
    static std::string sendMsg_with_expires(std::string type, std::string &&content, std::string cookieValue=""){
        std::string all{};
        all += header(type, std::to_string(content.length()));
        all += "Set-Cookie: phone=" + cookieValue + "; domain=www.zjyxxn.cn; expires=0; path=/\n";
        all += "\n";
        all += content;
        all += "\r\n";
        return all;
    }

    static std::string getCookie(mg_connection *c, const http_message *msg){
        std::string phone="";
        try {
            struct mg_str *cookie_header;
            cookie_header=mg_get_http_header(const_cast<http_message*>(msg), "Cookie");    //获取Cookie 整条
            if(cookie_header == nullptr)
                throw "";
            std::string getCookie = std::string(cookie_header->p,cookie_header->len);
            auto pos = getCookie.find("phone=");
            if(pos!=std::string::npos){
                pos=pos+6;
                while (pos<getCookie.size()&&getCookie.at(pos)!=';') {
                    phone+=getCookie.at(pos);
                    pos++;
                }
            }
        } catch (...){
            LOG::ErrorLog(__FILE__,__LINE__,"other error");
        }
        return phone;
    }
};


template<typename T>
void DealHttpMsg::login(mg_connection *c, const http_message *msg, T * umm)
{
    char phone[20],pwd[20];
    memset(phone,0,20);
    memset(pwd,0,20);

    mg_get_http_var(&msg->query_string, "phone", phone, sizeof(phone));
    mg_get_http_var(&msg->query_string, "pwd", pwd, sizeof(pwd));

    //UserManageInterface* umm = new UserManageInterface(HttpServer::_db);
    int rtnValue = umm->login(std::string(phone), std::string(pwd));
    std::string sendmsg;
    switch (rtnValue) {
    case 0:
        LOG::SuccessLog(__FILE__, __LINE__, "login success");
        //send result to http,"0" means login success.
        sendmsg = DealHttpMsg::sendMsg(zjy::s_HttpType::PLAIN,"0", true, phone);
        mg_printf(c,"%s", sendmsg.c_str());

        HttpServer::s_session.insert(std::make_pair(phone, umm));
        break;
    case 1:
        LOG::ErrorLog(__FILE__, __LINE__, "password error");
        sendmsg = DealHttpMsg::sendMsg(zjy::s_HttpType::PLAIN,"1");
        mg_printf(c,"%s", sendmsg.c_str());
        umm->~UserManageInterface();
        break;
    case 2:
        LOG::ErrorLog(__FILE__, __LINE__, "this user isn't exit");
        sendmsg = DealHttpMsg::sendMsg(zjy::s_HttpType::PLAIN,"2");
        mg_printf(c,"%s", sendmsg.c_str());
        umm->~UserManageInterface();
        break;
    case 3:
        LOG::ErrorLog(__FILE__, __LINE__, "sql error");
        break;
    }
}

template<typename T>
void DealHttpMsg::closeBrower(mg_connection *c, const http_message *, T *umm)
{
    HttpServer::s_session.erase(umm->getPersonInfo().getPhone());
    HttpServer::_db->disConnect(umm->getMYSqlSocket());
    umm->~UserManageInterface();
}

template<typename T>
void DealHttpMsg::logoff(mg_connection *c, const http_message *msg, T *umm)
{
    HttpServer::s_session.erase(umm->getPersonInfo().getPhone());
    std::string sendmsg;
    sendmsg = DealHttpMsg::sendMsg_with_expires(zjy::s_HttpType::PLAIN,"success", umm->getPersonInfo().getPhone());
    mg_printf(c,"%s", sendmsg.c_str());
    HttpServer::_db->disConnect(umm->getMYSqlSocket());
    umm->~UserManageInterface();
}

template<typename T>
void DealHttpMsg::regist(mg_connection *c, const http_message *msg, T *umm)
{
    char phone[20],pwd[20],name[20];
    mg_get_http_var(&msg->query_string, "phone", phone, sizeof(phone));
    mg_get_http_var(&msg->query_string, "pwd", pwd, sizeof(pwd));
    mg_get_http_var(&msg->query_string, "name", name, sizeof(name));
    User user;
    user.setPhone(phone);
    user.setPassword(pwd);
    user.setName(name);
    int registNum=umm->regist(user);
    if(0==registNum){
        LOG::SuccessLog(__FILE__, __LINE__, "regist success");
        std::string sysMsg{"mkdir "+zjy::pic::picRoot::path + phone};
        system(sysMsg.c_str());
        std::string sendmsg{ DealHttpMsg::sendMsg(zjy::s_HttpType::PLAIN,"0")};
        mg_printf(c, "%s", sendmsg.c_str());
    } else if(1==registNum) {
        LOG::ErrorLog(__FILE__, __LINE__, "regist fail");
        std::string sendmsg{ DealHttpMsg::sendMsg(zjy::s_HttpType::PLAIN,"1")};
        mg_printf(c, "%s", sendmsg.c_str());
    }else{
        LOG::ErrorLog(__FILE__, __LINE__, "regist fail");
        std::string sendmsg{ DealHttpMsg::sendMsg(zjy::s_HttpType::PLAIN,"2")};
        mg_printf(c, "%s", sendmsg.c_str());
    }
}
template<typename T>
void DealHttpMsg::modifyPersonMsg(mg_connection *c, const http_message *msg, T *umm)
{
    char name[20],pwd[20],birthday[20],sex[20],age[20];
    memset(name,0,sizeof (name));
    memset(pwd,0,sizeof (pwd));
    memset(birthday,0,sizeof (birthday));
    memset(sex,0,sizeof (sex));
    memset(age,0,sizeof (age));
    mg_get_http_var(&msg->query_string, "name", name, sizeof(name));
    mg_get_http_var(&msg->query_string, "pwd", pwd, sizeof(pwd));
    mg_get_http_var(&msg->query_string, "birthday", birthday, sizeof(birthday));
    mg_get_http_var(&msg->query_string, "sex", sex, sizeof(sex));
    mg_get_http_var(&msg->query_string, "age", age, sizeof(age));
//    UserManageInterface *u=new UserManageInterface(HttpServer::_db);
//    char phone[20];
//    memset(&phone,0,sizeof(phone));
//    mg_get_http_var(&msg->query_string, "phone", phone, sizeof(phone));
    User user;
    memset(&user,0,sizeof(user));
    user.setPhone(umm->getPersonInfo().getPhone());
    user.setAge(std::atoi(age));
    user.setSex(std::atoi(sex));
    user.setName(name);
    user.setBirthday(birthday);
    user.setPassword(pwd);
    if(umm->modify(user)){
        LOG::SuccessLog(__FILE__, __LINE__, __func__);
        std::string sendmsg{ DealHttpMsg::sendMsg(zjy::s_HttpType::PLAIN,"0", true, user.getPhone())};
        mg_printf(c,"%s", sendmsg.c_str());
    } else {
        LOG::ErrorLog(__FILE__, __LINE__, "modify personal information error");
        std::string sendmsg{ DealHttpMsg::sendMsg(zjy::s_HttpType::PLAIN,"1")};
        mg_printf(c,"%s", sendmsg.c_str());
    }
}


template<typename T>
void DealHttpMsg::repSendPersonalInfo(mg_connection *c, const http_message *msg, T *umm)
{
    char phone[20];
    mg_get_http_var(&msg->query_string, "phone", phone, sizeof(phone));
    User user{umm->getPersonInfo()};
    std::string repStr = JSON::usertoJson(&user);
    std::string sendmsg{ DealHttpMsg::sendMsg(zjy::s_HttpType::JSON, repStr,false) };
    mg_printf(c,"%s", sendmsg.c_str());
    LOG::SuccessLog(__FILE__, __LINE__, __func__);
}

template<typename T>
void DealHttpMsg::repSendVIPLevel(mg_connection *c, const http_message *msg, T *umm)
{
    std::string phone = getCookie(c,msg);
    std::string sendmsg{ DealHttpMsg::sendMsg(zjy::s_HttpType::PLAIN, std::to_string(umm->getVipLevel()),false) };
    mg_printf(c,"%s", sendmsg.c_str());
    LOG::SuccessLog(__FILE__, __LINE__, __func__);
}

template<typename T>
void DealHttpMsg::repSendPicNameByType(mg_connection *c, const http_message *msg, T *umm)
{
    std::string sendMsg;
    char typeValue[5];
    mg_get_http_var(&msg->query_string, "type", typeValue,sizeof(typeValue));
    int type = atoi(typeValue);
    std::list<std::string> slist;
    switch (type) {
    case -1:
        slist = umm->imagesPathList(zjy::pic::picSortType::Recycler);
        break;
    case 0:
        slist = umm->imagesPathList(zjy::pic::picSortType::None);
        break;
    case 1:
        slist = umm->imagesPathList(zjy::pic::picSortType::Time);
        break;
    case 3:
        slist = umm->imagesPathList(zjy::pic::picSortType::Image);
        break;
    case 4:
        slist = umm->imagesPathList(zjy::pic::picSortType::Other);
        break;
    default:
        LOG::ErrorLog(__FILE__,__LINE__,"html send error type");
    }

    std::string json{ JSON::stringListToJson(slist) };
    std::string sendmsg{ DealHttpMsg::sendMsg(zjy::s_HttpType::JSON, json) };
    mg_printf(c, "%s", sendmsg.c_str());
}

template<typename T>
void DealHttpMsg::repSendPicNameByCustom(mg_connection *c, const http_message *msg, T *umm)
{

    std::string sendMsg;
    char typeValue[20];
    memset(typeValue,0,20);
    mg_get_http_var(&msg->query_string, "type", typeValue,sizeof(typeValue));
    std::list<std::string> slist;
    slist = umm->imagesPathListCustom(typeValue);
    std::string json{ JSON::stringListToJson(slist) };
    std::string sendmsg{ DealHttpMsg::sendMsg(zjy::s_HttpType::JSON, json) };
    mg_printf(c, "%s", sendmsg.c_str());
}

template<typename T>
void DealHttpMsg::repSendpicData(mg_connection *c, const http_message *msg, T *umm/*, zjy::OnResCallback respond*/)
{
    char picName[50];
    memset(picName,0,50);
    mg_get_http_var(&msg->query_string, "pic", picName, sizeof(picName));
    std::string picNamePath{};
    picNamePath += zjy::pic::picRoot::path;
    std::string logmsg{"get picture name: "};
//    logmsg += std::to_string(umm->getPersonInfo().getPhone());
//    logmsg += "/";
    picNamePath += getCookie(c,msg) + "/" +picName;
    logmsg += picNamePath;
    LOG::SuccessLog(__FILE__,__LINE__,logmsg);

    std::ifstream f;
    f.open(picNamePath, std::ios_base::binary | std::ios_base::in);
    f.seekg(0, std::ifstream::end);
    ssize_t len = f.tellg();
    f.seekg(0, std::ifstream::beg);
    if(len<=0){
        std::string sendPicMsg{ DealHttpMsg::sendMsg(zjy::s_HttpType::JPEG,"")};
        mg_send(c, sendPicMsg.c_str(), static_cast<int>(sendPicMsg.size()));
        f.close();
        LOG::ErrorLog(__FILE__, __LINE__, "picture data is lesser or equal than 0");
        return;
    }
    char *str = new char[len];
    memset(str, 0, static_cast<size_t>(len));
    f.read(str, len);
    std::string cpPic(str, static_cast<size_t>(len));
    std::string sendPicMsg{ DealHttpMsg::sendMsg(zjy::s_HttpType::JPEG,cpPic)};
    int mid=1;
    if(sendPicMsg.size() < static_cast<unsigned long>(((mid<<31)-1))){
        mg_send(c, sendPicMsg.c_str(), static_cast<int>(sendPicMsg.size()));
    }
    else{
        //to deal with the picture data size > max(int)
    }
    f.close();
    delete [] str;

}

template<typename T = UserManageInterface>
void DealHttpMsg::repMovePicToRecycle(mg_connection *c, const http_message *msg, T *umm)
{
    char picName[30];
    memset(picName,0,30);
    mg_get_http_var(&msg->query_string, "picName", picName, sizeof(picName));
    std::string picNamePath;
    picNamePath += zjy::pic::picRoot::path;
    std::string user=getCookie(c,msg);
    picNamePath += user + picName;
    std::string logmsg{"move to recycle's picture name: "};
    logmsg += picNamePath;
    LOG::SuccessLog(__FILE__, __LINE__, logmsg);
    bool isMove = umm->moveToRecycler(picName);
    if( isMove ){
        LOG::SuccessLog(__FILE__, __LINE__, "move to recycle");
        std::string sendmsg{ DealHttpMsg::sendMsg(zjy::s_HttpType::PLAIN,"success")};
        mg_printf(c, "%s", sendmsg.c_str());
    } else {
        LOG::ErrorLog(__FILE__, __LINE__, "can't move to recycle");
        std::string sendmsg{ DealHttpMsg::sendMsg(zjy::s_HttpType::PLAIN,"failed")};
        mg_printf(c, "%s", sendmsg.c_str());
    }
}

template<typename T = UserManageInterface>
void DealHttpMsg::repRecoverPicToRecycle(mg_connection *c, const http_message *msg, T *umm)
{
    char picName[30];
    memset(picName,0,30);
    mg_get_http_var(&msg->query_string, "picName", picName, sizeof(picName));
    std::string picNamePath;
    picNamePath += zjy::pic::picRoot::path;
    std::string user=getCookie(c,msg);
    picNamePath += user + picName;
    std::string logmsg{"recover picture's name: "};
    logmsg += picNamePath;
    LOG::SuccessLog(__FILE__, __LINE__, logmsg);
    bool isRecover = umm->recoverFromRecycler(picName);
    if( isRecover ){
        LOG::SuccessLog(__FILE__, __LINE__, "recover from recycle");
        std::string sendmsg{ DealHttpMsg::sendMsg(zjy::s_HttpType::PLAIN,"success")};
        mg_printf(c, "%s", sendmsg.c_str());
    } else {
        LOG::ErrorLog(__FILE__, __LINE__, "recover from recycle");
        std::string sendmsg{ DealHttpMsg::sendMsg(zjy::s_HttpType::PLAIN,"failed")};
        mg_printf(c, "%s", sendmsg.c_str());
    }
}

template<typename T>
void DealHttpMsg::repDeletepicInDisk(mg_connection *c, const http_message *msg, T *umm)
{
    char picName[30];
    memset(picName,0,30);
    mg_get_http_var(&msg->query_string, "picName", picName, sizeof(picName));
    std::string picNamePath;
    picNamePath += zjy::pic::picRoot::path;
    picNamePath + picName;
    std::string logmsg{"get picture name: "};
    logmsg += picName;
    LOG::SuccessLog(__FILE__, __LINE__, logmsg);
    bool isDelete = umm->delOnePic(picName);
    if( isDelete ){
        LOG::SuccessLog(__FILE__, __LINE__, "delete from disk");
        std::string sendmsg{ DealHttpMsg::sendMsg(zjy::s_HttpType::PLAIN,"success")};
        mg_printf(c, "%s", sendmsg.c_str());
    } else {
        LOG::ErrorLog(__FILE__, __LINE__, "can't delete from disk");
        std::string sendmsg{ DealHttpMsg::sendMsg(zjy::s_HttpType::PLAIN,"failed")};
        mg_printf(c, "%s", sendmsg.c_str());
    }
}

template<typename T>
void DealHttpMsg::repDeleteAllUserpicInDisk(mg_connection *c, const http_message */*msg*/, T *umm)
{
    bool isDelete = umm->delAllPic();
    if( isDelete ){
        LOG::SuccessLog(__FILE__, __LINE__, "delete all pic from disk");
        std::string sendmsg{ DealHttpMsg::sendMsg(zjy::s_HttpType::PLAIN,"success")};
        mg_printf(c, "%s", sendmsg.c_str());
    } else {
        LOG::ErrorLog(__FILE__, __LINE__, "can't all pic delete from disk");
        std::string sendmsg{ DealHttpMsg::sendMsg(zjy::s_HttpType::PLAIN,"failed")};
        mg_printf(c, "%s", sendmsg.c_str());
    }
}

template<typename T>
void DealHttpMsg::repUploadpic(mg_connection *c, const http_message *msg, T *umm/*, zjy::OnResCallback respond*/)
{
//    http_message* h = const_cast<http_message*>(msg);
//    websocket_message* ws = reinterpret_cast<websocket_message*>(h);
}

template<typename T>
void DealHttpMsg::repDownloadpic(mg_connection *c, const http_message *msg, T *umm/*, zjy::OnResCallback respond*/)
{
    char picName[30],type[10];
    memset(picName,0,30);
    memset(type,0,10);
    mg_get_http_var(&msg->query_string, "pic", picName, sizeof(picName));
    mg_get_http_var(&msg->query_string, "type", type, sizeof(type));
    std::string sendPicMsg{};
    cv::Mat pic=cv::imread(zjy::pic::picRoot::path + getCookie(c,msg)+ "/"+ picName);
    if(type==zjy::pic::picFormatSuffix::JPG){
        std::string picDst = ImageBaseTool::matToBinary(pic,zjy::pic::picFormatSuffix::JPG);
        sendPicMsg = DealHttpMsg::sendMsg(zjy::s_HttpType::JPEG,picDst);
    }else if(type == zjy::pic::picFormatSuffix::JPEG){
        std::string picDst = ImageBaseTool::matToBinary(pic,zjy::pic::picFormatSuffix::JPEG);
        sendPicMsg = DealHttpMsg::sendMsg(zjy::s_HttpType::JPEG,picDst);
    }else if(type == zjy::pic::picFormatSuffix::PNG){
        std::string picDst = ImageBaseTool::matToBinary(pic,zjy::pic::picFormatSuffix::PNG);
        sendPicMsg =DealHttpMsg::sendMsg(zjy::s_HttpType::PNG,picDst);
    }else if(type == zjy::pic::picFormatSuffix::TIFF){
        std::string picDst = ImageBaseTool::matToBinary(pic,zjy::pic::picFormatSuffix::TIFF);
        //sendPicMsg =DealHttpMsg::sendMsg(zjy::s_HttpType::GIF,picDst);
    }else{

    }
    int mid=1;
    if(sendPicMsg.size() < static_cast<unsigned long>(((mid<<31)-1))){
        mg_send(c, sendPicMsg.c_str(), static_cast<int>(sendPicMsg.size()));
    }
    else{
        //to deal with the picture data size > max(int)
    }
}

template<typename T>
void DealHttpMsg::repDeleteAllInRecycle(mg_connection *c, const http_message *msg, T *umm)
{
    std::string picNamePath;
    picNamePath += zjy::pic::picRoot::path;
    std::string user=getCookie(c,msg);
    picNamePath += user +"/";
    std::list<std::string> slist;
    slist = umm->imagesPathList(zjy::pic::picSortType::Recycler);
    for(std::string pic:slist){
        std::string logmsg{"get picture name: "};
        logmsg += picNamePath+pic;
        LOG::SuccessLog(__FILE__, __LINE__, logmsg);
        bool isDelete = umm->delOnePic(pic);
        LOG::testLog(pic);
        if( isDelete ){
            LOG::SuccessLog(__FILE__, __LINE__, "delete from disk");
        } else {
            LOG::ErrorLog(__FILE__, __LINE__, "can't delete from disk");
        }
    }
    LOG::SuccessLog(__FILE__, __LINE__, "move to recycle");
    std::string sendmsg{ DealHttpMsg::sendMsg(zjy::s_HttpType::PLAIN,"success")};
    mg_printf(c, "%s", sendmsg.c_str());

}

template<typename T>
void DealHttpMsg::repGetURL(mg_connection *c, const http_message *msg, T *umm/*, zjy::OnResCallback respond*/)
{
    User user = umm->getPersonInfo();
    std::string url=Pay::getAlipay(user.getPhone());
    std::string sendmsg=DealHttpMsg::sendMsg(zjy::s_HttpType::PLAIN,url);
    mg_printf(c, "%s", sendmsg.c_str());
}

template<typename T>
void DealHttpMsg::repParseOrder(mg_connection *c, const http_message *msg, T *umm/*, zjy::OnResCallback respond*/)
{
    char order[30];
    memset(order,0,30);
    mg_get_http_var(&msg->query_string, "out_trade_no", order, sizeof(order));
    std::string url;
    url.empty();
    const char* pos=msg->uri.p;
    while(*pos!='\0' && *pos!=' '){
        url.push_back(*pos);
        pos++;
    }
    std::string isPay{Pay::parseURL(url)};
    if(isPay=="True"){
        std::string phone;
        for(int i=14;i<25;i++){
            phone.push_back(order[i]);
        }
        if(umm->getPersonInfo().getPhone()==phone){
            if(umm->setVIPLevel()==0){
                LOG::SuccessLog(__FILE__,__LINE__,"set vip level success");
                std::string html;
                html=std::string("<!DOCTYPE html>")+"<html lang=\"en\">"+
                                       "<head>"+
                                       "    <meta charset=\"UTF-8\">"+
                                       "    <title>支付结果</title>"+
                                       "    <script>"+
                                       "        function redic() {"+
                                       "            console.debug(\"支付成功\")"+
                                       "            window.location.href=\"index.html\""+
                                       "        }"+
                                       "    </script>"+
                                       "</head>"+
                                       "<body>"+
                                       "<a href=\"https://www.zjyxxn.cn/mainPage/index.html\">支付成功，点击返回</a>"+
                                       "</body>"+
                                       "</html>";
                std::string sendmsg=DealHttpMsg::sendMsg(zjy::s_HttpType::HTML,html);
                mg_printf(c, "%s", sendmsg.c_str());
            }else{
                LOG::ErrorLog(__FILE__,__LINE__,"set vip level false");
            }
        }else{
            LOG::ErrorLog(__FILE__,__LINE__,"about cookie's bug");
        }

    }else{

    }
}



#endif // SENDHTTPMSG_H
