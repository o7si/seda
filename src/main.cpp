#include "pch.h"
#include "log.h"
#include "config.h"

#include "seda/stage.h"
#include "stage/TestStage.hpp"

int main(int argc, char* argv[])
{
    REGISTER_STAGE(CountStage)
    REGISTER_STAGE(MaxMinStage)
    REGISTER_STAGE(MultiStage)
    REGISTER_STAGE(BinaryStage)
    o7si::config::load("/root/reps/seda/conf/test.conf");

    return 0;
};
