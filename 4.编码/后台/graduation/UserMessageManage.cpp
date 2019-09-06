#include "UserMessageManage.h"

UserMessageManage::UserMessageManage()
    :_childConn(rtnMYSQL())
{

}

UserMessageManage::~UserMessageManage()
{

}

bool UserMessageManage::regist(User aUser)
{
    std::string query_sql;
    query_sql = "insert into user (`name`, `phone`, `password`) VALUES ('"+aUser.getName()+"', '"+aUser.getPhone()+"', '"+aUser.getPassword()+"')";
    int res = mysql_real_query(_childConn,query_sql.c_str(),query_sql.size());
    if(!res){
        std::string msg{"insert success. "};
        msg += "insert" + std::to_string(mysql_affected_rows(_childConn))+"row";
        LOG("insert success");
        return true;
    } else {
        LOG("[Error]: insert into user error");
        return false;
    }
}

bool UserMessageManage::login(string aPhone, string aPwd)
{
    std::string query_sql;
    query_sql = "select password from user where phone="+aPhone;
    mysql_query(this->_childConn, "set names gbk");
    if (mysql_query(this->_childConn,query_sql.c_str())){
        LOG(mysql_error(this->_childConn));
    } else {
        LOG("login query success");
    }
    MYSQL_RES* res = mysql_store_result(this->_childConn);
    MYSQL_ROW col_res = mysql_fetch_row(res);
    if( strcmp(col_res[0],aPwd.c_str()) == 0) {
        return true;
    } else {
        LOG("this count isn't exit");
        return false;
    }
}

bool UserMessageManage::logOff(string aPhone, string aPwd)
{
    //delete the root where storage user's images
    std::string order{"sudo rm -R "+zjy::picRoot::path + aPhone};
    system(order.c_str());
    std::string query_sql{"delete from imagepath where userID='" + aPhone + "'"};
    if(mysql_query(this->_childConn,query_sql.c_str())) {
        std::string errmsg{"[Error]: delete picture path in database(imagepath) false"};
        errmsg += mysql_error(this->_childConn);
        LOG(errmsg.c_str());
    } else {
        LOG("[Success]: delete picture path in database(imagepath) success");
    }
    query_sql.clear();
    query_sql="delete from user where phone='" + aPhone + "'";
    if(mysql_query(this->_childConn,query_sql.c_str())) {
        std::string errmsg{"[Error]: delete picture path in database(user) false"};
        errmsg += mysql_error(this->_childConn);
        LOG(errmsg.c_str());
    } else {
        LOG("[Success]: delete picture path in database(user) success");
    }
}

bool UserMessageManage::addImage(string phone, std::string describe, int count)
{
    time_t t = time(nullptr);
    char ch[64] = {0};
    strftime(ch, sizeof(ch) - 1, "%Y%m%d%H%M%S", localtime(&t));
    std::string time{ch};
    time += std::to_string(count);
    std::string query_sql{"insert into imagepath (`userID`, `path`, `isRecyclers`, `isImage`, `update`, `describe`) VALUES "
                          "('"+phone+"', '" +phone +"/"+ time + ".jpeg" + "', DEFAULT, DEFAULT, '" + time + "', '" + describe + "')"};
    int res = mysql_real_query(_childConn,query_sql.c_str(),query_sql.size());
    if(!res){
        std::string msg{"insert success. "};
        msg += "affect " + std::to_string(mysql_affected_rows(_childConn))+" row";
        LOG(msg.c_str());
        return true;
    } else {
        LOG("[Error]: insert into user error");
        return false;
    }
}


std::list<string> UserMessageManage::imagesPathList(string phone, sortType type)
{
    std::list<std::string> pathList;
    std::string query_sql;
    query_sql = "select path from imagepath where userID='";
    query_sql += phone;
    query_sql +="'";
    if(mysql_query(this->_childConn,query_sql.c_str())) {
        std::string errmsg{"[Error]: select imagesNameList false"};
        errmsg += mysql_error(this->_childConn);
        LOG(errmsg.c_str());
    } else {
        LOG("[Success]: select imagesNameList success");
    }
    MYSQL_RES* res = mysql_store_result(this->_childConn);
    MYSQL_ROW col_res;
    while ((col_res = mysql_fetch_row(res))) {
        pathList.push_back(col_res[0]);
    }
    return pathList;
}

bool UserMessageManage::moveToRecycler(string phone, string path)
{
    std::string query_sql{"update imagepath set path='" + path + "' where phone='" + phone + "'"};
    int res = mysql_query(this->_childConn, query_sql.c_str());
    if(!res){
        LOG("[Success]: move picture to recycler.");
        return true;
    } else {
        std::string errmsg{"[Error]: move picture to recycler failed. "};
        errmsg += mysql_error(this->_childConn);
        LOG(errmsg.c_str());
        return false;
    }
}

bool UserMessageManage::delOnePic(string path)
{

    std::string query_sql{"delete from imagepath where path='" + path +"'"};
    int res = mysql_query(this->_childConn, query_sql.c_str());
    if(!res){
        LOG("[Success]: delete picture in database.");

        std::string picpath{zjy::picRoot::path + path};
        if(0 == remove(picpath.c_str())){
            LOG("[Success]: delete picture in memory.");
        }
        return true;
    } else {
        std::string errmsg{"[Error]: delete picture failed. "};
        errmsg += mysql_error(this->_childConn);
        LOG(errmsg.c_str());
        return false;
    }

}

bool UserMessageManage::delAllPic(string phone)
{
    std::string query_sql{"delete from imagepath where phone='" + phone +"'"};
    int res = mysql_query(this->_childConn, query_sql.c_str());
    if(!res){
        LOG("[Success]: delete all picture in database.");
    } else {
        std::string errmsg{"[Error]: delete picture to recycler failed . "};
        errmsg += mysql_error(this->_childConn);
        LOG(errmsg.c_str());
    }

    vector<string> files;//storage file name
    std::string cate_dir{zjy::picRoot::path + phone + "/"};
    DIR *dir;
    struct dirent *ptr;

    if ((dir=opendir(cate_dir.c_str())) == nullptr) {
        LOG("Open dir error...");
        return false;
    }

    while ((ptr=readdir(dir)) != nullptr)
    {
        string filename{ptr->d_name};
        if(filename.find(".jpeg")!=std::string::npos){
            files.push_back(filename);
        }
    }
    closedir(dir);
    std::vector<string>::iterator beg=files.begin();
    auto pos=beg;
    std::vector<string>::iterator end=files.end();
    LOG("delete all picture in memory");
    while(pos != end){
        string f{cate_dir+(*pos)};
        if(0 == remove(f.c_str())){
            std::string message{"   [Success]: delete picture "+(*pos)};
            LOG(message.c_str());
        }else{
            std::string message{"   [Error]: delete pic failed "+(*pos)};
            LOG(message.c_str());
            break;
        }
        pos++;
    }
}



bool UserMessageManage::managePersonalMessage(User aUser)
{
    std::string query_sql{"update user set name='"};
    query_sql += aUser.getName();
    query_sql += "', password='";
    query_sql += aUser.getPassword();
    query_sql += "', age=";
    query_sql += std::to_string(aUser.getAge());
    query_sql += ", sex=";
    query_sql += std::to_string(aUser.getSex());
    query_sql += ", birthday='";
    query_sql += aUser.getBirthday();
    query_sql += "' where phone='";
    query_sql += aUser.getPhone();
    query_sql += "'";
    int res = mysql_real_query(this->_childConn, query_sql.c_str(), query_sql.size());
    if (res == 0) {
        LOG("update success");
        return true;
    } else {
        LOG(mysql_error(this->_childConn));
        return false;
    }
}

