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
    NB_JOINTS = 33
    def __init__(self, name:str="PythonAddon", port:int=5621, address:str="localhost") -> None:
        self.__LOGIN_MSG:bytes = ("FullBowody-"+name).encode("utf-8")
        self.__sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.__sock.setblocking(False)
        self.__thread = Thread(target=self.__retrieve_positions)
        self.__running = False
        self.__port = port
        self.__addr = address
        self.bodyPosition:list[BodyJoint] = None
        self.__callback = None
    
    def start(self) -> bool:
        try:
            self.__running = True
            r = self.__sock.sendto(self.__LOGIN_MSG, (self.__addr, self.__port))
            self.__thread.start()
            return True
        except:
            self.__running = False
            return False

    def stop(self) -> bool:
        if not self.__running:
            return False
        self.__running = False
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
                self.bodyPosition = points
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