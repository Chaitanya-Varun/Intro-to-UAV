# import the necessary packages
from collections import deque
from imutils.video import VideoStream
import numpy as np
import argparse
import cv2
import imutils
import time
import os
# construct the argument parse and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-v", "--video",
                help="path to the (optional) video file")
ap.add_argument("-b", "--buffer", type=int, default=10000,
                help="max buffer size")
args = vars(ap.parse_args())

# define the lower and upper boundaries of the "green"
# ball in the HSV color space, then initialize the
# list of tracked points
greenLower = (29, 86, 6)
greenUpper = (64, 255, 255)
pts = deque(maxlen=args["buffer"])
# if a video path was not supplied, grab the reference
# to the webcam
if not args.get("video", False):
    vs = VideoStream(src=0).start()
# otherwise, grab a reference to the video file
else:
    vs = cv2.VideoCapture(args["video"])
    # width = int(vs.get(cv2.CAP_PROP_FRAME_WIDTH))
    # height = int(vs.get(cv2.CAP_PROP_FRAME_HEIGHT))
    # # print(width, height)
    # fps = round(vs.get(cv2.CAP_PROP_FPS))
    fourcc = cv2.VideoWriter_fourcc(*'MP4V')
    # # fourcc = cv2.VideoWriter_fourcc(*'MPEG')
    # # out = cv2.VideoWriter('output.avi', fourcc, fps, (width, height))
    # # video = cv2.VideoWriter('pr_video.mp4', -1, 1, (width, height))
    # frame_size = (width, height)

    # # out = cv2.VideoWriter('output_video_from_file.mp4',
    # #                       cv2.VideoWriter_fourcc('M', 'J', 'P', 'G'), fps, frame_size)

    # out = cv2.VideoWriter('output_video_from_file.mp4',
    #                       fourcc, fps, frame_size)
    writer = None
# allow the camera or video file to warm up
time.sleep(2.0)

# keep looping
while True:
    # grab the current frame
    frame = vs.read()
    # handle the frame from VideoCapture or VideoStream
    frame = frame[1] if args.get("video", False) else frame
    # if we are viewing a video and we did not grab a frame,
    # then we have reached the end of the video
    if frame is None:
        break
    # resize the frame, blur it, and convert it to the HSV
    # color space
    frame = imutils.resize(frame, width=600)
    blurred = cv2.GaussianBlur(frame, (11, 11), 0)
    hsv = cv2.cvtColor(blurred, cv2.COLOR_BGR2HSV)
    # construct a mask for the color "green", then perform
    # a series of dilations and erosions to remove any small
    # blobs left in the mask
    mask = cv2.inRange(hsv, greenLower, greenUpper)
    mask = cv2.erode(mask, None, iterations=2)
    mask = cv2.dilate(mask, None, iterations=2)
    # find contours in the mask and initialize the current
    # (x, y) center of the ball
    cnts = cv2.findContours(
        mask.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    cnts = imutils.grab_contours(cnts)
    center = None
    # only proceed if at least one contour was found
    if len(cnts) > 0:
        print("Contours detected!!")
        # find the largest contour in the mask, then use
        # it to compute the minimum enclosing rectangle and
        # centroid
        c = max(cnts, key=cv2.contourArea)
        rect = cv2.minAreaRect(c)
        box = cv2.boxPoints(rect)
        box = np.int0(box)
        x1 = min(box[:, 0])  # top-left pt. is the leftmost of the 4 points
        # bottom-right pt. is the rightmost of the 4 points
        x2 = max(box[:, 0])
        y1 = min(box[:, 1])  # top-left pt. is the uppermost of the 4 points
        # bottom-right pt. is the lowermost of the 4 points
        y2 = max(box[:, 1])
        cv2.rectangle(frame, (x1, y1), (x2, y2), (255, 0, 0), 3)
        center = (int((x1+x2)/2), int((y1+y2)/2))
        print(center)
        cv2.circle(frame, center, 5, (0, 0, 255), -1)

    # update the points queue
    pts.appendleft(center)
    # loop over the set of tracked points
    for i in range(1, len(pts)):
        # if either of the tracked points are None, ignore
        # them
        if pts[i - 1] is None or pts[i] is None:
            continue
        # otherwise, compute the thickness of the line and
        # draw the connecting lines
        thickness = int(np.sqrt(args["buffer"] / float(i + 1)) * 2.5)
        cv2.line(frame, pts[i - 1], pts[i], (0, 0, 255), 4)  # thickness)
    # show the frame to our screen
    if writer is None:
        (h, w) = frame.shape[:2]
        name = "FinalEffort.mp4"
        fps = vs.get(cv2.CAP_PROP_FPS)
        writer = cv2.VideoWriter(name, fourcc, fps, (w, h), True)
        # zeros = np.zeros((h, w), dtype="uint8")
    writer.write(frame)
    # out.write(frame)
    # print(frame.size)
    cv2.imshow("Frame", frame)

    key = cv2.waitKey(1) & 0xFF
    # if the 'q' key is pressed, stop the loop
    # if key == ord("q"):
    timestr = time.strftime("%Y%m%d-%H%M%S")
    cv2.imwrite(os.getcwd()+'/FinalFrames/'+timestr+'.png', frame)
    # break
# if we are not using a video file, stop the camera video stream
if not args.get("video", False):
    vs.stop()
# otherwise, release the camera
else:
    vs.release()

# close all windows
writer.release()
cv2.destroyAllWindows()
