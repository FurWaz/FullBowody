############################################################
#                                                          #
#        Python script for body position estimation        #
#        This script uses google's mediapipe pose          #
#        solution to estimate the body position of         #
#        the given frame.                                  #
#                                                          #
############################################################

import cv2, sys,time
import numpy as np
import mediapipe as mp
mpPose = mp.solutions.pose
pose = mpPose.Pose()

time.sleep(1)
sys.stdout.write("READY")
sys.stdout.flush()

while True:
    word = sys.stdin.buffer.readline()
    if (str(word) == "STOP\n"):
        break
    size = int(word)
    data = sys.stdin.buffer.read(size)
    img = cv2.imdecode(np.frombuffer(data, dtype=np.uint8), cv2.IMREAD_COLOR)
    result = pose.process(img)
    if (result.pose_landmarks):
        string = ""
        for ld in result.pose_landmarks.landmark:
            try:
                string += str(round(ld.x, 5)).split(".")[1]
                string += "|"
                string += str(round(ld.y, 5)).split(".")[1]
                string += "|"
                string += str(round(ld.visibility, 5)).split(".")[1]
                string += "\n"
            except: pass
        sys.stdout.write(string)
        sys.stdout.flush()
quit()