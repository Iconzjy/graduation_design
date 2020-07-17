#include <mysql/mysql.h>
#include <pthread.h>
#include "log.h"
#include "zjynamespace.h"
#ifndef DBPOOL_H
#define DBPOOL_H


class DBPool
{  
public:
    MYSQL* getDBConn();
    void disConnect(MYSQL* aConn);
 //   static DBPool *createDBPool();
    static bool _isCreateDBPool;
    static DBPool *a;
    static DBPool *createDBPool(){
        if(_isCreateDBPool == true){
            return nullptr;
        } else {
            DBPool *a = new DBPool();
            _isCreateDBPool=true;
            return a;
        }
    }
    ~DBPool();
private:
    DBPool();
    pthread_mutex_t lock;
    MYSQL* _pools[10];
    bool _record[10];//true means used
};

#endif // DBPOOL_H
