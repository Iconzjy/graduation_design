#ifndef IMAGEPATH_H
#define IMAGEPATH_H

/**
 * 这个对应数据库中的图片存储的位置，为了减小服务器的存储容量，只在服务器中存储一张图片，在数据库中存储每个模块的位置，直到回收站中删除才删除图片
 */
class ImagePath {

private:
	/**
	 * 这个是用户的手机号，手机号作为主键
	 */
	string userID;
	/**
	 * 存储的路径
	 */
	string path;
	struct Type type;
};

#endif
