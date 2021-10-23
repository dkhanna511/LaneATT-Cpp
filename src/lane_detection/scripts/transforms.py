############################################################
## Transformations between camera and Lidar               ##
## LIDAR_pts -> [R,t] -> Camera_pts -> K -> Camera_pixels ##
############################################################

import numpy as np
import cv2

#********************************************************************* #
# Convert [nx3] points in LIDAR frame to [nx3] points in camera frame  #
# ******************************************************************** # 
def lidarToCam3D(lidarPts, R, T):
    lidarPtsH = np.ones((lidarPts.shape[0], 4)) 
    lidarPtsH[:,0:3] = lidarPts                   # Lidar Points in homogeneous coordinates ([X,Y,Z,1])
    ptsCam3D = np.dot(R, np.dot(T, lidarPtsH.T)).T
    return ptsCam3D.astype('float32')

#************************************************* #
# Convert 3D points in camera frame to Lidar frame #
# ************************************************ #
def camToLidar3D(camPts, R, T):
    # print("New Method")
    R_ = np.eye(4,4)
    T_ = np.eye(4,4)

    r = T[0:3,0:3]
    t = T[0:3,3]   
    #camPtsH = np.ones((camPts.shape[0], 4))
    #camPtsH[:,0:3] = camPts
    ptsLidar = np.dot(np.linalg.inv(r), camPts.T).T

    for i in range(ptsLidar.shape[0]):
        ptsLidar[i] += -np.dot(np.linalg.inv(r),t) 

    return ptsLidar[:,0:3].astype('float32')

# *************************************************** #
# Filter out points in camera frame that lie on lane  #
# *************************************************** #
def getPtsOnLane(camPts, mask, K, img):
    camPtsInFov = []
    camPtsLane = []
    camPts = camPts[camPts[:,2]>0]
    camPix = np.dot(K, camPts.T).T

    # convert to pixels and filter out points whose corresponding pixels are outside the image
    for i in range(camPts.shape[0]):

        [col, row, x] = np.dot(K, camPts[i])
        if 0 < int(col/x) < 1242 and 0 < int(row/x) < 375:
            camPtsInFov.append(camPts[i])
            
        
            if mask[int(row/x), int(col/x)] > 0:
                camPtsLane.append(camPts[i])
                cv2.circle(img, (int(col/x), int(row/x)), 2, (0,0,255), -1)


    camPtsInFov = np.array(camPtsInFov).reshape(-1,3).astype('float32')
    camPtsLane = np.array(camPtsLane).reshape(-1,3).astype('float32')

    return camPtsLane, camPtsInFov, img

