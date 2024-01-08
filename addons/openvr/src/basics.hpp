#pragma once
#include <string.h>
#include <math.h>
#include <openvr/openvr_driver.h>

#define println(x) vr::VRDriverLog()->Log(x);
#define loop(x) for(int i = 0; i < x; i++)
float deg2rad = 3.1415926f / 180.f;
float rad2deg = 180.f / 3.1415926f;

typedef struct Euler {
    double x, y, z;
    Euler() {this->x = 0; this->y = 0; this->z = 0;}
    Euler(float x, float y, float z) {this->x = x; this->y = y; this->z = z;}
} Euler;

Euler quaternion2euler(vr::HmdQuaternion_t q) {
    Euler angles;

    // roll (x-axis rotation)
    double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
    double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
    angles.x = std::atan2(sinr_cosp, cosr_cosp);

    // pitch (y-axis rotation)
        // double sinp = 2 * (q.w * q.y - q.z * q.x);
        // if (std::abs(sinp) >= 1)
        //     angles.y = std::copysign(M_PI / 2, sinp); // use 90 degrees if out of range
        // else
        //     angles.y = std::asin(sinp);
    double t = 2 * (q.w * q.y - q.z * q.x);
    angles.y = std::asin(t);

    // yaw (z-axis rotation)
    double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
    double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
    angles.z = std::atan2(siny_cosp, cosy_cosp);

    return angles;
}

vr::HmdQuaternion_t euler2quaternion(Euler angle) // yaw (Z), pitch (Y), roll (X)
{
    // Abbreviations for the various angular functions
    double cy = cos(angle.z * 0.5);
    double sy = sin(angle.z * 0.5);
    double cp = cos(angle.y * 0.5);
    double sp = sin(angle.y * 0.5);
    double cr = cos(angle.x * 0.5);
    double sr = sin(angle.x * 0.5);

    vr::HmdQuaternion_t q;
    q.w = cr * cp * cy + sr * sp * sy;
    q.x = sr * cp * cy - cr * sp * sy;
    q.y = cr * sp * cy + sr * cp * sy;
    q.z = cr * cp * sy - sr * sp * cy;

    return q;
}

inline vr::HmdQuaternion_t HmdQuaternion_Init( double w, double x, double y, double z )
{
    vr::HmdQuaternion_t quat;
    quat.w = w;
    quat.x = x;
    quat.y = y;
    quat.z = z;
    return quat;
}

void HmdMatrix_SetIdentity(vr::HmdMatrix34_t *pMatrix)
{
    pMatrix->m[0][0] = 1.f;
    pMatrix->m[0][1] = 0.f;
    pMatrix->m[0][2] = 0.f;
    pMatrix->m[0][3] = 0.f;
    pMatrix->m[1][0] = 0.f;
    pMatrix->m[1][1] = 1.f;
    pMatrix->m[1][2] = 0.f;
    pMatrix->m[1][3] = 0.f;
    pMatrix->m[2][0] = 0.f;
    pMatrix->m[2][1] = 0.f;
    pMatrix->m[2][2] = 1.f;
    pMatrix->m[2][3] = 0.f;
}

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

namespace TRACKER
{
    const unsigned char HIPS = 1;
    const unsigned char FEET_L = 2;
    const unsigned char FEET_R = 3;
}