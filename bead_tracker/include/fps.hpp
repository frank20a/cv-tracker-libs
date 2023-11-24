#pragma once

#include <iostream>
#include <chrono>

class FPS {
public:
    FPS();
    void start();
    double update();
    void stop();
    double fps();

private:
    std::chrono::time_point<std::chrono::system_clock> start_t, stop_t, prev_t;
    long unsigned int frames = 0;
    bool running;
};