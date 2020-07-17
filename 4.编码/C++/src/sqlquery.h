#include <list>
#include <vector>
#include <mysql/mysql.h>
#include "log.h"
#include "user.h"
#include "imagepath.h"
#include "zjynamespace.h"

#ifndef SQLQUERY_H
#define SQLQUERY_H

/***********************************************
 * the first parameter is the database's connector,
*in this class, i define that if function implement
* successful, the second parameter will be assigned
* with 0(like linux kernel functions)(some functions
* will return other informations,like imagesPathList
* , otherwise, it will assigned with 1 if it  faild
*
************************************************/

class SQLQuery
{
public:
    SQLQuery() = delete;
    //update user message(personal message)
    static void managePersonalMessage(MYSQL* aConn, void *rtnValue, User aUser);
    //regist a new count
    static void regist(MYSQL* aConn, void *rtnValue, User aUser);
    //login and
    static void login(MYSQL* aConn, void *rtnValue, std::string aPhone, std::string aPwd);

    //log off
    static void logOff(MYSQL* aConn, void *rtnValue, std::string aPhone, std::string aPwd);

    //get person information by phone number
    static void getPersonInfo(MYSQL* aConn, void *rtnValue, std::string aPhone);

    //add picture path in database,as a record
    //the 4 parameter used to describe picture
    //count must be lower than 10;
    //@format image's format whitch want to save
    static void addImage(MYSQL* aConn, void *rtnValue, std::string phone, std::string describe, int count=0/*Avoid multiple images in the same second*/,std::string format = zjy::pic::picFormatSuffix::JPEG);
    //get user's images storage pathes(include picture's name)
    static void imagesPathList(MYSQL* aConn, void *rtnValue, std::string phone, zjy::pic::picSortType type = zjy::pic::picSortType::None);
    static void imagesPathListCustom(MYSQL* aConn, void *rtnValue, std::string phone);
    //move picture to recycler
    static void moveToRecycler(MYSQL* aConn, void *rtnValue, std::string phone, std::string picName);
    //recover picture from recycler
    static void recoverFromRecycler(MYSQL* aConn, void *rtnValue, std::string phone, std::string picName);
    //delete picture from system
    static void delOnePicInDB(MYSQL* aConn, void *rtnValue, std::string phone, std::string imagefile);
    //delete user's all pictures from system
    static void delAllPicInDB(MYSQL* aConn, void *rtnValue, std::string phone);
    //
    static void getVIPLvel(MYSQL* aConn, void *rtnValue, std::string phone);
    static void setVIPLvel(MYSQL* aConn, void *rtnValue, std::string phone);

    static void setImageDescribe(MYSQL* aConn, void *rtnValue, std::string phone, std::string describe,std::string imagefile);
};

#endif // SQLQUERY_H
