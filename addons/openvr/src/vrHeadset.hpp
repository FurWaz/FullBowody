#include "./basics.hpp"
#include "./receiver.hpp"

//Head tracking pos and rot vars
static double yaw = 0, pitch = 0, roll = 0;
static double pX = 0, pY = 0, pZ = 0;
static double t0, t1, t2, t3, t4, t5;

class VRHeadset : public vr::ITrackedDeviceServerDriver, public vr::IVRDisplayComponent
{
private:
    vr::TrackedDeviceIndex_t id;
    vr::PropertyContainerHandle_t propContainer;

    std::string serial;
    std::string model;

    int winX, winY, winW, winH, renderW, renderH;
    float displayFreq, secFromVSyncToPhotons, IPD;

public:
    VRHeadset()
    {
        this->id = vr::k_unTrackedDeviceIndexInvalid;
        this->propContainer = vr::k_ulInvalidPropertyContainer;

        this->IPD = vr::VRSettings()->GetFloat(vr::k_pch_SteamVR_Section, vr::k_pch_SteamVR_IPD_Float);
        // char ser[1024];
        // vr::VRSettings()->GetString(vr::k_pch_Null_Section, vr::k_pch_Null_SerialNumber_String, ser, sizeof(ser));
        // this->serial = ser;
        // char mod[1024];
        // vr::VRSettings()->GetString(vr::k_pch_Null_Section, vr::k_pch_Null_ModelNumber_String, mod, sizeof(mod));
        // this->model = mod;
        this->model = "FullBowody";
        this->serial = "CustomHMD";

        this->winX = vr::VRSettings()->GetInt32(vr::k_pch_Null_Section, vr::k_pch_Null_WindowX_Int32);
        this->winY = vr::VRSettings()->GetInt32(vr::k_pch_Null_Section, vr::k_pch_Null_WindowY_Int32);
        this->winW = vr::VRSettings()->GetInt32(vr::k_pch_Null_Section, vr::k_pch_Null_WindowWidth_Int32);
        this->winH = vr::VRSettings()->GetInt32(vr::k_pch_Null_Section, vr::k_pch_Null_WindowHeight_Int32);
        this->renderW = vr::VRSettings()->GetInt32(vr::k_pch_Null_Section, vr::k_pch_Null_RenderWidth_Int32);
        this->renderH = vr::VRSettings()->GetInt32(vr::k_pch_Null_Section, vr::k_pch_Null_RenderHeight_Int32);
        this->secFromVSyncToPhotons = vr::VRSettings()->GetFloat(vr::k_pch_Null_Section , vr::k_pch_Null_SecondsFromVsyncToPhotons_Float);
        this->displayFreq = vr::VRSettings()->GetFloat(vr::k_pch_Null_Section, vr::k_pch_Null_DisplayFrequency_Float);
    }

    vr::EVRInitError Activate(vr::TrackedDeviceIndex_t id)
    {
        this->id = id;
        this->propContainer = vr::VRProperties()->TrackedDeviceToPropertyContainer(this->id);

        vr::VRProperties()->SetStringProperty(this->propContainer, vr::Prop_ModelNumber_String, this->model.c_str());
        vr::VRProperties()->SetStringProperty(this->propContainer, vr::Prop_RenderModelName_String, this->model.c_str());
        vr::VRProperties()->SetFloatProperty(this->propContainer, vr::Prop_UserIpdMeters_Float, this->IPD);
        vr::VRProperties()->SetFloatProperty(this->propContainer, vr::Prop_UserHeadToEyeDepthMeters_Float, 0.f);
        vr::VRProperties()->SetFloatProperty(this->propContainer, vr::Prop_DisplayFrequency_Float, this->displayFreq);
        vr::VRProperties()->SetFloatProperty(this->propContainer, vr::Prop_SecondsFromVsyncToPhotons_Float, this->secFromVSyncToPhotons);

        vr::VRProperties()->SetUint64Property(this->propContainer, vr::Prop_CurrentUniverseId_Uint64, 2);
        vr::VRProperties()->SetBoolProperty(this->propContainer, vr::Prop_IsOnDesktop_Bool, false);
        vr::VRProperties()->SetBoolProperty(this->propContainer, vr::Prop_DisplayDebugMode_Bool, true);

        bool bSetupIconUsingExternalResourceFile = true;
        if (!bSetupIconUsingExternalResourceFile) {
            // Setup properties directly in code.
            // Path values are of the form {drivername}\icons\some_icon_filename.png
            vr::VRProperties()->SetStringProperty(this->propContainer, vr::Prop_NamedIconPathDeviceOff_String, "{null}/icons/headset_sample_status_off.png");
            vr::VRProperties()->SetStringProperty(this->propContainer, vr::Prop_NamedIconPathDeviceSearching_String, "{null}/icons/headset_sample_status_searching.gif");
            vr::VRProperties()->SetStringProperty(this->propContainer, vr::Prop_NamedIconPathDeviceSearchingAlert_String, "{null}/icons/headset_sample_status_searching_alert.gif");
            vr::VRProperties()->SetStringProperty(this->propContainer, vr::Prop_NamedIconPathDeviceReady_String, "{null}/icons/headset_sample_status_ready.png");
            vr::VRProperties()->SetStringProperty(this->propContainer, vr::Prop_NamedIconPathDeviceReadyAlert_String, "{null}/icons/headset_sample_status_ready_alert.png");
            vr::VRProperties()->SetStringProperty(this->propContainer, vr::Prop_NamedIconPathDeviceNotReady_String, "{null}/icons/headset_sample_status_error.png");
            vr::VRProperties()->SetStringProperty(this->propContainer, vr::Prop_NamedIconPathDeviceStandby_String, "{null}/icons/headset_sample_status_standby.png");
            vr::VRProperties()->SetStringProperty(this->propContainer, vr::Prop_NamedIconPathDeviceAlertLow_String, "{null}/icons/headset_sample_status_ready_low.png");
        }

        return vr::VRInitError_None;
    }

    void Deactivate()
    {
        this->id = vr::k_unTrackedDeviceIndexInvalid;
    }
    void EnterStandby() {}
    void LeaveStandby() {}
    void PowerOff() {}
    std::string GetSerialNumber() {return this->serial;}
    
    void DebugRequest(const char *pchRequest, char *pchResponseBuffer, uint32_t unResponseBufferSize)
    {
        if (unResponseBufferSize >= 1)
            pchResponseBuffer[0] = 0;
    }

    void GetWindowBounds(int32_t* pnX, int32_t* pnY, uint32_t* pnW, uint32_t* pnH)
    {
        *pnX = this->winX;
        *pnY = this->winY;
        *pnW = this->winW;
        *pnH = this->winH;
    }

    bool IsDisplayOnDesktop()
    {
        return true;
    }

    bool IsDisplayRealDisplay()
    {
        return false;
    }

    void GetRecommendedRenderTargetSize(uint32_t* pnW, uint32_t* pnH)
    {
        *pnW = this->renderW;
        *pnH = this->renderH;
    }

    void GetEyeOutputViewport(vr::EVREye eEye, uint32_t* pnX, uint32_t* pnY, uint32_t* pnW, uint32_t* pnH)
    {
        *pnY = 0;
        *pnW = this->winW / 2;
        *pnH = this->winH;

        if (eEye == vr::Eye_Left) *pnX = 0;
        else *pnX = this->winW / 2;
    }

    void GetProjectionRaw(vr::EVREye eEye, float *pfLeft, float *pfRight, float *pfTop, float *pfBottom)
    {
        *pfLeft = -1.0;
        *pfRight = 1.0;
        *pfTop = -1.0;
        *pfBottom = 1.0;
    }

    vr::DistortionCoordinates_t ComputeDistortion(vr::EVREye eEye, float fU, float fV)
    {
        vr::DistortionCoordinates_t coords;
        coords.rfBlue[0] = fU;
        coords.rfBlue[1] = fV;
        coords.rfGreen[0] = fU;
        coords.rfGreen[1] = fV;
        coords.rfRed[0] = fU;
        coords.rfRed[1] = fV;
        return coords;
    }

    vr::DriverPose_t GetPose()
    {
        vr::DriverPose_t pose = {0};
        pose.poseIsValid = true;
        pose.result = vr::TrackingResult_Running_OK;
        pose.deviceIsConnected = true;

        pose.qWorldFromDriverRotation = HmdQuaternion_Init(1, 0, 0, 0);
        pose.qDriverFromHeadRotation = HmdQuaternion_Init(1, 0, 0, 0);

        //Simple change yaw, pitch, roll with numpad keys
        // if ((GetAsyncKeyState(VK_NUMPAD3) & 0x8000) != 0) yaw += 0.01;
        // if ((GetAsyncKeyState(VK_NUMPAD1) & 0x8000) != 0) yaw += -0.01;
        // if ((GetAsyncKeyState(VK_NUMPAD4) & 0x8000) != 0) pitch += 0.01;
        // if ((GetAsyncKeyState(VK_NUMPAD6) & 0x8000) != 0) pitch += -0.01;
        // if ((GetAsyncKeyState(VK_NUMPAD8) & 0x8000) != 0) roll += 0.01;
        // if ((GetAsyncKeyState(VK_NUMPAD2) & 0x8000) != 0) roll += -0.01;
        // if ((GetAsyncKeyState(VK_NUMPAD9) & 0x8000) != 0) {yaw = 0; pitch = 0; roll = 0;}

        // if ((GetAsyncKeyState(VK_UP) & 0x8000) != 0) pZ += -0.01;
        // if ((GetAsyncKeyState(VK_DOWN) & 0x8000) != 0) pZ += 0.01;
        // if ((GetAsyncKeyState(VK_LEFT) & 0x8000) != 0) pX += -0.01;
        // if ((GetAsyncKeyState(VK_RIGHT) & 0x8000) != 0) pX += 0.01;
        // if ((GetAsyncKeyState(VK_PRIOR) & 0x8000) != 0) pY += 0.01;
        // if ((GetAsyncKeyState(VK_NEXT) & 0x8000) != 0) pY += -0.01;
        // if ((GetAsyncKeyState(VK_END) & 0x8000) != 0) {pX = 0; pY = 0; pZ = 0;}

        // pose.vecPosition[0] = pX;
        // pose.vecPosition[1] = pY;
        // pose.vecPosition[2] = pZ;

        pose.vecPosition[0] = 0;
        pose.vecPosition[1] = 0;
        pose.vecPosition[2] = 0;

        //Convert yaw, pitch, roll to quaternion
        t0 = cos(yaw * 0.5);
        t1 = sin(yaw * 0.5);
        t2 = cos(roll * 0.5);
        t3 = sin(roll * 0.5);
        t4 = cos(pitch * 0.5);
        t5 = sin(pitch * 0.5);

        //Set head tracking rotation
        // pose.qRotation.w = t0 * t2 * t4 + t1 * t3 * t5;
        // pose.qRotation.x = t0 * t3 * t4 - t1 * t2 * t5;
        // pose.qRotation.y = t0 * t2 * t5 + t1 * t3 * t4;
        // pose.qRotation.z = t1 * t2 * t4 - t0 * t3 * t5;
        pose.qRotation.w = 1;
        pose.qRotation.x = 0;
        pose.qRotation.y = 0;
        pose.qRotation.z = 0;

        return pose;
    }

    void RunFrame()
    {
        if (this->id != vr::k_unTrackedDeviceIndexInvalid)
            vr::VRServerDriverHost()->TrackedDevicePoseUpdated(this->id, this->GetPose(), sizeof(vr::DriverPose_t));
    }

    void* GetComponent(const char* nameAndVersion)
    {
        if (!stricmp(nameAndVersion, vr::IVRDisplayComponent_Version))
            return (vr::IVRDisplayComponent*) this;
        return NULL;
    }

    ~VRHeadset()
    {

    }
};