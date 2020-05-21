#include "usermanageinterface.h"
#include "sqlquery.h"
#include "imagebasetool.h"

UserManageInterface::UserManageInterface(MYSQL *conn)
    :_isTempUser(true), _conn(conn)
{
    this->mtm = new MyTimer();
    this->_user = new User();
    this->_viplevel = new int(0);
}

UserManageInterface::UserManageInterface(DBPool* const db)
    :_isTempUser(true), _conn(db->getDBConn())
{
    this->mtm = new MyTimer();
    this->_user = new User();
    this->_viplevel = new int(0);
}



UserManageInterface::~UserManageInterface()
{
    delete this->mtm;
    delete this->_viplevel;
    delete this->_user;
}

bool UserManageInterface::updateInfo(User aUser)
{
    if(this->_user->getPhone().empty() || this->_isTempUser == true){
        LOG::ErrorLog(__FILE__, __LINE__, "you need to login or regist first");
        return false;
    }
    int success=-1;
    SQLQuery::managePersonalMessage(this->_conn, &success, aUser);
    if(success == 0){
        LOG::SuccessLog(__FILE__, __LINE__, "update user information in UserManageInterface.cpp");
        return true;
    } else {
        LOG::ErrorLog(__FILE__, __LINE__, mysql_error(this->_conn));
        return false;
    }
}

bool UserManageInterface::setUser(User& aUser)
{
    this->_user = &aUser;
}

int UserManageInterface::regist(User aUser)
{
    int success=-1;
    SQLQuery::regist(this->_conn, &success, aUser);
    if(success == 0){
        LOG::SuccessLog(__FILE__, __LINE__, "regist success in UserManageInterface.cpp");
        return 0;
    } else if(success == 1) {
        LOG::ErrorLog(__FILE__, __LINE__, "user exist");
        return 1;
    }else{
        LOG::ErrorLog(__FILE__, __LINE__, mysql_error(this->_conn));
        return 2;
    }
}

int UserManageInterface::login(std::string aPhone, std::string aPwd)
{
    if(aPhone.empty() || aPwd.empty()){
	LOG::ErrorLog(__FILE__,__LINE__,"phone or pwd enpty");
	return -1;
    }
    int issuccess=-1;
    SQLQuery::login(this->_conn, &issuccess, aPhone, aPwd);
    if(issuccess == 0){
        this->_isTempUser = false;
        SQLQuery::getPersonInfo(this->_conn, this->_user, aPhone);
        *this->_viplevel = this->getVipLevel();
        LOG::SuccessLog(__FILE__, __LINE__, "login success in UserManageInterface.cpp");
        return issuccess;
    } else if(issuccess == 1){
        LOG::ErrorLog(__FILE__, __LINE__, "password error");
        return issuccess;
    }else if(issuccess == 2){
        LOG::ErrorLog(__FILE__, __LINE__, "user not exit");
        return issuccess;
    }else if(issuccess == 3){
        LOG::ErrorLog(__FILE__, __LINE__, "mysql error");
        return issuccess;
    }else{

    }
}

bool UserManageInterface::logOff(std::string aPwd)
{
    if(this->_user->getPhone().empty() || this->_isTempUser == true){
        LOG::ErrorLog(__FILE__, __LINE__, "you need to login or regist first");
        return false;
    }
    int value = -1;
    SQLQuery::logOff(this->_conn, &value, this->_user->getPhone(), aPwd);
    if(value != 0) {
        LOG::ErrorLog(__FILE__, __LINE__, "log off fail");
        return false;
    } else {
        LOG::SuccessLog(__FILE__, __LINE__, "logoff");
        return true;
    }
}

bool UserManageInterface::modify(User& user) const
{
    int rtnvalue=-1;
    SQLQuery::managePersonalMessage(this->_conn, &rtnvalue, user);
    if(rtnvalue == 0){
        this->_user->setAge(user.getAge());
        this->_user->setSex(user.getSex());
        this->_user->setName(user.getName());
        this->_user->setBirthday(user.getName());
        this->_user->setPassword(user.getPassword());
        LOG::SuccessLog(__FILE__, __LINE__, __func__);
        return true;
    } else {
        LOG::ErrorLog(__FILE__, __LINE__, __func__);
        return false;
    }
}

User UserManageInterface::getPersonInfo() const
{
    if(this->_user->getPhone().empty() || this->_isTempUser == true){
        LOG::ErrorLog(__FILE__, __LINE__, "you need to login or regist first");
    }
    return *this->_user;
}

bool UserManageInterface::isThisUser(std::string aPhone) const
{
    if(aPhone == this->_user->getPhone()){
        return true;
    }else{
        return false;
    }
}

std::string UserManageInterface::uploadImage(std::string &pic_src, std::string format, std::string describe, int count)
{
    if(this->_user->getPhone().empty() || this->_isTempUser == true){
        LOG::ErrorLog(__FILE__, __LINE__, "you need to login or regist first");
        return "-1";
    }
    char rtnValue[64];
    memset(rtnValue,0,sizeof (rtnValue));
    SQLQuery::addImage(this->_conn, &rtnValue, this->_user->getPhone(), describe, count,format);
    if(strcmp(rtnValue, "-1") == 0){
        LOG::testLog(rtnValue);
        LOG::testLog(std::to_string(strcmp(rtnValue,"-1")));
        LOG::ErrorLog(__FILE__, __LINE__, "add image fail");
        return "-1";
    } else {
        int isSaved = ImageBaseTool::saveImage(const_cast<char*>(pic_src.c_str()), zjy::pic::picRoot::path+this->_user->getPhone()+"/",rtnValue,pic_src.size());
        if(isSaved!=0){
            LOG::ErrorLog(__FILE__,__LINE__,"saved pic error");
            //here need to delete record from database
            //or you can use saveImage(...) as a callback function,then,input it to addImage(..)
            //when addImage insert database success, just call saveImage(..),if fail, don't call it
            //the you can delete record here.

        }
        LOG::SuccessLog(__FILE__, __LINE__, "UserManageInterface.cpp add image success ");
        return std::string(rtnValue);//return pic name
    }
}

std::list<std::string> UserManageInterface::imagesPathList(zjy::pic::picSortType type)
{
    if(this->_user->getPhone().empty() || this->_isTempUser == true){
        LOG::ErrorLog(__FILE__, __LINE__, "you need to login or regist first");
        return std::list<std::string>();
    }
    std::list<std::string> rtnValue{};
    SQLQuery::imagesPathList(this->_conn, &rtnValue, this->_user->getPhone(), type);
    return rtnValue;
}

std::list<std::string> UserManageInterface::imagesPathListCustom(std::string type)
{
    if(this->_user->getPhone().empty() || this->_isTempUser == true){
        LOG::ErrorLog(__FILE__, __LINE__, "you need to login or regist first");
        return std::list<std::string>();
    }
    std::list<std::string> rtnValue{};
    std::list<std::pair<std::string,std::string>> pisc{};
    SQLQuery::imagesPathListCustom(this->_conn, &pisc, this->_user->getPhone());
    for(std::pair<std::string,std::string> s:pisc){
        std::list<std::string> ls=JSON::jsonToStringList_value(s.second);
        auto pos=std::find(ls.begin(),ls.end(),type);
        if(pos!=ls.end()){
            rtnValue.push_back(s.first);
        }
    }
    return rtnValue;
}

bool UserManageInterface::moveToRecycler(std::string picName)
{
    if(this->_user->getPhone().empty() || this->_isTempUser == true){
        LOG::ErrorLog(__FILE__, __LINE__, "you need to login or regist first");
        return false;
    }
    int rtnValue = -1;
    SQLQuery::moveToRecycler(this->_conn, &rtnValue, this->_user->getPhone(), picName);
    if(rtnValue != 0){
        LOG::ErrorLog(__FILE__, __LINE__, "move to recycler fail");
        return false;
    } else {
        LOG::SuccessLog(__FILE__, __LINE__, "move to recycler");
        return  true;
    }
}

bool UserManageInterface::recoverFromRecycler(std::string picName)
{
    if(this->_user->getPhone().empty() || this->_isTempUser == true){
        LOG::ErrorLog(__FILE__, __LINE__, "you need to login or regist first");
        return false;
    }
    int rtnValue = -1;
    SQLQuery::recoverFromRecycler(this->_conn, &rtnValue, this->_user->getPhone(), picName);
    if(rtnValue != 0){
        LOG::ErrorLog(__FILE__, __LINE__, "recover from recycler fail");
        return false;
    } else {
        LOG::SuccessLog(__FILE__, __LINE__, "recover from recycler");
        return  true;
    }
}

bool UserManageInterface::delOnePic(std::string picpath)
{
    if(this->_user->getPhone().empty() || this->_isTempUser == true){
        LOG::ErrorLog(__FILE__, __LINE__, "you need to login or regist first");
        return false;
    }
    int rtnValue = -1;
    SQLQuery::delOnePicInDB(this->_conn, &rtnValue, this->_user->getPhone(),picpath);
    if(rtnValue != 0){
        LOG::ErrorLog(__FILE__, __LINE__, "delete picture in memory fail");
        return false;
    } else {
        std::string picPath{zjy::pic::picRoot::path + this->getPersonInfo().getPhone() +"/"+ picpath};
        LOG::testLog(picPath);
        if(0 == remove(picPath.c_str())){
            LOG::SuccessLog(__FILE__, __LINE__, "delete picture in memory.");
        }
        return  true;
    }
}

bool UserManageInterface::delAllPic()
{
    if(this->_user->getPhone().empty() || this->_isTempUser == true){
        LOG::ErrorLog(__FILE__, __LINE__, "you need to login or regist first");
        return false;
    }
    int rtnValue = -1;
    SQLQuery::delAllPicInDB(this->_conn, &rtnValue, this->_user->getPhone());
    if(rtnValue != 0){
        LOG::ErrorLog(__FILE__, __LINE__, "delete pic in disk fail");
        return false;
    } else {
        std::vector<std::string> files;//storage file name
        std::string cate_dir{zjy::pic::picRoot::path + this->_user->getPhone() + "/"};
        DIR *dir;
        struct dirent *ptr;

        if ((dir=opendir(cate_dir.c_str())) == nullptr) {
            LOG::ErrorLog(__FILE__, __LINE__, "Open dir error...");
            int* value = reinterpret_cast<int*>(rtnValue);
            *value = 1;
        }

        while ((ptr=readdir(dir)) != nullptr)
        {
            std::string filename{ptr->d_name};
            if(filename.find(".jpeg")!=std::string::npos){
                files.push_back(filename);
            }
        }
        closedir(dir);
        std::vector<std::string>::iterator beg=files.begin();
        auto pos=beg;
        std::vector<std::string>::iterator end=files.end();
	LOG::SuccessLog(__FILE__,__LINE__,"delete all picture in memory");
        while(pos != end){
            std::string f{cate_dir+(*pos)};
            if(0 == remove(f.c_str())){
                std::string message{"   delete picture "+(*pos)};
                LOG::SuccessLog(__FILE__, __LINE__, message.c_str());
                int* value = reinterpret_cast<int*>(rtnValue);
                *value = 0;
            }else{
                std::string message{"   delete pic failed "+(*pos)};
                LOG::ErrorLog(__FILE__, __LINE__, message.c_str());
                int* value = reinterpret_cast<int*>(rtnValue);
                *value = 1;
                break;
            }
            pos++;
        }
        LOG::SuccessLog(__FILE__, __LINE__, "delete pic in disk");
        return  true;
    }
}

int UserManageInterface::getVipLevel()
{
    if(this->_user->getPhone().empty() || this->_isTempUser == true){
        LOG::ErrorLog(__FILE__, __LINE__, "you need to login or regist first");
        return false;
    }
    SQLQuery::getVIPLvel(this->_conn, this->_viplevel, this->_user->getPhone());
    return *this->_viplevel;
}

int UserManageInterface::setVIPLevel()
{
    if(this->_user->getPhone().empty() || this->_isTempUser == true){
        LOG::ErrorLog(__FILE__, __LINE__, "you need to login or regist first");
        return 1;
    }
    SQLQuery::setVIPLvel(this->_conn, this->_viplevel, this->_user->getPhone());
    return 0;
}

int UserManageInterface::saveImage(char *src,  size_t len, std::string phone, std::string format, std::string describe)
{
    char saveFilename[100];
    MYSQL* sql;
    sql = mysql_init(nullptr);
    if(!sql) {
        LOG::ErrorLog(__FILE__,__LINE__, __func__);
        exit(EXIT_FAILURE);
    }
    using namespace zjy::sql;
    sql = mysql_real_connect(sql,host.c_str(),user.c_str(),pwd.c_str(),db.c_str(),port,nullptr,0);
    if(sql){
        LOG::SuccessLog(__FILE__, __LINE__, "Connection success\n");
        memset(saveFilename,0,100);
        SQLQuery::addImage(sql,saveFilename,phone,describe,0,format);
    }else{
        LOG::ErrorLog(__FILE__,__LINE__, __func__);
        mysql_close(sql);
        return 2;
    }

    int isSaved= ImageBaseTool::saveImage(src,zjy::pic::picRoot::path+phone+"/"+std::string(saveFilename),len,phone);
    if(isSaved != 0){
        LOG::SuccessLog(__FILE__,__LINE__,"image existed");
        int isdel;
        SQLQuery::delOnePicInDB(sql,&isdel,phone, std::string(saveFilename));
        if(isdel!=0){
            LOG::ErrorLog(__FILE__,__LINE__,"something error");
        }
        LOG::ErrorLog(__FILE__,__LINE__," save image error");
        mysql_close(sql);
        return 1;
    }else{
        int isset;
        cv::Mat pic= cv::imread(zjy::pic::picRoot::path+phone+"/"+std::string(saveFilename));
        std::list<std::string> ls = zjyObjectDetection::image_detection(pic);
        std::string des = JSON::stringListToJson(ls);
        SQLQuery::setImageDescribe(sql,&isset,phone,des,std::string(saveFilename));
        mysql_close(sql);
        return 0;
    }
}

MYSQL *UserManageInterface::getMYSqlSocket()const
{
    return this->_conn;
}

MyTimer::MyTimer()
{
    this->start=clock();
}

void MyTimer::setBegin()
{
    this->start=clock();
}

double MyTimer::getTimer_hour()
{
    this->end=clock();
    return (double)(end-start)/CLOCKS_PER_SEC/3600;//hour
}
