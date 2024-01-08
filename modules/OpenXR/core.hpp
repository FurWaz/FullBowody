#include <openxr/openxr.h>
#include <iostream>
#include <vector>

std::string XRCODE2STR(int code);

namespace owo
{
    class OpenXRCore
    {
    private:
        XrInstance instance;
        XrSession session;
        XrSystemId systemId;

        XrEnvironmentBlendMode envBlendMode;
        XrReferenceSpaceType appSpaceType;
        XrSpace appSpace;
        XrAction poseAction;

        uint32_t nb_extensions;
        std::vector<XrExtensionProperties> extensionsProperties;
        std::vector<std::string> extensionsNames;
        std::vector<const char*> appExtensions;

        XrApplicationInfo createXrApplicationInfo(XrVersion apiVersion, std::string applicationName, uint32_t applicationVersion, std::string engineName, uint32_t engineVersion)
        {
            XrApplicationInfo res;
            res.apiVersion = apiVersion;
            strcpy(res.applicationName, applicationName.c_str());
            res.applicationVersion = applicationVersion;
            strcpy(res.engineName, engineName.c_str());
            res.engineVersion = engineVersion;
            return res;
        }

        bool isExtensionAvailable(std::string name)
        {
            bool result = false;
            for (int i = 0; i < this->extensionsNames.size(); i++)
            {
                std::string extName = this->extensionsNames.at(i);
                if (strcmp(name.c_str(), extName.c_str()) == 0)
                {
                    result = true;
                    break;
                }
            }
            return result;
        }

        void retreiveAvailableExtensions()
        {
            xrEnumerateInstanceExtensionProperties(nullptr, 0, &this->nb_extensions, nullptr);
            this->extensionsProperties = std::vector<XrExtensionProperties>(this->nb_extensions, {XR_TYPE_EXTENSION_PROPERTIES});
            xrEnumerateInstanceExtensionProperties(nullptr, this->nb_extensions, &this->nb_extensions, this->extensionsProperties.data());

            this->extensionsNames = std::vector<std::string>();
            for (XrExtensionProperties ext: this->extensionsProperties)
                extensionsNames.push_back(ext.extensionName);

            this->appExtensions = std::vector<const char*>();
            if (this->isExtensionAvailable("XR_EXT_hand_tracking"))
                this->appExtensions.push_back("XR_EXT_hand_tracking");
        }

        void initInstance()
        {
            std::cout << ">> Info: creating instance ..." << std::endl;
            XrInstanceCreateInfo info{XR_TYPE_INSTANCE_CREATE_INFO};
            this->retreiveAvailableExtensions();
            info.applicationInfo = this->createXrApplicationInfo(XR_CURRENT_API_VERSION, "FullBowody", 1, "FurWaz", 1);
            info.enabledExtensionCount = this->appExtensions.size();
            info.enabledExtensionNames = this->appExtensions.data();
            XrResult res = xrCreateInstance(&info, &instance);
            std::cout << ">> Info: xrCreateInstance returned [" << XRCODE2STR(res) << "]" << std::endl;
        }

        void initSystem()
        {
            std::cout << ">> Info: initializing system ..." << std::endl;
            XrSystemGetInfo systemInfo{XR_TYPE_SYSTEM_GET_INFO};
            systemInfo.formFactor = XrFormFactor{XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY};
            XrResult res = XR_TIMEOUT_EXPIRED;
            while (res != XR_SUCCESS)
            {
                res = xrGetSystem(this->instance, &systemInfo, &this->systemId);
                std::cout << ">> Info: xrGetSystem returned [" << XRCODE2STR(res) << "]" << std::endl;

                if (res == XR_ERROR_FORM_FACTOR_UNAVAILABLE)
                {
                    std::cout << ">> No headset detected, try again in 1s ..." << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
            }

            XrViewConfigurationType viewConfigType{XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO};
            uint32_t nb_modes;
            xrEnumerateEnvironmentBlendModes(this->instance, systemId, viewConfigType, 0, &nb_modes, nullptr);
            std::vector<XrEnvironmentBlendMode> blendModes(nb_modes);
            xrEnumerateEnvironmentBlendModes(this->instance, systemId, viewConfigType, nb_modes, &nb_modes, blendModes.data());

            this->envBlendMode = blendModes[0];
        }

        void initView()
        {
            std::cout << ">> Info: creating views ..." << std::endl;
            uint32_t nb_views;
            xrEnumerateViewConfigurations(this->instance, this->systemId, 0, &nb_views, nullptr);
            std::vector<XrViewConfigurationType> viewConfs(nb_views);
            xrEnumerateViewConfigurations(this->instance, this->systemId, nb_views, &nb_views, viewConfs.data());
            XrViewConfigurationProperties props;
            xrGetViewConfigurationProperties(this->instance, this->systemId, viewConfs[0], &props);

            uint32_t nb_confViews;
            xrEnumerateViewConfigurationViews(this->instance, this->systemId, viewConfs[0], 0, &nb_confViews, nullptr);
            std::vector<XrViewConfigurationView> confViews(nb_confViews, {XR_TYPE_VIEW_CONFIGURATION_VIEW});
            xrEnumerateViewConfigurationViews(this->instance, this->systemId, viewConfs[0], nb_confViews, &nb_confViews, confViews.data());
        }

        void initActions()
        {
            std::cout << ">> Info: creating actions ..." << std::endl;
            XrActionSetCreateInfo actionSetInfo{XR_TYPE_ACTION_SET_CREATE_INFO};
            strcpy(actionSetInfo.actionSetName, "move_hand");
            strcpy(actionSetInfo.localizedActionSetName, "Move Hand");
            actionSetInfo.priority = 1;
            XrActionSet actionSet;
            XrResult res = xrCreateActionSet(this->instance, &actionSetInfo, &actionSet);
            std::cout << ">> Info: xrCreateActionSet returned [" << XRCODE2STR(res) << "]" << std::endl;

            XrPath handPaths[2];
            xrStringToPath(this->instance, "/user/hand/left", &handPaths[0]);
            xrStringToPath(this->instance, "/user/hand/right", &handPaths[1]);

            XrActionCreateInfo actionInfo{XR_TYPE_ACTION_CREATE_INFO};
            actionInfo.actionType = XR_ACTION_TYPE_POSE_INPUT;
            strcpy_s(actionInfo.actionName, "hand_pose");
            strcpy_s(actionInfo.localizedActionName, "Hand Pose");
            actionInfo.countSubactionPaths = 2;
            actionInfo.subactionPaths = handPaths;
            res = xrCreateAction(actionSet, &actionInfo, &this->poseAction);
            std::cout << ">> Info: xrCreateAction returned [" << XRCODE2STR(res) << "]" << std::endl;
        }

        void initSession()
        {
            std::cout << ">> Info: initializing session ..." << std::endl;

            XrSessionCreateInfo createInfo{XR_TYPE_SESSION_CREATE_INFO};
            createInfo.systemId = this->systemId;
            createInfo.next = NULL;

            XrResult res = xrCreateSession(this->instance, &createInfo, &this->session);
            std::cout << ">> Info: xrCreateSession returned [" << XRCODE2STR(res) << "]" << std::endl;
        }

        void initSpaces()
        {
            std::cout << ">> Info: initializing spaces ..." << std::endl;
            this->appSpaceType = XR_REFERENCE_SPACE_TYPE_LOCAL;
            XrReferenceSpaceCreateInfo spaceCreateInfo{XR_TYPE_REFERENCE_SPACE_CREATE_INFO};
            spaceCreateInfo.referenceSpaceType = this->appSpaceType;
            spaceCreateInfo.poseInReferenceSpace = XrPosef{{0, 0, 0, 1}, {0, 0, 0}};
            XrResult res = xrCreateReferenceSpace(this->session, &spaceCreateInfo, &this->appSpace);
            std::cout << ">> Info: xrCreateReferenceSpace returned [" << XRCODE2STR(res) << "]" << std::endl;
        }
        
    public:
        OpenXRCore()
        {

        }

        void start()
        {
            std::cout << ">> STARTING OPENXR <<" << std::endl;
            this->initInstance();
            this->initSystem();
            this->initView();
            this->initActions();
            this->initSession();
            this->initSpaces();
        }

        void stop()
        {
            XrResult res2 = xrDestroyInstance(instance);
            std::cout << ">> Info: xrDestroyInstance returned [" << XRCODE2STR(res2) << "]" << std::endl;
            std::cout << ">> ENDING OPENXR <<" << std::endl;
        }
        
        ~OpenXRCore()
        {

        }
    };
}

std::string XRCODE2STR(int code)
{
    switch (code)
    {
        case 1: return "XR_TIMEOUT_EXPIRED";
        case 3: return "XR_SESSION_LOSS_PENDING";
        case 4: return "XR_EVENT_UNAVAILABLE";
        case 7: return "XR_SPACE_BOUNDS_UNAVAILABLE";
        case 8: return "XR_SESSION_NOT_FOCUSED";
        case 9: return "XR_FRAME_DISCARDED";
        case -1: return "XR_ERROR_VALIDATION_FAILURE";
        case -2: return "XR_ERROR_RUNTIME_FAILURE";
        case -3: return "XR_ERROR_OUT_OF_MEMORY";
        case -4: return "XR_ERROR_API_VERSION_UNSUPPORTED";
        case -6: return "XR_ERROR_INITIALIZATION_FAILED";
        case -7: return "XR_ERROR_FUNCTION_UNSUPPORTED";
        case -8: return "XR_ERROR_FEATURE_UNSUPPORTED";
        case -9: return "XR_ERROR_EXTENSION_NOT_PRESENT";
        case -10: return "XR_ERROR_LIMIT_REACHED";
        case -11: return "XR_ERROR_SIZE_INSUFFICIENT";
        case -12: return "XR_ERROR_HANDLE_INVALID";
        case -13: return "XR_ERROR_INSTANCE_LOST";
        case -14: return "XR_ERROR_SESSION_RUNNING";
        case -16: return "XR_ERROR_SESSION_NOT_RUNNING";
        case -17: return "XR_ERROR_SESSION_LOST";
        case -18: return "XR_ERROR_SYSTEM_INVALID";
        case -19: return "XR_ERROR_PATH_INVALID";
        case -20: return "XR_ERROR_PATH_COUNT_EXCEEDED";
        case -21: return "XR_ERROR_PATH_FORMAT_INVALID";
        case -22: return "XR_ERROR_PATH_UNSUPPORTED";
        case -23: return "XR_ERROR_LAYER_INVALID";
        case -24: return "XR_ERROR_LAYER_LIMIT_EXCEEDED";
        case -25: return "XR_ERROR_SWAPCHAIN_RECT_INVALID";
        case -26: return "XR_ERROR_SWAPCHAIN_FORMAT_UNSUPPORTED";
        case -27: return "XR_ERROR_ACTION_TYPE_MISMATCH";
        case -28: return "XR_ERROR_SESSION_NOT_READY";
        case -29: return "XR_ERROR_SESSION_NOT_STOPPING";
        case -30: return "XR_ERROR_TIME_INVALID";
        case -31: return "XR_ERROR_REFERENCE_SPACE_UNSUPPORTED";
        case -32: return "XR_ERROR_FILE_ACCESS_ERROR";
        case -33: return "XR_ERROR_FILE_CONTENTS_INVALID";
        case -34: return "XR_ERROR_FORM_FACTOR_UNSUPPORTED";
        case -35: return "XR_ERROR_FORM_FACTOR_UNAVAILABLE";
        case -36: return "XR_ERROR_API_LAYER_NOT_PRESENT";
        case -37: return "XR_ERROR_CALL_ORDER_INVALID";
        case -38: return "XR_ERROR_GRAPHICS_DEVICE_INVALID";
        case -39: return "XR_ERROR_POSE_INVALID";
        case -40: return "XR_ERROR_INDEX_OUT_OF_RANGE";
        case -41: return "XR_ERROR_VIEW_CONFIGURATION_TYPE_UNSUPPORTED";
        case -42: return "XR_ERROR_ENVIRONMENT_BLEND_MODE_UNSUPPORTED";
        case -44: return "XR_ERROR_NAME_DUPLICATED";
        case -45: return "XR_ERROR_NAME_INVALID";
        case -46: return "XR_ERROR_ACTIONSET_NOT_ATTACHED";
        case -47: return "XR_ERROR_ACTIONSETS_ALREADY_ATTACHED";
        case -48: return "XR_ERROR_LOCALIZED_NAME_DUPLICATED";
        case -49: return "XR_ERROR_LOCALIZED_NAME_INVALID";
        case -50: return "XR_ERROR_GRAPHICS_REQUIREMENTS_CALL_MISSING";
        case -51: return "XR_ERROR_RUNTIME_UNAVAILABLE";
        case -1000003000: return "XR_ERROR_ANDROID_THREAD_SETTINGS_ID_INVALID_KHR";
        case -1000003001: return "XR_ERROR_ANDROID_THREAD_SETTINGS_FAILURE_KHR";
        case -1000039001: return "XR_ERROR_CREATE_SPATIAL_ANCHOR_FAILED_MSFT";
        case -1000053000: return "XR_ERROR_SECONDARY_VIEW_CONFIGURATION_TYPE_NOT_ENABLED_MSFT";
        case -1000055000: return "XR_ERROR_CONTROLLER_MODEL_KEY_INVALID_MSFT";
        case -1000066000: return "XR_ERROR_REPROJECTION_MODE_UNSUPPORTED_MSFT";
        case -1000097000: return "XR_ERROR_COMPUTE_NEW_SCENE_NOT_COMPLETED_MSFT";
        case -1000097001: return "XR_ERROR_SCENE_COMPONENT_ID_INVALID_MSFT";
        case -1000097002: return "XR_ERROR_SCENE_COMPONENT_TYPE_MISMATCH_MSFT";
        case -1000097003: return "XR_ERROR_SCENE_MESH_BUFFER_ID_INVALID_MSFT";
        case -1000097004: return "XR_ERROR_SCENE_COMPUTE_FEATURE_INCOMPATIBLE_MSFT";
        case -1000097005: return "XR_ERROR_SCENE_COMPUTE_CONSISTENCY_MISMATCH_MSFT";
        case -1000101000: return "XR_ERROR_DISPLAY_REFRESH_RATE_UNSUPPORTED_FB";
        case -1000108000: return "XR_ERROR_COLOR_SPACE_UNSUPPORTED_FB";
        case -1000142001: return "XR_ERROR_SPATIAL_ANCHOR_NAME_NOT_FOUND_MSFT";
        case -1000142002: return "XR_ERROR_SPATIAL_ANCHOR_NAME_INVALID_MSFT";
        default: return "XR_SUCCESS";
    }
    return "";
}