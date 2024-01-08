#pragma once
#include "./basics.hpp"
#include "./receiver.hpp"

class VRStation : public vr::ITrackedDeviceServerDriver
{
private:
    int index;
    std::string serial;

    vr::TrackedDeviceIndex_t id;
    vr::PropertyContainerHandle_t propContainer;
    vr::VRInputComponentHandle_t comphaptic;
    vr::VRInputComponentHandle_t HButtons[2];

public:
    VRStation()
    {
        this->id = vr::k_unTrackedDeviceIndexInvalid;
        this->propContainer = vr::k_ulInvalidPropertyContainer;
    }

    VRStation(int index = 0)
    {
        this->id = vr::k_unTrackedDeviceIndexInvalid;
        this->propContainer = vr::k_ulInvalidPropertyContainer;
        this->setStationIndex(index);
    }

    void RunFrame()
    {
        if (this->id != vr::k_unTrackedDeviceIndexInvalid) {
            vr::VRServerDriverHost()->TrackedDevicePoseUpdated(this->id, GetPose(), sizeof(vr::DriverPose_t));
        }
    }

    void ProcessEvent(const vr::VREvent_t& ev)
    {
        switch (ev.eventType)
        {
            case vr::VREvent_Input_HapticVibration:
                if (ev.data.hapticVibration.componentHandle == this->comphaptic);
                    // haptic feedback
                break;
        }
    }

    vr::DriverPose_t GetPose()
    {
        vr::DriverPose_t pose = {0};
        pose.poseIsValid = true;
        pose.result = vr::TrackingResult_Running_OK;
        pose.deviceIsConnected = true;

        pose.qWorldFromDriverRotation = HmdQuaternion_Init(1, 0, 0, 0);
        pose.qDriverFromHeadRotation = HmdQuaternion_Init(1, 0, 0, 0);

        Point3f center, up, right, left, forward;
        float cyaw, croll, cpitch;
        croll = 0;
        
        if (this->index == 1)
        {
            cpitch = 45 * deg2rad;
            cyaw = 225 * deg2rad;
            pose.vecPosition[0] = 1;
            pose.vecPosition[1] = 1;
            pose.vecPosition[2] = 2;
        } else
        {
            cpitch = 45 * deg2rad;
            cyaw = 45 * deg2rad;
            pose.vecPosition[0] = -1;
            pose.vecPosition[1] = -1;
            pose.vecPosition[2] = 2;
        }

        //Convert yaw, pitch, roll to quaternion
        float ct0 = cos(croll * 0.5);
        float ct1 = sin(croll * 0.5);
        float ct2 = cos(cpitch * 0.5);
        float ct3 = sin(cpitch * 0.5);
        float ct4 = cos(cyaw * 0.5);
        float ct5 = sin(cyaw * 0.5);

        //Set controller rotation
        pose.qRotation.w = ct0 * ct2 * ct4 + ct1 * ct3 * ct5;
        pose.qRotation.x = ct0 * ct3 * ct4 - ct1 * ct2 * ct5;
        pose.qRotation.y = ct0 * ct2 * ct5 + ct1 * ct3 * ct4;
        pose.qRotation.z = ct1 * ct2 * ct4 - ct0 * ct3 * ct5;

        return pose;
    }

    vr::EVRInitError Activate(vr::TrackedDeviceIndex_t id)
    {
        this->id = id;
        this->propContainer = vr::VRProperties()->TrackedDeviceToPropertyContainer(this->id);

        // Set some universe ID (Must be 2 or higher)
        vr::VRProperties()->SetUint64Property(this->propContainer, vr::Prop_CurrentUniverseId_Uint64, 5621);
        vr::VRProperties()->SetStringProperty(this->propContainer, vr::Prop_ModelNumber_String, "TrackingReference");
        vr::VRProperties()->SetStringProperty(this->propContainer, vr::Prop_RenderModelName_String, "locator");
        vr::VRProperties()->SetStringProperty(this->propContainer, vr::Prop_NamedIconPathDeviceReady_String, "{null}/icons/trackingreference_ready.png");
        vr::VRProperties()->SetStringProperty(this->propContainer, vr::Prop_NamedIconPathDeviceOff_String, "{null}/icons/trackingreference_not_ready.png");
        vr::VRProperties()->SetStringProperty(this->propContainer, vr::Prop_NamedIconPathDeviceSearching_String, "{null}/icons/trackingreference_not_ready.png");
        vr::VRProperties()->SetStringProperty(this->propContainer, vr::Prop_NamedIconPathDeviceSearchingAlert_String, "{null}/icons/trackingreference_not_ready.png");
        vr::VRProperties()->SetStringProperty(this->propContainer, vr::Prop_NamedIconPathDeviceReadyAlert_String, "{null}/icons/trackingreference_not_ready.png");
        vr::VRProperties()->SetStringProperty(this->propContainer, vr::Prop_NamedIconPathDeviceNotReady_String, "{null}/icons/trackingreference_not_ready.png");
        vr::VRProperties()->SetStringProperty(this->propContainer, vr::Prop_NamedIconPathDeviceStandby_String, "{null}/icons/trackingreference_not_ready.png");
        vr::VRProperties()->SetStringProperty(this->propContainer, vr::Prop_NamedIconPathDeviceAlertLow_String, "{null}/icons/trackingreference_not_ready.png");

        return vr::VRInitError_None;
    }

    void Deactivate() {this->id = vr::k_unTrackedDeviceIndexInvalid;}
    void EnterStandby() {}
    void LeaveStandBy() {}
    void PowerOff() {}
    void* GetComponent(const char *nameAndVersion) {return NULL;}
    std::string GetSerialNumber()
    {
        return this->serial;
    }

    void DebugRequest(const char *pchRequest, char *pchResponseBuffer, uint32_t unResponseBufferSize)
    {
        if (unResponseBufferSize >= 1)
            pchResponseBuffer[0] = 0;
    }

    void setStationIndex(int index)
    {
        this->index = index;
        this->serial = "Station"+std::to_string(this->index);
    }

    ~VRStation()
    {

    }
};