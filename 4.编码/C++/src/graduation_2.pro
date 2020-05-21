TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        dbpool.cpp \
        httpserver.cpp \
        imagebasetool.cpp \
        imagepath.cpp \
        json.cpp \
        log.cpp \
        main.cpp \
        mongoose.c \
        objectdetection.cpp \
        pay.cpp \
        sqlquery.cpp \
        user.cpp \
        usermanageinterface.cpp

HEADERS += \
    dbpool.h \
    dealhttpmsg.hpp \
    headers.h \
    httpserver.h \
    imagebasetool.h \
    imagepath.h \
    json.hpp \
    log.h \
    mongoose.h \
    objectdetection.h \
    pay.h \
    sqlquery.h \
    user.h \
    usermanageinterface.h \
    zjynamespace.h


INCLUDEPATH +=/usr/include/mysql/mysql.h
LIBS +=/usr/lib/x86_64-linux-gnu/libmysqlclient.so

INCLUDEPATH +=/OpenCV/build-opencv-4.1.0-with-contrib/include/opencv4
LIBS +=/OpenCV/build-opencv-4.1.0-with-contrib/lib/*.so

DISTFILES += \
    object_detection_classes_yolov3.txt \
    yolov3-tiny.cfg \
    yolov3-tiny.weights \
    人脸识别Model/face_dect.py \
    人脸识别Model/graph.pbtxt \
    人脸识别Model/index.jpeg \
    人脸识别Model/record.txt \
    人脸识别Model/sorted_inference_graph.pb \
    文字识别/trained_classifierNM1.xml \
    文字识别/trained_classifierNM2.xml
