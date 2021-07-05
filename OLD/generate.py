from cv2 import aruco
import cv2
from PIL import Image

aruco_dict = aruco.Dictionary_get(aruco.DICT_4X4_250)

dims = (2100, 2970)
padding = 100
size = int((dims[0]/2-padding*1.5))
im = Image.new('RGB', dims, (255, 255, 255))

im.paste(Image.fromarray(aruco.drawMarker(aruco_dict, 1, size)), [padding, padding])
im.paste(Image.fromarray(aruco.drawMarker(aruco_dict, 2, size)).rotate(90), [padding, dims[1]-size-padding])
im.paste(Image.fromarray(aruco.drawMarker(aruco_dict, 3, size)).rotate(180), [dims[0]-size-padding, dims[1]-size-padding])
im.paste(Image.fromarray(aruco.drawMarker(aruco_dict, 4, size)).rotate(270), [dims[0]-size-padding, padding])
im.save("./markers.png", 'PNG')