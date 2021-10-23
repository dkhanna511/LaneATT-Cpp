################################################
# Projecting Lane Markers in LIDAR point cloud #
################################################


#################################################################################################################
## STEPS INVOLVED :- 1.) Project LIDAR point cloud in camera frame                                              #
##                   2.) Convert points in camera frame to pixels                                               #
##                   3.) Remove points whose corresponding pixels are outside the image size                    # 
##                   4.) Get pixels which lie on the lane masks and filter the corresponding 3d points          #
##                   5.) Reproject the filtered 3D points to LIDAR frame using extrinsic calibration parameters #
#################################################################################################################

import cv2
import numpy as np

import matplotlib.pyplot as plt
import open3d as o3d
import math
# import open3d as o3d
# from scipy import spatial
# import bezier
########################
# Check for Python-Pcl #
########################
try:
    import pcl
    import pcl.pcl_visualization
except Exception as e:
    print("Error in import python-pcl: {}".format(e))

    
import readCalib
import fitPlane
import camLidarHomography as CLH

#*********************************** #
# Project Lane Marker in PointCloud  #
#*********************************** #
def getLanePoints(lidarPts, laneCoordsImage, R, K, T):

    ##############################################
    LANEPOINTS = CLH.getPtsOnLidar(lidarPts, laneCoordsImage, R, K, T)
    

    return LANEPOINTS
    
def laneMarkings3d(laneCoordsImage, inputPtCloud, cameraInfo):

    point_cloud = pcl.PointCloud(inputPtCloud.astype('float32'))
    
    # now read the mask
    roadPlane, coeffs_   = fitPlane.fitPlane(point_cloud, 0.08)
    roadPlane = np.array(roadPlane).reshape(-1,3)
    
    # now read the calibration file
    # calibName = maskName.replace(".png", ".txt")
    pathCalib_extrinsic = "/home/dheeraj/Lane_Mapping/kitti_calib/Calib/um_000000.txt"
    T,K,R = readCalib.getMatrices(cameraInfo, pathCalib_extrinsic)
    
    lanePoints = getLanePoints(roadPlane, laneCoordsImage, R, K, T)
    # print("here")
    return lanePoints


if __name__=="__main__":
    print("--------- Project lane markings in PointCloud -------------")
    
    pathImg = "laneMasks_5/MaskedImages"
    pathPcd = "laneMasks_5/PointCloud"
    pathCalib_intrinsic = "laneMasks_5/Calib/intrinsic_params.npy"
    pathCalib_translation = "laneMasks_5/Calib/translation_LTC.npy"
    pathCalib_rotation = "laneMasks_5/Calib/rotation_LTC.npy"
    pathImgsRGB = "laneMasks_5/RGB_Images"

    # laneMarkings3d(pathImg, pathPcd, pathCalib_intrinsic,pathCalib_translation,pathCalib_rotation, pathImgsRGB)
