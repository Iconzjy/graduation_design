#ifndef USER_H
#define USER_H

/**
 * �����Ӧ���ݿ�������ֶ�
 */
class User {

private:
	string name;
	string phone;
	string password;
	string birthday;
	int VIPLevel;
	int sex;
	int age;

public:
	bool setName(string name);

	bool setPhone(string phone);

	bool setPassword(string pwd);

	bool setBirthday(string birthday);

	bool setVIPLevel(string level);

	bool setSex(int sex);

	bool setAge(int age);

	string getName();

	string getPhone();

	string getPassword();

	string getBirthday();

	int getVIPLevel();

	int getSex();

	int getAge();

	/**
	 * ����Ϊdelete
	 */
	User();

	User(string phoneNum, string pwd);
};

#endif
