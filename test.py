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
pose = mpPose.Pose()

class Stream:
    def __init__(self, adress, pose) -> None:
        self.cap = cv2.VideoCapture(adress)
        self.img = np.zeros((720,1280,3), np.uint8)
        self.pose = pose
        self.i = 0

    def update(self, shift=0):
        ret, self.img = self.cap.read()
        imRGB = cv2.cvtColor(self.img, cv2.COLOR_BGR2RGB)
        result = self.pose.process(imRGB)
        black = np.zeros(self.img.shape, dtype=np.uint8)
        if (result.pose_landmarks):
            mpDraw.draw_landmarks(black, result.pose_landmarks, mpPose.POSE_CONNECTIONS)
            if (self.i > 10):
                self.i = 0
                print(result.pose_landmarks.landmark[15])
        self.i += 1
        surf = pygame.image.frombuffer(black.tobytes(), black.shape[1::-1], "BGR")
        surf = pygame.transform.scale(surf, [360, 640])
        pygame.display.get_surface().blit(surf, [shift*360, 0])

    def stop(self):
        self.cap.release()
        pass

s1 = Stream("http://192.168.0.50:8080/video", pose)
screen = pygame.display.set_mode([360, 640])
run = True

while run:
    for e in pygame.event.get():
        if (e.type == pygame.QUIT):
            run = False
    s1.update(0)
    pygame.display.flip()

s1.stop()
pygame.quit()
cv2.destroyAllWindows()