#include "conntodb.h"

ConnToDB::ConnToDB()
{
    init();
}

ConnToDB::~ConnToDB()
{
    mysql_close(_conn);
}

MYSQL *ConnToDB::rtnMYSQL()
{
    return _conn;
}

void ConnToDB::init()
{
    using namespace zjy::sql;
    _conn=mysql_init(nullptr);//一般都是传递一个NULL
    if(!_conn){
        //log event
        LOG("[Error]: mysql init error.");
        exit(EXIT_FAILURE);
    }

    _conn=mysql_real_connect(_conn,host.c_str(),user.c_str(),pwd.c_str(),db.c_str(),port,nullptr,0);
    if(_conn){
        LOG("Connection success\n");
    }else{
        LOG("Connection failed\n");
    }

}
