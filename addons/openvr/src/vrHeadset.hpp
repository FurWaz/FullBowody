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
        this->serial = "FullBowody Headset";
    }

    vr::EVRInitError Activate(vr::TrackedDeviceIndex_t id)
    {
        this->id = id;
        this->propContainer = vr::VRProperties()->TrackedDeviceToPropertyContainer(this->id);

        this->IPD = vr::VRSettings()->GetFloat(vr::k_pch_SteamVR_Section, vr::k_pch_SteamVR_IPD_Float);
        // this->winX = vr::VRSettings()->GetInt32(vr::k_pch_Null_Section, vr::k_pch_Null_WindowX_Int32);
        // this->winY = vr::VRSettings()->GetInt32(vr::k_pch_Null_Section, vr::k_pch_Null_WindowY_Int32);
        // this->winW = vr::VRSettings()->GetInt32(vr::k_pch_Null_Section, vr::k_pch_Null_WindowWidth_Int32);
        // this->winH = vr::VRSettings()->GetInt32(vr::k_pch_Null_Section, vr::k_pch_Null_WindowHeight_Int32);
        // this->renderW = vr::VRSettings()->GetInt32(vr::k_pch_Null_Section, vr::k_pch_Null_RenderWidth_Int32);
        // this->renderH = vr::VRSettings()->GetInt32(vr::k_pch_Null_Section, vr::k_pch_Null_RenderHeight_Int32);
        this->winX = 0;
        this->winY = 0;
        this->winW = 1000;
        this->winH = 500;
        this->renderW = 1000;
        this->renderH = 500;
        this->secFromVSyncToPhotons = vr::VRSettings()->GetFloat(vr::k_pch_Null_Section , vr::k_pch_Null_SecondsFromVsyncToPhotons_Float);
        this->displayFreq = 60.f;

        vr::VRProperties()->SetStringProperty(this->propContainer, vr::Prop_ModelNumber_String, this->serial.c_str());
        vr::VRProperties()->SetStringProperty(this->propContainer, vr::Prop_SerialNumber_String, this->serial.c_str());
        vr::VRProperties()->SetFloatProperty(this->propContainer, vr::Prop_UserIpdMeters_Float, this->IPD);
        // vr::VRProperties()->SetFloatProperty(this->propContainer, vr::Prop_UserHeadToEyeDepthMeters_Float, 0.f);
        vr::VRProperties()->SetFloatProperty(this->propContainer, vr::Prop_DisplayFrequency_Float, this->displayFreq);
        // vr::VRProperties()->SetFloatProperty(this->propContainer, vr::Prop_SecondsFromVsyncToPhotons_Float, this->secFromVSyncToPhotons);

        vr::VRProperties()->SetUint64Property(this->propContainer, vr::Prop_CurrentUniverseId_Uint64, 5621);
        // vr::VRProperties()->SetBoolProperty(this->propContainer, vr::Prop_IsOnDesktop_Bool, false);
        // vr::VRProperties()->SetBoolProperty(this->propContainer, vr::Prop_DisplayDebugMode_Bool, true);

        vr::VRProperties()->SetStringProperty(this->propContainer, vr::Prop_NamedIconPathDeviceOff_String, "{null}/icons/headset_sample_status_off.png");
        vr::VRProperties()->SetStringProperty(this->propContainer, vr::Prop_NamedIconPathDeviceSearching_String, "{null}/icons/headset_sample_status_searching.gif");
        vr::VRProperties()->SetStringProperty(this->propContainer, vr::Prop_NamedIconPathDeviceSearchingAlert_String, "{null}/icons/headset_sample_status_searching_alert.gif");
        vr::VRProperties()->SetStringProperty(this->propContainer, vr::Prop_NamedIconPathDeviceReady_String, "{null}/icons/headset_sample_status_ready.png");
        vr::VRProperties()->SetStringProperty(this->propContainer, vr::Prop_NamedIconPathDeviceReadyAlert_String, "{null}/icons/headset_sample_status_ready_alert.png");
        vr::VRProperties()->SetStringProperty(this->propContainer, vr::Prop_NamedIconPathDeviceNotReady_String, "{null}/icons/headset_sample_status_error.png");
        vr::VRProperties()->SetStringProperty(this->propContainer, vr::Prop_NamedIconPathDeviceStandby_String, "{null}/icons/headset_sample_status_standby.png");
        vr::VRProperties()->SetStringProperty(this->propContainer, vr::Prop_NamedIconPathDeviceAlertLow_String, "{null}/icons/headset_sample_status_ready_low.png");

        
        println((
            "Initialisation done: winW="+std::to_string(this->winW)+", winH="+std::to_string(this->winH)+
            ", winX="+std::to_string(this->winX)+", winY="+std::to_string(this->winY)+
            ", renderW="+std::to_string(this->renderW)+", renderH="+std::to_string(this->renderH)
        ).c_str());

        return vr::VRInitError_None;
    }

    void Deactivate()
    {
        this->id = vr::k_unTrackedDeviceIndexInvalid;
    }
    void EnterStandby() {}
    void LeaveStandby() {}
    void PowerOff() {}
    std::string GetSerialNumber() {return this->serial.c_str();}
    
    void DebugRequest(const char *pchRequest, char *pchResponseBuffer, uint32_t unResponseBufferSize)
    {
        if (unResponseBufferSize >= 1)
            pchResponseBuffer[0] = 0;
    }

    void GetWindowBounds(int32_t* pnX, int32_t* pnY, uint32_t* pnWidth, uint32_t* pnHeight)
    {
        *pnX = this->winX;
        *pnY = this->winY;
        *pnWidth = this->winW;
        *pnHeight = this->winH;
    }

    bool IsDisplayOnDesktop()
    {
        return true;
    }

    bool IsDisplayRealDisplay()
    {
        return false;
    }

    void GetRecommendedRenderTargetSize(uint32_t* pnWidth, uint32_t* pnHeight)
    {
        *pnWidth = this->winW;
        *pnHeight = this->winH;
    }

    void GetEyeOutputViewport(vr::EVREye eEye, uint32_t* pnX, uint32_t* pnY, uint32_t* pnWidth, uint32_t* pnHeight)
    {
        *pnY = 0;
        *pnWidth = this->winW / 2;
        *pnHeight = this->winH;

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

        // pose.vecPosition[0] = bodyPos[CONSTANT::JOINT_HEAD].x;
        // pose.vecPosition[1] = bodyPos[CONSTANT::JOINT_HEAD].y;
        // pose.vecPosition[2] = bodyPos[CONSTANT::JOINT_HEAD].z;
        pose.vecPosition[0] = 0;
        pose.vecPosition[1] = 0;
        pose.vecPosition[2] = 0;

        float yaw = 0;
        float roll = 0;
        float pitch = 0;

        t0 = cos(yaw * 0.5);
        t1 = sin(yaw * 0.5);
        t2 = cos(roll * 0.5);
        t3 = sin(roll * 0.5);
        t4 = cos(pitch * 0.5);
        t5 = sin(pitch * 0.5);

        pose.qRotation.w = t0 * t2 * t4 + t1 * t3 * t5;
        pose.qRotation.x = t0 * t3 * t4 - t1 * t2 * t5;
        pose.qRotation.y = t0 * t2 * t5 + t1 * t3 * t4;
        pose.qRotation.z = t1 * t2 * t4 - t0 * t3 * t5;

        return pose;
    }

    void RunFrame()
    {
        if (this->id != vr::k_unTrackedDeviceIndexInvalid)
            vr::VRServerDriverHost()->TrackedDevicePoseUpdated(this->id, this->GetPose(), sizeof(vr::DriverPose_t));
    }

    void* GetComponent(const char* nameAndVersion)
    {
        if (!_stricmp(nameAndVersion, vr::IVRDisplayComponent_Version))
            return (vr::IVRDisplayComponent*) this;
        return nullptr;
    }

    ~VRHeadset()
    {

    }
};