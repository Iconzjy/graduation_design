#include <string>
#include <set>
#ifndef IMAGEPATH_H
#define IMAGEPATH_H


/**
 * 这个对应数据库中的图片存储的位置，为了减小服务器的存储容量，只在服务器中存储一张图片，在数据库中存储每个模块的位置，直到回收站中删除才删除图片
 */
class ImagePath
{
    /**
     * 这个是用户的手机号，手机号作为主键
     */
private: std::string _userID;
    /**
     * 存储的路径
     */
    std::string _path;
    /**
     * 回收站
     */
    bool _isRecyclers;
    /**
     * 判断是图片还是视频
     */
    bool _isImage;
    /**
     * 上传时间，根据上传时间分类
     */
    std::string _upDate;
    /**
     * 自定义分类
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

#endif // IMAGEPATH_H
