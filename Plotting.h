// Copyright (c) Conni Bilham & Lucy Coward 2022, All Rights Reserved.

#ifndef GALAXYSIMULATION_PLOTTING_H
#define GALAXYSIMULATION_PLOTTING_H

#include "Star.h"
#include <logging.h>
#include <Vector3.h>
//#include <matplot/matplot.h>

//using namespace matplot;

class Plotting {
    void plot_line(std::vector<double> y, std::string title) {
//        plt::plot(y);
//        plt::title(title);
    }

    void draw_kinetic_energy(std::vector<Star*>* star_list, int timeframe) {
        std::vector<long double> x, y = {};

        std::cout.precision(32);

        for (int i = 0; i <= timeframe; ++i) {
            long double total_kinetic_energy = 0;
            x.push_back(i);
            for (auto &star : *star_list) {
                total_kinetic_energy += star->kinetic_energy(i);
            }
            logging::info("Total kinetic energy at time " + std::to_string(i) + " is " + std::to_string(total_kinetic_energy));
//            auto potenergy = -((gravitationalConstant * obj1Mass * obj2Mass) / distance(obj1, obj2))

            y.push_back(total_kinetic_energy);
        }
//        matplot::plot(x, y);
    }

public:
//    void test(std::vector<Star*> star_list) {
//        auto f = figure<matplot::backend::gnuplot>(true);
//
//        std::vector<double> x = {};
//        std::vector<double> y = {};
//        std::vector<double> z = {};
//        std::vector<double> vx = {};
//        std::vector<double> vy = {};
//        std::vector<double> vz = {};
//        std::vector<double> time = {};
//        for (int i = 0; i <= star_list[0]->history.size(); ++i) {
//
//            for (auto star: star_list) {
//                star->velocity = star->velocity * parsecsPerYear_to_metersPerSecond;
//                ++i;
//                time.push_back(i);
//                x.push_back(star->history[i].position.x);
//                y.push_back(star->history[i].position.y);
//                z.push_back(star->history[i].position.z);
//            }
//        }
//// plot multiple graphs
////        matplot::subplot(1, 2, 2);
//
//
//        matplot::subplot(3, 1, 0);
//        matplot::title("Y Position");
//        matplot::plot(time, y);
//
//        matplot::subplot(3, 1, 1);
//        matplot::title("X Position");
//        matplot::plot(time, x);
//
//        matplot::subplot(3, 1, 2);
//        matplot::title("Z Position");
//        matplot::plot(time, z);
//
//        matplot::show();
//
//
////        matplot::subplot(1, 2, 1);
////        draw_kinetic_energy(&star_list, 10);
//        matplot::show();
//
//
//
////        plt::subplot(2, 2, 2);
////        plt::plot(x, y);
//
////        plt::show();
//    }
};

#endif //GALAXYSIMULATION_PLOTTING_H
