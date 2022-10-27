// Copyright (c) Conni Bilham & Lucy Coward 2022, All Rights Reserved.

#ifndef GALAXYSIMULATION_LOGGING_H
#define GALAXYSIMULATION_LOGGING_H

#include <fstream>
#include <iostream>
#include <chrono>
#include <ctime>

using namespace std;
struct tm;

class logging {
    // Get current date/time, format is YYYY-MM-DD.HH:mm:ss
    static std::string currentDateTime() {
        time_t     now = time(nullptr);
        struct tm  tstruct;
        char       buf[80];
        tstruct = *localtime(&now);
        // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
        // for more information about date/time format
        strftime(buf, sizeof(buf), "%X", &tstruct);

        return buf;
    }

    template <typename T1, typename T2>
    static void log_to_file(string prx, T1 arg1 = "", T2 arg2 = "") {
        ofstream log_file("log.txt", ios::app);
        log_file << "[ " << currentDateTime() << " ] - " << prx << " - " << arg1 << arg2 << endl;
        log_file.close();
    }

    template <typename T1, typename T2>
    static void log(string prx, T1 arg1 = "", T2 arg2 = "", bool should_output = true, bool should_flush = false) {
        log_to_file(prx, arg1, arg2);

        if(should_output) {
            if(!should_flush)
                cout << "[ " << currentDateTime() << " ] - " << prx << " - " << arg1 << arg2 << endl;
            else
                std::cout << '\r' << "[ " << currentDateTime() << " ] - " << prx << " - " << arg1 << arg2 << std::flush;
        }

    }

public:
    template <typename T1, typename T2>
    static void verbose(T1 arg1, T2 arg2, bool should_output = true, bool should_flush = false) {
         log("[ VERBOSE ]", arg1, arg2, should_output, should_flush);
    }

    template <typename T1, typename T2>
    static void debug(T1 arg1, T2 arg2, bool should_output = true, bool should_flush = false) {
//        log("[ DEBUG ]", arg1, arg2, should_output, should_flush);
    }

    template <typename T1, typename T2>
    static void info(T1 arg1, T2 arg2, bool should_output = true, bool should_flush = false) {
        log("[ INFO ]", arg1, arg2, should_output, should_flush);
    }
};

#endif //GALAXYSIMULATION_LOGGING_H
