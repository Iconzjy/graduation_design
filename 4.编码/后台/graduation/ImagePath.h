#include <string>
#include <set>

#ifndef __ImagePath_h__
#define __ImagePath_h__

class ImagePath;

/**
 * �����Ӧ���ݿ��е�ͼƬ�洢��λ�ã�Ϊ�˼�С�������Ĵ洢������ֻ�ڷ������д洢һ��ͼƬ�������ݿ��д洢ÿ��ģ���λ�ã�ֱ������վ��ɾ����ɾ��ͼƬ
 */
class ImagePath
{
	/**
	 * ������û����ֻ��ţ��ֻ�����Ϊ����
	 */
private: std::string _userID;
	/**
	 * �洢��·��
	 */
    std::string _path;
	/**
	 * ����վ
     */
    bool _isRecyclers;
	/**
	 * �ж���ͼƬ������Ƶ
	 */
    bool _isImage;
	/**
	 * �ϴ�ʱ�䣬�����ϴ�ʱ�����
	 */
    std::string _upDate;
	/**
	 * �Զ������
	 */
   std::set<std::string> _customerType;

   std::string _describe;

public:

   std::string getUserID();
    std::string getPath();
    bool getIsRecyclers();
    bool getIsImage();
    std::string getUpDate();
    std::set<std::string> getCustomerType();
    std::string getDescribe();
    void setUserID(std::string aUserID);
    void setPath(std::string aPath);
    void setIsRecyclers(bool aIsRecyclers);
    void setIsImage(bool aIsImage);
    void setUpDate(std::string aUpDate);
    void setCustomerType(std::set<std::string> aCustomerType);
    void setDescribe(std::string aDescribe);
};

#endif
