import mediapipe as mp
import numpy as np
mpPose = mp.solutions.pose
pose = mpPose.Pose()

import cv2, queue, threading, time, sys

# bufferless VideoCapture
class VideoCapture:

  def __init__(self, name):
    self.cap = cv2.VideoCapture(name)
    self.q = np.zeros((1280, 720, 3), np.uint8)
    t = threading.Thread(target=self._reader)
    t.daemon = True
    t.start()

  # read frames as soon as they are available, keeping only most recent one
  def _reader(self):
    while True:
      ret, frame = self.cap.read()
      if not ret: break
      self.q = frame

  def read(self):
    return self.q

cap = VideoCapture(sys.argv[1])
while True:
    frame = cap.read()
    result = pose.process(cv2.cvtColor(frame, cv2.COLOR_BGR2RGB))
    if (result.pose_landmarks):
        try:
            string = ""
            for ld in result.pose_landmarks.landmark:
                string += str(round(ld.x, 5)).split(".")[1]
                string += "|"
                string += str(round(ld.y, 5)).split(".")[1]
                string += "\n"
            f = open("./result.txt", "w")
            f.write(string)
            f.close()
        except: pass
    cv2.imshow("frame", frame)
    if (cv2.waitKey(1) == 27):
        break