//
// Created by Conni Bilham on 04/10/2022.
//

#ifndef GALAXYSIMULATION_LOGGING_H
#define GALAXYSIMULATION_LOGGING_H

#include <iostream>
#include <iostream>
#include <chrono>
#include <ctime>

using namespace std;


class logging {
    // Get current date/time, format is YYYY-MM-DD.HH:mm:ss
    static const std::string currentDateTime() {
        time_t     now = time(0);
        struct tm  tstruct;
        char       buf[80];
        tstruct = *localtime(&now);
        // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
        // for more information about date/time format
        strftime(buf, sizeof(buf), "%X", &tstruct);

        return buf;
    }

    template <typename T1, typename T2>
    static void log(string prx, T1 arg1 = "", T2 arg2 = "") {
        cout << currentDateTime() << " - " << prx << " - " << arg1 << arg2 << endl;
    }

public:
    template <typename T1, typename T2>
    static void verbose(T1 arg1, T2 arg2) {
        log("[VERBOSE]", arg1, arg2);
    }
    template <typename T1, typename T2>
    static void info(T1 arg1, T2 arg2) {
        log("[INFORMATION]", arg1, arg2);
    }
};

inline logging l;
#endif //GALAXYSIMULATION_LOGGING_H
