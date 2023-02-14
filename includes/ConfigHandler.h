// Copyright (c) Conni Bilham & Lucy Coward 2023, All Rights Reserved.

#ifndef GALAXYSIMULATION_CONFIGHANDLER_H
#define GALAXYSIMULATION_CONFIGHANDLER_H

#include <Vector3.h>
#include <ConfigStruct.h>

struct Config {
public:
    void loadConfig(const char *config_name = "default");
    void loadConfig(int config_index);

    void saveConfig(const char *config_name);

    void listConfigs();
};

#endif //GALAXYSIMULATION_CONFIGHANDLER_H
