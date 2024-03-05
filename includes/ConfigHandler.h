// Copyright (c) Conni Bilham & Lucy Coward 2023, All Rights Reserved.

#ifndef GALAXYSIMULATION_CONFIGHANDLER_H
#define GALAXYSIMULATION_CONFIGHANDLER_H

#include <Vector3.h>
#include <ConfigStruct.h>

struct Config {
public:

    static void saveConfig(string path, ConfigStruct config) {
        std::ofstream o(path);
        ConfigStruct conf;
        json jsonNewConfig = config;
        o << std::setw(4) << jsonNewConfig << std::endl;
        o.close();
        logging::info("Config file saved.");
    }

    static ConfigStruct loadConfig(string path) {
        // read a JSON file
        if(!std::filesystem::exists(path)) {
            logging::error("Couldn't find config file.");
//        std::ofstream o(path);
//        ConfigStruct conf;
//        json jsonNewConfig = conf;
//        o << std::setw(4) << jsonNewConfig << std::endl;
//        o.close();
//        logging::info("Config file created.");
            logging::info("Error: Config file not found.");
            exit(1);
        }
        logging::info("Config Found.");

        ConfigStruct config;
        {
            json configJson;
            std::ifstream conf(path);
            conf >> configJson;
            conf.close();
            config = configJson;

            logging::info("Config loaded");

            // Print all json components
            std::cout << std::setw(4) << config.version.major << std::endl;

            for (auto it = configJson.begin(); it != configJson.end(); ++it) {
                std::cout << "- " << it.key() << ": ";
                if (it.value().is_object() || it.value().is_array()) {
                    std::cout << std::endl;
                    for (auto inner_it = it.value().begin(); inner_it != it.value().end(); ++inner_it) {
                        std::cout << "  - " << inner_it.key() << ": " << inner_it.value() << std::endl;
                    }
                } else {
                    std::cout << it.value() << std::endl;
                }
            }
        }
        return config;
    }
};

#endif //GALAXYSIMULATION_CONFIGHANDLER_H
