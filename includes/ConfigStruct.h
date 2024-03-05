// Copyright (c) Conni Bilham & Lucy Coward 2023, All Rights Reserved.

#ifndef GALAXYSIMULATION_CONFIGSTRUCT_H
#define GALAXYSIMULATION_CONFIGSTRUCT_H

#include <json.h>
#include "Vector3.h"
#include "logging.h"

using json = nlohmann::json;

struct ConfigStruct {
public:
    struct Version {
        int major = 1;
        int minor = 0;
        int patch = 0;
    } version;

    struct ClientSettings {
        int animation_speed = 5;

    } client_settings;

    struct RegionMatrix {
        Vector_t<int> region_divisions = {1, 1, 1};

        // This is the amount the regions are scaled by
        // after stars regions are resized for the new region divisions
        float regions_scale_min_PFit = 1.01f;
        float regions_scale_max_PFit = 1.01f;

        // This is the amount regions overlap each other
        float overlap_factor = 0.0003f;

    } region_matrix;

    struct StarGenerator {
        bool enabled = false;

        const char *modes[3] = {"None", "Gaussian", "Uniform"};
        int active_mode = 0;

        int star_count = 0;
        float mean_mass = 0;
        float std_mass = 0;

        struct Global {
        } global;
        Vectorr min_position = Vectorr();
        Vectorr max_position = Vectorr();

        struct Gaussian {

        } gaussian;
        Vectorr velocity_at_origin = Vectorr();
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

    struct Simulation {
        int simulation_frames  = 0;
        long double time_scale = 0; // Unit: years

        // Internal usage: not user accessible
        long double time_step  = 0;
    } simulation;
};

#pragma region Serialization

#pragma region Version Serialization

static void to_json(json &j, const ConfigStruct::Version &c) {
    j = json{
            {"major", c.major},
            {"minor", c.minor},
            {"patch", c.patch}
    };
}

static void from_json(const json &j, ConfigStruct::Version &c) {
    c.major = j.at("major").get<int>();
    c.minor = j.at("minor").get<int>();
    c.patch = j.at("patch").get<int>();
}

#pragma endregion

#pragma region ClientSettings Serialization

static void to_json(json &j, const ConfigStruct::ClientSettings &c) {
    j = json{
            {"animation_speed", c.animation_speed}
    };
}

static void from_json(const json &j, ConfigStruct::ClientSettings &c) {
    c.animation_speed = j.at("animation_speed").get<int>();
}

#pragma endregion

#pragma region RegionMatrix Serialization

static void to_json(json &j, const ConfigStruct::RegionMatrix &c) {
    j = json{
            {"region_divisions",        c.region_divisions},
            {"regions_scale_min_PFit",  c.regions_scale_min_PFit},
            {"regions_scale_max_PFit",  c.regions_scale_max_PFit},
            {"overlap_factor",          c.overlap_factor}
    };
}

static void from_json(const json &j, ConfigStruct::RegionMatrix &c) {
    // Print out region div
    c.region_divisions = j.at("region_divisions").get<Vector_t<int>>();
    c.regions_scale_min_PFit = j.at("regions_scale_min_PFit").get<float>();
    c.regions_scale_max_PFit = j.at("regions_scale_max_PFit").get<float>();
    c.overlap_factor = j.at("overlap_factor").get<float>();
}

#pragma endregion

#pragma region StarGenerator Serialization

static void to_json(json &j, const ConfigStruct::StarGenerator &c) {
    j = json{
            {"enabled",             c.enabled},
            {"active_mode",         c.active_mode},
            {"star_count",          c.star_count},
            {"mean_mass",           c.mean_mass},
            {"std_mass",            c.std_mass},
//            {"min_position",        c.},
//            {"max_position",        c.max_position},
            {"velocity_at_origin",  c.velocity_at_origin},
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

static void from_json(const json &j, ConfigStruct::StarGenerator &c) {
    c.enabled = j.at("enabled").get<bool>();
    c.active_mode = j.at("active_mode").get<int>();
    c.star_count = j.at("star_count").get<int>();
    c.mean_mass = j.at("mean_mass").get<float>();
    c.std_mass = j.at("std_mass").get<float>();
//    c.min_position = j.at("min_position").get<Vectorr>();
//    c.cmax_position = j.at("max_position").get<Vectorr>();
    c.velocity_at_origin = j.at("velocity_at_origin").get<Vectorr>();
    c.variation_velocity = j.at("variation_velocity").get<Vectorr>();
    c.variation_direction = j.at("variation_direction").get<Vectorr>();
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

#pragma region Simulation Serialization

static void to_json(json &j, const ConfigStruct::Simulation &c) {
    j = json{
            {"simulation_frames",   c.simulation_frames},
            {"time_scale",          c.time_scale}
    };
}

static void from_json(const json &j, ConfigStruct::Simulation &c) {
    c.simulation_frames = j.at("simulation_frames").get<int>();
    c.time_scale = j.at("time_scale").get<long double>();
}

#pragma endregion

#pragma region Config Serialization

static void to_json(json &j, const ConfigStruct &c) {
    j = json{
            {"version",         c.version},
            {"client_settings", c.client_settings},
            {"region_matrix",   c.region_matrix},
            {"star_generation", c.star_generation},
            {"simulation",      c.simulation },
    };
}

static void from_json(const json &j, ConfigStruct &c) {
    c.version = j.at("version").get<ConfigStruct::Version>();
    c.client_settings = j.at("client_settings").get<ConfigStruct::ClientSettings>();
    c.region_matrix = j.at("region_matrix").get<ConfigStruct::RegionMatrix>();
    c.star_generation = j.at("star_generation").get<ConfigStruct::StarGenerator>();
    c.simulation = j.at("simulation").get<ConfigStruct::Simulation>();
}

#pragma endregion

#pragma endregion

#endif //GALAXYSIMULATION_CONFIGSTRUCT_H
