#pragma once
#include "./vrController.hpp"
#include "./vrStation.hpp"
#include "./vrTracker.hpp"
#include "./vrHeadset.hpp"

class VRDriver : vr::IServerTrackedDeviceProvider
{
private:
    VRHeadset* headset = nullptr;
    VRController* leftControl = nullptr;
    VRController* rightControl = nullptr;
    Receiver receiver;
    
    VRTracker* hipTracker = nullptr;
    VRTracker* feetLTracker = nullptr;
    VRTracker* feetRTracker = nullptr;

    VRStation* station1 = nullptr;
    VRStation* station2 = nullptr;
    
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

            this->hipTracker = new VRTracker(TRACKER::HIPS);
            this->feetLTracker = new VRTracker(TRACKER::FEET_L);
            this->feetRTracker = new VRTracker(TRACKER::FEET_R);

            // this->station1 = new VRStation(1);
            // this->station2 = new VRStation(2);

            vr::VRServerDriverHost()->TrackedDeviceAdded(this->headset->GetSerialNumber().c_str(), vr::TrackedDeviceClass_HMD, this->headset);
            vr::VRServerDriverHost()->TrackedDeviceAdded(this->leftControl->GetSerialNumber().c_str(), vr::TrackedDeviceClass_Controller, this->leftControl);
            vr::VRServerDriverHost()->TrackedDeviceAdded(this->rightControl->GetSerialNumber().c_str(), vr::TrackedDeviceClass_Controller, this->rightControl);

            vr::VRServerDriverHost()->TrackedDeviceAdded(this->hipTracker->GetSerialNumber().c_str(), vr::TrackedDeviceClass_GenericTracker, this->hipTracker);
            vr::VRServerDriverHost()->TrackedDeviceAdded(this->feetLTracker->GetSerialNumber().c_str(), vr::TrackedDeviceClass_GenericTracker, this->feetLTracker);
            vr::VRServerDriverHost()->TrackedDeviceAdded(this->feetRTracker->GetSerialNumber().c_str(), vr::TrackedDeviceClass_GenericTracker, this->feetRTracker);

            vr::VRServerDriverHost()->TrackedDeviceAdded(this->station1->GetSerialNumber().c_str(), vr::TrackedDeviceClass_TrackingReference, this->station1);
            vr::VRServerDriverHost()->TrackedDeviceAdded(this->station2->GetSerialNumber().c_str(), vr::TrackedDeviceClass_TrackingReference, this->station2);
        }
        return err;
    }

    void Cleanup()
    {
        if (this->headset)
            delete this->headset;
        if (this->leftControl)
            delete this->leftControl;
        if (this->rightControl)
            delete this->rightControl;
            
        if (this->hipTracker)
            delete this->hipTracker;
        if (this->feetLTracker)
            delete this->feetLTracker;
        if (this->feetRTracker)
            delete this->feetRTracker;
            
        if (this->station1)
            delete this->station1;
        if (this->station2)
            delete this->station2;

        this->receiver.Stop();
    }

    void RunFrame()
    {
        vr::VREvent_t ev;
        while (vr::VRServerDriverHost()->PollNextEvent(&ev, sizeof(ev)))
        {
            // if (this->leftControl)
                this->leftControl->ProcessEvent(ev);
            // if (this->rightControl)
                this->rightControl->ProcessEvent(ev);
        }
        
        // this->headset->RunFrame();
        this->leftControl->RunFrame();
        this->rightControl->RunFrame();
        
        this->hipTracker->RunFrame();
        this->feetLTracker->RunFrame();
        this->feetRTracker->RunFrame();

        // this->station1->RunFrame();
        // this->station2->RunFrame();
    }

    ~VRDriver()
    {
        
    }
};