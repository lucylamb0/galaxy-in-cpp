// Copyright (c) Conni Bilham & Lucy Coward 2022, All Rights Reserved.

#include <string>
#include <chrono>
#include "includes/Vector3.h"
#include "Star.h"
#include "includes.h"
#include "includes/Region.h"

#include "data_exporter.h"
#include "data_parser.h"

#include "Simulator.h"

#include "includes/logging.h"

#include <matplotlibcpp.hpp>
namespace plt = matplotlibcpp;

// TODO: CLEAN UP THE CODE, lots of commented out sections that i don't know if it is needed or not
// CODE USES PARSECS, SOLAR MASS, PC/YEAR and PC/YEAR^2
std::vector<Star*> star_list = {};
data_exporter ExportHandler = data_exporter(&star_list, "Stars_gaussian.test.dump.txt");

void compute_region_com(Region* region) {
    // TODO: Should consider the mass of the star when calculating the COM
    for (Star* star : region->stars_in_region) {
        // Setup a shorthand com for usage in this local module
        auto *centreMass = &region->centreMass;
        if(!centreMass->initiated) {

            // Set the centre of mass for the region considering the single star inside
            centreMass->position = star->position;
            centreMass->mass = star->mass;
            centreMass->initiated = true;
        } else {
            // Calculate new centre of mass including the new star.
            Vector d = centreMass->position - star->position;
            float mass_bias = star->mass - centreMass->mass;

            centreMass->position -= d * mass_bias;
            centreMass->mass += star->mass;

        }
    }
}
// TODO: Maybe make smaller functions for this
// this function generates stars and puts them into the star list and class
// all velocities will in general go clockwise around the origin
// (for x,y plane) the pos x pos y quadrant velocity will be 0<=x, 0>=y
// (for x,y plane) the pos x neg y quadrant velocity will be 0>=x, 0>=y
// (for x,y plane) the neg x pos y quadrant velocity will be 0<=x, 0<=y
// (for x,y plane) the neg x neg y quadrant velocity will be 0>=x, 0<=y

#include "includes/Random.h"

void star_generator_uniform(
        long int number_of_stars,
        float mean_mass, // in solar masses
        float std_dev_mass, // in solar masses
        Vector min_position, // minimum position of a star can have
        Vector max_position, // maximum position of a star can have
        Vector velocity_at_origin, // average velocity of a star can have at the origin pc/year
        Vector variation_velocity, // variation of velocity of a star can have pc/year
        Vector variation_in_direction, // the variation in direction of the velocity in radians +-
        int arms, // number of arms in the galaxy
        int number_of_stars_per_arm, // number of stars per arm
        std::vector<float> arm_positions, // the positions of the arms in radians
        float arm_width, // the width of the arms in parsecs
        float arm_height, // the height of the arms in parsecs
        float arm_length, // the length of the arms in pc
        float arm_offset, // the offset of the arms in radians
        RegionMatrix* parent_region_matrix // the region matrix that the stars are in
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

        Vector position = Vector(
                dis_position_x(gen),
                dis_position_y(gen),
                dis_position_z(gen)
        );

        Vector rel_pos(((max_position.x - std::abs(position.x)) / max_position.x),
                       ((max_position.y - std::abs(position.y)) / max_position.y),
                       ((max_position.z - std::abs(position.z)) / max_position.z));

        Vector velocity = Vector(
            // get velocities using relative positions to the centre of the galaxy using inverse square law
                std::sqrt(rel_pos.y) * std::cos((rel_pos.x * PI) / 2) * dis_velocity_x(gen),
                std::sqrt(rel_pos.x) * std::cos((rel_pos.y * PI) / 2) * dis_velocity_y(gen),

            // z velocity does not need to have a cos function as the direction of orbit is not in the z direction
                std::sqrt(rel_pos.z) * velocity_z
        );

        // rotate the velocity by a random amount in the x and y direction
        velocity.rotate(dis_direction_x(gen), dis_direction_y(gen), 0);

        // add the star to the star list
        star_list.emplace_back(new Star(i,
                position, velocity,
                Vector(0, 0, 0),
                mass,
                parent_region_matrix
        ));
    } //TODO: add arms to the galaxy (need to generate an area for the stars to be in and then move the stars to that area that are nearby)
    if (arms > 0) { // if there are arms
        for (int i = 0; i < arms; i++) { // for each arm
            // set the position of the arm to the origin
            Vector position = Vector(0, 0, 0);

            // set the position vector of the arm to the y-axis
            // and scale the position vector by the length of the arm
            Vector position_vector = Vector(0, 1, 0, arm_length);

            // make a volume for the arm along the position vector
            Vector point_1 = Vector(0 + arm_width, 0, 0 + arm_height); // width in x, heoght in z
            Vector point_2 = Vector(0 - arm_width, 0, 0 + arm_height);
            Vector point_3 = Vector(0 + arm_width, 0, 0 - arm_height);
            Vector point_4 = Vector(0 - arm_width, 0, 0 - arm_height);
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

            Vector min = {
                    std::min({point_1.x, point_2.x, point_3.x, point_4.x}),
                    std::min({point_1.y, point_2.y, point_3.y, point_4.y}),
                    std::min({point_1.z, point_2.z, point_3.z, point_4.z})
            };
            Vector max = {
                    std::max({point_1.x, point_2.x, point_3.x, point_4.x}),
                    std::max({point_1.y, point_2.y, point_3.y, point_4.y}),
                    std::max({point_1.z, point_2.z, point_3.z, point_4.z})
            };


            std::uniform_real_distribution<long double> star_pos_x(min.x, max.x); // define a distribution for the x position of the stars
            std::uniform_real_distribution<long double> star_pos_y(min.y, max.y); // define a distribution for the y position of the stars
            std::uniform_real_distribution<long double> star_pos_z(min.z, max.z); // define a distribution for the z position of the stars

            for (int j = 1; j < number_of_stars_per_arm + 1; j++) { // for each star in the arm
                Vector position_star_arm = Vector(star_pos_x(gen), star_pos_y(gen), star_pos_z(gen));
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
                Vector rel_pos(((max.x - std::abs(position.x)) / max.x), ((max.y - std::abs(position.y)) / max.y),
                               ((max.z - std::abs(position.z)) / max.z));
                Vector velocity_star_arm = Vector(
                        std::sqrt(rel_pos.y) * std::cos((rel_pos.x * PI) / 2) * dis_velocity_x(
                                gen), // get velocities using relative positions to the centre of the galaxy using inverse square law
                        std::sqrt(rel_pos.x) * std::cos((rel_pos.y * PI) / 2) * dis_velocity_y(gen),
                        std::sqrt(rel_pos.z) *
                        velocity_z // z velocity does not need to have a cos function as the direction of orbit is not in the z direction
                );
                velocity_star_arm.rotate(dis_direction_x(gen), dis_direction_y(gen),
                                         0); // rotate the velocity by a random amount in the x and y direction
                star_list.emplace_back(new Star( // add the star to the star list
                        j + number_of_stars + (number_of_stars_per_arm * i), // get the id of the star
                        position_star_arm,
                        velocity_star_arm,
                        Vector(0, 0, 0),
                        mass,
                        parent_region_matrix,
                        0
                ));
            }
        }
    }
    std::cout << "Galaxy mass: " << galaxy_mass << std::endl;
    std::cout << "Number of stars: " << number_of_stars << std::endl;
}

void star_generator_gaussian(
        long int number_of_stars,
        float mean_mass, // in solar masses
        float std_dev_mass, // in solar masses
        Vector min_position, // minimum position of a star can have
        Vector max_position, // maximum position of a star can have
        Vector velocity_at_origin, // average velocity of a star can have at the origin pc/year
        Vector variation_velocity, // variation of velocity of a star can have pc/year
        Vector variation_in_direction, // the variation in direction of the velocity in radians +-
        int arms, // number of arms in the galaxy
        int number_of_stars_per_arm, // number of stars per arm
        std::vector<float> arm_positions, // the positions of the arms in radians
        float arm_width, // the width of the arms in parsecs
        float arm_height, // the height of the arms in parsecs
        float arm_length, // the length of the arms in pc
        float arm_offset, // the offset of the arms in radians
        RegionMatrix* parent_region_matrix, // the region matrix that the stars are in
        Vector gaussian_mean, // mean of the gaussian distribution (0 is the centre of the galaxy)
        Vector gaussian_standard_deviation // standard deviation of the gaussian distribution (should be under 1) (x and y should be the same)
        ) {
    Random random = Random();
//    std::random_device rd; // obtain a random number from hardware
//    std::mt19937 gen(rd()); // seed the generator

    // random number generators for each variable

    // using long double instead of float
    auto dis_mass = random.normal_distro(mean_mass, std_dev_mass);

    auto dis_position = random.uniform_real_distro_vector(min_position, max_position);
    auto dis_velocity = random.uniform_real_distro_vector(velocity_at_origin, variation_velocity);
    auto dis_direction = random.uniform_real_distro_vector(Vector(-variation_in_direction.x, -variation_in_direction.y, -variation_in_direction.z), variation_in_direction);

    // for gaussian distribution we need to first get gaussian random distances from the origin instead of gaussian xyz positions individually
    auto dis_gaussian = random.normal_distribution_vector(gaussian_mean, gaussian_standard_deviation);

    // random bool
    long double galaxy_mass = 0;
    std::uniform_int_distribution<int> dis_bool(0, 1);
    for (int i = 1; i < number_of_stars+1; i++) {
        long double velocity_z = dis_bool(random.gen) != 1 ? -dis_velocity.z(random.gen) : dis_velocity.z(random.gen);

        Vector gaussian_norm = {
                dis_gaussian.x(random.gen),
                dis_gaussian.y(random.gen),
                dis_gaussian.z(random.gen)
        };

        while (gaussian_norm.x > 1)
            gaussian_norm.x = dis_gaussian.x(random.gen);

        while (gaussian_norm.y > 1)
            gaussian_norm.y = dis_gaussian.y(random.gen);

        while (gaussian_norm.z > 1)
            gaussian_norm.z = dis_gaussian.z(random.gen);


        float mass = dis_mass(random.gen);
        galaxy_mass += mass;

        Vector position = Vector(gaussian_norm.x * max_position.x, gaussian_norm.y * max_position.y, gaussian_norm.y * max_position.z);
//        // normalise the position vector
//        position.normalise();
//        // multiply the position vector by the distance from the origin
//        position = position * dis_distance_from_origin(random.gen);
//        // create the velocity vector
        Vector rel_pos(((max_position.x - std::abs(position.x))/max_position.x),((max_position.y - std::abs(position.y))/max_position.y), ((max_position.z - std::abs(position.z))/max_position.z));
        Vector velocity = Vector(
                std::sqrt(rel_pos.y) * std::cos((rel_pos.x * PI)/2) * dis_velocity.x(random.gen), // get velocities using relative positions to the centre of the galaxy using inverse square law
                std::sqrt(rel_pos.x) * std::cos((rel_pos.y * PI)/2) * dis_velocity.y(random.gen),
                std::sqrt(rel_pos.z) * velocity_z // z velocity does not need to have a cos function as the direction of orbit is not in the z direction
        );
        velocity.rotate(dis_direction.x(random.gen), dis_direction.y(random.gen),
                        0); // rotate the velocity by a random amount in the x and y direction
        star_list.emplace_back(new Star( // add the star to the star list
                i, // get the id of the star
                position,
                velocity,
                Vector(0, 0, 0),
                mass,
                parent_region_matrix,
                0
        ));
    }

    if (arms > 0) { // if there are arms
        for (int i = 0; i <= arms; i++) { // for each arm
            Vector origin = Vector(0, 0, 0); // set the position of the arm to the origin
            Vector position_vector = Vector(0, 1, 0, arm_length); // set the position vector of the arm to the y axis // and scale the position vector by the length of the arm

            // make a volume for the arm along the position vector
            Vector point_1 = Vector(0 + arm_width, 0, 0 + arm_height); // width in x, height in z
            Vector point_2 = Vector(0 - arm_width, 0, 0 + arm_height);
            Vector point_3 = Vector(0 + arm_width, 0, 0 - arm_height);
            Vector point_4 = Vector(0 - arm_width, 0, 0 - arm_height);

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
            Vector min = {
                    std::min({point_1.x, point_2.x, point_3.x, point_4.x}),
                    std::min({point_1.y, point_2.y, point_3.y, point_4.y}),
                    std::min({point_1.z, point_2.z, point_3.z, point_4.z})
            };
            Vector max = {
                    std::max({point_1.x, point_2.x, point_3.x, point_4.x}),
                    std::max({point_1.y, point_2.y, point_3.y, point_4.y}),
                    std::max({point_1.z, point_2.z, point_3.z, point_4.z})
            };
            std::uniform_real_distribution<long double> star_pos_x(min.x, max.x); // define a distribution for the x position of the stars
            std::uniform_real_distribution<long double> star_pos_y(min.y, max.y); // define a distribution for the y position of the stars
            std::uniform_real_distribution<long double> star_pos_z(min.z, max.z); // define a distribution for the z position of the stars
            std::normal_distribution<long double> star_distance_from_origin(
                    0,
                    std::sqrt((min.x * min.x) +
                              (min.y * min.y) +
                              (min.z * min.z))
            );

            for (int j = 1; j < number_of_stars_per_arm+1; j++) { // for each star in the arm
                Vector gaussian_norm = {
                        dis_gaussian.x(random.gen),
                        dis_gaussian.y(random.gen),
                        dis_gaussian.z(random.gen)
                };

                while (gaussian_norm.x > 1)
                    gaussian_norm.x = dis_gaussian.x(random.gen);

                while (gaussian_norm.y > 1)
                    gaussian_norm.y = dis_gaussian.y(random.gen);

                while (gaussian_norm.z > 1)
                    gaussian_norm.z = dis_gaussian.z(random.gen);

                Vector position_star_arm = gaussian_norm * max;

                if (!(position_star_arm >= min && position_star_arm <= max)) {
//                    std::cout << "star position is not in the arm" << std::endl;
                    logging::verbose("star position is not in the arm", "Galaxy::Galaxy");
                    j -= 1;
                    continue;
                }
                logging::info("star position is in the arm", "Galaxy::Galaxy");


                float mass = dis_mass(random.gen); // get the mass of the star
                galaxy_mass += mass; // add the mass of the star to the mass of the galaxy

                long double velocity_z = dis_bool(random.gen) != 1 ? -dis_velocity.z(random.gen) : dis_velocity.z(random.gen);

                Vector rel_pos(((max.x - std::abs(origin.x)) / max.x),
                               ((max.y - std::abs(origin.y)) / max.y),
                               ((max.z - std::abs(origin.z)) / max.z)
                );

                Vector velocity_star_arm = Vector(
                        std::sqrt(rel_pos.y) * std::cos((rel_pos.x * PI)/2) * dis_velocity.x(random.gen), // get velocities using relative positions to the centre of the galaxy using inverse square law
                        std::sqrt(rel_pos.x) * std::cos((rel_pos.y * PI)/2) * dis_velocity.y(random.gen),
                        std::sqrt(rel_pos.z) * velocity_z // z velocity does not need to have a cos function as the direction of orbit is not in the z direction
                );

                velocity_star_arm.rotate(dis_direction.x(random.gen), dis_direction.y(random.gen), 0); // rotate the velocity by a random amount in the x and y direction
                star_list.emplace_back(new Star( // add the star to the star list
                        j + number_of_stars + (number_of_stars_per_arm * i), // get the id of the star
                        position_star_arm,
                        velocity_star_arm,
                        Vector(0,0,0),
                        mass,
                        parent_region_matrix
                ));
            }
        }
    }
    logging::info("Galaxy Mass: ", galaxy_mass);
}

RegionMatrix regionMatrix;

int main(int arg_count, char** args) {
    std::string data_set_path = "D:\\JET BRAINS\\galaxy-in-cpp/star_data.csv";
    if (arg_count == 2) {
        data_set_path = args[1];
    }
    if (arg_count > 4) {
        data_set_path = args[1];

        std::cout << "[ Data Set Path ]    [" << data_set_path << "]" << std::endl;

        Vector startPosition = Vector(std::stof(args[2]), std::stof(args[3]), std::stof(args[4]));
        std::cout << "[ Start Position X ] [" << startPosition.x << "]" << std::endl;
        std::cout << "[ Start Position Y ] [" << startPosition.y << "]" << std::endl;
        std::cout << "[ Start Position Z ] [" << startPosition.z << "]" << std::endl;

        Vector endPosition = Vector(std::stof(args[5]), std::stof(args[6]), std::stof(args[7]));
        std::cout << "[ End Position X ]   [" << endPosition.x << "]" << std::endl;
        std::cout << "[ End Position Y ]   [" << endPosition.y << "]" << std::endl;
        std::cout << "[ End Position Z ]   [" << endPosition.z << "]" << std::endl;

        Vector divisions = Vector(std::stof(args[8]), std::stof(args[9]), std::stof(args[10]));
        std::cout << "[ divisions X ]      [" << divisions.x << "]" << std::endl;
        std::cout << "[ divisions Y ]      [" << divisions.y << "]" << std::endl;
        std::cout << "[ divisions Z ]      [" << divisions.z << "]" << std::endl;

        regionMatrix = RegionMatrix(
                startPosition, // Start position
                endPosition, // End position
                divisions               // Amount of divisions on the z, y, z
                // Overlap factor
        );
    } else {
        regionMatrix = RegionMatrix(
                Vector(-100000, -100000, -100000), // Start position
                Vector(100000, 100000, 100000), // End position
                Vector(10, 10, 10),               // Amount of divisions on the z, y, z
                0.0003f         // Overlap factor
        );
    }

    std::cout << "Hello, World!" << std::endl; // classic hello world of course <3
    logging::info("Hello, World! (FROM THE LOGGING FRAMEWORK)", "");
    logging::info("Regions: ", regionMatrix.regions.size());
    logging::info("Outputting log to: ", "<UNDEFINED>");

    int number_of_stars = 2500;
    int number_of_stars_per_arm = 10;
    float mean_mass = 10;
    float std_mass = 0.1;
    Vector min_position = Vector(-10, -10, -0.5);
    Vector max_position = Vector(10, 10, 0.5);
    Vector velocityAtOrigin = Vector(0.00000002556727896654, 0.00000002556727896654, 0.0000000002);
    Vector variation_velocity = Vector(0.00000001056727896654, 0.00000001056727896654, 0.00000000002);
    long double variation_direction_x = 0.0174533;
    long double variation_direction_y = 0.0174533;

#define degrees_to_radians(degrees) ((float)(degrees * PI / 180.0))
#define radians_to_degrees(radians) ((float)(radians * 180.0 / PI))

    int number_of_arms = 0;
    std::vector<float> angle_of_arms = { };
    float arm_width = 1;
    float arm_length = 2;
    float arm_height = 1;
    float arm_offset = 0.2;

    Vector gaussian_mean = Vector(0, 0, 0);
    Vector gaussian_std = Vector(0.2, 0.2, 0.2);

//    star_list.emplace_back(new Star(
//            2,                                // ID
//            Vector(1.2477e-8, 0, 0),    // Position
//            Vector( 0, 2.02269032e-6, 0),    // Velocity
//            Vector(0, 0, 0), // Acceleration
//            3.69396868e-8,
//            &regionMatrix,       // Parent region matrix
//            0
//    ));

    star_generator_uniform(
            number_of_stars,
            mean_mass,
            std_mass,
            min_position,
            max_position,
            velocityAtOrigin,
            variation_velocity,
            Vector(variation_direction_x, variation_direction_y),
            number_of_arms,
            number_of_stars_per_arm,
            angle_of_arms,
            arm_width,
            arm_height,
            arm_length,
            arm_offset,
            &regionMatrix
//            gaussian_mean,
//            gaussian_std
    );

    // dump the star list to a csv file
//    data_exporter ExportHandler = data_exporter(&star_list);
//    ExportHandler.start_dumping("\"Stars.test.dump.txt\"");


    Vector min = Vector(-1, -1, -1);
    Vector max = Vector(1, 1, 1);

    for (auto star: star_list)
        star->position.getBounds(min, max);

    min.scale(1.01);
    max.scale(1.01);

    regionMatrix = RegionMatrix(min, max, Vector(10, 10, 10));

    logging::info("Assigning regions.", "");
// converting data to meters (for now)
    static unsigned long averageStarRegionCount = 0;
    for (auto star: star_list) {
        star->find_regions();

        if (star->id % 5000 == 0) {
            averageStarRegionCount = averageStarRegionCount == 0 ? star->regions_we_are_in.size() :
                                     (averageStarRegionCount + star->regions_we_are_in.size()) / 2;

            auto progress = to_string(star->id) + "/" + to_string(star_list.size());
            auto percent = to_string((int) ((float) star->id / (float) star_list.size() * 100));
            auto message = "Progress " + percent + "% [" + progress + "] Stars, in an avrg of " +
                           to_string(averageStarRegionCount) + " regions";
            logging::verbose(message, "");
        }
    }
    // compute region coms
    for (Region *region: regionMatrix.regions)
        compute_region_com(region);

    logging::info("Finished assigning regions", "");
// don't know what the hell averageStarUpdateTime is, I think it is for keeping track of how long it takes
//    static long long averageAccelerationUpdateTime = -1;
    static long long averageStarUpdateTime = -1;

    int thread_count = std::thread::hardware_concurrency() - 1;

    Simulator simulator = Simulator(thread_count,&star_list);
    simulator.output_info();
    auto work_queue = simulator.work_queue;

    for (int simFrame = 0; simFrame <= simulationFrames; ++simFrame) {
        auto starUpdateStartTime = std::chrono::high_resolution_clock::now();

        // Generate a work queue for the threads to handle
        simulator.generateWorkQueue();

        // begin threading
        struct thread_data {
        public:
            int thread_id;
            std::vector<Star *> *work_queue;
        };
        auto threads = std::vector<std::thread>{};
//goto A;
        for (int i = 0; i < thread_count; ++i) {
            threads.emplace_back(std::thread([&work_queue, i]() {
                int tasksComplete = 0;
                int myTasks = work_queue->at(i).size();
                double averageTime = 0.f;
                for (auto star: work_queue->at(i)) {
                    auto timeTaken = star->acceleration_update_region_com(true);
                         timeTaken += star->acceleration_update_stars_in_region(false);

                    averageTime = (averageTime == -1 ? timeTaken : (averageTime + timeTaken) / 2);

                    ++tasksComplete;
                    if (tasksComplete % (1000 + (i * 1000)) == 0) {
                        auto progress = to_string(tasksComplete) + "/" + to_string(myTasks);
                        auto percent = to_string((int) ((float) tasksComplete / (float) myTasks * 100));

                        logging::info(
                                "[Thread " + to_string(i) + "] Acceleration Status - " + percent + "% [" + progress +
                                "] [" + to_string(averageTime) + "ms]", "", true, true);

                        if (averageTime > 10) {
                            logging::info("Average time is too high", "", true, true);
                        }
//                        std::cout << '\r' << "Acceleration Update Status - [Thread " + to_string(i) + "] " + percent + "% [" + progress + "], Average Time Taken: " +
                        //                                           to_string(averageTime) + "ms" << std::flush;
                    }
                }
            }));
        }
        A:
        int thread_index = 0;
        for (auto &thread: threads) {
            thread.join();
            logging::verbose("Thread " + to_string(thread_index) + " joined");

            // Initiate live data dumps
//            ExportHandler.csv_full_dump_Star(work_queue->at(thread_index));

            ++thread_index;
        }

        // Clear the stars in the regions, so we can update the centre of masses and reassign the stars their regions
        for (auto region: regionMatrix.regions) {
            region->stars_in_region.clear();
            region->centreMass.reset();
        }
        // updating star positions and velocities and adding them to the regions
        for (auto star: star_list) {
            star->velocity_update(); // Update the stars velocity
            star->position_update(); // Update the stars position

            star->find_regions();
        }

        for (auto region: regionMatrix.regions) {
            compute_region_com(region);
        }

        if (simFrame % 1000 == 0) {
            auto progress = to_string(simFrame) + "/" + to_string(simulationFrames);
            auto percent = to_string((int) ((float) simFrame / (float) simulationFrames * 100));

            logging::info("[ Acceleration Cycle ] - " + percent + "% - " + progress, "", true, true);
        }
//        if (simFrame % ((int)((simFrame / simulationFrames) * 10)) == 0) {
//            std::cout << (simFrame / simulationFrames) * 100 << "% Complete - Stars" << std::endl;
//        }

        auto starUpdateEndTime = std::chrono::high_resolution_clock::now();
        auto starUpdateDuration = std::chrono::duration_cast<std::chrono::milliseconds>(
                starUpdateStartTime - starUpdateEndTime).count();

        if (averageStarUpdateTime == -1)
            averageStarUpdateTime = starUpdateDuration;

        else
            averageStarUpdateTime = (averageStarUpdateTime + starUpdateDuration) / 2;


        logging::verbose("Finished an acceleration cycle: ", simFrame + 1);

        logging::verbose("Re assigning stars to regions", "", false, false);
        min = Vector(-1, -1, -1);
        max = Vector(1, 1, 1);
        for (auto star: star_list) {
            if (star->position.x > max.x) {
                max.x = star->position.x;
            } else if (star->position.x < min.x) {
                min.x = star->position.x;
            }
            if (star->position.y > max.y) {
                max.y = star->position.y;
            } else if (star->position.y < min.y) {
                min.y = star->position.y;
            }
            if (star->position.z > max.z) {
                max.z = star->position.z;
            } else if (star->position.z < min.z) {
                min.z = star->position.z;
            }
        }
        min = min * 1.01;
        max = max * 1.01;

        regionMatrix = RegionMatrix(
                Vector(min.x, min.y, min.z), // Start position
                Vector(max.x, max.y, max.z), // End position
                Vector(10, 10, 10)               // Amount of divisions on the z, y, z
        );

        averageStarRegionCount = 0;
        for (auto star: star_list) {
            star->find_regions();

            averageStarRegionCount = averageStarRegionCount == 0 ? star->regions_we_are_in.size() :
                                     (averageStarRegionCount + star->regions_we_are_in.size()) / 2;

            if (star->id % 5000 == 0) {

                auto progress = to_string(star->id) + "/" + to_string(star_list.size());
                auto percent = to_string((int) ((float) star->id / (float) star_list.size() * 100));
                auto message = "Progress " + percent + "% [" + progress + "] Stars, in an avrg of " +
                               to_string(averageStarRegionCount) + " regions";
                logging::verbose(message, "");
            }
        }
        // compute region coms
        for (Region *region: regionMatrix.regions) {
            compute_region_com(region);
        }
        logging::verbose("Finished re assigning stars to regions", "", false, false);
    }

    ExportHandler.start_dumping();
//    data_exporter ExportHandler2 = data_exporter(&star_list);
////    ExportHandler2.start_dumping("Stars_gaussian.test.dump.txt");
    return 0;
}

