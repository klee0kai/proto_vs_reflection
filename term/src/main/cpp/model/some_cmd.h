//
// Created by panda on 07.06.2023.
//

#ifndef DATABRIDGE_SOME_CMD_H
#define DATABRIDGE_SOME_CMD_H

#include <map>
#include "meta.h"

struct some_cmd {
    int id;
    long long count;
    float value;
    double valueD;
    std::string name;
    std::list<meta> metas;
};

#endif //DATABRIDGE_SOME_CMD_H
