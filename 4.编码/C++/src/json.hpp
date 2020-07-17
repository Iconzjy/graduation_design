#include <string>
#include <list>
#include <map>
#include <utility>
#ifndef JSON_H
#define JSON_H

class JSON
{
public:
public:
    static std::string stringListToJson(std::list<std::string> sList);
    static std::list<std::string> jsonToStringList_value(std::string json);
    template<typename T>
    static std::string usertoJson(T* u);
    static std::map<std::string, std::string>jsonToMap(std::string json);
private:
    static void eatSpace(std::string& s);
    JSON();
};
template<typename T>
std::string JSON::usertoJson(T *u)
{

    std::string rtnString{"{"};
    rtnString += "\"name\":\"" + u->getName();
    rtnString += "\",\"age\":\"" + std::to_string(u->getAge());
    rtnString += "\",\"sex\":\"" + std::to_string(u->getSex());
    rtnString += "\",\"phone\":\"" + u->getPhone();
    rtnString += "\",\"birthday\":\"" + u->getBirthday();
    rtnString += "\",\"password\":\"" + u->getPassword();
    rtnString += "\",\"VIPLevel\":\"" + std::to_string(u->getVIPLevel());
    rtnString += "\"}";
    return rtnString;
}

#endif // JSON_H
