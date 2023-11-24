#pragma once

#include <Spinnaker.h>
#include <opencv2/core.hpp>

#include <iostream>
#include <string>

#include "camera_container.hpp"

using namespace Spinnaker;
using namespace Spinnaker::GenApi;
using namespace Spinnaker::GenICam;


class SpinnakerCameraContainer : public CameraContainer {
public:
    SpinnakerCameraContainer(CameraPtr &cam) : _cam(cam) { processor = new ImageProcessor; };
    ~SpinnakerCameraContainer();

    enum AcquisitionMode {
        CONTINUOUS, SINGLE_FRAME, MULTI_FRAME
    };
    enum BufferMode {
        NEWEST_FIRST, NEWEST_ONLY, OLDEST_FIRST, OLDEST_FIRST_OVERWRITE
    };

    // Overload CameraContainer
    bool begin();
    bool end();
    bool get_frame(cv::Mat &image);
    
    void init();
    void deinit();
    void set_acquisition_mode(AcquisitionMode mode);
    void set_buffer_mode(BufferMode mode);
    void setup_processor(Spinnaker::ColorProcessingAlgorithm algorithm);
    void setup_processor() { setup_processor(SPINNAKER_COLOR_PROCESSING_ALGORITHM_HQ_LINEAR); };
    void print_info();
    

private:
    enum FLAGS {
        ACQ_MODE_SET,
        PROCESSOR_SET,
        FLAG_COUNT
    };

    CameraPtr _cam = nullptr;
    INodeMap *nm_TL, *nm, *nm_Stream;
    SystemPtr system;
    ImageProcessor *processor;
    std::string serial_number;
    const char acq_modes[3][16] = {"Continuous", "SingleFrame", "MultiFrame"};
    const char buf_modes[4][32] = {"NewestFirst", "NewestOnly", "OldestFirst", "OldestFirstOverwrite"};
    bool flags[FLAG_COUNT] = {false};
};