// Copyright (c) Conni Bilham & Lucy Coward 2023, All Rights Reserved.

//
// Created by Conni Bilham on 26/01/2023.
//

#ifndef GALAXYSIMULATION_CONFIGHANDLER_H
#define GALAXYSIMULATION_CONFIGHANDLER_H

#include <Vector3.h>

static struct Config {
public:
    int animation_speed = 5;

    struct RegionMatrix {
        Vectorr region_divisions = Vectorr{1, 1, 1};

        // This is the amount the regions are scaled by
        // after stars regions are resized for the new region divisions
        float regions_scale_min_PFit = 1.01f;
        float regions_scale_max_PFit = 1.01f;

        // This is the amount regions overlap each other
        float overlap_factor = 0.0003f;

    } RegionMatrix;

    // Complete & Serializable
    struct StarGenerator {
        bool enabled = false;

        const char *modes[3] = {"None", "Gaussian", "Uniform"};
        int active_mode = 0;

        int star_count = 0;
        float mean_mass = 0;
        float std_mass = 0;

        Vectorr min_position = Vectorr(0, 0, 0);
        Vectorr max_position = Vectorr();

        Vectorr velocityAtOrigin = Vectorr();
        Vectorr variation_velocity = Vectorr();
        Vectorr variation_direction = Vectorr();

        std::vector<float> angle_of_arms = {};
        int number_of_arms = 0;
        int stars_per_arm = 0;
        float arm_width = 0;
        float arm_length = 0;
        float arm_height = 0;
        float arm_offset = 0;

        Vectorr gaussian_mean = Vectorr();
        Vectorr gaussian_std = Vectorr();

    } star_generation;

} config;

#pragma region StarGenerator Serialization

static void to_json(json &j, const Config::StarGenerator &c) {
    j = json{
            {"enabled",             c.enabled},
            {"active_mode",         c.active_mode},
            {"star_count",          c.star_count},
            {"mean_mass",           c.mean_mass},
            {"std_mass",            c.std_mass},
            {"min_position",        c.min_position},
            {"max_position",        c.max_position},
            {"velocityAtOrigin",    c.velocityAtOrigin},
            {"variation_velocity",  c.variation_velocity},
            {"variation_direction", c.variation_direction},
            {"angle_of_arms",       c.angle_of_arms},
            {"number_of_arms",      c.number_of_arms},
            {"stars_per_arm",       c.stars_per_arm},
            {"arm_width",           c.arm_width},
            {"arm_length",          c.arm_length},
            {"arm_height",          c.arm_height},
            {"arm_offset",          c.arm_offset},
            {"gaussian_mean",       c.gaussian_mean},
            {"gaussian_std",        c.gaussian_std}
    };
}

static void from_json(const json &j, Config::StarGenerator &c) {
        c.enabled = j.at("enabled").get<bool>();
        c.active_mode = j.at("active_mode").get<int>();
        c.star_count = j.at("star_count").get<int>();
        c.mean_mass = j.at("mean_mass").get<float>();
        c.std_mass = j.at("std_mass").get<float>();
        c.min_position = j.at("min_position").get<Vectorr >();
        c.max_position = j.at("max_position").get<Vectorr >();
        c.velocityAtOrigin = j.at("velocityAtOrigin").get<Vectorr >();
        c.variation_velocity = j.at("variation_velocity").get<Vectorr >();
        c.variation_direction = j.at("variation_direction").get<Vectorr >();
        c.angle_of_arms = j.at("angle_of_arms").get<std::vector<float>>();
        c.number_of_arms = j.at("number_of_arms").get<int>();
        c.stars_per_arm = j.at("stars_per_arm").get<int>();
        c.arm_width = j.at("arm_width").get<int>();
        c.arm_length = j.at("arm_length").get<int>();
        c.arm_height = j.at("arm_height").get<int>();
        c.arm_offset = j.at("arm_offset").get<int>();
        c.gaussian_mean = j.at("gaussian_mean").get<Vectorr >();
        c.gaussian_std = j.at("gaussian_std").get<Vectorr >();
    }

#pragma endregion




#endif //GALAXYSIMULATION_CONFIGHANDLER_H
