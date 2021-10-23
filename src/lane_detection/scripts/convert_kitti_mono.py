#! /usr/bin/python3
# rospy for the subscriber
import rospy
import message_filters
# ROS Image message
from sensor_msgs.msg import Image,  CameraInfo,  PointCloud2
# from lane_detection.msg import  lane_detect
# ROS Image message -> OpenCV2 image converter
from cv_bridge import CvBridge, CvBridgeError
# OpenCV2 for saving an image
import rospy
import std_msgs
import cv2
import ros_numpy
import readCalib
import laneMapper
# from main import lan
# Instantiate CvBridge
import sensor_msgs.point_cloud2 as pcl2
bridge = CvBridge()
import numpy as np


image_publisher = rospy.Publisher('/camera/image_raw', Image)
camera_info_publisher = rospy.Publisher('/camera/camera_info', CameraInfo)
    
def image_callback(image, camera_info):
    print("Received an image!")
    
    # lane_mapper = PointCloud()
    # rospy.sleep(1.)
    header = std_msgs.msg.Header()
    header.stamp = rospy.Time.now()    
    header.frame_id = 'map'
    
    try:
        print("hello")
        # Convert your ROS Image message to OpenCV2
        cv2_img = bridge.imgmsg_to_cv2(image, "bgr8")
    except CvBridgeError as e:
        print(e)
        print("world")

    camera_info.roi.height = camera_info.height 
    camera_info.roi.width = camera_info.width 
    camera_info.roi.do_rectify = False

    camera_info.height = 0
    camera_info.width = 0
    camera_info.distortion_model = ''
    camera_info.D = []
    camera_info.K = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
    camera_info.R = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
    camera_info.P = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
    camera_info.binning_x = 1
    camera_info.binning_y = 1
    image_publisher.publish(image)
    camera_info_publisher.publish(camera_info)
    

def main():
    rospy.init_node('image_listener')
    # Define your image topic
    lane_image_topic =  "/kitti/camera_color_left/image_raw"
    camera_info_topic = "/kitti/camera_color_left/camera_info"
    image_info = message_filters.Subscriber(lane_image_topic, Image)
    camera_info = message_filters.Subscriber(camera_info_topic, CameraInfo)
    
    # Set up your subscriber and define its callback
    # rospy.Subscriber(lane_image_topic, Image, image_callback)
    print("hello world ")
    ts = message_filters.ApproximateTimeSynchronizer([image_info, camera_info], 1, 1)
    ts.registerCallback(image_callback)
    # Spin until ctrl + c
    rospy.spin()

if __name__ == '__main__':
    main()
