cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 11)
project(bsTest)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build) 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -Wall -Wno-unused-variable -pthread")

#opencv and mysql libs
set(MySQLInclude "/usr/local/include/mysql")
set(MYSQLlibpath "/usr/lib/x86_64-linux-gnu")
set(OpenCVlibpath "/usr/local/lib")
set(mySrc "/home/ubuntu/bs_test/src")
include_directories("/usr/local/include/opencv4")
include_directories("/usr/local/include/mysql")

#my own cpp files
file(GLOB MYSOURCEFILE RELATIVE "${mySrc}" "${mySrc}/*")
set(filename)
add_executable(bsTest src/dbpool.cpp
    src/dbpool.h
    src/dealhttpmsg.cpp
    src/dealhttpmsg.hpp
    src/headers.h
    src/httpserver.cpp
    src/httpserver.h
    src/imagebasetool.cpp
    src/imagebasetool.h
    src/imagepath.cpp
    src/imagepath.h
    src/json.cpp
    src/json.hpp
    src/log.cpp
    src/log.h
    src/main.cpp
    src/mongoose.c
    src/mongoose.h
    src/objectdetection.cpp
    src/objectdetection.h
    src/pay.cpp
    src/pay.h
    src/sqlquery.cpp
    src/sqlquery.h
    src/user.cpp
    src/user.h
    src/usermanage.cpp.autosave
    src/usermanage.h.autosave
    src/usermanageinterface.cpp
    src/usermanageinterface.h
    src/zjynamespace.h)


#opencv
file(GLOB THIRDLIB RELATIVE "${OpenCVlibpath}" "${OpenCVlibpath}/*")
link_libraries("${OpenCVlibpath}")
foreach(sofile ${THIRDLIB})
    target_link_libraries(bsTest "${OpenCVlibpath}/${sofile}")
    message("${OpenCVlibpath}/${sofile}")
endforeach()

#mysql
link_libraries("${MYSQLlibpath}")
target_link_libraries(bsTest "${MYSQLlibpath}/libmysqlclient.so")
