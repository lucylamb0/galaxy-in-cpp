// Copyright (c) Conni Bilham & Lucy Coward 2023, All Rights Reserved.

#include "../star_generator.h"

void star_generator_gaussian(
        std::vector<Star*> *star_list,
        RegionMatrix* parent_region_matrix, // the region matrix that the stars are in

        long int number_of_stars,
        float mean_mass, // in solar masses
        float std_dev_mass, // in solar masses
        Vectorr min_position, // minimum position of a star can have
        Vectorr max_position, // maximum position of a star can have
        Vectorr velocity_at_origin, // average velocity of a star can have at the origin pc/year
        Vectorr variation_velocity, // variation of velocity of a star can have pc/year
        Vectorr variation_in_direction, // the variation in direction of the velocity in radians +-
        int arms, // number of arms in the galaxy
        int number_of_stars_per_arm, // number of stars per arm
        std::vector<float> arm_positions, // the positions of the arms in radians
        float arm_width, // the width of the arms in parsecs
        float arm_height, // the height of the arms in parsecs
        float arm_length, // the length of the arms in pc
        float arm_offset, // the offset of the arms in radians
        Vectorr gaussian_mean, // mean of the gaussian distribution (0 is the centre of the galaxy)
        Vectorr gaussian_standard_deviation // standard deviation of the gaussian distribution (should be under 1) (x and y should be the same)
) {
    Random random = Random();

    // random number generators for each variable
//    std::normal_distribution<float> dis_mass(mean_mass, std_dev_mass); // distribution for the mass of the stars
    auto dis_mass = random.normal_distro(mean_mass, std_dev_mass);

    auto dis_position_x = random.uniform_real_distro(min_position.x, max_position.x);
    auto dis_position_y = random.uniform_real_distro(min_position.y, max_position.y);
    auto dis_position_z = random.uniform_real_distro(min_position.z, max_position.z);

    auto dis_velocity_x = random.uniform_real_distro(velocity_at_origin.x - variation_velocity.x, velocity_at_origin.x + variation_velocity.x);
    auto dis_velocity_y = random.uniform_real_distro(velocity_at_origin.y - variation_velocity.y, velocity_at_origin.y + variation_velocity.y);
    auto dis_velocity_z = random.uniform_real_distro(velocity_at_origin.z - variation_velocity.z, velocity_at_origin.z + variation_velocity.z);

    auto dis_direction_x = random.uniform_real_distro(-variation_in_direction.x, variation_in_direction.x);
    auto dis_direction_y = random.uniform_real_distro(-variation_in_direction.y, variation_in_direction.y);

    // for gaussian distribution we need to first get gaussian random distances from the origin instead of gaussian xyz positions individually
    std::normal_distribution<long double> dis_gaussian_x(gaussian_mean.x, gaussian_standard_deviation.x);
    std::normal_distribution<long double> dis_gaussian_y(gaussian_mean.y, gaussian_standard_deviation.y);
    std::normal_distribution<long double> dis_gaussian_z(gaussian_mean.z, gaussian_standard_deviation.z);
    // random bool
    long double galaxy_mass = 0;
    std::uniform_int_distribution<int> dis_bool(0, 1);
    for (int i = 1; i < number_of_stars+1; i++) {
        long double velocity_z = dis_bool(random.gen) != 1 ? -dis_velocity_z(random.gen) : dis_velocity_z(random.gen);

        long double gaussian_x_norm = dis_gaussian_x(random.gen);
        long double gaussian_y_norm = dis_gaussian_y(random.gen);
        long double gaussian_z_norm = dis_gaussian_z(random.gen);
        while (gaussian_x_norm > 1) {
            gaussian_x_norm = dis_gaussian_x(random.gen);
        }
        while (gaussian_y_norm > 1) {
            gaussian_y_norm = dis_gaussian_y(random.gen);
        }
        while (gaussian_z_norm > 1) {
            gaussian_z_norm = dis_gaussian_z(random.gen);
        }

        float mass = dis_mass(random.gen);
        galaxy_mass += mass;

        Vectorr position = Vectorr(gaussian_x_norm * max_position.x, gaussian_y_norm * max_position.y, gaussian_z_norm * max_position.z);
//        // normalise the position vector
//        position.normalise();
//        // multiply the position vector by the distance from the origin
//        position = position * dis_distance_from_origin(random.gen);
//        // create the velocity vector
        Vectorr rel_pos(((max_position.x - std::abs(position.x))/max_position.x),((max_position.y - std::abs(position.y))/max_position.y), ((max_position.z - std::abs(position.z))/max_position.z));
        Vectorr velocity = Vectorr(
                std::sqrt(rel_pos.y) * std::cos((rel_pos.x * PI)/2) * dis_velocity_x(random.gen), // get velocities using relative positions to the centre of the galaxy using inverse square law
                std::sqrt(rel_pos.x) * std::cos((rel_pos.y * PI)/2) * dis_velocity_y(random.gen),
                std::sqrt(rel_pos.z) * velocity_z // z velocity does not need to have a cos function as the direction of orbit is not in the z direction
        );
        velocity.rotate(dis_direction_x(random.gen), dis_direction_y(random.gen),
                        0); // rotate the velocity by a random amount in the x and y direction
        star_list->emplace_back(new Star( // add the star to the star list
                i, // get the id of the star
                position,
                velocity,
                Vectorr(0, 0, 0),
                mass,
                parent_region_matrix,
                0
        ));
    }

    if (arms > 0) { // if there are arms
        for (int i = 0; i < arms; i++) { // for each arm
            Vectorr position = Vectorr(0, 0, 0); // set the position of the arm to the origin
            Vectorr position_vector = Vectorr(0, 1, 0); // set the position vector of the arm to the y axis
            position_vector.scale(arm_length); // scale the position vector by the length of the arm
            // make a volume for the arm along the position vector
            Vectorr point_1 = Vectorr(0 + arm_width, 0, 0 + arm_height); // width in x, heoght in z
            Vectorr point_2 = Vectorr(0 - arm_width, 0, 0 + arm_height);
            Vectorr point_3 = Vectorr(0 + arm_width, 0, 0 - arm_height);
            Vectorr point_4 = Vectorr(0 - arm_width, 0, 0 - arm_height);
            // we now have a pyramid with the base at the origin and the top at the position vector
            // we now need to rotate the pyramid by the angle of the arm
            float angle = arm_positions[i];// get the angle of the arm
            point_1.rotate(0, 0, angle); // rotate the points by the angle
            point_2.rotate(0, 0, angle);
            point_3.rotate(0, 0, angle);
            point_4.rotate(0, 0, angle);
            position_vector.rotate(0, 0, angle); // rotate the position vector by the angle
            // we now have a pyramid with the base at the origin and the top at the position vector all rotated by the angle
            // curve the pyramid by moving the top of the pyramid by the offset
            Vectorr position_vector_before_offset = position_vector;
            position_vector.rotate(0, 0, arm_offset); // rotate the position vector by the offset around the z axis
            // add stars to the arm
            long double min_x = std::min(point_1.x, std::min(point_2.x, std::min(point_3.x, point_4.x))); // get the minimum x value
            long double max_x = std::max(point_1.x, std::max(point_2.x, std::max(point_3.x, point_4.x))); // get the maximum x value
            long double min_y = std::min(point_1.y, std::min(point_2.y, std::min(point_3.y, point_4.y))); // get the minimum y value
            long double max_y = std::max(point_1.y, std::max(point_2.y, std::max(point_3.y, point_4.y))); // get the maximum y value
            long double min_z = std::min(point_1.z, std::min(point_2.z, std::min(point_3.z, point_4.z))); // get the minimum z value
            long double max_z = std::max(point_1.z, std::max(point_2.z, std::max(point_3.z, point_4.z))); // get the maximum z value
            Vectorr min = Vectorr(min_x, min_y, min_z); // create a vector with the minimum values
            Vectorr max = Vectorr(max_x, max_y, max_z); // create a vector with the maximum values
            std::uniform_real_distribution<long double> star_pos_x(min_x, max_x); // define a distribution for the x position of the stars
            std::uniform_real_distribution<long double> star_pos_y(min_y, max_y); // define a distribution for the y position of the stars
            std::uniform_real_distribution<long double> star_pos_z(min_z, max_z); // define a distribution for the z position of the stars
            std::normal_distribution<long double> star_distance_from_origin(
                    0,
                    std::sqrt((min.x * min.x) +
                              (min.y * min.y) +
                              (min.z * min.z)));

            for (int j = 1; j < number_of_stars_per_arm+1; j++) { // for each star in the arm

                long double gaussian_x_norm = dis_gaussian_x(random.gen);
                long double gaussian_y_norm = dis_gaussian_y(random.gen);
                long double gaussian_z_norm = dis_gaussian_z(random.gen);
                while (gaussian_x_norm > 1) {
                    gaussian_x_norm = dis_gaussian_x(random.gen);
                }
                while (gaussian_y_norm > 1) {
                    gaussian_y_norm = dis_gaussian_y(random.gen);
                }
                while (gaussian_z_norm > 1) {
                    gaussian_z_norm = dis_gaussian_z(random.gen);
                }

                Vectorr position_star_arm = Vectorr(gaussian_x_norm * max.x, gaussian_y_norm * max.y, gaussian_z_norm * max.z);

//                Vectorr position_star_arm = Vectorr(dis_position_x(random.gen), dis_position_y(random.gen), dis_position_z(random.gen));
//                // normalise the position vector
//                position_star_arm.normalise();
//                // multiply the position vector by the distance from the origin
//                position_star_arm = position_star_arm * dis_distance_from_origin(random.gen);
                if (not(position_star_arm.x >= min.x && position_star_arm.x <= max.x && position_star_arm.y >= min.y && position_star_arm.y <= max.y && position_star_arm.z >= min.z && position_star_arm.z <= max.z)) {
                    std::cout << "star position is not in the arm" << std::endl;
                    j -= 1;
                    continue;
                }

                float mass = dis_mass(random.gen); // get the mass of the star
                galaxy_mass += mass; // add the mass of the star to the mass of the galaxy

                long double velocity_z;
                if (dis_bool(random.gen) != 1) { // for getting a random negative or positive z velocity
                    velocity_z = -dis_velocity_z(random.gen);
                }
                else {
                    velocity_z = dis_velocity_z(random.gen);
                }
                Vectorr rel_pos(((max_x - std::abs(position.x))/max_x),((max_y - std::abs(position.y))/max_y), ((max_z - std::abs(position.z))/max_z));
                Vectorr velocity_star_arm = Vectorr(
                        std::sqrt(rel_pos.y) * std::cos((rel_pos.x * PI)/2) * dis_velocity_x(random.gen), // get velocities using relative positions to the centre of the galaxy using inverse square law
                        std::sqrt(rel_pos.x) * std::cos((rel_pos.y * PI)/2) * dis_velocity_y(random.gen),
                        std::sqrt(rel_pos.z) * velocity_z // z velocity does not need to have a cos function as the direction of orbit is not in the z direction
                );
                velocity_star_arm.rotate(dis_direction_x(random.gen), dis_direction_y(random.gen), 0); // rotate the velocity by a random amount in the x and y direction
                star_list->emplace_back(new Star( // add the star to the star list
                        j + number_of_stars + (number_of_stars_per_arm * i), // get the id of the star
                        position_star_arm,
                        velocity_star_arm,
                        Vectorr(0,0,0),
                        mass,
                        parent_region_matrix,
                        0
                ));
            }
        }
    }
    std::cout << "galaxy mass: " << galaxy_mass << std::endl;

}

void generate_gaussian(std::vector<Star*> *star_list, ConfigStruct::StarGenerator  config, RegionMatrix* parent_region_matrix) {
    star_generator_gaussian(
            star_list,
            parent_region_matrix,
            config.star_count,
            config.mean_mass,
            config.std_mass,
            config.min_position,
            config.max_position,
            config.velocity_at_origin,
            config.variation_velocity,
            config.variation_direction,
            config.number_of_arms,
            config.stars_per_arm,
            config.angle_of_arms,
            config.arm_width,
            config.arm_height,
            config.arm_length,
            config.arm_offset,
            config.gaussian_mean,
            config.gaussian_std
    );
}

void generate_uniform(
        RegionMatrix* parent_region_matrix, // the region matrix that the stars are in
        std::vector<Star*> *star_list,

        long int number_of_stars,
        float mean_mass, // in solar masses
        float std_dev_mass, // in solar masses
        Vectorr min_position, // minimum position of a star can have
        Vectorr max_position, // maximum position of a star can have
        Vectorr velocity_at_origin, // average velocity of a star can have at the origin pc/year
        Vectorr variation_velocity, // variation of velocity of a star can have pc/year
        Vectorr variation_in_direction, // the variation in direction of the velocity in radians +-
        int arms, // number of arms in the galaxy
        int number_of_stars_per_arm, // number of stars per arm
        std::vector<float> arm_positions, // the positions of the arms in radians
        float arm_width, // the width of the arms in parsecs
        float arm_height, // the height of the arms in parsecs
        float arm_length, // the length of the arms in pc
        float arm_offset // the offset of the arms in radians
) {
    std::random_device rd;
    std::mt19937 gen(rd());

// random number generators for each variable
    std::normal_distribution<float> dis_mass(mean_mass, std_dev_mass);

    std::uniform_real_distribution<long double> dis_position_x(min_position.x, max_position.x);
    std::uniform_real_distribution<long double> dis_position_y(min_position.y, max_position.y);
    std::uniform_real_distribution<long double> dis_position_z(min_position.z, max_position.z);

    std::uniform_real_distribution<long double> dis_velocity_x(velocity_at_origin.x - variation_velocity.x,
                                                               velocity_at_origin.x + variation_velocity.x);
    std::uniform_real_distribution<long double> dis_velocity_y(velocity_at_origin.y - variation_velocity.y,
                                                               velocity_at_origin.y + variation_velocity.y);
    std::uniform_real_distribution<long double> dis_velocity_z(velocity_at_origin.z - variation_velocity.z,
                                                               velocity_at_origin.z + variation_velocity.z);

    std::uniform_real_distribution<long double> dis_direction_x(-variation_in_direction.x, variation_in_direction.x);
    std::uniform_real_distribution<long double> dis_direction_y(-variation_in_direction.y, variation_in_direction.y);

// random bool
    long double galaxy_mass = 0;
    std::uniform_int_distribution<int> dis_bool(0, 1);
    for (int i = 1; i < number_of_stars + 1; i++) {
        // for getting a random negative or positive z velocity
        long double velocity_z = dis_bool(gen) ? -dis_velocity_z(gen) : dis_velocity_z(gen);

        float mass = dis_mass(gen);
        galaxy_mass += mass;

        Vectorr position = Vectorr(
                dis_position_x(gen),
                dis_position_y(gen),
                dis_position_z(gen)
        );

        Vectorr rel_pos(((max_position.x - std::abs(position.x)) / max_position.x),
                        ((max_position.y - std::abs(position.y)) / max_position.y),
                        ((max_position.z - std::abs(position.z)) / max_position.z));

        Vectorr velocity = Vectorr(
                // get velocities using relative positions to the centre of the galaxy using inverse square law
                std::sqrt(rel_pos.y) * std::cos((rel_pos.x * PI) / 2) * dis_velocity_x(gen),
                std::sqrt(rel_pos.x) * std::cos((rel_pos.y * PI) / 2) * dis_velocity_y(gen),

                // z velocity does not need to have a cos function as the direction of orbit is not in the z direction
                std::sqrt(rel_pos.z) * velocity_z
        );

        // rotate the velocity by a random amount in the x and y direction
        velocity.rotate(dis_direction_x(gen), dis_direction_y(gen), 0);

        // add the star to the star list
        star_list->emplace_back(new Star(i,
                                        position, velocity,
                                        Vectorr(0, 0, 0),
                                        mass,
                                        parent_region_matrix
        ));
    } //TODO: add arms to the galaxy (need to generate an area for the stars to be in and then move the stars to that area that are nearby)
    if (arms > 0) { // if there are arms
        for (int i = 0; i < arms; i++) { // for each arm
            // set the position of the arm to the origin
            Vectorr position = Vectorr(0, 0, 0);

            // set the position Vectorr of the arm to the y-axis
            // and scale the position Vectorr by the length of the arm
            Vectorr position_vector = Vectorr(0, 1, 0, arm_length);

            // make a volume for the arm along the position vector
            Vectorr point_1 = Vectorr(0 + arm_width, 0, 0 + arm_height); // width in x, heoght in z
            Vectorr point_2 = Vectorr(0 - arm_width, 0, 0 + arm_height);
            Vectorr point_3 = Vectorr(0 + arm_width, 0, 0 - arm_height);
            Vectorr point_4 = Vectorr(0 - arm_width, 0, 0 - arm_height);
            // we now have a pyramid with the base at the origin and the top at the position vector
            // we now need to rotate the pyramid by the angle of the arm
            float angle = arm_positions[i];// get the angle of the arm
            point_1.rotate(0, 0, angle); // rotate the points by the angle
            point_2.rotate(0, 0, angle);
            point_3.rotate(0, 0, angle);
            point_4.rotate(0, 0, angle);
            position_vector.rotate(0, 0, angle); // rotate the position vector by the angle
            // we now have a pyramid with the base at the origin and the top at the position vector all rotated by the angle
            // curve the pyramid by moving the top of the pyramid by the offset
            position_vector.rotate(0, 0, arm_offset); // rotate the position vector by the offset around the z axis
            // add stars to the arm

            Vectorr min = {
                    std::min({point_1.x, point_2.x, point_3.x, point_4.x}),
                    std::min({point_1.y, point_2.y, point_3.y, point_4.y}),
                    std::min({point_1.z, point_2.z, point_3.z, point_4.z})
            };
            Vectorr max = {
                    std::max({point_1.x, point_2.x, point_3.x, point_4.x}),
                    std::max({point_1.y, point_2.y, point_3.y, point_4.y}),
                    std::max({point_1.z, point_2.z, point_3.z, point_4.z})
            };


            std::uniform_real_distribution<long double> star_pos_x(min.x, max.x); // define a distribution for the x position of the stars
            std::uniform_real_distribution<long double> star_pos_y(min.y, max.y); // define a distribution for the y position of the stars
            std::uniform_real_distribution<long double> star_pos_z(min.z, max.z); // define a distribution for the z position of the stars

            for (int j = 1; j < number_of_stars_per_arm + 1; j++) { // for each star in the arm
                Vectorr position_star_arm = Vectorr(star_pos_x(gen), star_pos_y(gen), star_pos_z(gen));
                if (not(position_star_arm.x >= min.x && position_star_arm.x <= max.x && position_star_arm.y >= min.y &&
                        position_star_arm.y <= max.y && position_star_arm.z >= min.z && position_star_arm.z <= max.z)) {
                    std::cout << "star position is not in the arm" << std::endl;
                    j -= 1;
                    continue;
                }

                float mass = dis_mass(gen); // get the mass of the star
                galaxy_mass += mass; // add the mass of the star to the mass of the galaxy

                long double velocity_z = dis_bool(gen) != 1 ? -dis_velocity_z(gen) : dis_velocity_z(gen);

                // create the position vector
                Vectorr rel_pos(((max.x - std::abs(position.x)) / max.x), ((max.y - std::abs(position.y)) / max.y),
                                ((max.z - std::abs(position.z)) / max.z));
                Vectorr velocity_star_arm = Vectorr(
                        std::sqrt(rel_pos.y) * std::cos((rel_pos.x * PI) / 2) * dis_velocity_x(
                                gen), // get velocities using relative positions to the centre of the galaxy using inverse square law
                        std::sqrt(rel_pos.x) * std::cos((rel_pos.y * PI) / 2) * dis_velocity_y(gen),
                        std::sqrt(rel_pos.z) *
                        velocity_z // z velocity does not need to have a cos function as the direction of orbit is not in the z direction
                );
                velocity_star_arm.rotate(dis_direction_x(gen), dis_direction_y(gen),
                                         0); // rotate the velocity by a random amount in the x and y direction
                star_list->emplace_back(new Star( // add the star to the star list
                        j + number_of_stars + (number_of_stars_per_arm * i), // get the id of the star
                        position_star_arm,
                        velocity_star_arm,
                        Vectorr(0, 0, 0),
                        mass,
                        parent_region_matrix,
                        0
                ));
            }
        }
    }
    logging::info("Galaxy Mass: ", galaxy_mass);
    logging::info("Number of stars: ", number_of_stars);
}

void generate_random() {

}
