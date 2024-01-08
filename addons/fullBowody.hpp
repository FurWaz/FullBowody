#pragma once

namespace CONSTANT
{
    /** @brief Number of body joints in the app*/
    const unsigned char NB_JOINTS = 19;
    // body joints
    const unsigned char JOINT_HEAD = 0;
    const unsigned char JOINT_NOSE = 1;
    const unsigned char JOINT_NECK = 2;
    const unsigned char JOINT_SHOULDER_R = 3;
    const unsigned char JOINT_ELBOW_R = 4;
    const unsigned char JOINT_WRIST_R = 5;
    const unsigned char JOINT_HAND_R = 6;
    const unsigned char JOINT_HIP_R = 7;
    const unsigned char JOINT_KNEE_R = 8;
    const unsigned char JOINT_ANKLE_R = 9;
    const unsigned char JOINT_FEET_R = 10;
    const unsigned char JOINT_SHOULDER_L = 11;
    const unsigned char JOINT_ELBOW_L = 12;
    const unsigned char JOINT_WRIST_L = 13;
    const unsigned char JOINT_HAND_L = 14;
    const unsigned char JOINT_HIP_L = 15;
    const unsigned char JOINT_KNEE_L = 16;
    const unsigned char JOINT_ANKLE_L = 17;
    const unsigned char JOINT_FEET_L = 18;
    
    /** @brief Number of connections between the body joints */
    const unsigned char NB_CONNECTIONS = 18;
    /** @brief Connections between each joint of the body for a wired representation */
    const unsigned char POSE_CONNECTIONS[NB_CONNECTIONS][2] = {
        {JOINT_HEAD, JOINT_NECK},
        {JOINT_HEAD, JOINT_NOSE},
        {JOINT_NECK, JOINT_SHOULDER_R},
        {JOINT_NECK, JOINT_SHOULDER_L},
        {JOINT_SHOULDER_R, JOINT_ELBOW_R},
        {JOINT_SHOULDER_L, JOINT_ELBOW_L},
        {JOINT_ELBOW_R, JOINT_WRIST_R},
        {JOINT_ELBOW_L, JOINT_WRIST_L},
        {JOINT_WRIST_R, JOINT_HAND_R},
        {JOINT_WRIST_L, JOINT_HAND_L},
        {JOINT_NECK, JOINT_HIP_R},
        {JOINT_NECK, JOINT_HIP_L},
        {JOINT_HIP_R, JOINT_KNEE_R},
        {JOINT_HIP_L, JOINT_KNEE_L},
        {JOINT_KNEE_R, JOINT_ANKLE_R},
        {JOINT_KNEE_L, JOINT_ANKLE_L},
        {JOINT_ANKLE_R, JOINT_FEET_R},
        {JOINT_ANKLE_L, JOINT_FEET_L}
    };
}

typedef struct Point3f
{
    float x, y, z;
    Point3f(float x, float y, float z): x(x), y(y), z(z) {}
    Point3f(): x(0), y(0), z(0) {}
} Point3f;
Point3f bodyPos[CONSTANT::NB_JOINTS];

#ifdef _WIN32
    #include <thread>
    #include <string>
    #include <WinSock2.h>
    #include "./basics.hpp"

    class FBConnection
    {
    private:
        std::thread receiver;
        bool running;
        WSAData wsa;
        sockaddr_in server_addr;
        int sock;
        bool ready;

        unsigned short SERVER_PORT = 5621;
        std::string SERVER_IP = "127.0.0.1";
        std::string LOGIN_MSG = "[ON]FullBowody-Plugin";
        std::string LOGOUT_MSG = "[OFF]FullBowody-Plugin";

    public:
        FBConnection()
        {
            for(int i = 0; i < CONSTANT::NB_JOINTS; i++)
                bodyPos[i] = Point3f();
            this->Init();
        }

        FBConnection(std::string extensionName, std::string serverIP = "127.0.0.1", unsigned short serverPORT = 5621)
        {
            for(int i = 0; i < CONSTANT::NB_JOINTS; i++)
                bodyPos[i] = Point3f();
            this->LOGIN_MSG = "[ON]FullBowody-"+extensionName;
            this->LOGOUT_MSG = "[OFF]FullBowody-"+extensionName;
            this->SERVER_IP = serverIP;
            this->SERVER_PORT = serverPORT;
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
            }
        }

        ~FBConnection()
        {
            this->Stop();
            closesocket(this->sock);
            WSACleanup();
        }
    };
#elif __linux__
    
#endif