;; Auto-generated. Do not edit!


(when (boundp 'lane_detection::lane_detect)
  (if (not (find-package "LANE_DETECTION"))
    (make-package "LANE_DETECTION"))
  (shadow 'lane_detect (find-package "LANE_DETECTION")))
(unless (find-package "LANE_DETECTION::LANE_DETECT")
  (make-package "LANE_DETECTION::LANE_DETECT"))

(in-package "ROS")
;;//! \htmlinclude lane_detect.msg.html
(if (not (find-package "GEOMETRY_MSGS"))
  (ros::roseus-add-msgs "geometry_msgs"))
(if (not (find-package "SENSOR_MSGS"))
  (ros::roseus-add-msgs "sensor_msgs"))
(if (not (find-package "STD_MSGS"))
  (ros::roseus-add-msgs "std_msgs"))


(defclass lane_detection::lane_detect
  :super ros::object
  :slots (_header _num_of_lanes _lane _num_of_points _frame ))

(defmethod lane_detection::lane_detect
  (:init
   (&key
    ((:header __header) (instance std_msgs::Header :init))
    ((:num_of_lanes __num_of_lanes) 0)
    ((:lane __lane) (let (r) (dotimes (i 0) (push (instance geometry_msgs::Point :init) r)) r))
    ((:num_of_points __num_of_points) (make-array 0 :initial-element 0.0 :element-type :float))
    ((:frame __frame) (instance sensor_msgs::Image :init))
    )
   (send-super :init)
   (setq _header __header)
   (setq _num_of_lanes (round __num_of_lanes))
   (setq _lane __lane)
   (setq _num_of_points __num_of_points)
   (setq _frame __frame)
   self)
  (:header
   (&rest __header)
   (if (keywordp (car __header))
       (send* _header __header)
     (progn
       (if __header (setq _header (car __header)))
       _header)))
  (:num_of_lanes
   (&optional __num_of_lanes)
   (if __num_of_lanes (setq _num_of_lanes __num_of_lanes)) _num_of_lanes)
  (:lane
   (&rest __lane)
   (if (keywordp (car __lane))
       (send* _lane __lane)
     (progn
       (if __lane (setq _lane (car __lane)))
       _lane)))
  (:num_of_points
   (&optional __num_of_points)
   (if __num_of_points (setq _num_of_points __num_of_points)) _num_of_points)
  (:frame
   (&rest __frame)
   (if (keywordp (car __frame))
       (send* _frame __frame)
     (progn
       (if __frame (setq _frame (car __frame)))
       _frame)))
  (:serialization-length
   ()
   (+
    ;; std_msgs/Header _header
    (send _header :serialization-length)
    ;; int32 _num_of_lanes
    4
    ;; geometry_msgs/Point[] _lane
    (apply #'+ (send-all _lane :serialization-length)) 4
    ;; float32[] _num_of_points
    (* 4    (length _num_of_points)) 4
    ;; sensor_msgs/Image _frame
    (send _frame :serialization-length)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; std_msgs/Header _header
       (send _header :serialize s)
     ;; int32 _num_of_lanes
       (write-long _num_of_lanes s)
     ;; geometry_msgs/Point[] _lane
     (write-long (length _lane) s)
     (dolist (elem _lane)
       (send elem :serialize s)
       )
     ;; float32[] _num_of_points
     (write-long (length _num_of_points) s)
     (dotimes (i (length _num_of_points))
       (sys::poke (elt _num_of_points i) (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
       )
     ;; sensor_msgs/Image _frame
       (send _frame :serialize s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; std_msgs/Header _header
     (send _header :deserialize buf ptr-) (incf ptr- (send _header :serialization-length))
   ;; int32 _num_of_lanes
     (setq _num_of_lanes (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; geometry_msgs/Point[] _lane
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _lane (let (r) (dotimes (i n) (push (instance geometry_msgs::Point :init) r)) r))
     (dolist (elem- _lane)
     (send elem- :deserialize buf ptr-) (incf ptr- (send elem- :serialization-length))
     ))
   ;; float32[] _num_of_points
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _num_of_points (instantiate float-vector n))
     (dotimes (i n)
     (setf (elt _num_of_points i) (sys::peek buf ptr- :float)) (incf ptr- 4)
     ))
   ;; sensor_msgs/Image _frame
     (send _frame :deserialize buf ptr-) (incf ptr- (send _frame :serialization-length))
   ;;
   self)
  )

(setf (get lane_detection::lane_detect :md5sum-) "1ec713989d8e6daf5b7ceba45fb0a04f")
(setf (get lane_detection::lane_detect :datatype-) "lane_detection/lane_detect")
(setf (get lane_detection::lane_detect :definition-)
      "std_msgs/Header header
int32 num_of_lanes
geometry_msgs/Point[] lane
float32[] num_of_points
sensor_msgs/Image frame

================================================================================
MSG: std_msgs/Header
# Standard metadata for higher-level stamped data types.
# This is generally used to communicate timestamped data 
# in a particular coordinate frame.
# 
# sequence ID: consecutively increasing ID 
uint32 seq
#Two-integer timestamp that is expressed as:
# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')
# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')
# time-handling sugar is provided by the client library
time stamp
#Frame this data is associated with
string frame_id

================================================================================
MSG: geometry_msgs/Point
# This contains the position of a point in free space
float64 x
float64 y
float64 z

================================================================================
MSG: sensor_msgs/Image
# This message contains an uncompressed image
# (0, 0) is at top-left corner of image
#

Header header        # Header timestamp should be acquisition time of image
                     # Header frame_id should be optical frame of camera
                     # origin of frame should be optical center of camera
                     # +x should point to the right in the image
                     # +y should point down in the image
                     # +z should point into to plane of the image
                     # If the frame_id here and the frame_id of the CameraInfo
                     # message associated with the image conflict
                     # the behavior is undefined

uint32 height         # image height, that is, number of rows
uint32 width          # image width, that is, number of columns

# The legal values for encoding are in file src/image_encodings.cpp
# If you want to standardize a new string format, join
# ros-users@lists.sourceforge.net and send an email proposing a new encoding.

string encoding       # Encoding of pixels -- channel meaning, ordering, size
                      # taken from the list of strings in include/sensor_msgs/image_encodings.h

uint8 is_bigendian    # is this data bigendian?
uint32 step           # Full row length in bytes
uint8[] data          # actual matrix data, size is (step * rows)

")



(provide :lane_detection/lane_detect "1ec713989d8e6daf5b7ceba45fb0a04f")


