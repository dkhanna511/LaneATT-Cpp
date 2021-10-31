#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sstream> // for converting the command line parameter to integer
#include <torch/script.h>
#include <torch/torch.h>//#define _GLIBCXX_USE_CXX11_ABI 0
#include <iostream>
#include <memory>
#include <algorithm>
#include <c10/cuda/CUDAStream.h>
#include <ATen/cuda/CUDAEvent.h>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <chrono>
#include <sensor_msgs/image_encodings.h>
#include <string>
#include "geometry_msgs/Point.h"

#include "lane_detection/lane_detect.h"
static const std::string OPENCV_WINDOW = "Image window";

void create_lane_viz(const int num_of_lanes, const std::vector<geometry_msgs::Point>& lane_output,const std::vector<float> num_of_points,  cv::Mat img)
{

	std::vector<std::vector<cv::Point> > lane_coords;
	int j = 0;
	     int count = 0;
	 for(int i =0 ;i < lane_output.size();)
	 {
	 	std::vector<cv::Point> tuple;
		int j_max = num_of_points[count];
	 	//int count = 0;
	 	for(int j = 0; j < j_max; j++)
	 	{
	 		geometry_msgs::Point point = lane_output[i];
	 		int point_x = point.x;
	 		int point_y = point.y;
	        i++;
	 		tuple.push_back(cv::Point(point_x, point_y));
	 	}
		count++;
	 	lane_coords.push_back(tuple);
	 }


  std::cout<<"num of lanes : "<<lane_coords.size()<<std::endl;
	//count=0;
      for(int i =0; i<lane_coords.size();i++)
      {
        for(int j = 0; j<lane_coords[i].size()-1;j++)
		{
	    cv::line(img, lane_coords[i][j], lane_coords[i][j+1], cv::Scalar(255,0,0), 2, cv::LINE_AA);
        }
      }
	
	}


class Visualizer{
	ros::NodeHandle nh_;
	image_transport::ImageTransport it_;
	image_transport::Publisher image_pub_;
	ros::Subscriber subscribe_tl;
	ros::Subscriber subscribe_objects;
	int video_init = 1;
	std::string input_topic, published_topic;
	cv::VideoWriter video;

	
public:
	Visualizer()
	: it_(nh_)
	{
		ros::param::get("/image_pub_lane_output", input_topic);
		ros::param::get("/visualizer_output", published_topic);
		// subscrive to input video feed and publish output video feed
		image_pub_ = it_.advertise(published_topic, 1);
		// subscribe_tl = n.subscribe("/detection/image_detector/tl", 1, &visualizer::tl_callback);
		subscribe_objects = nh_.subscribe(input_topic, 1, &Visualizer::objectCb, this);
	}

	~Visualizer()
	{
		cv::destroyWindow(OPENCV_WINDOW);
	}
	
	
	void objectCb(const lane_detection::lane_detect& msg){
		cv_bridge::CvImagePtr cv_ptr;
		try
		{
			cv_ptr = cv_bridge::toCvCopy(msg.frame, sensor_msgs::image_encodings::BGR8);
		}
		catch (cv_bridge::Exception& e)
		{
			ROS_ERROR("cv_bridge exception: %s", e.what());
			return;
		}

		std::cout<<"IMAGE LOADED"<<std::endl;

		// if(video_init==1){
        //                 video.open("/home/dheeraj/lane_detection/output4.avi",cv::VideoWriter::fourcc('M','J','P','G'),10, cv::Size(frame_width,frame_height));
        //                 video_init = 0;
        //         }

		create_lane_viz(msg.num_of_lanes, msg.lane, msg.num_of_points, cv_ptr->image);
		// video.write(cv_ptr->image);

		image_pub_.publish(cv_ptr->toImageMsg());
	}
	
	
};

int main(int argc, char **argv)
{
	// Check if video source has been passed as a parameter
	// if(argv[1] == NULL) return 1;

	ros::init(argc, argv, "viz_lane");
	Visualizer  viz;
	ros::spin();
	return 0;

}
