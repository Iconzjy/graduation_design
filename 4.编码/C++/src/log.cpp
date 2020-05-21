#include "log.h"

void LOG::SuccessLog(std::string file, int line, std::string msg)
{
#ifdef DEBUGMODEL
    std::cerr << "[Success]:  In file " << file << " at line " << std::to_string(line) << ", " << msg << std::endl;
#endif
}

void LOG::ErrorLog(std::string file, int line, std::string msg)
{
#ifdef DEBUGMODEL
    std::cerr << "[ERROR]: In file " << file << " at line " << std::to_string(line) << ", " << msg << std::endl;
#endif
}

void LOG::testLog(std::string &msg)
{
    std::cerr << "-----------------------------" << std::endl;
    std::cerr << msg << std::endl;
    std::cerr << "-----------------------------" << std::endl;
}
void LOG::testLog(std::string&& msg)
{
    std::cerr << "-----------------------------" << std::endl;
    std::cerr << msg << std::endl;
    std::cerr << "-----------------------------" << std::endl;
}
