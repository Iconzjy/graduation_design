#include <list>
#include <iterator>
#include <algorithm>
#include "user.h"
#include "dbpool.h"
#ifndef USERMANAGEINTERFACE_H
#define USERMANAGEINTERFACE_H

class MyTimer{
public:
    MyTimer();
    void setBegin();
    double getTimer_hour();
private:
    clock_t start;
    clock_t end;

};
class UserManageInterface
{
public:
    UserManageInterface(MYSQL* conn);
    UserManageInterface(DBPool* const db);
    ~UserManageInterface();

    /*this part is the interface of class SQLQuery, so, there are the same function names*/
    bool updateInfo(User aUser);//managePersonalMessage
    bool setUser(User &aUser);
    int regist(User aUser);
    int login(std::string aPhone, std::string aPwd);
    bool logOff(std::string aPwd);
    //modify personal information
    bool modify(User &user) const;
    User getPersonInfo() const;

    //
    bool isThisUser(std::string aPhone) const;
    /*
     * @para uploadImage: if success, return image name,else return -1;
     */
    std::string uploadImage(std::string& pic_src,std::string format, std::string describe, int count=0/*Avoid multiple images in the same second*/);
    std::list<std::string> imagesPathList(zjy::pic::picSortType type = zjy::pic::picSortType::None);
    std::list<std::string> imagesPathListCustom(std::string type);

    bool moveToRecycler(std::string picName);
    bool recoverFromRecycler(std::string picName);
    //delete pic in disk and database(use sqlquery class)
    bool delOnePic(std::string picpath);
    bool delAllPic();
    int getVipLevel();
    int setVIPLevel();

    /*@describe: json format, used to sorted*/
    static int saveImage(char *src, size_t len, std::string phone, std::string format,std::string describe);

    MYSQL* getMYSqlSocket() const;

public:
    MyTimer* mtm;
private:
    bool _isTempUser;
//    bool _loginSuccess;
    int* _viplevel;
    User* _user;
    MYSQL* _conn;


};


#endif // USERMANAGE_H
