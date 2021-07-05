from numpy import true_divide
from modules.camera import Camera
from modules.constants import BODYJOINT
import cv2, time
import pygame

screen = pygame.display.set_mode([720, 1280])

cam = Camera()
cam.debugMode = True
cam.openAdress("http://192.168.0.50:8080/video")
cam.startTracker()
cam.startCapture()

run = True
while run:
    for e in pygame.event.get():
        if e.type == pygame.QUIT:
            run = False
    frame = cam.getFrame()
    img = pygame.image.frombuffer(frame.tobytes(), frame.shape[1::-1], "RGB")
    screen.blit(img, [0, 0])
    pygame.display.flip()
    time.sleep(0.01)

cam.stopTracker()
cam.stopCapture()
pygame.quit()