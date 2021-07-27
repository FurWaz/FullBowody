import sys
import numpy as np
import cv2

while True:
    size = int(sys.stdin.buffer.readline())
    data = sys.stdin.buffer.read(size)
    d = np.fromstring(data, dtype=np.uint8)
    img = cv2.imdecode(d, cv2.IMREAD_COLOR)
    cv2.imshow("Preview", img)
    cv2.waitKey(1)