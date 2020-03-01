#include "log.h"

int main(int argc, char* argv[])
{
    LOG(DEBUG, SYs) << "debug";
    LOG(INFO, SYS) << "info";
    LOG(WARN, sys) << "warn";
    LOG(ERROR, sys) << "error";
    LOG(FATAL, sys) << "fatal";
    return 0;    
} 
