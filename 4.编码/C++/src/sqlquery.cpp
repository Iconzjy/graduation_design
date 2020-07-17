#include "sqlquery.h"

void SQLQuery::managePersonalMessage(MYSQL *aConn, void *rtnValue, User aUser)
{
    std::string query_sql{"update user set "};
    if(!aUser.getName().empty()){
        query_sql += "name='";
        query_sql += aUser.getName();
    }
    if(!aUser.getPassword().empty()){
        query_sql += "', password='";
        query_sql += aUser.getPassword();
    }
    query_sql += "', age='";
    query_sql += std::to_string(aUser.getAge());
    query_sql += "', sex='";
    query_sql += std::to_string(aUser.getSex());
    if(!aUser.getBirthday().empty()){
        query_sql += "', birthday='";
        query_sql += aUser.getBirthday();
    }
    query_sql += "' where phone='";
    if(!aUser.getPhone().empty()){
        query_sql += aUser.getPhone();
    }
    query_sql += "'";
    int res = mysql_real_query(aConn, query_sql.c_str(), query_sql.size());
    if (res == 0) {
        int* value = reinterpret_cast<int*>(rtnValue);
        *value = 0;
        LOG::SuccessLog(__FILE__,__LINE__,"update success");
    } else {
        LOG::ErrorLog(__FILE__,__LINE__,mysql_error(aConn));
        int* value = reinterpret_cast<int*>(rtnValue);
        *value = 1;
    }
}

void SQLQuery::regist(MYSQL *aConn, void *rtnValue, User aUser)
{
    std::string query_sql;
    query_sql = "insert into user (`name`, `phone`, `password`) VALUES ('"+aUser.getName()+"', '"+aUser.getPhone()+"', '"+aUser.getPassword()+"')";
    int res = mysql_real_query(aConn,query_sql.c_str(),query_sql.size());
    if(!res){
        std::string msg{"insert success. "};
        msg += "insert" + std::to_string(mysql_affected_rows(aConn))+"row";
        LOG::SuccessLog(__FILE__,__LINE__,"insert success");
        int* value = reinterpret_cast<int*>(rtnValue);
        *value = 0;
    } else {
        LOG::ErrorLog(__FILE__,__LINE__, "insert into user error");
        int* value = reinterpret_cast<int*>(rtnValue);

        std::string s="select * from user where phone="+aUser.getPhone();
        mysql_real_query(aConn,s.c_str(),s.size());
        MYSQL_RES* res = mysql_store_result(aConn);
        if(mysql_affected_rows(aConn)==1){
            mysql_free_result(res);
            *value = 1;//user exist
        }else{
            mysql_free_result(res);
            *value = 2;//other error
        }
    }
}


void SQLQuery::login(MYSQL *aConn, void *rtnValue, std::string aPhone, std::string aPwd)
{
    std::string query_sql;
    query_sql = "select password from user where phone="+aPhone;
    mysql_query(aConn, "set names gbk");
    if (0!=mysql_query(aConn,query_sql.c_str())){
        LOG::ErrorLog(__FILE__, __LINE__ , mysql_error(aConn));
        int* value = reinterpret_cast<int*>(rtnValue);
        *value = 3;//other error
        return;
    }
    MYSQL_RES* res = mysql_store_result(aConn);
    if(mysql_affected_rows(aConn)==0){
        LOG::ErrorLog(__FILE__, __LINE__, "this user isn't exit");
        int* value = reinterpret_cast<int*>(rtnValue);
        *value = 2;//user not exit
        mysql_free_result(res);
        return;
    }
    MYSQL_ROW col_res = mysql_fetch_row(res);
    if( strcmp(col_res[0], aPwd.c_str()) == 0) {
        int* value = reinterpret_cast<int*>(rtnValue);
        *value = 0;
    } else {
        LOG::ErrorLog(__FILE__, __LINE__, "password error");
        int* value = reinterpret_cast<int*>(rtnValue);
        *value = 1;
    }
    mysql_free_result(res);
}

void SQLQuery::logOff(MYSQL *aConn, void *rtnValue, std::string aPhone, std::string aPwd)
{
    int* value = reinterpret_cast<int*>(rtnValue);
    *value = 1;
    SQLQuery::login(aConn, value, aPhone, aPwd);
    if(*value != 0){
        LOG::ErrorLog(__FILE__, __LINE__, "logoff fail cause the password error");
        return;
    }
    //delete the root where storage user's images
    std::string order{"sudo rm -R "+zjy::pic::picRoot::path + aPhone};
    system(order.c_str());
    std::string query_sql{"delete from imagepath where userID='" + aPhone + "'"};
    if(mysql_query(aConn,query_sql.c_str())) {
        std::string errmsg{"delete picture path in database(imagepath) false. "};
        errmsg += mysql_error(aConn);
        LOG::ErrorLog(__FILE__, __LINE__, errmsg.c_str());
    } else {
        LOG::SuccessLog(__FILE__, __LINE__, "delete picture path in database(imagepath) success");
    }
    query_sql.clear();
    query_sql="delete from user where phone='" + aPhone + "'";
    if(mysql_query(aConn,query_sql.c_str())) {
        std::string errmsg{"[Error]: delete picture path in database(user) false"};
        errmsg += mysql_error(aConn);
        LOG::ErrorLog(__FILE__, __LINE__, errmsg.c_str());
        *value = 1;
    } else {
        LOG::SuccessLog(__FILE__, __LINE__, "delete picture path in database(user) success");
        *value = 0;
    }
}

void SQLQuery::getPersonInfo(MYSQL *aConn, void *rtnValue, std::string aPhone)
{
    User *u = reinterpret_cast<User*>(rtnValue);

    std::string query_sql{"select * from user where phone="};
    query_sql += aPhone;
    mysql_query(aConn, "set names gbk");
    if(mysql_query(aConn, query_sql.c_str())) {
        LOG::ErrorLog(__FILE__, __LINE__, mysql_error(aConn));
        return;
    } else {
        LOG::SuccessLog(__FILE__, __LINE__, "get personal information");
    }
    MYSQL_RES* res = mysql_store_result(aConn);
    MYSQL_ROW col_res = mysql_fetch_row(res);
    if(col_res[0])
        u->setName(col_res[0]);
    else
        u->setName("null");
    if(col_res[1])
        u->setPhone(col_res[1]);
    if(col_res[2])
        u->setPassword(col_res[2]);
    if(col_res[3])
        u->setBirthday(col_res[3]);
    else
        u->setBirthday("null");
    if(col_res[4])
        u->setVIPLevel(std::stoi(col_res[4]));
    else
        u->setVIPLevel(0);
    if(col_res[5])
        u->setSex(std::stoi(col_res[5]));
    else
        u->setSex(-1);
    if(col_res[6])
        u->setAge(std::stoi(col_res[6]));
    else
        u->setAge(-1);
}

void SQLQuery::addImage(MYSQL *aConn, void *rtnValue, std::string phone, std::string describe, int count, std::string format)
{
    //return picture name
    time_t t = time(nullptr);
    char ch[64] = {0};
    strftime(ch, sizeof(ch) - 1, "%Y%m%d%H%M%S", localtime(&t));
    std::string time{ch};
    time += std::to_string(count);
    std::string query_sql{"insert into imagepath (`userID`, `path`, `isRecyclers`, `isImage`, `update`, `describe`) VALUES "
                          "('"+phone+"', '" + time + format + "', DEFAULT, DEFAULT, '" + time + "', '" + describe + "')"};
    int res = mysql_real_query(aConn,query_sql.c_str(),query_sql.size());
    if(!res){
        std::string msg{"insert success. "};
        msg += "affect " + std::to_string(mysql_affected_rows(aConn))+" row";
        LOG::SuccessLog(__FILE__, __LINE__, msg.c_str());
//        char* value = reinterpret_cast<char*>(rtnValue);
        time+=format;
        memcpy(rtnValue,time.c_str(),time.size());
    } else {
        LOG::ErrorLog(__FILE__, __LINE__, "insert image into user error");
//        int* value = reinterpret_cast<int*>(rtnValue);
        char err[3]{"-1"};
        memcpy(rtnValue, err, sizeof(err));
    }
}

void SQLQuery::imagesPathList(MYSQL *aConn, void *rtnValue, std::string phone, zjy::pic::picSortType type)
{
    //this para means pic sort type
    std::string type_sql{""};
    switch (type) {
    case zjy::pic::picSortType::None :
        type_sql =" and isRecyclers = '0'";
        break;
    case zjy::pic::picSortType::Time :
        type_sql += " and isRecyclers = '0' order by `update`";
        break;
    case zjy::pic::picSortType::Recycler :
        type_sql += " and isRecyclers = '1'";
        break;
    case zjy::pic::picSortType::Image :
        type_sql += " and isRecyclers = '0' and isImage = '1'";
        break;
    default:
        break;
    }
    std::list<std::string>* pathList = reinterpret_cast<std::list<std::string>*>(rtnValue);
    std::string query_sql;
    query_sql = "select path from imagepath where userID='";
    query_sql += phone;
    query_sql += "'";
    query_sql += type_sql;
    if(mysql_query(aConn, query_sql.c_str())) {
        std::string errmsg{"[Error]: select imagesNameList false"};
        errmsg += mysql_error(aConn);
        LOG::ErrorLog(__FILE__, __LINE__, errmsg.c_str());
    } else {
        LOG::SuccessLog(__FILE__, __LINE__, "select imagesNameList success");
    }
    MYSQL_RES* res = mysql_store_result(aConn);
    MYSQL_ROW col_res;
    while ((col_res = mysql_fetch_row(res))) {
        pathList->push_back(col_res[0]);
    }
}

void SQLQuery::imagesPathListCustom(MYSQL *aConn, void *rtnValue, std::string phone)
{
    std::list<std::pair<std::string,std::string>>* pathList = reinterpret_cast<std::list<std::pair<std::string,std::string>>*>(rtnValue);
    std::string query_sql;
    query_sql = "select `path`,`describe` from imagepath where userID='";
    query_sql += phone;
    query_sql += "'";
    if(mysql_query(aConn, query_sql.c_str())) {
        std::string errmsg{"[Error]: select imagesNameList false"};
        errmsg += mysql_error(aConn);
        LOG::ErrorLog(__FILE__, __LINE__, errmsg.c_str());
    } else {
        LOG::SuccessLog(__FILE__, __LINE__, "select imagesNameList success");
    }
    MYSQL_RES* res = mysql_store_result(aConn);
    MYSQL_ROW col_res;
    while ((col_res = mysql_fetch_row(res))) {
        if(!std::string(col_res[1]).empty()&&std::string(col_res[1])!="{}"){
            pathList->push_back(std::make_pair(col_res[0],col_res[1]));
        }
    }
}

void SQLQuery::moveToRecycler(MYSQL *aConn, void *rtnValue, std::string phone, std::string picName)
{
    std::string query_sql{"update imagepath set isRecyclers='1' where userID='" + phone + "' and path='"+picName+"'"};
    int res = mysql_query(aConn, query_sql.c_str());
    if(!res){
        LOG::SuccessLog(__FILE__, __LINE__, "move picture to recycler.");
        int* value = reinterpret_cast<int*>(rtnValue);
        *value = 0;
    } else {
        std::string errmsg{"move picture to recycler failed. "};
        errmsg += mysql_error(aConn);
        LOG::ErrorLog(__FILE__, __LINE__, errmsg.c_str());
        int* value = reinterpret_cast<int*>(rtnValue);
        *value = 1;
    }
}

void SQLQuery::recoverFromRecycler(MYSQL *aConn, void *rtnValue, std::string phone, std::string picName)
{
    std::string query_sql{"update imagepath set isRecyclers='0' where userID='" + phone + "' and path='"+picName+"'"};
    int res = mysql_query(aConn, query_sql.c_str());
    if(!res){
        LOG::SuccessLog(__FILE__, __LINE__, "recover picture from recycler.");
        int* value = reinterpret_cast<int*>(rtnValue);
        *value = 0;
    } else {
        std::string errmsg{"recover picture from recycler failed. "};
        errmsg += mysql_error(aConn);
        LOG::ErrorLog(__FILE__, __LINE__, errmsg.c_str());
        int* value = reinterpret_cast<int*>(rtnValue);
        *value = 1;
    }
}

void SQLQuery::delOnePicInDB(MYSQL *aConn, void *rtnValue, std::string phone, std::string imagefile)
{
    std::string query_sql{"delete from imagepath where userID='" + phone + "' and path='" + imagefile+"'"};
    int res = mysql_query(aConn, query_sql.c_str());
    if(!res){
	    LOG::SuccessLog(__FILE__,__LINE__,"[Success]: delete picture in database.");
//        std::string picpath{zjy::pic::picRoot::path + path};
//        if(0 == remove(picpath.c_str())){
//            LOG::SuccessLog(__FILE__, __LINE__, "delete picture in memory.");
//        }
        int* value = reinterpret_cast<int*>(rtnValue);
        *value = 0;
    } else {
        std::string errmsg{"[Error]: delete picture failed. "};
        errmsg += mysql_error(aConn);
        LOG::ErrorLog(__FILE__, __LINE__, errmsg.c_str());
        int* value = reinterpret_cast<int*>(rtnValue);
        *value = 1;
    }
}

void SQLQuery::delAllPicInDB(MYSQL *aConn, void *rtnValue, std::string phone)
{
    std::string query_sql{"delete from imagepath where phone='" + phone +"'"};
    int res = mysql_query(aConn, query_sql.c_str());
    if(!res){
        LOG::SuccessLog(__FILE__, __LINE__, "delete all picture in database.");
    } else {
        std::string errmsg{"[Error]: delete picture to recycler failed . "};
        errmsg += mysql_error(aConn);
        LOG::ErrorLog(__FILE__, __LINE__, errmsg.c_str());
    }

//    std::vector<std::string> files;//storage file name
//    std::string cate_dir{zjy::pic::picRoot::path + phone + "/"};
//    DIR *dir;
//    struct dirent *ptr;

//    if ((dir=opendir(cate_dir.c_str())) == nullptr) {
//        LOG::ErrorLog(__FILE__, __LINE__, "Open dir error...");
//        int* value = reinterpret_cast<int*>(rtnValue);
//        *value = 1;
//    }

//    while ((ptr=readdir(dir)) != nullptr)
//    {
//        std::string filename{ptr->d_name};
//        if(filename.find(".jpeg")!=std::string::npos){
//            files.push_back(filename);
//        }
//    }
//    closedir(dir);
//    std::vector<std::string>::iterator beg=files.begin();
//    auto pos=beg;
//    std::vector<std::string>::iterator end=files.end();
//    LOG("delete all picture in memory");
//    while(pos != end){
//        std::string f{cate_dir+(*pos)};
//        if(0 == remove(f.c_str())){
//            std::string message{"   delete picture "+(*pos)};
//            LOG::SuccessLog(__FILE__, __LINE__, message.c_str());
//            int* value = reinterpret_cast<int*>(rtnValue);
//            *value = 0;
//        }else{
//            std::string message{"   delete pic failed "+(*pos)};
//            LOG::ErrorLog(__FILE__, __LINE__, message.c_str());
//            int* value = reinterpret_cast<int*>(rtnValue);
//            *value = 1;
//            break;
//        }
//        pos++;
//    }
}

void SQLQuery::getVIPLvel(MYSQL *aConn, void *rtnValue, std::string phone)
{
    int* vip = nullptr;
    vip = reinterpret_cast<int*>(rtnValue);

    std::string query_sql{"select VIPLevel from user where phone="};
    query_sql += phone;
    mysql_query(aConn, "set names gbk");
    if(mysql_query(aConn, query_sql.c_str())) {
        LOG::ErrorLog(__FILE__, __LINE__, mysql_error(aConn));
    } else {
        LOG::SuccessLog(__FILE__, __LINE__, "get personal information");
    }
    MYSQL_RES* res = mysql_store_result(aConn);
    MYSQL_ROW col_res = mysql_fetch_row(res);
    if(col_res[0]){
        *vip = std::stoi(col_res[0]);
    } else {
        LOG::ErrorLog(__FILE__, __LINE__, mysql_error(aConn));
    }
}

void SQLQuery::setVIPLvel(MYSQL *aConn, void *rtnValue, std::string phone)
{
    int* vip = nullptr;
    vip = reinterpret_cast<int*>(rtnValue);

    std::string query_sql{"update user set VIPLevel=1 where phone='"};
    query_sql += phone;
    query_sql += "'";
    mysql_query(aConn, "set names gbk");

    int res = mysql_real_query(aConn, query_sql.c_str(), query_sql.size());
    if (res == 0) {
        int* value = reinterpret_cast<int*>(rtnValue);
        *value = 0;
        LOG::SuccessLog(__FILE__,__LINE__,"update success");
    } else {
        LOG::ErrorLog(__FILE__,__LINE__,mysql_error(aConn));
        int* value = reinterpret_cast<int*>(rtnValue);
        *value = 1;
    }
}

void SQLQuery::setImageDescribe(MYSQL *aConn, void *rtnValue, std::string phone, std::string describe, std::string imagefile)
{
    std::string query_sql{"update imagepath set `describe`='" + describe + "' where `userID`='" + phone + "' and `path`='" + imagefile+"'"};
    int res = mysql_query(aConn, query_sql.c_str());
    if(!res){
        LOG::SuccessLog(__FILE__, __LINE__, "update describe success.");
        int* value = reinterpret_cast<int*>(rtnValue);
        *value = 0;
    } else {
        std::string errmsg{"update describe failed. "};
        errmsg += mysql_error(aConn);
        LOG::ErrorLog(__FILE__, __LINE__, errmsg.c_str());
        int* value = reinterpret_cast<int*>(rtnValue);
        *value=1;
    }
}
