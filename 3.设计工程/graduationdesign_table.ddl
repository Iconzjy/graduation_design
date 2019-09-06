DROP TABLE IF EXISTS `User`;
DROP TABLE IF EXISTS ImagePath;
CREATE TABLE `User` (
  name     varchar(20) NOT NULL comment '用户名', 
  phone    varchar(12) NOT NULL comment '主键，用户电话（用户id）', 
  password varchar(20) NOT NULL comment '密码，', 
  birthday varchar(30) comment '出生日期', 
  VIPLevel int(100), 
  sex      int(1) DEFAULT 0 comment '性别,0表示男，1表示女', 
  age      int(3) comment '年龄') CHARACTER SET UTF8;
CREATE TABLE ImagePath (
  userID       varchar(12) NOT NULL comment '用户id，与user中的phone一样', 
  path         varchar(255) NOT NULL comment '图片路径', 
  isRecyclers  bit(1) DEFAULT 0 comment '是否在回收站中,0代表不在，1,代表在', 
  isImage      bit(1) DEFAULT 0 comment '判断是否为图片，否的话为视频', 
  `update`     varchar(30) NOT NULL comment '上传时间，用于按时间分类', 
  customerType varchar(255) comment '用户自定义分类，默认用”/ "作为分隔符') CHARACTER SET UTF8;
