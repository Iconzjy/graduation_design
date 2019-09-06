
#include "User.h"


User::User(string aPhoneNum, string aPwd) {
}

bool User::setName(string aName) {
    this->_name = aName;
}

bool User::setPhone(string aPhone) {
    this->_phone = aPhone;
}

bool User::setPassword(string aPwd) {
    this->_password = aPwd;
}

bool User::setBirthday(string aBirthday) {
    this->_birthday = aBirthday;
}

bool User::setVIPLevel(int aLevel) {
    this->_vIPLevel = aLevel;
}

bool User::setSex(int aSex) {
    this->_sex = aSex;
}

bool User::setAge(int aAge) {
    this->_age = aAge;
}

string User::getName() {
	return this->_name;
}

string User::getPhone() {
	return this->_phone;
}

string User::getPassword() {
	return this->_password;
}

string User::getBirthday() {
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

void User::getAttribute() {
	throw "Not yet implemented";
}

void User::setAttribute(string aAttribute) {
	throw "Not yet implemented";
}

