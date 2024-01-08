#pragma once
#include "./basics.hpp"
#include "./receiver.hpp"

class VRTracker : public vr::ITrackedDeviceServerDriver
{
private:
    int index;
    std::string serial;

    vr::TrackedDeviceIndex_t id;
    vr::PropertyContainerHandle_t propContainer;
    vr::VRInputComponentHandle_t comphaptic;
    vr::VRInputComponentHandle_t HButtons[2];

public:
    VRTracker()
    {
        this->id = vr::k_unTrackedDeviceIndexInvalid;
        this->propContainer = vr::k_ulInvalidPropertyContainer;
    }

    VRTracker(int index = 0)
    {
        this->id = vr::k_unTrackedDeviceIndexInvalid;
        this->propContainer = vr::k_ulInvalidPropertyContainer;
        this->setTrackerIndex(index);
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
        switch (this->index)
        {
            case TRACKER::HIPS:
                right = bodyPos[CONSTANT::JOINT_HIP_R];
                left = bodyPos[CONSTANT::JOINT_HIP_L];
                up = bodyPos[CONSTANT::JOINT_NECK];
                pose.vecPosition[0] = (left.x + right.x) / 2;
                pose.vecPosition[1] = (left.y + right.y) / 2;
                pose.vecPosition[2] = (left.z + right.z) / 2;
                center = Point3f(pose.vecPosition[0], pose.vecPosition[1], pose.vecPosition[2]);
                cyaw = atan2(
                    center.x-right.x,
                    center.z-right.z
                ) - 1.5707963;
                cpitch = atan2(
                    up.y-center.y,
                    up.x-center.x
                );
                croll = atan2(
                    left.y-right.y,
                    sqrtf( pow(left.x-right.x, 2) + pow(left.x-right.x, 2) )
                );
                break;
            case TRACKER::FEET_L:
                forward = bodyPos[CONSTANT::JOINT_FEET_L];
                up = bodyPos[CONSTANT::JOINT_KNEE_L];
                center = bodyPos[CONSTANT::JOINT_ANKLE_L];
                pose.vecPosition[0] = center.x;
                pose.vecPosition[1] = center.y;
                pose.vecPosition[2] = center.z;
                cyaw = atan2(
                    center.x-forward.x,
                    center.z-forward.z
                ) + 3.1415926;
                cpitch = atan2(
                    up.y-center.y,
                    up.x-center.x
                );
                croll = atan2(
                    up.y-center.y,
                    sqrtf( pow(left.x-forward.x, 2) + pow(left.x-forward.x, 2) )
                );
                break;
            case TRACKER::FEET_R:
                forward = bodyPos[CONSTANT::JOINT_FEET_R];
                up = bodyPos[CONSTANT::JOINT_KNEE_R];
                center = bodyPos[CONSTANT::JOINT_ANKLE_R];
                pose.vecPosition[0] = center.x;
                pose.vecPosition[1] = center.y;
                pose.vecPosition[2] = center.z;
                cyaw = atan2(
                    center.x-forward.x,
                    center.z-forward.z
                ) + 3.1415926;
                cpitch = atan2(
                    up.y-center.y,
                    up.x-center.x
                );
                croll = atan2(
                    up.y-center.y,
                    sqrtf( pow(left.x-forward.x, 2) + pow(left.x-forward.x, 2) )
                );
                break;
        }

        // float distXZ = sqrtf( pow(bodyPos[handIndex].x-bodyPos[wristIndex].x, 2) + pow(bodyPos[handIndex].x-bodyPos[wristIndex].x, 2) );
        // float cpitch = atan2(
        //     bodyPos[handIndex].x-bodyPos[wristIndex].x,
        //     bodyPos[handIndex].z-bodyPos[wristIndex].z
        // ) + 3.1415926;
        // float croll = atan2(
        //     bodyPos[handIndex].y-bodyPos[wristIndex].y,
        //     distXZ
        // );
        // float cyaw = 0;

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

        // line below maybe causing trackers to be assigned as controllers
        // vr::VRProperties()->SetStringProperty(this->propContainer, vr::Prop_ControllerType_String, "vive_tracker");
        // vr::VRProperties()->SetInt32Property(this->propContainer, vr::Prop_ControllerRoleHint_Int32, vr::TrackedControllerRole_OptOut);
        // vr::VRDriverInput()->CreateBooleanComponent(this->propContainer, "/input/system/click", &HButtons[0]);
        // vr::VRDriverInput()->CreateBooleanComponent(this->propContainer, "/input/system/touch", &HButtons[1]);

        vr::VRProperties()->SetStringProperty(this->propContainer, vr::Prop_InputProfilePath_String, "vive_tracker");

        vr::VRProperties()->SetStringProperty(this->propContainer, vr::Prop_ModelNumber_String, "ViveMV");
        vr::VRProperties()->SetStringProperty(this->propContainer, vr::Prop_ManufacturerName_String, "HTC");
        vr::VRProperties()->SetStringProperty(this->propContainer, vr::Prop_RenderModelName_String, "vr_tracker_05_wireless_b");

        vr::VRProperties()->SetInt32Property(this->propContainer, vr::Prop_DeviceClass_Int32, vr::TrackedDeviceClass_GenericTracker);
        vr::VRProperties()->SetStringProperty(this->propContainer, vr::Prop_TrackingSystemName_String, "VR Tracker");
        vr::VRProperties()->SetStringProperty(this->propContainer, vr::Prop_SerialNumber_String, this->serial.c_str());

        vr::VRProperties()->SetStringProperty(this->propContainer, vr::Prop_InputProfilePath_String, "{null}/input/example_tracker_bindings.json");
        vr::VRDriverInput()->CreateHapticComponent(this->propContainer, "/output/haptic", &this->comphaptic);

        vr::VRProperties()->SetUint64Property(this->propContainer, vr::Prop_CurrentUniverseId_Uint64, 5621);

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

    void setTrackerIndex(int index)
    {
        this->index = index;
        this->serial = "Tracker"+std::to_string(this->index);
    }

    ~VRTracker()
    {

    }
};