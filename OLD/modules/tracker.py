import mediapipe, time
import numpy as np
from threading import Thread
mpPose = mediapipe.solutions.pose

class Tracker:
    POSE_CONNECTIONS = mpPose.POSE_CONNECTIONS
    def __init__(self):
        self.trackers = []
        self.results = []
        self.frames = []
        self.nb_trackers = 0

        self.trackingThread = Thread(target=self._track)
        self.shouldTrack = False

    def startTracking(self):
        self.shouldTrack = True
        self.trackingThread.start()
    
    def stopTracking(self):
        self.shouldTrack = False
        self.trackingThread.join()

    def _track(self):
        while self.shouldTrack:
            self.update()
    
    def update(self):
        for i in range(self.nb_trackers):
            self.results[i] = self.trackers[i].process(self.frames[i])
        time.sleep(0.01)

    def setFrame(self, trackerID, frame):
        self.frames[trackerID] = frame

    def getTracking(self, trackerID):
        return self.results[trackerID]

    def addTracker(self, static_image_mode=False, model_complexity=1, smooth_landmarks=True, min_detection_confidence=0.5, min_tracking_confidence=0.5):
        self.trackers.append(mpPose.Pose(static_image_mode, model_complexity, smooth_landmarks, min_detection_confidence, min_tracking_confidence))
        self.results.append(None)
        self.frames.append(np.zeros([720, 1280, 3], dtype=np.uint8))
        self.nb_trackers += 1
        return self.nb_trackers-1