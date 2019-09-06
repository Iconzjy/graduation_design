#ifndef USERMESSAGEMANAGE_H
#define USERMESSAGEMANAGE_H

class UserMessageManage {


public:
	bool regist();

	bool login(string phone, string pwd);

	bool logout(string phone, string pwd);

	bool managePersonalMessage(int User);
};

#endif
