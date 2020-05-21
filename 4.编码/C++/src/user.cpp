#include "user.h"

User::User()
{

}

User::User(std::string aPhoneNum, std::string aPwd) {
    this->_phone = aPhoneNum;
    this->_password = aPwd;
}

void User::setName(std::string aName) {
    this->_name = aName;
}

void User::setPhone(std::string aPhone) {
    this->_phone = aPhone;
}

void User::setPassword(std::string aPwd) {
    this->_password = aPwd;
}

void User::setBirthday(std::string aBirthday) {
    this->_birthday = aBirthday;
}

void User::setVIPLevel(int aLevel) {
    this->_vIPLevel = aLevel;
}

void User::setSex(int aSex) {
    this->_sex = aSex;
}

void User::setAge(int aAge) {
    this->_age = aAge;
}

std::string User::getName() {
    return this->_name;
}

std::string User::getPhone() {
    return this->_phone;
}

std::string User::getPassword() {
    return this->_password;
}

std::string User::getBirthday() {
    return this->_birthday;
}

int User::getVIPLevel() {
    return this->_vIPLevel;
}

int User::getSex() {
    return this->_sex;
}

int User::getAge() {
    return this->_age;
}

