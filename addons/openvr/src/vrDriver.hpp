#pragma once
#include "./vrController.hpp"
#include "./vrHeadset.hpp"

class VRDriver : vr::IServerTrackedDeviceProvider
{
private:
    // VRHeadset* headset = nullptr;
    VRController* leftControl = nullptr;
    VRController* rightControl = nullptr;
    Receiver receiver;
    
public:
    const char* const* GetInterfaceVersions() {return vr::k_InterfaceVersions;}
    bool ShouldBlockStandbyMode() {return false;}
    void EnterStandby() {}
    void LeaveStandby() {}

    VRDriver()
    {
        
    }

    vr::EVRInitError Init(vr::IVRDriverContext* context)
    {
        vr::EVRInitError err = vr::InitServerDriverContext(context);
        if (err == vr::VRInitError_None)
        {
            this->receiver.Start();
            // this->headset = new VRHeadset();
            this->leftControl = new VRController();
            this->rightControl = new VRController();
            this->leftControl->setControllerIndex(1);
            this->rightControl->setControllerIndex(2);

            // vr::VRServerDriverHost()->TrackedDeviceAdded(this->headset->GetSerialNumber().c_str(), vr::TrackedDeviceClass_HMD, this->headset);
            vr::VRServerDriverHost()->TrackedDeviceAdded(this->leftControl->GetSerialNumber().c_str(), vr::TrackedDeviceClass_Controller, this->leftControl);
            vr::VRServerDriverHost()->TrackedDeviceAdded(this->rightControl->GetSerialNumber().c_str(), vr::TrackedDeviceClass_Controller, this->rightControl);
        }
        return err;
    }

    void Cleanup()
    {
        // if (this->headset)
        //     delete this->headset;
        if (this->leftControl)
            delete this->leftControl;
        if (this->rightControl)
            delete this->rightControl;
        this->receiver.Stop();
    }

    void RunFrame()
    {
        // vr::VREvent_t ev;
        // while (vr::VRServerDriverHost()->PollNextEvent(&ev, sizeof(ev)))
        // {
        //     if (this->leftControl)
        //         this->leftControl->ProcessEvent(vrEvent);
        //     if (this->rightControl)
        //         this->rightControl->ProcessEvent(vrEvent);
        // }
        
        // this->headset->RunFrame();
        this->leftControl->RunFrame();
        this->rightControl->RunFrame();
    }

    ~VRDriver()
    {
        
    }
};