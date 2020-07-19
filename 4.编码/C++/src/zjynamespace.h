#ifndef ZJYNAMESPACE_H
#define ZJYNAMESPACE_H


#include <functional>
#include <unordered_map>
#include <opencv2/opencv.hpp>
#include "mongoose.h"

namespace zjy {

    /***************************
     * define in log.h
    #if true
        #define DEBUGMODEL
    #endif
    ****************************/

    //this function is used to call opencv those image processing function
    using OnResCallback = std::function<void (cv::Mat*, cv::Mat)>;

    //add UserManageInterface class as T
    template <typename T>
    using ReqHandler = std::function<void (mg_connection*, http_message*, T*/*, OnResCallback*/)>;

    //this function is used to storage the request mapping table(url and callback function)
    template <typename T>
    using ReqTables=std::unordered_map<std::string, zjy::ReqHandler<T>>;



    namespace sql {
    const std::string host{"localhost"};
    const int port{3306};
    const std::string user{"root"};
    const std::string db{"graduation"};
    const std::string pwd{""};
    }

    namespace pic {
    enum picSortType{
        None=0,//all pictures;
        Time=1,//sort by upload Date/time;
        Recycler=-1,//sort by recyclers;
        Image=3,
        Other=4//sort by...... i don't know
    };

    enum picFormat {
        JPEG=0,
        jpg=1,
        png=2,
        TIFF=3,
        other=4
    };

    namespace picFormatSuffix {
    const std::string JPEG{".jpeg"};
    const std::string JPG{".jpg"};
    const std::string PNG{".png"};
    const std::string TIFF{".tiff"};
    }

    //here is path where storage picture
    namespace picRoot {
    const std::string path{"/home/ubuntu/bs_test/"};

    }
    }



#ifndef zjyhttp
#define zjyhttp
    namespace s_HttpType{
        const std::string HTML{"text/html"};
        const std::string PLAIN{"text/plain"};
        const std::string XML{"text/xml"};
        const std::string GIF{"image/gif"};
        const std::string JPEG{"image/jpeg"};
        const std::string PNG{"image/png"};
        const std::string JSON{"application/json"};
    }
#endif

}


#endif // ZJYNAMESPACE_H
