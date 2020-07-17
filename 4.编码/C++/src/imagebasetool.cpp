#include "imagebasetool.h"


void ImageBaseTool::toJpeg(cv::Mat &src, std::string &dst)
{
    if(src.empty()){
        LOG::ErrorLog(__FILE__, __LINE__, "image is empty");
        dst.clear();
        return;
    }
    std::string type{zjy::pic::picFormatSuffix::JPEG};
    temporaryImage(src, dst, type);
    return;
}

void ImageBaseTool::toPng(cv::Mat &src, std::string &dst)
{
    toOther(src, dst,zjy::pic::picFormatSuffix::PNG);
}

void ImageBaseTool::toJpg(cv::Mat &src, std::string &dst)
{
    toOther(src, dst,zjy::pic::picFormatSuffix::JPG);
}

void ImageBaseTool::toTiff(cv::Mat &src, std::string &dst)
{
    toOther(src, dst,zjy::pic::picFormatSuffix::TIFF);
}

int ImageBaseTool::saveImage(char *src, std::string picPath, std::string picName, size_t len)
{
    std::vector<std::string> pics = getFiles(picPath);
    std::string pic_wantToStorage;
    pic_wantToStorage=picPath+picName;
    std::ofstream file;
    file.open(pic_wantToStorage, std::ios_base::binary | std::ios_base::out);
    for(size_t i = 0; i < len; i++){
        file << *(src+i);
    }
    //@src_pic pic which user want to upload, write to file just now
    cv::Mat src_pic = cv::imread(std::string(picPath+picName).c_str());
    if(pics.size()==0){
        return 0;//not pic in this file, and save it
    }
    for(std::string v : pics){
        bool repeat = isRepeat(picPath+v,src_pic,95.0);
        LOG::testLog(std::to_string(repeat));
        if(!repeat)
            return 0;//not repeat,and save success
        else{
            system(std::string("rm "+pic_wantToStorage).c_str());
            return 1;//picture repeat, remove this picture file.
        }
    }
}
int ImageBaseTool::saveImage(char *src, std::string file, size_t len,std::string phone)
{
    std::vector<std::string> pics = getFiles(zjy::pic::picRoot::path+phone+"/");
    std::string pic_wantToStorage;
    pic_wantToStorage=file;
    std::ofstream f;
    f.open(pic_wantToStorage, std::ios_base::binary | std::ios_base::out);
    for(size_t i = 0; i < len; i++){
        f << *(src+i);
    }
    f.close();
    //@src_pic pic which user want to upload, write to file just now
    cv::Mat src_pic = cv::imread(pic_wantToStorage);
    if(pics.size()==0){
        return 0;//not pic in this file, and save it
    }
    for(std::string v : pics){
        bool repeat = isRepeat(zjy::pic::picRoot::path+phone+"/"+v,src_pic,95.0);
        if(!repeat)
            continue;
        else{
            system(std::string("rm "+pic_wantToStorage).c_str());
            return 1;//picture repeat, remove this picture file.
        }
    }
    return 0;//not repeat,and save success
}

int ImageBaseTool::deleteImageOnDisk(std::string file)
{
    if(remove(file.c_str())==0){
        LOG::SuccessLog(__FILE__,__LINE__,"remove file success");
        return 0;
    }else{
        LOG::ErrorLog(__FILE__,__LINE__,"remove file error");
        return 1;
    }
}

zjy::pic::picFormat ImageBaseTool::parseImageFormat(std::string filename)
{
    std::string format{filename.substr(filename.find_last_of('.'))};
    if(format == zjy::pic::picFormatSuffix::JPG)
        return zjy::pic::picFormat::jpg;
    else if(format == zjy::pic::picFormatSuffix::PNG)
        return zjy::pic::picFormat::png;
    else if(format == zjy::pic::picFormatSuffix::JPEG)
        return zjy::pic::picFormat::JPEG;
    else if(format == zjy::pic::picFormatSuffix::TIFF)
        return zjy::pic::picFormat::TIFF;
    else
        return zjy::pic::picFormat::other;
}

cv::Mat ImageBaseTool::binaryToMat(std::string data)
{
    cv::Mat pic;
    std::vector<uchar> picData;
    std::string::iterator beg=data.begin();
    while (beg!=data.end()) {
        picData.push_back(*beg);
        beg++;
    }
    pic = cv::imdecode(picData,cv::IMREAD_COLOR);
    return pic;
}

std::string ImageBaseTool::matToBinary(cv::Mat &src, std::string type)
{
    std::string str;
    std::vector<unsigned char> buff;
    cv::imencode(type, src, buff);
    str.resize(buff.size());
    memcpy(&str[0], buff.data(), buff.size());
    return str;
}


static const std::string base64_chars{"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"};
std::string ImageBaseTool::base64_encode(const char *bytes_to_encode, unsigned int in_len)
{
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for(i = 0; (i <4) ; i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i)
    {
        for(j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; (j < i + 1); j++)
            ret += base64_chars[char_array_4[j]];

        while((i++ < 3))
            ret += '=';
    }

    return ret;
}

std::string ImageBaseTool::base64_decode(const std::string &encoded_string)
{
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;

    while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i ==4) {
            for (i = 0; i <4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                ret += char_array_3[i];
            i = 0;
        }
    }

    if (i) {
        for (j = i; j <4; j++)
            char_array_4[j] = 0;

        for (j = 0; j <4; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
    }

    return ret;
}

std::vector<std::string> ImageBaseTool::getFiles(std::string dirPath)
{
    std::vector<std::string> pics;
    std::string command = "touch temp.log | ls "+dirPath+" > temp.log";
    system(command.c_str());
    std::fstream f_log{"temp.log"};
    if(f_log.fail()){
        LOG::ErrorLog(__FILE__,__LINE__,__func__);
        return pics;
    }
    std::string f_name;
    while(getline(f_log,f_name)){
        pics.push_back(f_name);
    }
    system("rm temp.log");
    return pics;
}

bool ImageBaseTool::isRepeat(std::string filename, const cv::Mat &src, double minValue)
{
    cv::Mat localImage = cv::imread(filename);
    if(localImage.size() != src.size()){
        cv::resize(localImage, localImage, src.size());
    }
    //    double rtnPSNR = getPSNR(localImage, src);
    cv::Scalar rtnMSSIM = getMSSIM(localImage, src);
    double rtnV1 = rtnMSSIM.val[0]*100;
    double rtnV2 = rtnMSSIM.val[1]*100;
    double rtnV3 = rtnMSSIM.val[2]*100;
    if(rtnV1 >= minValue && rtnV2 >= minValue && rtnV3 >= minValue){
        return true;
    } else {
        return false;
    }
}

void ImageBaseTool::picResize(cv::Mat &src, std::string &dst, double times, std::string type)
{
    cv::Mat d;
    cv::resize(src, d, cv::Size(), src.rows*times, src.cols*times);
    temporaryImage(d,dst,type);
}

void ImageBaseTool::toOther(cv::Mat &src, std::string &dst, std::string format)
{
    if(src.empty()){
        LOG::ErrorLog(__FILE__, __LINE__, "image is empty");
        dst.clear();
        return;
    }
    temporaryImage(src, dst, format);
}

void ImageBaseTool::temporaryImage(cv::Mat &src, std::string &dst, std::string format)
{
    time_t t = time(nullptr);
    char ch[64] = {0};
    strftime(ch, sizeof(ch) - 1, "%Y%m%d%H%M%S", localtime(&t));
    std::string picname{ch+format};
    cv::imwrite(picname, src);
    std::ifstream f;
    f.open(picname, std::ios_base::binary | std::ios_base::in);
    f.seekg(0,std::ifstream::end);
    ssize_t len = f.tellg();
    f.seekg(0,std::ifstream::beg);
    char *str = new char[len];
    memset(str,0,static_cast<size_t>(len));
    f.read(str,len);
    if(len<=0){
        LOG::ErrorLog(__FILE__, __LINE__, "temporary Image size <0");
        return;
    }
    dst.copy(str, static_cast<size_t>(len));
    //    dst.resize(static_cast<unsigned long>(len));
    //    memcpy(const_cast<char*>(dst.c_str()), str, static_cast<unsigned long>(len));
    std::string rm{"sudo rm "+picname};
    system(rm.c_str());
    delete[] str;
}

double ImageBaseTool::getPSNR(const cv::Mat &src, const cv::Mat &dst)
{
    cv::Mat s1;
    absdiff(src, dst, s1);       // |src - dst|
    s1.convertTo(s1, CV_32F);  // cannot make a square on 8 bits
    s1 = s1.mul(s1);           // |src - dst|^2   

    cv::Scalar s = sum(s1);         // sum elements per channel

    double sse = s.val[0] + s.val[1] + s.val[2]; // sum channels

    if( sse <= 1e-10) // for small values return zero
        return 0;
    else
    {
        double  mse =sse /(double)(src.channels() * src.total());
        double psnr = 10.0*log10((255*255)/mse);
        return psnr;
    }
}

cv::Scalar ImageBaseTool::getMSSIM(const cv::Mat &src, const cv::Mat &dst)
{
    const double C1 = 6.5025, C2 = 58.5225;
    int d = CV_32F;
    cv::Mat I1, I2;
    src.convertTo(I1, d);
    dst.convertTo(I2, d);
    cv::Mat I2_2 = I2.mul(I2);
    cv::Mat I1_2 = I1.mul(I1);
    cv::Mat I1_I2 = I1.mul(I2);
    cv::Mat mu1, mu2;
    GaussianBlur(I1, mu1, cv::Size(11, 11), 1.5);
    GaussianBlur(I2, mu2, cv::Size(11, 11), 1.5);
    cv::Mat mu1_2 = mu1.mul(mu1);
    cv::Mat mu2_2 = mu2.mul(mu2);
    cv::Mat mu1_mu2 = mu1.mul(mu2);
    cv::Mat sigma1_2, sigma2_2, sigma12;
    GaussianBlur(I1_2, sigma1_2, cv::Size(11, 11), 1.5);
    sigma1_2 -= mu1_2;
    GaussianBlur(I2_2, sigma2_2, cv::Size(11, 11), 1.5);
    sigma2_2 -= mu2_2;
    GaussianBlur(I1_I2, sigma12, cv::Size(11, 11), 1.5);
    sigma12 -= mu1_mu2;
    cv::Mat t1, t2, t3;
    t1 = 2 * mu1_mu2 + C1;
    t2 = 2 * sigma12 + C2;
    t3 = t1.mul(t2);
    t1 = mu1_2 + mu2_2 + C1;
    t2 = sigma1_2 + sigma2_2 + C2;
    t1 = t1.mul(t2);
    cv::Mat ssim_map;
    divide(t3, t1, ssim_map);
    cv::Scalar mssim = mean( ssim_map );
    return mssim;
}
