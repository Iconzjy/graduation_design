#include "zjynamespace.h"
#ifndef OBJECTDETECTION_H
#define OBJECTDETECTION_H


#include <opencv2/text.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <list>
#include <set>
/*
 * zjy
 * 2019/12/31
 *
*/
namespace zjyObjectDetection {
std::string textDetectCSER(cv::Mat& src);
void textDetect2(cv::Mat& src);
std::list<std::string> image_detection(cv::Mat &pic);
int head_detect(cv::Mat& src);
std::string tencentOCR(std::string& src);
}
#endif // OBJECTDETECTION_H
