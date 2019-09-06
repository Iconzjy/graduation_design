TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        ImagePath.cpp \
        User.cpp \
        UserMessageManage.cpp \
        conntodb.cpp \
        httpserver.cpp \
        inithttpcallbackfuction.cpp \
        log.cpp \
        main.cpp \
        mongoose.c

HEADERS += \
    ImagePath.h \
    User.h \
    UserMessageManage.h \
    conntodb.h \
    headers.h \
    httpserver.h \
    inithttpcallbackfuction.h \
    log.h \
    mongoose.h

INCLUDEPATH +=/usr/include/mysql/mysql.h
LIBS +=/usr/lib/x86_64-linux-gnu/libmysqlclient.so

INCLUDEPATH +=/OpenCV/build-opencv-4.1.0-with-contrib/include/opencv4
LIBS +=/OpenCV/build-opencv-4.1.0-with-contrib/lib/*.so
