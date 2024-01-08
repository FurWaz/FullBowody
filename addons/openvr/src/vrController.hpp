#pragma once
#include "./basics.hpp"
#include "./receiver.hpp"

class VRController : public vr::ITrackedDeviceServerDriver
{
private:
    int index;
    std::string serial;

    vr::TrackedDeviceIndex_t id;
    vr::PropertyContainerHandle_t propContainer;
    vr::VRInputComponentHandle_t comphaptic;
    vr::VRInputComponentHandle_t HButtons[4], HAnalog[3];

public:
    VRController()
    {
        this->id = vr::k_unTrackedDeviceIndexInvalid;
        this->propContainer = vr::k_ulInvalidPropertyContainer;
    }

    void RunFrame()
    {
        if (this->index == 1) // Left controller
        {
            // vr::VRDriverInput()->UpdateBooleanComponent(HButtons[0], (0x8000 & GetAsyncKeyState('Z')) != 0, 0); //Application Menu
            // vr::VRDriverInput()->UpdateBooleanComponent(HButtons[1], (0x8000 & GetAsyncKeyState('C')) != 0, 0); //Grip
            // vr::VRDriverInput()->UpdateBooleanComponent(HButtons[2], (0x8000 & GetAsyncKeyState('V')) != 0, 0); //System
            // vr::VRDriverInput()->UpdateBooleanComponent(HButtons[3], (0x8000 & GetAsyncKeyState('1')) != 0, 0); //Trackpad

            // vr::VRDriverInput()->UpdateScalarComponent(HAnalog[0], 0.0, 0); //Trackpad x
            // vr::VRDriverInput()->UpdateScalarComponent(HAnalog[1], 0.0, 0); //Trackpad y

            // if ((GetAsyncKeyState('2') & 0x8000) != 0)
            //     vr::VRDriverInput()->UpdateScalarComponent(HAnalog[0], 1.0, 0);
            // if ((GetAsyncKeyState('3') & 0x8000) != 0)
            //     vr::VRDriverInput()->UpdateScalarComponent(HAnalog[1], 1.0, 0);

            // if ((GetAsyncKeyState('X') & 0x8000) != 0) //Trigger
            //     vr::VRDriverInput()->UpdateScalarComponent(HAnalog[2], 1.0, 0);
            // else vr::VRDriverInput()->UpdateScalarComponent(HAnalog[2], 0.0, 0);
        } else // Right controller
        {
            // vr::VRDriverInput()->UpdateBooleanComponent(HButtons[0], (0x8000 & GetAsyncKeyState(190)) != 0, 0); //Application Menu
            // vr::VRDriverInput()->UpdateBooleanComponent(HButtons[1], (0x8000 & GetAsyncKeyState(191)) != 0, 0); //Grip
            // vr::VRDriverInput()->UpdateBooleanComponent(HButtons[2], (0x8000 & GetAsyncKeyState('N')) != 0, 0); //System
            // vr::VRDriverInput()->UpdateBooleanComponent(HButtons[3], (0x8000 & GetAsyncKeyState('2')) != 0, 0); //Trackpad

            // vr::VRDriverInput()->UpdateScalarComponent(HAnalog[0], 0.0, 0); //Trackpad x
            // vr::VRDriverInput()->UpdateScalarComponent(HAnalog[1], 0.0, 0); //Trackpad y

            // if ((GetAsyncKeyState('4') & 0x8000) != 0) //Trigger
            //     vr::VRDriverInput()->UpdateScalarComponent(HAnalog[2], 1.0, 0);
            // else vr::VRDriverInput()->UpdateScalarComponent(HAnalog[2], 0.0, 0);
        }

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

        unsigned char wristIndex, handIndex;
        float cyaw, croll, cpitch;
        if (this->index == 1)                                        // left controller
        {
            wristIndex = CONSTANT::JOINT_WRIST_L;
            handIndex = CONSTANT::JOINT_HAND_L;
            pose.vecPosition[0] = bodyPos[CONSTANT::JOINT_HAND_L].x;
            pose.vecPosition[1] = bodyPos[CONSTANT::JOINT_HAND_L].y;
            pose.vecPosition[2] = bodyPos[CONSTANT::JOINT_HAND_L].z;
        } else                                                       // Right controller
        {
            wristIndex = CONSTANT::JOINT_WRIST_R;
            handIndex = CONSTANT::JOINT_HAND_R;
            pose.vecPosition[0] = bodyPos[CONSTANT::JOINT_HAND_R].x;
            pose.vecPosition[1] = bodyPos[CONSTANT::JOINT_HAND_R].y;
            pose.vecPosition[2] = bodyPos[CONSTANT::JOINT_HAND_R].z;
        }

        cyaw = atan2(
            bodyPos[handIndex].x-bodyPos[wristIndex].x,
            bodyPos[handIndex].z-bodyPos[wristIndex].z
        ) + 3.1415926;
        cpitch = atan2(
            bodyPos[handIndex].y-bodyPos[wristIndex].y,
            sqrtf( pow(bodyPos[handIndex].x-bodyPos[wristIndex].x, 2) + pow(bodyPos[handIndex].x-bodyPos[wristIndex].x, 2) )
        );
        croll = 0;

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

        vr::VRProperties()->SetStringProperty(this->propContainer, vr::Prop_ControllerType_String, "vive_controller");
        vr::VRProperties()->SetStringProperty(this->propContainer, vr::Prop_InputProfilePath_String, "vive_controller");

        vr::VRProperties()->SetStringProperty(this->propContainer, vr::Prop_ModelNumber_String, "ViveMV");
        vr::VRProperties()->SetStringProperty(this->propContainer, vr::Prop_ManufacturerName_String, "HTC");
        vr::VRProperties()->SetStringProperty(this->propContainer, vr::Prop_RenderModelName_String, "vr_controller_vive_1_5");

        vr::VRProperties()->SetStringProperty(this->propContainer, vr::Prop_TrackingSystemName_String, "VR Controller");
        vr::VRProperties()->SetInt32Property(this->propContainer, vr::Prop_DeviceClass_Int32, vr::TrackedDeviceClass_Controller);
        
        // controller right / left changements
        switch (this->index)
        {
            case 1:
            vr::VRProperties()->SetStringProperty(this->propContainer, vr::Prop_SerialNumber_String, "CTRLSerial1");
            case 2:
            vr::VRProperties()->SetStringProperty(this->propContainer, vr::Prop_SerialNumber_String, "CTRLSerial2");
                break;
        }

        //  Buttons handles
        uint64_t supportedButtons = 0xFFFFFFFFFFFFFFFFULL;
        vr::VRProperties()->SetUint64Property(this->propContainer, vr::Prop_SupportedButtons_Uint64, supportedButtons);

        // controller right / left changements
        switch (this->index)
        {
            case 1:
            vr::VRProperties()->SetInt32Property(this->propContainer, vr::Prop_ControllerRoleHint_Int32, vr::TrackedControllerRole_LeftHand);
                break;
            case 2:
            vr::VRProperties()->SetInt32Property(this->propContainer, vr::Prop_ControllerRoleHint_Int32, vr::TrackedControllerRole_RightHand);
                break;
        }

        vr::VRProperties()->SetStringProperty(this->propContainer, vr::Prop_InputProfilePath_String, "{null}/input/mycontroller_profile.json");
        vr::VRDriverInput()->CreateBooleanComponent(this->propContainer, "/input/application_menu/click", &HButtons[0]);
        vr::VRDriverInput()->CreateBooleanComponent(this->propContainer, "/input/grip/click", &HButtons[1]);
        vr::VRDriverInput()->CreateBooleanComponent(this->propContainer, "/input/system/click", &HButtons[2]);
        vr::VRDriverInput()->CreateBooleanComponent(this->propContainer, "/input/trackpad/click", &HButtons[3]);

        // Analog handles
        vr::VRDriverInput()->CreateScalarComponent(
            this->propContainer, "/input/trackpad/x", &HAnalog[0],
            vr::EVRScalarType::VRScalarType_Absolute, vr::EVRScalarUnits::VRScalarUnits_NormalizedTwoSided
        );
        vr::VRDriverInput()->CreateScalarComponent(
            this->propContainer, "/input/trackpad/y", &HAnalog[1],
            vr::EVRScalarType::VRScalarType_Absolute, vr::EVRScalarUnits::VRScalarUnits_NormalizedTwoSided
        );
        vr::VRDriverInput()->CreateScalarComponent(
            this->propContainer, "/input/trigger/value", &HAnalog[2],
            vr::EVRScalarType::VRScalarType_Absolute, vr::EVRScalarUnits::VRScalarUnits_NormalizedOneSided
        );

        vr::VRProperties()->SetInt32Property(this->propContainer, vr::Prop_Axis0Type_Int32, vr::k_eControllerAxis_TrackPad);
        // create haptic component
        vr::VRDriverInput()->CreateHapticComponent(this->propContainer, "/output/haptic", &this->comphaptic);

        return vr::VRInitError_None;
    }

    void Deactivate() {this->id = vr::k_unTrackedDeviceIndexInvalid;}
    void EnterStandby() {}
    void LeaveStandBy() {}
    void PowerOff() {}
    void* GetComponent(const char *nameAndVersion) {return NULL;}
    std::string GetSerialNumber()
    {
        switch (this->index)
        {
            case 1:
                return "CtrlSerial1";
            case 2:
                return "CtrlSerial2";
        }
        return "";
    }

    void DebugRequest(const char *pchRequest, char *pchResponseBuffer, uint32_t unResponseBufferSize)
    {
        if (unResponseBufferSize >= 1)
            pchResponseBuffer[0] = 0;
    }

    void setControllerIndex(int index) {this->index = index;}

    ~VRController()
    {

    }
};