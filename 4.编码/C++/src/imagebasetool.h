#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
#include "headers.h"
#ifndef IMAGEBASETOOL_H
#define IMAGEBASETOOL_H
//https://blog.csdn.net/tt_ren/article/details/53227900

class ImageBaseTool
{
public:
    //static function, used to deal with images;
    static void toJpeg(cv::Mat& src, std::string &dst);
    static void toPng(cv::Mat& src, std::string &dst);
    static void toJpg(cv::Mat& src, std::string &dst);
    static void toTiff(cv::Mat& src, std::string &dst);
    static int saveImage(char* src, std::string picPath, std::string picName, size_t len);
    static int saveImage(char* src, std::string file, size_t len, std::string phone);
    static int deleteImageOnDisk(std::string file);
    static zjy::pic::picFormat parseImageFormat(std::string filename);
    static cv::Mat binaryToMat(std::string data);
    static std::string matToBinary(cv::Mat& src,std::string type=zjy::pic::picFormatSuffix::JPG);
    static std::string base64_encode(const char *bytes_to_encode, unsigned int in_len);
    static std::string base64_decode(std::string const& encoded_string);
    /*
     * get all files throgh one file director
     * @param dirPath is file director
    */
    static std::vector<std::string> getFiles(std::string dirPath);

    /*
     * @param filename is the full path
     * @param src is local Image which is user want to upload to his/her cloud photo album
     * @param minValue(0~100.0) is the similarity score, which you limit the smallest value,
     *        the higher the value, the higer the correlation,
     *        if you input 100.0, that means only both image equal, and it will return true.
    */
    static bool isRepeat(std::string filename, const cv::Mat& src, double minValue = 90.0);

    /*@param type zjy::pic::picFormatSuffix
     * @param times 倍数
    */
    static void picResize(cv::Mat& src, std::string &dst, double times, std::string type);
    //vip function


private:
    /*@para: type zjy::pic::picFormatSuffix*/
    static void toOther(cv::Mat& src, std::string &dst, std::string format);
    static void temporaryImage(cv::Mat& src, std::string &dst, std::string format);
    /*
     * use PSNR algorithm to compute the correlation(相关度) of both Images
     * the higher the value, the higer the correlation
    */
    static double getPSNR(const cv::Mat& src, const cv::Mat& dst);

    /*
     * @para: src and dst are the both Images that you need to compare
     * 数据以scalar格式保存，读取scalar内的数据即可获取相应的相似度值，其中值的范围在0～1之间，1为完全一致，0为完全不一至。
     *   mssimV.val[0]
     *   mssimV.val[1]
     *   mssimV.val[2]
     * type:double
    */
    static cv::Scalar getMSSIM(const cv::Mat& src, const cv::Mat& dst);

    //base64

    static inline bool is_base64(unsigned char c) {
        return (isalnum(c) || (c == '+') || (c == '/'));
    }
    /*图像指纹来实现两张图片是否相同
     */

};

#endif // IMAGEBASETOOL_H
