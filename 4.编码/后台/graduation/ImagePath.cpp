#include "ImagePath.h"
std::string ImagePath::getUserID() {
	return this->_userID;
}

std::string ImagePath::getPath() {
	return this->_path;
}

bool ImagePath::getIsRecyclers() {
	return this->_isRecyclers;
}

bool ImagePath::getIsImage() {
	return this->_isImage;
}

std::string ImagePath::getUpDate() {
	return this->_upDate;
}

std::set<std::string> ImagePath::getCustomerType() {
    return this->_customerType;
}

std::string ImagePath::getDescribe()
{
    return this->_describe;
}

void ImagePath::setUserID(std::string aUserID) {
	this->_userID = aUserID;
}

void ImagePath::setPath(std::string aPath) {
	this->_path = aPath;
}

void ImagePath::setIsRecyclers(bool aIsRecyclers) {
	this->_isRecyclers = aIsRecyclers;
}

void ImagePath::setIsImage(bool aIsImage) {
	this->_isImage = aIsImage;
}

void ImagePath::setUpDate(std::string aUpDate) {
	this->_upDate = aUpDate;
}

void ImagePath::setCustomerType(std::set<std::string> aCustomerType) {
    this->_customerType = aCustomerType;
}

void ImagePath::setDescribe(std::string aDescribe)
{
    this->_describe = aDescribe;
}

