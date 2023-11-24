#include "spinnaker_camera.hpp"


bool SpinnakerCameraContainer::begin() {
    if (!flags[ACQ_MODE_SET] || !flags[PROCESSOR_SET]) 
        return false;

    try {
        if (_cam->IsStreaming())
            return true;

        _cam->BeginAcquisition();
        return true;
    } catch (Spinnaker::Exception &e) {
        std::cout << "Error begining acquisition: " << e.what() << std::endl;
        return false;
    }
}

bool SpinnakerCameraContainer::end() {
    try {
        if (!_cam->IsStreaming())
            return true;

        _cam->EndAcquisition();
        return true;
    } catch (Spinnaker::Exception &e) {
        std::cout << "Error ending acquisition: " << e.what() << std::endl;
        return false;
    }
}

bool SpinnakerCameraContainer::get_frame(cv::Mat &image) {
    if(!flags[ACQ_MODE_SET] || !flags[PROCESSOR_SET])
        return false;

    if (!_cam->IsStreaming())
        return false;

    try {
        ImagePtr raw_image = _cam->GetNextImage(1000);
        if (raw_image->IsIncomplete()) {
            std::cout << "Image incomplete with image status " << raw_image->GetImageStatus() << std::endl;
            return false;
        }

        ImagePtr converted_image = processor->Convert(raw_image, PixelFormat_BGR8);
        image = cv::Mat(
            converted_image->GetHeight(), 
            converted_image->GetWidth(), 
            CV_8UC3, 
            converted_image->GetData(), 
            converted_image->GetStride()
        ).clone();

        // std::cout << "X: " << converted_image->GetHeight() << " Y: " << converted_image->GetWidth() << std::endl;

        raw_image->Release();

        return true;
    } catch (Spinnaker::Exception &e) {
        std::cout << "Error acquiring images: " << e.what() << std::endl;
        return false;
    }
}

void SpinnakerCameraContainer::init() {
    _cam->Init();
    
    try {
        nm_Stream = &_cam->GetTLStreamNodeMap();
        nm_TL = &_cam->GetTLDeviceNodeMap();
        nm = &_cam->GetNodeMap();
    } catch(Spinnaker::Exception &e) {
        std::cout << "Error initializing camera: " << e.what() << std::endl;
    }

    CStringPtr serial_number_node = nm_TL->GetNode("DeviceSerialNumber");
    if(IsReadable(serial_number_node)){
        serial_number = serial_number_node->GetValue();
    } else {
        std::cout << "Unable to get serial number" << std::endl;
        exit(-1);
    }

    setup_processor();
}

void SpinnakerCameraContainer::deinit() {
    end();
    this->_cam->DeInit();
}

SpinnakerCameraContainer::~SpinnakerCameraContainer() {
    deinit();
    this->_cam = nullptr;
}

void SpinnakerCameraContainer::set_acquisition_mode(AcquisitionMode mode) {
    try {

        // Get Acquisition enum node
        CEnumerationPtr acq_mode_enum = nm->GetNode("AcquisitionMode");
        if(!IsReadable(acq_mode_enum) || !IsWritable(acq_mode_enum)) {
            std::cout << "[Camera " << serial_number << "] Unable to get acquisition mode enum node. Aborting..." << std::endl;
            exit(-1);
        }

        // Get entry for continuous acquisition mode
        CEnumEntryPtr acq_mode_val = acq_mode_enum->GetEntryByName(acq_modes[mode]);
        if(!IsReadable(acq_mode_val)) {
            std::cout << "[Camera " << serial_number << "] Unable to get acquisition mode entry. Aborting..." << std::endl;
            exit(-1);
        }

        acq_mode_enum->SetIntValue(acq_mode_val->GetValue());

        std::cout << "[Camera " << serial_number << "] Acquisition mode set to " << mode << std::endl;

    } catch (Spinnaker::Exception &e) {
        std::cout << "[Camera " << serial_number << "] Error setting acquisition mode: " << e.what() << std::endl;
    }

    flags[ACQ_MODE_SET] = true;
}

void SpinnakerCameraContainer::set_buffer_mode(BufferMode mode) {
    try {
        CEnumerationPtr buff_handle_node = nm_Stream->GetNode("StreamBufferHandlingMode");
        if(!IsReadable(buff_handle_node) || !IsWritable(buff_handle_node)) {
            std::cout << "[Camera " << serial_number << "] Unable to get buffer handling mode node. Aborting..." << std::endl;
            exit(-1);
        }

        CEnumEntryPtr buff_handle_val = buff_handle_node->GetEntryByName(buf_modes[mode]);
        if(!IsReadable(buff_handle_val)) {
            std::cout << "[Camera " << serial_number << "] Unable to get buffer handling mode entry. Aborting..." << std::endl;
            exit(-1);
        }

        buff_handle_node->SetIntValue(buff_handle_val->GetValue());

        std::cout << "[Camera " << serial_number << "] Buffer mode set to " << mode << std::endl;

    } catch (Spinnaker::Exception &e) {
        std::cout << "[Camera " << serial_number << "] Error setting buffer mode: " << e.what() << std::endl;
    }

    flags[ACQ_MODE_SET] = true;
}

void SpinnakerCameraContainer::setup_processor(Spinnaker::ColorProcessingAlgorithm algorithm) {
    processor->SetColorProcessing(algorithm);
    processor->SetNumDecompressionThreads(6);

    flags[PROCESSOR_SET] = true;
}

void SpinnakerCameraContainer::print_info() {
    std::cout << std::endl << "   ***   CAMERA INFO   ***   " << std::endl;
    try {
        GenApi::FeatureList_t features;
        const GenApi::CCategoryPtr category = nm_TL->GetNode("DeviceInformation");

        if (GenApi::IsAvailable(category) && GenApi::IsReadable(category)) {
            category->GetFeatures(features);

            for (auto it = features.begin(); it != features.end(); ++it) {
                const GenApi::CNodePtr pfeature_node = *it;
                std::cout << pfeature_node->GetName() << " : ";
                GenApi::CValuePtr pvalue = (GenApi::CValuePtr)pfeature_node;
                std::cout << (IsReadable(pvalue) ? pvalue->ToString() : "Node not readable");
                std::cout << std::endl;
            }
        } else {
            std::cout << "Device control information not available." << std::endl;
        }
    } catch (Spinnaker::Exception &e) {
        std::cout << "Error getting device info: " << e.what() << std::endl;
    }
}
