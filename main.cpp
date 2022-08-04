#include <sstream>
#include <string>
#include <fstream>
#include <chrono>
#include "Vector3.h"
#include "Star.h"
#include "includes.h"
#include "Region.h"
#include "MilkshapeModel.h"

std::vector<Star*> star_list = {};

#include <filesystem>
#include <io.h>
#include <thread>

void threadFunc(int threadID, std::vector<std::vector<Star*>> work_queue) {
    int tasksComplete = 0;
    int myTasks = work_queue.at(threadID).size();
    double averageTime = -1.0;
    for (auto star : work_queue.at(threadID)) {
        auto timeTaken = star->acceleration_update(star_list);
        averageTime = averageTime == -1.0 ? timeTaken : (averageTime + timeTaken) / 2;
        ++tasksComplete;
        if(tasksComplete % (1000 + (threadID * 100)) == 0) {
            std::cout << "Thread " << threadID << ": " << tasksComplete << "/" << myTasks <<
                      " - Average Time Taken: " << averageTime << "ms" << std::endl;
        }
    }
}

Vector playSpaceStart = Vector(-10,-10,-10);
Vector playSpaceStop = Vector(10, 10, 10);
RegionMatrix regionMatrix = RegionMatrix();
void getPointsRegions(Vector point = {-1, 5, 1}) {
    std::cout << "point: " << point.x << ", " << point.y << ", " << point.z << std::endl;
    std::vector<int> regions_we_are_in = {};

    auto tmp = (point.x - playSpaceStart.x) / regionMatrix.step.x;
    auto tmp2 = std::floor(tmp);
    auto remainder = tmp - tmp2;
    int index = (tmp2) * regionMatrix.divisions.y * regionMatrix.divisions.z; //indexing works

    std::cout << "idnex: " << index << std::endl;

    int neighbour_above_x = -1;
    int neighbour_below_x = -1;
    int neighbour_above_y = -1;
    int neighbour_below_y = -1;
    int neighbour_above_z = -1;
    int neighbour_below_z = -1;
    int mode_neighbours = 1; // 1 = 1 region, 2 = 2 regions, 4 = 4 regions, 8 = 8 regions

    if(remainder <= regionMatrix.overlap_factor) { // need to check all 3 directions to see if we are in a neighbour region
        // We are overlapping the below region
        neighbour_below_x = index + (-1 * regionMatrix.divisions.y * regionMatrix.divisions.z);
//        if(regionMatrix.regions.at(neighbour_below).contains_x(point)) {
//            regions_we_are_in.push_back(neighbour_below);
//            std::cout << "We are definetly overlapping the below region: " << neighbour_below << std::endl;
//        } else {
//            std::cout << "We are not overlapping the below region: " << neighbour_below << std::endl;
//        }
    }
    else if (remainder >= 1 - regionMatrix.overlap_factor) {
        // We are overlapping the above region
        neighbour_above_x = index + (regionMatrix.divisions.y * regionMatrix.divisions.z);
//        if(regionMatrix.regions.at(neighbour_above).contains_x(point)) {
//            regions_we_are_in.push_back(neighbour_above);
//            std::cout << "We are definetly overlapping the above region: " << neighbour_above << std::endl;
//        } else {
//            std::cout << "We are not overlapping the above region: " << neighbour_above << std::endl;
//        }
    }

    tmp = (point.y - playSpaceStart.y) / regionMatrix.step.y;
    tmp2 = std::floor(tmp);
    remainder = tmp - tmp2;
    index += (tmp2) * regionMatrix.divisions.z;

    regions_we_are_in.push_back(index);

//    neighbour_above = -1;
//    neighbour_below = -1;

    if(remainder <= regionMatrix.overlap_factor) {
        // We are overlapping the below region
//        neighbour_below = index + (-1 * regionMatrix.divisions.z);
//        if(regionMatrix.regions.at(neighbour_below).contains_y(point)) {
//            regions_we_are_in.push_back(neighbour_below);
//            std::cout << "We are definetly overlapping the below region: " << neighbour_below << std::endl;
//        } else {
//            std::cout << "We are not overlapping the below region: " << neighbour_below << std::endl;
//        }
    }
    else if (remainder >= 1 - regionMatrix.overlap_factor) {
        // We are overlapping the above region
//        neighbour_above = index + (regionMatrix.divisions.z);
//        if(regionMatrix.regions.at(neighbour_above).contains_y(point)) {
//            regions_we_are_in.push_back(neighbour_above);
//            std::cout << "We are definetly overlapping the above region: " << neighbour_above << std::endl;
//        } else {
//            std::cout << "We are not overlapping the above region: " << neighbour_above << std::endl;
//        }
    }


    tmp = (point.z - playSpaceStart.z) / regionMatrix.step.z;
    tmp2 = std::floor(tmp);
    remainder = tmp - tmp2;
    index += (tmp2);


//    neighbour_above = -1;
//    neighbour_below = -1;

    if(remainder <= regionMatrix.overlap_factor) {
        // We are overlapping the below region
//        neighbour_below = index + -1;
//        if(regionMatrix.regions.at(neighbour_below).contains_z(point)) {
//            regions_we_are_in.push_back(neighbour_below);
//            std::cout << "We are definetly overlapping the below region: " << neighbour_below << std::endl;
//        } else {
//            std::cout << "We are not overlapping the below region: " << neighbour_below << std::endl;
//        }
    }
    else if (remainder >= 1 - regionMatrix.overlap_factor) {
        // We are overlapping the above region
//        neighbour_above = index + 1;
//        if(regionMatrix.regions.at(neighbour_above).contains_z(point)) {
//            regions_we_are_in.push_back(neighbour_above);
//            std::cout << "We are definetly overlapping the above region: " << neighbour_above << std::endl;
//        } else {
//            std::cout << "We are not overlapping the above region: " << neighbour_above << std::endl;
//        }
    }
    std::cout << index << std::endl;
    regions_we_are_in.push_back(index);

    int startingX = 0;
//    startingX

    int x = 0;
    for(auto region : regionMatrix.regions) {
        std::cout << x << ": " << region.min.x << " " << region.min.y << " " << region.min.z << " --- " << region.max.x << " " << region.max.y << " " << region.max.z << std::endl;
        ++x;
    }
    int p = 0;
    std::cout << "We are in the following regions: " << std::endl;
    for(auto region : regionMatrix.regions) {
        if(region.contains(point)){
            std::cout << "Point is def in region: " << p << std::endl;
        }
        ++p;
    }
    std::cout << "============================" << std::endl;

//    for(int index : regions_we_are_in) {
//        std::cout << "Overlapping region: " << index << std::endl;
//        if(regionMatrix.regions.at(index).contains(point)) {
//            std::cout << "Point is in region: " << index << std::endl;
//        }
//    }
}

static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

bool initialiseEngine() {
    glfwSetErrorCallback(glfw_error_callback);
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }
    std::cout << "GLFW initialised!" << std::endl;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    std::cout << "GLSL version: " << glsl_version << std::endl;
    std::cout << "GLFW window hints set!" << std::endl;

    glfwWindowHint(GLFW_SAMPLES, 4);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( globals->window_size.width, globals->window_size.height, "e", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window.\n" );
        glfwTerminate();
        return -1;
    }
    globals->window = window;
    std::cout << "GLFW window created!" << std::endl;

    glfwMakeContextCurrent(window);
    std::cout << "GLFW window context set!" << std::endl;

    static bool vSync = true;
    glfwSwapInterval(vSync); // Enable vsync
    std::cout << "GLFW vsync " << (vSync ? "enabled!" : "disabled!") << std::endl;

    std::cout << "Setting up IMGUI" << std::endl;
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    std::cout << "IMGUI initialised!" << std::endl;

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    std::cout << "GLEW initialised!" << std::endl;

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    if(globals->mouse_captured) {
        // Hide the mouse and enable unlimited movement
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    std::cout << "GLFW input modes set!" << std::endl;

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, globals->window_size.width/2, globals->window_size.height/2);
    std::cout << "GLFW cursor set!" << std::endl;

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);

    std::cout << "GL initialised!" << std::endl;
}

void cleanupEngine() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}

int main() {
    initialiseEngine();

//    GLuint VertexArrayID;
//    glGenVertexArrays(1, &VertexArrayID);
//    glBindVertexArray(VertexArrayID);
//
//    // Create and compile our GLSL program from the shaders
//    GLuint programID = LoadShaders(
//            "TransformVertexShader.vertexshader",
//                                    "TextureFragmentShader.fragmentshader" );
//
//    // Get a handle for our "MVP" uniform
//    GLuint MatrixID = glGetUniformLocation(programID, "MVP");
//
//    // Load the texture
//    GLuint Texture = loadDDS("uvmap.DDS");
//
//    // Get a handle for our "myTextureSampler" uniform
//    GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");
//
//    // Read our .obj file
//    std::vector<glm::vec3> vertices;
//    std::vector<glm::vec2> uvs;
//    std::vector<glm::vec3> normals; // Won't be used at the moment.
//    bool res = loadOBJ("cube.obj", vertices, uvs, normals);
//
//    // Load it into a VBO
//
//    GLuint vertexbuffer;
//    glGenBuffers(1, &vertexbuffer);
//    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
//
//    GLuint uvbuffer;
//    glGenBuffers(1, &uvbuffer);
//    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
//    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

    Model *pModel = NULL;   // Holds The Model Data
    pModel = new MilkshapeModel();
    if ( pModel->loadModelData( "dwarf1.ms3d" ) == false )
    {
        std::cout << "Couldn't load the model dwarf1.ms3d" << std::endl;
        return 0;                                   // If Model Didn't Load, Quit
    }

    pModel->reloadTextures();

    do{

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // feed inputs to dear imgui, start new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // render your GUI
        ImGui::Begin("Demo window");
        ImGui::Text(globals->mouse_captured ? "Mouse captured" : "Mouse not captured");
        ImGui::Checkbox("Mouse capture", &globals->mouse_captured);

        ImGui::Text("Frame time: %.2fms", globals->frame_time);

        ImGui::Text("Camera Angles: %.2f, %.2f", globals->camera_angles.pitch, globals->camera_angles.yaw);

        ImGui::Button("Hello!");
        ImGui::End();

        // Render dear imgui into screen
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//
////        gluLookAt( 75, 75, 75, 0, 0, 0, 0, 1, 0 );
////
////        glRotatef(0.5,0.0f,1.0f,0.0f);
//
//        // Use our shader
//        glUseProgram(programID);
//
//        // Compute the MVP matrix from keyboard and mouse input
        computeMatricesFromInputs();
        glm::mat4 ProjectionMatrix = getProjectionMatrix();
        glm::mat4 ViewMatrix = getViewMatrix();
        glm::mat4 ModelMatrix = glm::mat4(1.0);
        glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
//
//        // Send our transformation to the currently bound shader,
//        // in the "MVP" uniform
//        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
//
//        // Bind our texture in Texture Unit 0
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, Texture);
//        // Set our "myTextureSampler" sampler to use Texture Unit 0
//        glUniform1i(TextureID, 0);
//
//        // 1rst attribute buffer : vertices
//        glEnableVertexAttribArray(0);
//        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//        glVertexAttribPointer(
//                0,                  // attribute
//                3,                  // size
//                GL_FLOAT,           // type
//                GL_FALSE,           // normalized?
//                0,                  // stride
//                (void*)0            // array buffer offset
//        );
//
//        // 2nd attribute buffer : UVs
//        glEnableVertexAttribArray(1);
//        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
//        glVertexAttribPointer(
//                1,                                // attribute
//                2,                                // size
//                GL_FLOAT,                         // type
//                GL_FALSE,                         // normalized?
//                0,                                // stride
//                (void*)0                          // array buffer offset
//        );
//
//        // Draw the triangle !
//        glDrawArrays(GL_TRIANGLES, 0, vertices.size() );
//
//        glDisableVertexAttribArray(0);
//        glDisableVertexAttribArray(1);

//        glLoadIdentity();                   // Reset The View

        static float tmp = 0;
        tmp += 0.1f;
        gluLookAt(0,0,tmp, 0,0,0, 0,1,0);

        static int yrot = 0;

        glRotatef(180.f,0.0f,1.0f,0.0f);

        pModel->draw();

//        yrot+=1.0f;

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    cleanupEngine();
    std::cout << "Hello, World!" << std::endl;
    regionMatrix = RegionMatrix(playSpaceStart, playSpaceStop, Vector(2, 2, 2));

    getPointsRegions();

    return 0;

//    std::vector<Region> regions = regionMatrix.getRegions(point);
    std::cout << "Regions: " << regionMatrix.regions.size() << std::endl;

    std::cout << regionMatrix.regions.size() << std::endl;

//    std::cout << regionMatrix.
//    int outPointsXStartsAt =

    Region region(Vector(-100, -100, -100), Vector(100, 100, 100));
    if(region.contains(Vector(0, 0, 0))) {
        std::cout << "Region contains origin" << std::endl;
    } else {
        std::cout << "Region does not contain origin" << std::endl;
    }

    char tmp[256];
    getcwd(tmp, 256);
    std::cout << "Current working directory: " << tmp << std::endl;
    std::ifstream infile(L"C:\\Users\\User\\Documents\\coding\\Star Stuff\\test\\C version\\star_data.csv");
    std::string line;

//    int cnt = 0;
    while (std::getline(infile, line))
    {
//        ++cnt; if(cnt > 10) break;

        std::istringstream iss(line);
        auto split_str = Utils::split(line, ',');

        star_list.emplace_back( new Star(
                std::stoi(split_str.at(0)),     // ID
    Vector(std::stof(split_str.at(2)), std::stof(split_str.at(3)), std::stof(split_str.at(4))), // Position
    Vector(std::stof(split_str.at(5)), std::stof(split_str.at(6)), std::stof(split_str.at(7))), // Velocity
    Vector(0,0,0))); // Acceleration

    }

    std::cout << "Finished reading file" << std::endl;

    for (auto star : star_list) {
        star->position = star->position * parsec;
        star->velocity = star->velocity * parsec_per_year;
        star->history_position.emplace_back(star->position.x, star->position.y, star->position.z);
    }

    std::cout << "Finished converting data to parsecs" << std::endl;

//    static long long averageAccelerationUpdateTime = -1;
    static long long averageStarUpdateTime = -1;

    const int loops = 10000;
    for (int i = 0; i < loops; ++i) {
        auto starUpdateStartTime = std::chrono::high_resolution_clock::now();

#define MULTI_THREADED 1

#if MULTI_THREADED
        auto work_queue = std::vector<std::vector<Star*>>{};
        auto thread_count = std::thread::hardware_concurrency() - 1;
        auto star_count = star_list.size();
        auto star_per_thread = star_count / thread_count;
        std::cout << "Star count: " << star_count << std::endl;
        std::cout << "Thread count: " << thread_count << std::endl;
        std::cout << "Star per thread: " << star_per_thread << std::endl;
        for (int i = 0; i < thread_count; ++i) {
            work_queue.emplace_back(std::vector<Star*>{});
            for (int j = 0; j < star_per_thread; ++j) {
                work_queue.at(i).emplace_back(star_list.at(i * star_per_thread + j));
            }
        }
        auto threads = std::vector<std::thread>{};
        for (int threadID = 0; threadID < thread_count; ++threadID) {
            threads.emplace_back(std::thread(threadFunc, threadID, work_queue));
        }
//        for (int i = 0; i < thread_count; ++i) {
//            threads.emplace_back(std::thread([&work_queue, i]() {
//                int tasksComplete = 0;
//                int myTasks = work_queue.at(i).size();
//                int averageTime = -1;
//                for (auto star : work_queue.at(i)) {
//                    auto timeTaken = star->acceleration_update(star_list);
//                    averageTime = averageTime == -1 ? timeTaken : (averageTime + timeTaken) / 2;
//                    ++tasksComplete;
//#define OUTPUT_EVERY_N_TASKS 100
//                    if(tasksComplete % (OUTPUT_EVERY_N_TASKS + (i * OUTPUT_EVERY_N_TASKS)) == 0) {
//                        std::cout << "Thread " << i << ": " << tasksComplete << "/" << myTasks << " - Average Time Taken: " << averageTime << "ms" << std::endl;
//                    }
//                }
//            }));
//        }

        for (auto &thread : threads) {
            thread.join();
        }
#endif
#ifndef MULTI_THREADED
        for (auto star : star_list) {
            auto accelerationDuration = star.acceleration_update(star_list);

            if (averageAccelerationUpdateTime == -1) {
                averageAccelerationUpdateTime = accelerationDuration;
            } else {
                averageAccelerationUpdateTime = (averageAccelerationUpdateTime + accelerationDuration) / 2;
            }

            if (star.id % 100 == 0) {
                std::cout << star.id << " / " << star_list.size() << " Complete - Average Acceleration Update Time: " << averageAccelerationUpdateTime << "ms" << std::endl;
            }
        }
#endif

        for (auto star : star_list) {
            star->velocity_update();
            star->position_update();
        }
        if (i % 10 == 0) {
            std::cout << (i / loops) * 100 << "% Complete - Stars" << std::endl;
        }

        auto starUpdateEndTime = std::chrono::high_resolution_clock::now();
        auto starUpdateDuration = std::chrono::duration_cast<std::chrono::milliseconds>(starUpdateStartTime-starUpdateEndTime).count();

        if (averageStarUpdateTime == -1) {
            averageStarUpdateTime = starUpdateDuration;
        } else {
            averageStarUpdateTime = (averageStarUpdateTime + starUpdateDuration) / 2;
        }
        std::cout << "Finished a stars acceleration update: " << i << std::endl;
    }
    return 0;
}

