
(cl:in-package :asdf)

(defsystem "lane_detection-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :geometry_msgs-msg
               :sensor_msgs-msg
               :std_msgs-msg
)
  :components ((:file "_package")
    (:file "lane_detect" :depends-on ("_package_lane_detect"))
    (:file "_package_lane_detect" :depends-on ("_package"))
  ))