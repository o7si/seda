#include "pch.h"
#include "log.h"
#include "config.h"

#include "seda/stage.h"
#include "stage/TestStage.hpp"

void reg()
{
    REGISTER_STAGE(CountStage, 1)
    REGISTER_STAGE(MaxMinStage, 2)
    REGISTER_STAGE(MultiStage, 3)
    REGISTER_STAGE(BinaryStage, 4)
}

int main(int argc, char* argv[])
{
    o7si::config::load("/root/reps/seda/conf/test.conf");
    reg();
    reg();

    return 0;
};
