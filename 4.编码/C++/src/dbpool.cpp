#include "dbpool.h"

MYSQL *DBPool::getDBConn()
{
    int i=0;
    while(true){
        if(false == this->_record[i]){
            this->_record[i] = true;
            return this->_pools[i];
        }
        if(i == 9){
            i=0;
        }
        i++;
    }
}

void DBPool::disConnect(MYSQL *aConn)
{
    for(int i=0; i<10; i++){
        if(aConn == this->_pools[i]){
            aConn = nullptr;
            this->_record[i] = false;
        }
    }
}



DBPool::~DBPool()
{
    for(int i=0; i<10; i++){
        mysql_close(this->_pools[i]);
    }
//    delete [] a;
}
DBPool::DBPool()
{
    for (int i=0; i<10; i++) {
        this->_record[i] = false;
    }
    using namespace zjy::sql;
    pthread_mutex_lock(&lock);
    for(int i=0; i<10; i++){
        this->_pools[i] = new MYSQL;
        memset(this->_pools[i], 0, sizeof(MYSQL));
        MYSQL* sql;
        sql = mysql_init(nullptr);
        if(!sql) {
            LOG::ErrorLog(__FILE__,__LINE__, __func__);
            exit(EXIT_FAILURE);
        }

        sql = mysql_real_connect(sql,host.c_str(),user.c_str(),pwd.c_str(),db.c_str(),port,nullptr,0);
        if(sql){
            this->_pools[i] = sql;
            LOG::SuccessLog(__FILE__, __LINE__, "Connection success\n");
        }else{
            LOG::ErrorLog(__FILE__,__LINE__, __func__);
        }
    }
    pthread_mutex_unlock(&lock);
}
