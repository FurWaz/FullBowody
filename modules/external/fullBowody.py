import socket
from threading import Thread

class FullBowodyConnection:
    def __init__(self) -> None:
        self.__LOGIN_MSG = b"FullBowody-BlenderAddon"
        self.__sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.__thread = Thread(target=self.__retrieve_positions)
        self.__running = False
        self.bodyPosition = None
        self.callback = None
    
    def start(self) -> bool:
        try:
            self.__running = True
            self.__sock.sendto(self.__LOGIN_MSG, ("localhost", 5621))
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

    def setCallback(self, callback):
        self.callback = callback
    
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
                if (self.callback != None):
                    self.callback()
            except: self.__running = False


####################
#                  #
#    HOW TO USE    #
#                  #
####################

def connectionDemo():
    #first, create the connection
    connection = FullBowodyConnection()

    # [OPTIONAL] You can define a callback function that will be called when a new body position is recieved
    def onNewPosition():
        print("New body position !")
    connection.setCallback(onNewPosition)

    #next, start the connection
    connection.start()

    #now, you can get the current body position at anytime
    print("Current body position: ", connection.bodyPosition)

    #at the end of the program, don't forget to stop the connection
    connection.stop()