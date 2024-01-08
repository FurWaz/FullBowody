#include <stdlib.h>
#include "vrDriver.hpp"

class VRDriver;
extern "C" __declspec(dllexport)
void* HmdDriverFactory(const char* interfaceName, int* return_code)
{
	if (strcmp(interfaceName, vr::IServerTrackedDeviceProvider_Version) == 0)
		return (vr::IServerTrackedDeviceProvider*) new VRDriver();
		
	if (return_code)
		*return_code = vr::VRInitError_Init_InterfaceNotFound;
	return NULL;
}