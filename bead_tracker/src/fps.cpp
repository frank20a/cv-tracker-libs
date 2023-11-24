#include "fps.hpp"


FPS::FPS() {
    running = false;
}

void FPS::start() {
    if (!running) {
        start_t = std::chrono::system_clock::now();
        prev_t = start_t;
        running = true;
    }
}

double FPS::update() {
    if (running) {
        frames++;
        long long int elapsed = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - prev_t).count();
        prev_t = std::chrono::system_clock::now();
        return 1000000.0 / elapsed;
    }
    return -1;
}

void FPS::stop() {
    if (running) {
        stop_t = std::chrono::system_clock::now();
        running = false;
    }
}

double FPS::fps() {
    if (running)
        return frames / (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start_t).count() / 1000.0);
    else
        return frames / (std::chrono::duration_cast<std::chrono::milliseconds>(stop_t - start_t).count() / 1000.0);
}