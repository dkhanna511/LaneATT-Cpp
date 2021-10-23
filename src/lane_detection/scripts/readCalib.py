# Read the text file to read the transformation between LIDAR and camera
import os
import sys
import numpy as np

def getMatrices(camera_info, calib_path):
    # calibFile_intrinsic = open(intrinsic, "rt")
    print("hello world")
    calibFile = open(calib_path, "rt")
    
    content = calibFile.read()
    
    split_content = content.split('\n')
    
    
    # split_content = camera_info.split('\n')
    K_params = camera_info.P
    T_params = str(split_content[5])
    R_params = camera_info.R
    T_params = T_params.split(" ")
    
    T_list = []
    R_list = []
    K_list = []

    for i in range(len(K_params)):
        K_list.append(float(K_params[i]))

    for i in range(1, len(T_params)):
        T_list.append(float(T_params[i]))

    for i in range(len(R_params)):
        R_list.append(float(R_params[i]))

    T = np.asanyarray(T_list).reshape(3,4)
    K = np.asanyarray(K_list).reshape(3,4)
    # print("Instrinsic Parameters : {}\n".format(K))
    R = np.asanyarray(R_list).reshape(3,3)
    # print("Instrinsic Parameters that we return : {} \n".format(K[:,0:3]))

    return T,K[:,0:3],R