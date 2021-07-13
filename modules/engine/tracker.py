import socket, cv2, sys
import numpy as np
import mediapipe as mp
from threading import Thread
mpPose = mp.solutions.pose
pose = mpPose.Pose()

port = int(sys.argv[1])

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(("localhost", port))
link, addr = sock.recvfrom(100)

run = True

class VideoCapture:

    def __init__(self, name):
        self.cap = cv2.VideoCapture(name)
        self.q = np.zeros((1280, 720, 3), np.uint8)
        self.t = Thread(target=self._reader)
        self.t.daemon = True
        self.t.start()

    def _reader(self):
        while run:
            ret, frame = self.cap.read()
            if not ret: break
            self.q = frame

    def read(self):
        return self.q

    def release(self):
        self.t.join()

cap = VideoCapture(link.decode('utf-8'))

while run:
    frame = cap.read()
    result = pose.process(cv2.cvtColor(frame, cv2.COLOR_BGR2RGB))
    if (result.pose_landmarks):
        try:
            string = ""
            index = 0
            for ld in result.pose_landmarks.landmark:
                string += str(round(ld.id, 5))
                string += "|"
                string += str(round(ld.x, 5))
                string += "|"
                string += str(round(ld.y, 5))
                string += "\n"
                index += 1
            print(index)
            sock.sendto(string.encode("utf-8"), ("localhost", addr[1]))
        except: pass

cap.release()