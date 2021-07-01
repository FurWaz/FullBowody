import threading
import cv2
import numpy as np
import pygame
from threading import Thread
import mediapipe as mp
pygame.init()

haar = cv2.CascadeClassifier("./haarcascades\haarcascade_frontalface_default.xml")
haar2 = cv2.CascadeClassifier("./haarcascades\haarcascade_profileface.xml")

mpDraw = mp.solutions.drawing_utils
mpPose = mp.solutions.pose
pose1 = mpPose.Pose()
pose2 = mpPose.Pose()

class Stream:
    def __init__(self, adress, pose) -> None:
        self.cap = cv2.VideoCapture(adress)
        self.img = np.zeros((720,1280,3), np.uint8)
        self.pose = pose

    def update(self, shift=0):
        imRGB = cv2.cvtColor(self.img, cv2.COLOR_BGR2RGB)
        result = self.pose.process(imRGB)
        black = np.zeros(self.img.shape, dtype=np.uint8)
        if (result.pose_landmarks):
            mpDraw.draw_landmarks(black, result.pose_landmarks, mpPose.POSE_CONNECTIONS)
        surf = pygame.image.frombuffer(black.tobytes(), black.shape[1::-1], "BGR")
        surf = pygame.transform.scale(surf, [360, 640])
        pygame.display.get_surface().blit(surf, [shift*360, 0])
    
    def getImage(self):
        ret, self.img = self.cap.read()

    def stop(self):
        self.cap.release()
        pass

s1 = Stream("http://192.168.0.14:8080/video", pose1)
s2 = Stream("http://192.168.0.50:8080/video", pose2)
screen = pygame.display.set_mode([720, 640])
run = True

def u2():
    while run: s2.getImage()
def u1():
    while run: s1.getImage()
t1 = Thread(target=u1)
t2 = Thread(target=u2)
t1.start()
t2.start()

while run:
    for e in pygame.event.get():
        if (e.type == pygame.QUIT):
            run = False
    s1.update(0)
    s2.update(1)
    pygame.display.flip()

t2.join()
t1.join()

s2.stop()
s1.stop()
pygame.quit()
cv2.destroyAllWindows()