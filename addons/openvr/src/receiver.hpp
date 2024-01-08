#pragma once
#include <thread>
#include <string>
#include <WinSock2.h>
#include "./basics.hpp"

typedef struct Point3f
{
    float x, y, z;
    Point3f(float x, float y, float z): x(x), y(y), z(z) {}
    Point3f(): x(0), y(0), z(0) {}
} Point3f;

Point3f bodyPos[CONSTANT::NB_JOINTS];
float HeadRot, HMDRot;
Point3f HeadPos, HMDPos;

class Receiver
{
private:
    std::thread receiver;
    bool running;
    WSAData wsa;
    sockaddr_in server_addr;
    int sock;
    bool ready;

    const unsigned short SERVER_PORT = 5621;
    const std::string SERVER_IP = "127.0.0.1";
    const std::string LOGIN_MSG = "[ON]FullBowody-VRPlugin";
    const std::string LOGOUT_MSG = "[OFF]FullBowody-VRPlugin";

    void calculateVRPositions()
    {
        // Set hand position relative to head orientation and position
        HeadRot = atan2(
            (bodyPos[CONSTANT::JOINT_NOSE].x-bodyPos[CONSTANT::JOINT_HEAD].x),
            (bodyPos[CONSTANT::JOINT_NOSE].z-bodyPos[CONSTANT::JOINT_HEAD].z)
        );
        HeadPos = bodyPos[CONSTANT::JOINT_HEAD];

        uint32_t nbDevices = 1;
        vr::TrackedDevicePose_t devicesInfo[vr::k_unMaxTrackedDeviceCount];
        vr::VRServerDriverHost()->GetRawTrackedDevicePoses(0, devicesInfo, nbDevices);
        vr::DriverPose_t DPose = matrix2pose(devicesInfo[0].mDeviceToAbsoluteTracking);
        HMDRot = atan2(
            DPose.qRotation.x,
            DPose.qRotation.z
        ) * -2;
        HMDPos = Point3f(DPose.vecPosition[0], DPose.vecPosition[1], DPose.vecPosition[2]);

        float angleShift = HMDRot - HeadRot;

        for (int i = CONSTANT::JOINT_NECK; i < CONSTANT::NB_JOINTS; i++)
        {
            Point3f* p = &bodyPos[i];
            // make position relative to head
            p->y = (p->y-HeadPos.y) + HMDPos.y;
            p->x -= HeadPos.x;
            p->z -= HeadPos.z;

            // convert from app coordinates to vr world coordinates
            float newX = -cos(angleShift) * p->x - sin(angleShift) * p->z;
            float newZ = cos(angleShift) * p->z - sin(angleShift) * p->x;
            p->x = newX + HMDPos.x;
            p->z = newZ + HMDPos.z;
        }
    }

    vr::DriverPose_t matrix2pose(vr::HmdMatrix34_t matrix)
    {
        vr::DriverPose_t res;
        res.deviceIsConnected = true;
        res.poseIsValid = true;
        res.vecPosition[0] = matrix.m[0][3];
        res.vecPosition[1] = matrix.m[1][3];
        res.vecPosition[2] = matrix.m[2][3];
        res.qRotation.w = sqrt(fmax(0, 1 + matrix.m[0][0] + matrix.m[1][1]+ matrix.m[2][2])) / 2;
        res.qRotation.x = sqrt(fmax(0, 1 + matrix.m[0][0] - matrix.m[1][1] - matrix.m[2][2])) / 2;
        res.qRotation.y = sqrt(fmax(0, 1 - matrix.m[0][0] + matrix.m[1][1] - matrix.m[2][2])) / 2;
        res.qRotation.z = sqrt(fmax(0, 1 - matrix.m[0][0] - matrix.m[1][1] + matrix.m[2][2])) / 2;
        res.qRotation.x = copysign(res.qRotation.x, matrix.m[2][1] - matrix.m[1][2]);
        res.qRotation.y = copysign(res.qRotation.y, matrix.m[0][2] - matrix.m[2][0]);
        res.qRotation.z = copysign(res.qRotation.z, matrix.m[1][0] - matrix.m[0][1]);
        return res;
    }

public:
    Receiver()
    {
        for(int i = 0; i < CONSTANT::NB_JOINTS; i++)
            bodyPos[i] = Point3f();
        this->Init();
    }

    void Init()
    {
        //init wsa
        this->ready = true;
        if (WSAStartup(MAKEWORD(2, 2), &this->wsa) != 0)
            this->ready = false;
        if (!this->ready) return;
        // create socket
        this->sock = socket(AF_INET, SOCK_DGRAM, 0);
        if ( this->sock == SOCKET_ERROR )
            this->ready = false;
        u_long state = 1;
        ioctlsocket(this->sock, FIONBIO, &state); // non blocking socket
    }

    void Start()
    {
        if (!this->ready || this->running) return;

        // address setup
        memset((char*) &this->server_addr, 0, sizeof(this->server_addr));
        this->server_addr.sin_family = AF_INET;
        this->server_addr.sin_port = htons(this->SERVER_PORT);
        this->server_addr.sin_addr.S_un.S_addr = inet_addr(this->SERVER_IP.c_str());

        // send login message
        sendto(this->sock, this->LOGIN_MSG.c_str(), this->LOGIN_MSG.size(), 0, (sockaddr*) &this->server_addr, sizeof(this->server_addr));
        this->running = true;
        this->receiver = std::thread(&Receiver::_check_for_data, this);
    }

    void Stop()
    {
        if (this->running)
        {
            this->running = false;
            this->receiver.join();
            sendto(this->sock, this->LOGOUT_MSG.c_str(), this->LOGOUT_MSG.size(), 0, (sockaddr*) &this->server_addr, sizeof(this->server_addr));
        }
    }

    void _check_for_data()
    {
        while (this->running)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
            char buf[1024];
            int servlen = sizeof(this->server_addr);
            if (recvfrom(this->sock, buf, sizeof(buf), 0, (sockaddr*) &this->server_addr, &servlen) == SOCKET_ERROR)
                continue;
            std::string data(buf);
            unsigned short cursor = 0;
            unsigned short index = 0;
            // get the raw body points from the string
            while (cursor < data.size() && index < CONSTANT::NB_JOINTS)
            {
                try 
                {
                    // get x coordonnate
                    std::string nbrX = "";
                    while (data[cursor] != '|')
                        nbrX += data[cursor++];
                    cursor++;

                    // get y coordonnate
                    std::string nbrY = "";
                    while (data[cursor] != '|')
                        nbrY += data[cursor++];
                    cursor++;

                    // get z coordonnate
                    std::string nbrZ = "";
                    while (data[cursor] != '\n')
                        nbrZ += data[cursor++];
                    cursor++;

                    // add coordonnates to vector
                    float coordX = std::stof(nbrX.c_str());
                    float coordZ = std::stof(nbrZ.c_str());
                    float coordY = std::stof(nbrY.c_str());
                    bodyPos[index] = Point3f(coordX, coordZ, coordY);
                } catch (std::exception &e) {break;}
                index++;
            }
            this->calculateVRPositions();
        }
    }

    ~Receiver()
    {
        this->Stop();
        closesocket(this->sock);
        WSACleanup();
    }
};