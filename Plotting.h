// Copyright (c) Conni Bilham & Lucy Coward 2022, All Rights Reserved.

//
// Created by Conni Bilham on 01/12/2022.
//

#ifndef GALAXYSIMULATION_PLOTTING_H
#define GALAXYSIMULATION_PLOTTING_H

#include "Star.h"
#include <logging.h>
#include <Vector3.h>
#include <external/matplotlibcpp.hpp>

namespace plt = matplotlibcpp;

class Plotting {
    void plot_line(std::vector<double> y, std::string title) {
        plt::plot(y);
        plt::title(title);
    }

    void draw_kinetic_energy(std::vector<Star> star_list, int timeframe) {
        std::vector<long double> x, y = {};

        for(int i = 0; i <= timeframe; ++i)
        for(auto &star : star_list) {
            star.kinetic_energy();
        }
    }

public:
    void test(std::vector<Star*> star_list) {
        std::vector<double> x = {};
        std::vector<double> y = {};
        for (auto star: star_list) {
            x.push_back(star->position.x);
            y.push_back(star->position.y);
        }
// plot multiple graphs
        plt::subplot(2, 2, 1);
        plt::title("Two lines on same graph");
        plt::plot_
        plt::show();



//        plt::subplot(2, 2, 2);
//        plt::plot(x, y);

        plt::show();
    }
};

#endif //GALAXYSIMULATION_PLOTTING_H
