#pragma once

#include <Spinnaker.h>
#include <SpinGenApi/SpinnakerGenApi.h>

#include <iostream>
#include <vector>

#include "spinnaker_camera.hpp"


class SpinSystem {
public:
    SpinSystem();
    ~SpinSystem();

    void init_cameras();
    void deinit_cameras();

    std::vector<SpinnakerCameraContainer> cameras;

private:
    Spinnaker::SystemPtr sys;
    Spinnaker::LibraryVersion lib_version;
    Spinnaker::CameraList cam_list;
};