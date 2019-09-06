#ifndef CONNTODB_H
#define CONNTODB_H

#include "headers.h"

class ConnToDB
{
public:
    ConnToDB();
    ~ConnToDB();

protected:
    MYSQL* rtnMYSQL();
private:
    void init();
    MYSQL* _conn;

};

#endif // CONNTODB_H
