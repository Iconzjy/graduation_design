#include <exception>
#include <string>
using namespace std;

#ifndef __UserMessageManage_h__
#define __UserMessageManage_h__

#include "headers.h"
#include "conntodb.h"

enum sortType{
    None=0,//all pictures;
    Time=1,//sort by upload Date/time;
    Recycler=2,//sort by recyclers;
    Other=3//sort by...... i don't know
};
class UserMessageManage : public ConnToDB
{
public:
    UserMessageManage();
    ~UserMessageManage();

    //update user message(personal message)
    bool managePersonalMessage(User aUser);
    //regist a new count
    bool regist(User aUser);
    //login and
    bool login(string aPhone, string aPwd);

    //log off
    bool logOff(string aPhone, string aPwd);

    //add picture path in database,as a record
    //the second parament used to describe picture
    //count must be lower than 10;
    bool addImage(std::string phone, std::string describe, int count=0/*Avoid multiple images in the same second*/);
    //get user's images storage pathes(include picture's name)
    std::list<std::string> imagesPathList(std::string phone, sortType type = sortType::None);
    //move picture to recycler
    bool moveToRecycler(std::string phone, std::string path);
    //delete picture from system
    bool delOnePic(std::string path);
    //delete user's all pictures from system
    bool delAllPic(std::string phone);

private:
    MYSQL* _childConn;
};

#endif
