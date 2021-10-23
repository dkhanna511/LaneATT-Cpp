#! /usr/bin/python3
# rospy for the subscriber
import rospy
import message_filters
# ROS Image message
from sensor_msgs.msg import Image,  CameraInfo,  PointCloud2
from lane_detection.msg import  lane_detect
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
import matplotlib.pyplot as plt
import numpy as np

hello = 0
def image_callback(lane_info, camera_info, lidar_info, image):
    print("Received an image!")
    lane_mapper_publisher = rospy.Publisher('/lane_mapper_pc', PointCloud2)
    # lane_mapper = PointCloud()
    # rospy.sleep(1.)
    header = std_msgs.msg.Header()
    # header.stamp = rospy.Time.now() 
    header.stamp = lane_info.header.stamp    
    
    header.frame_id = 'map'

    try:
        # Convert your ROS Image message to OpenCV2
        cv2_img = bridge.imgmsg_to_cv2(image, "bgr8")
    except CvBridgeError as e:
        print(e)
        print("world")

    # print("lane_coords : ", lane_info.lane)
    lane_coords = []
    #lane_info.lane = np.reshape(lane_info.lane, (len(lane_info.lane), 1))
    # lane_coords.append([lane_info.lane.x, lane_info.lane.y])
    for i in range(len(lane_info.lane)):
        xs, ys = lane_info.lane[i].x, lane_info.lane[i].y
        lane_coords.append([int(xs), int(ys), 1])

    count = 0
    val = np.array(lane_coords) 
    list =  []
    i =0
    while i< len(lane_info.lane):
        # print(lane_info.lane)
        val = []
        j_max = int(lane_info.num_of_points[count])

        for j in range(j_max):
            xs, ys= int(lane_info.lane[i].x), int(lane_info.lane[i].y)
            val.append((xs, ys))
            i = i+1
        count = count + 1

        list.append(val)

    overall_lanes = []
    for i in range(len(list)):
        overall_lanes.append(list[i][0][0])
        print("list[{}][0][0] : {}".format(i, list[i][0][0]))

    print("shape of list : {}".format(len(list)))
    print("list is : {}\n".format(list))
    print("overall_lanes : {}\n".format(overall_lanes))
    
    minimum = overall_lanes.index(min(overall_lanes))
    maximum  = overall_lanes.index(max(overall_lanes))

    print("minimum val = ", minimum)
    print("maximum val = ", maximum)
    
    x1= min(list[minimum])[0] - 100
    y1 =   375 
    print("x1 {} y1 {}".format(x1, y1))

    x2 = max(list[minimum][0]) -80
    y2 = max(list[minimum])[1] 
    print("x2 {} y2 {}".format(x2, y2))


    x3 = min(list[maximum])[0] + 80
    y3 = min(list[maximum])[1]
    print("x3 {} y3 {}".format(x3, y3))


    x4 = max(list[maximum])[0] + 100
    y4 = 375
    print("x4 {} y4 {}".format(x4, y4))



    # print("\n\nlist1 = ", list[0])
    # print("\n\nlist2 = ",list[1])
    # print("\n\nlist3 = ", list[2])
    print("\n\n\n\n\n\n")
    # print(min(list[0], list[1]))


    # M = np.array(([721.5377, 0.0, 609.5593, 44.85728], [0.0, 721.5377, 72.854, 0.2163791], [0.0, 0.0, 1.0, .002745884]))
    # maxHeight, maxWidth = cv2_img.shape[:2]
    # cv2.waitKey(0)

    # max_x, max_y = max(val[:,0]+50), max(val[:,1]+50)
    # print("val ", val)
    # print("minx : {} min y : {} max_x : {} max_y : {}".format(min_x, min_y, max_x, max_y))   
    # input_pts = np.float32([[0,375],[100,175],[800,175], [900, 375]])
    output_pts = np.float32([[0,1000],[0,0],[1000,0],[1000,1000]])
    # maxWidth = (max_x - min_x)
    # maxHeight = (max_y - min_y)
    # input_pts = np.float32([[min_x, min_y],[max_x, min_y], [min_x, max_y], [max_x, max_y]])
    # output_pts = np.float32([[0, 0],[0, maxHeight - 1],[maxWidth - 1, maxHeight - 1],[maxWidth - 1, 0]])

    input_pts = np.float32([[x1, y1], [x2, y2], [x3, y3], [x4, y4]])

    #print("lane_coords x : ", lane_info.lane[:,x])
    #print("lane_coords y : ", lane_info.lane[:,y])
    #print("lanes  shape: ", np.shape(lane_info.lane))
    
    output= cv2.polylines(cv2_img, [input_pts.astype(np.int32)],True, (0,0,255), thickness=5)    # M = cv2.getPerspectiveTransform(input_pts,output_pts)
    # cv2.imshow("frame", output)
    # cv2.waitKey(500)
    # # print(M)
    global hello
    hello = hello + 1
    # cv2.imwrite("frame{}.jpg".format(hello), output)
    cv2.imshow("frame", output)

    widthA = np.sqrt(((x4 - x1) ** 2) + ((y4 - y1) ** 2))
    print("widthA is : ", widthA)
    
    widthB = np.sqrt(((x3 - x2) ** 2) + ((y3 - y1) ** 2))
    print("widthB is : ", widthB)
    maxWidth = max(int(widthA), int(widthB))
    print("maxWidth is : ",maxWidth)
    
    # compute the height of the new image, which will be the
	# maximum distance between the top-right and bottom-right
	# y-coordinates or the top-left and bottom-left y-coordinates
    heightA = np.sqrt(((x3 - x4) ** 2) + ((y3 - y4) ** 2))
    heightB = np.sqrt(((x2 - x1) ** 2) + ((y2 - y1) ** 2))
    maxHeight = max(int(heightA), int(heightB))

    print("heightA is : ", heightA)
    print("heightB is : ", heightB)
    
    print("maxWHeight : {}".format(maxHeight))
    # dst = np.array([[0, 0], [maxWidth - 1, 0],[maxWidth - 1, maxHeight - 1],[0, maxHeight - 1]], dtype = "float32")

    dst= np.float32([[0, maxHeight +100 ], [0, 0], [maxWidth+100, 0], [maxWidth+100, maxHeight +100 ]])

    M = cv2.getPerspectiveTransform(input_pts, output_pts)
    
    out = cv2.warpPerspective(cv2_img,M,(maxWidth, maxHeight),flags=cv2.INTER_LINEAR)
    # cv2.imshow("frame_output", out)
    cv2.imwrite("frame_output{}.jpg".format(hello), out)
    # cv2.waitKey(500)

     #lane_coords.append([lane_info.lane[0], lane_info.lane[:].y])
    # print("lane_coords shape : ",lane_coords)
    
    
def main():
    print("Here")
    rospy.init_node('image_listener')
    # Define your image topic
    lane_output_topic =  "/camera/output_lane"
    image_topic = "/visualizer/output_video_lane"
    camera_info_topic = "/kitti/camera_color_left/camera_info"
    lidar_topic = "/kitti/velo/pointcloud"
    lane_info = message_filters.Subscriber(lane_output_topic, lane_detect)
    camera_info = message_filters.Subscriber(camera_info_topic, CameraInfo)
    lidar_info = message_filters.Subscriber(lidar_topic, PointCloud2)
    image_info = message_filters.Subscriber(image_topic, Image)


    # Set up your subscriber and define its callback
    # rospy.Subscriber(lane_image_topic, Image, image_callback)
    ts = message_filters.ApproximateTimeSynchronizer([lane_info, camera_info, lidar_info, image_info], 1, 1)
    ts.registerCallback(image_callback)
    # Spin until ctrl + c
    rospy.spin()

if __name__ == '__main__':
    main()
