import math

def get_camera_matrix(fovx, fovy, height, width):
## FOVX is the horizontal FOV angle of the camera
## FOVY is the vertical FOV angle of the camera
    x = width / 2
    y = height / 2
    fx = x / math.tan(fovx)
    fy = y / math.tan(fovy)
    f = open("out", "w")
    f.write(str(fx)+"\n")
    f.write(str(0)+"\n")
    f.write(str(x)+"\n")
    f.write(str(0)+"\n")
    f.write(str(fy)+"\n")
    f.write(str(y)+"\n")
    f.write(str(0)+"\n")
    f.write(str(0)+"\n")
    f.write(str(1)+"\n")
    f.close()

get_camera_matrix(60, 105, 1280, 720)