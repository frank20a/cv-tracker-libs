#include "spinsys.hpp"


SpinSystem::SpinSystem() {
    sys = Spinnaker::System::GetInstance();

    lib_version = sys->GetLibraryVersion();
    std::cout << "[SpinSys] Got Spinnaker v"
        << lib_version.major << "."
        << lib_version.minor << "."
        << lib_version.type << "."
        << lib_version.build << std::endl;

    cam_list = sys->GetCameras();
    if (cam_list.GetSize() == 0) {
        sys->ReleaseInstance();
        std::cout << "[SpinSys] No cameras found" << std::endl;
        exit(-1);
    } else {
        std::cout << "[SpinSys] Found " << cam_list.GetSize() << " cameras" << std::endl;
    }

    for(int i = 0; i < cam_list.GetSize(); i++) {
        CameraPtr cam = cam_list.GetByIndex(i);
        cameras.push_back(SpinnakerCameraContainer(cam));
    }
}

SpinSystem::~SpinSystem() {
    deinit_cameras();

    cameras.clear();
    cam_list.Clear();
    
    sys->ReleaseInstance();
}

void SpinSystem::init_cameras() {
    for(int i = 0; i < cameras.size(); i++) {
        cameras[i].init();
    }
}

void SpinSystem::deinit_cameras() {
    for (int i = 0; i < cameras.size(); i++) {
        cameras[i].deinit();
    }
}