import numpy as np
import cv2

# here insert the bgr values which you want to convert to hsv
pink = np.uint8([[[255, 16, 240]]])
hsvpink = cv2.cvtColor(pink, cv2.COLOR_BGR2HSV)
print(hsvpink)

lowerLimit = hsvpink[0][0][0] - 10, 100, 100
upperLimit = hsvpink[0][0][0] + 10, 255, 255

print(upperLimit)
print(lowerLimit)
