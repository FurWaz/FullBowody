import cv2
import mediapipe
from .utils import *
from .tracker import Tracker
from cv2 import aruco
from typing import List
from threading import Thread
import numpy as np

mpDraw = mediapipe.solutions.drawing_utils
aruco_dict = aruco.Dictionary_get(aruco.DICT_4X4_250)
aruco_param =  aruco.DetectorParameters_create()

tracker = Tracker()

class Camera:
    def __init__(self) -> None:
        self.cap:any = None
        self.frame:any = np.zeros([720, 1280, 3], dtype=np.uint8)
        self.gray:any = np.zeros([720, 1280, 1], dtype=np.uint8)
        self.debugMode:bool = False

        self.shouldCapture:bool = False
        self.captureThread = Thread(target=self._readFrame)
        self.trackerID = tracker.addTracker()

        self.resolution = Dimension()
        self.marker_corners = []
        
    def _readFrame(self) -> None:
        while self.shouldCapture:
            ret, frame = self.cap.read()
            self.frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
            self.gray = cv2.cvtColor(frame, cv2.COLOR_RGB2GRAY)
            tracker.setFrame(self.trackerID, self.frame)
            self.getAruco()

    def openDevice(self, deviceID) -> bool:
        self.cap = cv2.VideoCapture(deviceID)
        self.resolution = Dimension( self.cap.get(cv2.CAP_PROP_FRAME_WIDTH), self.cap.get(cv2.CAP_PROP_FRAME_HEIGHT) )
        return self.cap.isOpened()
    
    def openAdress(self, adress) -> bool:
        return self.openDevice(adress)

    def startCapture(self):
        self.shouldCapture = True
        self.captureThread.start()

    def stopCapture(self):
        self.shouldCapture = False
        self.captureThread.join()

    def getFrame(self) -> np.ndarray:
        pos = tracker.getTracking(self.trackerID)
        if (self.debugMode and pos != None):
            black = self.frame.copy() #np.zeros(self.frame.shape, dtype=np.uint8)
            mpDraw.draw_landmarks(black, pos.pose_landmarks, tracker.POSE_CONNECTIONS)
            for c in self.marker_corners:
                cv2.ellipse(black, np.array(c, dtype=np.uint32), (10, 10), 0, 0, 360, (255, 0, 0), -1)
            return black
        return self.frame

    def getBodyPositions(self) -> List[Joint]:
        return self.bodyPosition.pose_landmarks.landmark

    def getAruco(self):
        corners, ids, rejectedImgPoints = aruco.detectMarkers(self.gray, aruco_dict, parameters=aruco_param)
        if (len(corners) >= 3):
            self.marker_corners = []
            for c in corners:
                p1 = c[0][0]
                p2 = c[0][1]
                p3 = c[0][2]
                p4 = c[0][3]
                self.marker_corners.append([ (p1[0]+p2[0]+p3[0]+p4[0])/4, (p1[1]+p2[1]+p3[1]+p4[1])/4 ])

    def __str__(self):
        return "Camera(\n    resolution: "+self.resolution+"\n)"

    def startTracker(self):
        tracker.startTracking()
        
    def stopTracker(self):
        tracker.stopTracking()