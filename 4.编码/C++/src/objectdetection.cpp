#include "objectdetection.h"
using namespace std;
using namespace cv;
using namespace cv::text;
namespace zjy { namespace detection {
cv::String yolo_tiny_model = "/home/ubuntu/bs_test/src/yolov3-tiny.weights";
cv::String yolo_tiny_cfg = "/home/ubuntu/bs_test/src/yolov3-tiny.cfg";
std::string head_inference_pb="/home/ubuntu/bs_test/src/sorted_inference_graph.pb";
std::string head_graph_txt="/home/ubuntu/bs_test/src/graph.pbtxt";
} }
/*text detection begin*/
//文字检测与识别模块
void groups_draw(Mat &src, vector<Rect> &groups);
void er_show(vector<Mat> &channels, vector<vector<ERStat> > &regions);
std::string zjyObjectDetection::textDetectCSER(cv::Mat& src)
{
    std::string message;
    // Extract channels to be processed individually
    vector<Mat> channels;
    computeNMChannels(src, channels);

    int cn = (int)channels.size();
    // Append negative channels to detect ER- (bright regions over dark background)
    for (int c = 0; c < cn-1; c++)
        channels.push_back(255-channels[c]);

    // Create ERFilter objects with the 1st and 2nd stage default classifiers
    Ptr<ERFilter> er_filter1 = createERFilterNM1(loadClassifierNM1("/home/ubuntu/bs_test/src/文字识别/trained_classifierNM1.xml"),16,0.00015f,0.13f,0.2f,true,0.1f);
    Ptr<ERFilter> er_filter2 = createERFilterNM2(loadClassifierNM2("/home/ubuntu/bs_test/src/文字识别/trained_classifierNM2.xml"),0.5);

    vector<vector<ERStat> > regions(channels.size());
    // Apply the default cascade classifier to each independent channel (could be done in parallel)
    cout << "Extracting Class Specific Extremal Regions from " << (int)channels.size() << " channels ..." << endl;
    cout << "    (...) this may take a while (...)" << endl << endl;
    for (int c=0; c<(int)channels.size(); c++)
    {
        er_filter1->run(channels[c], regions[c]);
        er_filter2->run(channels[c], regions[c]);
    }


    // Detect character groups
    cout << "Grouping extracted ERs ... ";
    vector< vector<Vec2i> > region_groups;
    vector<Rect> groups_boxes;
    //erGrouping(src, channels, regions, region_groups, groups_boxes, ERGROUPING_ORIENTATION_HORIZ);
    erGrouping(src, channels, regions, region_groups, groups_boxes, ERGROUPING_ORIENTATION_ANY, "/home/ubuntu/OpenCV/opencv_contrib-master/modules/text/samples/trained_classifier_erGrouping.xml", 0.5);

    // draw groups
    groups_draw(src, groups_boxes);
    cout << "Done!" << endl << endl;
    cout << "Press 'space' to show the extracted Extremal Regions, any other key to exit." << endl << endl;
//    if ((waitKey()&0xff) == ' ')
//        er_show(channels,regions);

    // memory clean-up
    er_filter1.release();
    er_filter2.release();
    regions.clear();
    if (!groups_boxes.empty())
    {
        groups_boxes.clear();
    }
}
void groups_draw(Mat &src, vector<Rect> &groups)
{
    for (int i=(int)groups.size()-1; i>=0; i--)
    {
        if (src.type() == CV_8UC3)
            rectangle(src,groups.at(i).tl(),groups.at(i).br(),Scalar( 0, 255, 255 ), 3, 8 );
        else
            rectangle(src,groups.at(i).tl(),groups.at(i).br(),Scalar( 255 ), 3, 8 );
    }
}
void er_show(vector<Mat> &channels, vector<vector<ERStat> > &regions)
{
    for (int c=0; c<(int)channels.size(); c++)
    {
        Mat dst = Mat::zeros(channels[0].rows+2,channels[0].cols+2,CV_8UC1);
        for (int r=0; r<(int)regions[c].size(); r++)
        {
            ERStat er = regions[c][r];
            if (er.parent != NULL) // deprecate the root region
            {
                int newMaskVal = 255;
                int flags = 4 + (newMaskVal << 8) + FLOODFILL_FIXED_RANGE + FLOODFILL_MASK_ONLY;
                floodFill(channels[c],dst,Point(er.pixel%channels[c].cols,er.pixel/channels[c].cols),
                          Scalar(255),0,Scalar(er.level),Scalar(0),flags);
            }
        }
        char buff[20]; char *buff_ptr = buff;
        sprintf(buff, "channel %d", c);
    }
}
/*text detection end*/


/*text detection2 begin*/
using namespace cv;
Mat preprocess(Mat gray)
{
    //1.Sobel算子，x方向求梯度
    Mat sobel;
    Sobel(gray, sobel, CV_8U, 1, 0, 3);

    //2.二值化
    Mat binary;
    threshold(sobel, binary, 0, 255, THRESH_OTSU + THRESH_BINARY);

    //3.膨胀和腐蚀操作核设定
    Mat element1 = getStructuringElement(MORPH_RECT, Size(30, 9));
    //控制高度设置可以控制上下行的膨胀程度，例如3比4的区分能力更强,但也会造成漏检
    Mat element2 = getStructuringElement(MORPH_RECT, Size(24, 4));

    //4.膨胀一次，让轮廓突出
    Mat dilate1;
    dilate(binary, dilate1, element2);

    //5.腐蚀一次，去掉细节，表格线等。这里去掉的是竖直的线
    Mat erode1;
    erode(dilate1, erode1, element1);

    //6.再次膨胀，让轮廓明显一些
    Mat dilate2;
    dilate(erode1, dilate2, element2);

    //7.存储中间图片
//    imshow("binary.jpg", binary);
//    imshow("dilate1.jpg", dilate1);
//    imshow("erode1.jpg", erode1);
//    imshow("dilate2.jpg", dilate2);
    return dilate2;
}

vector<RotatedRect> findTextRegion(Mat img)
{
    vector<RotatedRect> rects;
    //1.查找轮廓
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(img, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE, Point(0, 0));

    //2.筛选那些面积小的
    for (int i = 0; i < contours.size(); i++)
    {
        //计算当前轮廓的面积
        double area = contourArea(contours[i]);

        //面积小于1000的全部筛选掉
        if (area < 1000)
            continue;

        //轮廓近似，作用较小，approxPolyDP函数有待研究
        double epsilon = 0.001*arcLength(contours[i], true);
        Mat approx;
        approxPolyDP(contours[i], approx, epsilon, true);

        //找到最小矩形，该矩形可能有方向
        RotatedRect rect = minAreaRect(contours[i]);

        //计算高和宽
        int m_width = rect.boundingRect().width;
        int m_height = rect.boundingRect().height;

        //筛选那些太细的矩形，留下扁的
        if (m_height > m_width * 1.2)
            continue;

        //符合条件的rect添加到rects集合中
        rects.push_back(rect);

    }
    return rects;
}

void detect(Mat& img)
{
    //1.转化成灰度图
    Mat gray;
    cvtColor(img, gray, CV_BGR2GRAY);

    //2.形态学变换的预处理，得到可以查找矩形的轮廓
    Mat dilation = preprocess(gray);

    //3.查找和筛选文字区域
    vector<RotatedRect> rects = findTextRegion(dilation);

    //4.用绿线画出这些找到的轮廓
//    for_each (rects.begin(),rects.end(),[](RotatedRect rect,Mat img){
//            cv::Point2f P[4];
//            rect.points(P);
//            for (int j = 0; j <= 3; j++)
//            {
//                line(img, P[j], P[(j + 1) % 4], Scalar(0,255,0), 2);
//            }
//        });
    for(int i=0;i<rects.size();i++){
        auto rect = rects.at(i);
        cv::Point2f P[4];
        rect.points(P);
        for (int j = 0; j <= 3; j++)
        {
            line(img, P[j], P[(j + 1) % 4], Scalar(0,255,0), 2);
        }
    }

    //5.显示带轮廓的图像
//    imshow("img", img);

//    waitKey(0);
}
void zjyObjectDetection::textDetect2(Mat &src)
{
    detect(src);
}
/*text detection2 end*/



/*object detection begin*/
#include <opencv2/dnn.hpp>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstdlib>
using namespace cv::dnn;
std::list<string> zjyObjectDetection::image_detection(cv::Mat &pic)
{
    using namespace zjy::detection;
    Net net = readNetFromDarknet(yolo_tiny_cfg, yolo_tiny_model);
    net.setPreferableBackend(DNN_BACKEND_OPENCV);
    net.setPreferableTarget(DNN_TARGET_CPU);
    std::vector<String> outNames = net.getUnconnectedOutLayersNames();
//    for (int i = 0; i < outNames.size(); i++) {
//        printf("output layer name : %s\n", outNames[i].c_str());
//    }

    vector<string> classNamesVec;
    ifstream classNamesFile("/home/ubuntu/bs_test/src/object_detection_classes_yolov3.txt");
    if (classNamesFile.is_open())
    {
        string className = "";
        while (std::getline(classNamesFile, className))
            classNamesVec.push_back(className);
    }

    // 加载图像

    Mat inputBlob = blobFromImage(pic, 1 / 255.F, Size(416, 416), Scalar(), true, false);
    net.setInput(inputBlob);

    // 检测
    std::vector<Mat> outs;
    net.forward(outs, outNames);
    vector<double> layersTimings;
    double freq = getTickFrequency() / 1000;
    double time = net.getPerfProfile(layersTimings) / freq;
    ostringstream ss;
    ss << "detection time: " << time << " ms";
    putText(pic, ss.str(), Point(20, 20), 0, 0.5, Scalar(0, 0, 255));
    vector<Rect> boxes;
    vector<int> classIds;
    vector<float> confidences;
    for (size_t i = 0; i<outs.size(); ++i)
    {
        float* data = (float*)outs[i].data;
        for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols)
        {
            Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
            Point classIdPoint;
            double confidence;
            minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
            if (confidence > 0.5)
            {
                int centerX = (int)(data[0] * pic.cols);
                int centerY = (int)(data[1] * pic.rows);
                int width = (int)(data[2] * pic.cols);
                int height = (int)(data[3] * pic.rows);
                int left = centerX - width / 2;
                int top = centerY - height / 2;

                classIds.push_back(classIdPoint.x);
                confidences.push_back((float)confidence);
                boxes.push_back(Rect(left, top, width, height));
            }
        }
    }

    std::set<std::string> st;
    std::list<std::string> ls;
    vector<int> indices;
    NMSBoxes(boxes, confidences, 0.5, 0.2, indices);
    cout<<indices.size()<<endl;
    for (size_t i = 0; i < indices.size(); ++i)
    {

        int idx = indices[i];
        Rect box = boxes[idx];
        String className = classNamesVec[classIds[idx]];
        putText(pic, className.c_str(), box.tl(), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(255, 0, 0), 2, 8);
        rectangle(pic, box, Scalar(0, 0, 255), 2, 8, 0);
        st.insert(className);
    }
    for_each(st.begin(),st.end(),[&ls](std::string s){ std::cout<<s<<std::endl; ls.push_back(s); });
    return ls;
}
/*object detection end*/

/*head detection begin*/
#include <opencv2/dnn.hpp>

int zjyObjectDetection::head_detect(cv::Mat& src)
{
    using namespace zjy::detection;
    int count=0;
    cv::dnn::Net net=readNetFromTensorflow(head_inference_pb,head_graph_txt);
    int h=src.rows;
    int w=src.cols;
    cv::Mat im_tensor=blobFromImage(src,1.0,cv::Size(300,300),cv::Scalar(),true,false);
    net.setInput(im_tensor);
    cv::Mat cvOut=net.forward();
    cv::Mat detectOut(cvOut.size[2],cvOut.size[3],CV_32F,cvOut.ptr<float>());
    for(int row=0;row<detectOut.rows;row++){
        float confidence = detectOut.at<float>(row,2);
        if(confidence>0.4){
            count++;
            int left=detectOut.at<float>(row,3)*w;
            int top=detectOut.at<float>(row,4)*h;
            int right=detectOut.at<float>(row,5)*w;
            int bottom=detectOut.at<float>(row,6)*h;

            cv::Rect rect;
            rect.x=left;
            rect.y=top;
            rect.width=right-left;
            rect.height=bottom-top;
            cv::rectangle(src,rect,Scalar(255,0,255),1,8,0);
        }
    }
    return count;
}
/*head detection end*/

#include "log.h"
/*tencent ocr begin*/
std::string zjyObjectDetection::tencentOCR(std::string& src)
{
    int cln_sock;
    cln_sock=socket(PF_INET,SOCK_STREAM,0);
    if(-1==cln_sock){
        LOG::ErrorLog(__FILE__,__LINE__, "sock() error");
        return nullptr;
    }
    struct sockaddr_in serv_addr;
    memset(&serv_addr,0,sizeof(serv_addr));
    char* addr="127.0.0.1";
    char* port="7996";
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr(addr);
    serv_addr.sin_port=htons(atoi(port));

    if(-1==connect(cln_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))){
        LOG::ErrorLog(__FILE__,__LINE__, "connect() error");
        close(cln_sock);
        return nullptr;
    }
    std::string rtnTencentOCR{};
    size_t len=src.size();
    if(!write(cln_sock,src.c_str(),len)){
        LOG::ErrorLog(__FILE__,__LINE__,"something error");
        close(cln_sock);
        return nullptr;
    }
    if(0!=shutdown(cln_sock,SHUT_WR)){
	LOG::ErrorLog(__FILE__,__LINE__,"something error");
        close(cln_sock);
        return nullptr;

    }
    char r[1024];
    memset(r,0,1024);
    while(ssize_t l = read(cln_sock,r,1024)){
        rtnTencentOCR.append(r);
    }
    close(cln_sock);
    return rtnTencentOCR;
}
/*tencent ocr end*/
