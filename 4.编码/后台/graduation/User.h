#include <exception>
#include <string>
using namespace std;

#ifndef __User_h__
#define __User_h__

class User;

/**
 * 这个对应数据库的数据字段
 */
class User
{
	private: string _name;
	private: string _phone;
	private: string _password;
	private: string _birthday;
	private: int _vIPLevel;
	private: int _sex;
	private: int _age;

	/// <summary>
	/// 设置为delete
	/// </summary>
    public: User()=delete;

	public: User(string aPhoneNum, string aPwd);

	public: bool setName(string aName);

	public: bool setPhone(string aPhone);

	public: bool setPassword(string aPwd);

	public: bool setBirthday(string aBirthday);

    public: bool setVIPLevel(int aLevel);

	public: bool setSex(int aSex);

	public: bool setAge(int aAge);

	public: string getName();

	public: string getPhone();

	public: string getPassword();

	public: string getBirthday();

	public: int getVIPLevel();

	public: int getSex();

	public: int getAge();

	public: void getAttribute();

	public: void setAttribute(string aAttribute);
};

#endif
