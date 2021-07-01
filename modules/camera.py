import cv2
import mediapipe
from .utils import *
from .tracker import Tracker
from typing import List
from threading import Thread
import numpy as np
import time

mpDraw = mediapipe.solutions.drawing_utils

tracker = Tracker()

class Camera:
    def __init__(self) -> None:
        self.cap:any = None
        self.frame:any = np.zeros([720, 1280, 3], dtype=np.uint8)
        self.debugMode:bool = False

        self.shouldCapture:bool = False
        self.captureThread = Thread(target=self._readFrame)
        self.trackerID = tracker.addTracker()

        self.resolution = Dimension()
        
    def _readFrame(self) -> None:
        while self.shouldCapture:
            ret, frame = self.cap.read()
            self.frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
            tracker.setFrame(self.trackerID, self.frame)

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
            black = np.zeros(self.frame.shape, dtype=np.uint8)
            mpDraw.draw_landmarks(self.frame, pos.pose_landmarks, tracker.POSE_CONNECTIONS)
        return self.frame

    def getBodyPositions(self) -> List[Joint]:
        return self.bodyPosition.pose_landmarks.landmark

    def __str__(self):
        return "Camera(\n    resolution: "+self.resolution+"\n)"

    def startTracker(self):
        tracker.startTracking()
        
    def stopTracker(self):
        tracker.stopTracking()