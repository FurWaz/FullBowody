import socket
from threading import Thread
from typing import List

class BodyJoint:
    def __init__(self, x:float=0, y:float=0, z:float=0) -> None:
        self.x:float = x
        self.y:float = y
        self.z:float = z

    def str(self):
        return "("+str(self.x)+", "+str(self.y)+", "+str(self.z)+")"

class FBConnection:
    NB_JOINTS = 19
    JOINT_HEAD = 0
    JOINT_NOSE = 1
    JOINT_NECK = 2
    JOINT_SHOULDER_R = 3
    JOINT_ELBOW_R = 4
    JOINT_WRIST_R = 5
    JOINT_HAND_R = 6
    JOINT_HIP_R = 7
    JOINT_KNEE_R = 8
    JOINT_ANKLE_R = 9
    JOINT_FEET_R = 10
    JOINT_SHOULDER_L = 11
    JOINT_ELBOW_L = 12
    JOINT_WRIST_L = 13
    JOINT_HAND_L = 14
    JOINT_HIP_L = 15
    JOINT_KNEE_L = 16
    JOINT_ANKLE_L = 17
    JOINT_FEET_L = 18

    NB_CONNECTIONS = 18
    POSE_CONNECTIONS = [
        [JOINT_HEAD, JOINT_NOSE],
        [JOINT_HEAD, JOINT_NECK],
        [JOINT_NECK, JOINT_SHOULDER_R],
        [JOINT_NECK, JOINT_SHOULDER_L],
        [JOINT_SHOULDER_R, JOINT_ELBOW_R],
        [JOINT_SHOULDER_L, JOINT_ELBOW_L],
        [JOINT_ELBOW_R, JOINT_WRIST_R],
        [JOINT_ELBOW_L, JOINT_WRIST_L],
        [JOINT_WRIST_R, JOINT_HAND_R],
        [JOINT_WRIST_L, JOINT_HAND_L],
        [JOINT_NECK, JOINT_HIP_R],
        [JOINT_NECK, JOINT_HIP_L],
        [JOINT_HIP_R, JOINT_KNEE_R],
        [JOINT_HIP_L, JOINT_KNEE_L],
        [JOINT_KNEE_R, JOINT_ANKLE_R],
        [JOINT_KNEE_L, JOINT_ANKLE_L],
        [JOINT_ANKLE_R, JOINT_FEET_R],
        [JOINT_ANKLE_L, JOINT_FEET_L]
    ]

    def __init__(self, name:str="PythonAddon", port:int=5621, address:str="localhost") -> None:
        self.__LOGIN_MSG:bytes = ("[ON]FullBowody-"+name).encode("utf-8")
        self.__LOGOUT_MSG:bytes = ("[OFF]FullBowody-"+name).encode("utf-8")
        self.__sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.__sock.setblocking(False)
        self.__thread = Thread(target=self.__retrieve_positions)
        self.__running = False
        self.__port = port
        self.__addr = address
        self.__bodyPosition:list[BodyJoint] = None
        self.__callback = None

    def __del__(self):
        self.stop()
    
    def start(self) -> bool:
        if self.__running: return
        try:
            self.__running = True
            self.__sock.sendto(self.__LOGIN_MSG, (self.__addr, self.__port))
            self.__thread.start()
            return True
        except:
            self.__running = False
            return False

    def getBodyPosition(self) -> list[BodyJoint]:
        return self.__bodyPosition

    def stop(self) -> bool:
        if not self.__running:
            return False
        self.__running = False
        self.__sock.sendto(self.__LOGOUT_MSG, (self.__addr, self.__port))
        self.__thread.join()
        return True

    def setCallback(self, callback):
        self.__callback = callback
    
    def __retrieve_positions(self):
        while self.__running:
            try:
                data, addr = self.__sock.recvfrom(1024)
                points = data.decode().split("\n")
                points.pop(len(points)-1)
                for i in range(len(points)):
                    points[i] = points[i].split("|")
                    for e in range(len(points[i])):
                        points[i][e] = float(points[i][e])
                self.__bodyPosition = points
                if (self.__callback != None):
                    self.__callback()
            except BlockingIOError: pass
            except Exception: self.__running = False

####################
#                  #
#    HOW TO USE    #
#                  #
####################

def connectionDemo():
    #first, create the connection
    conn = FBConnection()

    # [OPTIONAL] You can define a callback function that will be called when a new body position is recieved
    def onNewPosition():
        print("New body position !")
    conn.setCallback(onNewPosition)

    #next, start the connection
    conn.start()

    #now, you can get the current body position at anytime
    print("Current body position: ", conn.bodyPosition)

    #at the end of the program, don't forget to stop the connection
    conn.stop()