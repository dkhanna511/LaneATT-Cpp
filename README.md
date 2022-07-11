# LaneATT-Cpp
The repository includes code for LaneATT implementation in C++

## lane_detection package
Lane detection package contains two nodes:
  1. **image_pub_lane**: the main node to run lane detcetion pipeline. It subscribes to the camera node and gets the images from it, processes it, and publishes "/camera/output_lane" as output lane coordinates. The message header for the publishes message is mentioned in *msg* folder.
  2. **viz_lane**: the node to run vizuation of lanes overlayed on the images on a separete ROs topic. It subscribes to "/camera/output_lane", overlays the lanes on the image and publishes the result on an image topic called "/visualizer/output_video_lane".

//made some changes to check few things
## HOW TO:

