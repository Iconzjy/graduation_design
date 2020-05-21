#include <string>
#ifndef USER_H
#define USER_H


/**
 * 这个对应数据库的数据字段
 */
class User
{
private:
    std::string _name;
    std::string _phone;
    std::string _password;
    std::string _birthday;
    int _vIPLevel;
    int _sex;
    int _age;

    /// <summary>
    /// 设置为delete
    /// </summary>
public:
    User();

    User(std::string aPhoneNum, std::string aPwd);

    void setName(std::string aName);

    void setPhone(std::string aPhone);

    void setPassword(std::string aPwd);

    void setBirthday(std::string aBirthday);

    void setVIPLevel(int aLevel);

    void setSex(int aSex);

    void setAge(int aAge);

    std::string getName();

    std::string getPhone();

    std::string getPassword();

    std::string getBirthday();

    int getVIPLevel();

    int getSex();

    int getAge();
};

#endif // USER_H
