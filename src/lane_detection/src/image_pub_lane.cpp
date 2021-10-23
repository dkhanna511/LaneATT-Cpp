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
#include <cmath>
#include <fstream>
// nms library for laneatt
#include "nms/src/nms.cpp"

//importing msg files
#include "geometry_msgs/Point.h"
#include "lane_detection/lane_detect.h"

using namespace torch::indexing;
static const std::string OPENCV_WINDOW = "Image window";

class Detector {
		public:
		/***
		 * @brief constructor
		 * @param model_path - path of the TorchScript weight file
		 * @param device_type - inference with CPU/GPU
		 */


		//constructor to load the model from torchscript
		Detector(const std::string& model_path, const torch::DeviceType& device_type);

		/***
		 * @brief Non Max Supression
		 * @param Reg_Proposals - Region Proposals
		 * @param conf_threshold - confidence threshold
		 * @param attention_matrix - got from attention module of the model
		 *
		 * @return Proposals_List - the final output for the forward function of the model is proposals list
		 */

		std::vector<std::vector<torch::Tensor> > run_the_code(const cv::Mat& img, float conf_threshold);
		//void run_the_code(const cv::Mat& img, float conf_threshold);
		static std::vector<torch::Tensor> non_max_supression(const torch::Tensor& reg_proposals, int nms_thresh, int nms_topk,
					 float conf_threshold);

		/***
		 * @brief Decode - It takes the proposals list and then returns the output in the form os lanes and (x,y) coordinates
		 * @param Propsals_List - Contains the combination of region proposals and attention matrix
		 * @param as_lanes
		 * @return Proposals_List - the final output for the forward function of the model is proposals list
		 */

		static std::vector<std::vector<torch::Tensor> > decode(const std::vector<torch::Tensor>& proposals_list, bool as_lanes);

		static std::vector<torch::Tensor> proposals_to_pred(const torch::Tensor& proposals);
		cv::Mat generate_image(const std::vector<std::vector<torch::Tensor> >& prediction, cv::Mat& output_image);
		torch::jit::script::Module module_;
		torch::Device device_;
		bool half_;

};

//Constructor to load the torchscript model
Detector::Detector(const std::string& model_path, const torch::DeviceType& device_type) : device_(device_type) {
		try {
				// Deserialize the ScriptModule from a file using torch::jit::load().
				module_ = torch::jit::load(model_path);
		}
		catch (const c10::Error& e) {
				std::cerr << "Error loading the model!\n";
				std::exit(EXIT_FAILURE);
		}

		half_ = (device_ != torch::kCPU);
		module_.to(device_);
		if (half_) 
		{
        	module_.to(torch::kHalf);
    	}

	
		module_.eval();
}



std::vector<std::vector<torch::Tensor> >Detector::run_the_code(const cv::Mat& img, float conf_threshold) {
 //void Detector::run_the_code(const cv::Mat& img, float conf_threshold=0.5) {

	std::cout << "----------New Frame----------" << std::endl;
	cv::Mat img_input = img.clone();
	auto comp_start = std::chrono::high_resolution_clock::now();
	auto start = std::chrono::high_resolution_clock::now();
	// img_input2 = img_input

	float resized_img_width,resized_img_height;
	std::cout<<"resized image width : "<<resized_img_width<<std::endl;
	ros::param::get("/resize_image_width",resized_img_width);
    ros::param::get("/resize_image_height",resized_img_height);
	
	cv::resize(img_input, img_input, cv::Size(resized_img_width, resized_img_height));
	//cv::Mat image_input2 = img_input.clone();
	img_input.convertTo(img_input, CV_32FC3, 1.0f / 255.0f);
	auto tensor_img = torch::from_blob(img_input.data, {1, img_input.rows, img_input.cols, img_input.channels()}).to(device_);

	tensor_img = tensor_img.permute({0, 3, 1, 2}).contiguous();
    if (half_) {
        tensor_img = tensor_img.to(torch::kHalf);
    }

	std::vector<torch::jit::IValue> inputs;
	inputs.emplace_back(tensor_img);
	auto end = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);    
	std::cout << "preprocessing takes : " << duration.count() << " ms" << std::endl;
	start = std::chrono::high_resolution_clock::now();

	//std::vector<torch::jit::IValue> output = module_.forward(inputs);
	auto output = module_.forward(inputs);
	end = std::chrono::high_resolution_clock::now();

	duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "model takes : " << duration.count() << " ms" << std::endl;

	start = std::chrono::high_resolution_clock::now();
	auto nms_output = non_max_supression(output.toTensor(), 50, 10, conf_threshold);
	std::vector<std::vector<torch::Tensor> > prediction;
	prediction = decode(nms_output, true);
	
	end = std::chrono::high_resolution_clock::now();

	duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "post processing takes : " << duration.count() << " ms" << std::endl;
	
	std::cout<<"Type o prediction is :"<<typeid(prediction).name()<<std::endl;
	// std::cout<<"Shape of prediction[0][0] is :  "<<prediction[0][0].sizes()<<"\n\n";
	auto comp_end = std::chrono::high_resolution_clock::now();
	// std::cout<<"prediction[0][0][0] is : "<<prediction[0][0][0];
	
	duration = std::chrono::duration_cast<std::chrono::milliseconds>(comp_end - comp_start);
		
	std::cout << "inference takes : " << duration.count() << " ms"<<std::endl;

	return prediction;
}

cv::Mat Detector::generate_image(const std::vector<std::vector<torch::Tensor> >& prediction, cv::Mat& output_image)
{
	// cv::resize(output_image, output_image, cv::Size(640.0, 360.0));
	// int scale_y = 1288/640.0;
	// int scale_x = 964/360.0;
	std::cout<<"Image size is : "<<output_image.size();

	// std::cout<<"output_image in generate_msge : "<<output_image;
	std::vector<torch::Tensor> point_xs, point_ys;
	//std::tuple<torch::Tensor, torch::Tensor> hello;
	std::vector<std::vector<cv::Point> > lane_coords;
	
	int height, width ;
	height = output_image.size().height;
	width = output_image.size().width;
	std::cout<<"height of image is "<<height;
	std::cout<<"width of image is "<<width;
	if (prediction[0].size()>0)
	{
			for(int i = 0; i <prediction[0].size();i++)
			{
				// std::cout<<"predicrion["<<i<<"]"<<"is"<<prediction[i];
				// std::cout<<"i is : "<<i;
				std::vector<cv::Point> tuple;
				for (int j = 0;j< prediction[0][i].sizes()[0]-1; j++)
				{ torch::Tensor point_xs = prediction[0][i][j][0]*width;
					torch::Tensor point_ys = prediction[0][i][j][1]*height;
					tuple.push_back(cv::Point(point_xs.item<int>(), point_ys.item<int>()));
					
				}
			lane_coords.push_back(tuple);
			// std::cout<"Tuple is : "<<tuple<<std::endl;
			}
			std::cout<<"Length of lane coords is : "<<lane_coords.size()<<std::endl;
			
			for(int i =0; i<lane_coords.size();i++)
			{
				for(int j = 0; j<lane_coords[i].size()-1;j++)
				{
					cv::line(output_image, lane_coords[i][j], lane_coords[i][j+1], cv::Scalar(255,0,0), 2, cv::LINE_AA);
				}
			}  
	}

	std::cout<<"lane coords : "<<lane_coords;
	std::cout<<"\n\n\n\n\n\n";
	
	return output_image;
}
		
std::vector<torch::Tensor> Detector::non_max_supression(const torch::Tensor& reg_proposals, int nms_thresh=50, int nms_topk=1000,
																		float conf_threshold = 0.5)
{
		std::vector<torch::Tensor> proposals_list, keep_num_to_keep, anchor_inds_copy, proposals_dummy = {}, proposals_dummy_dummy, scores_dummy;
		//torch::Tensor above_threshold;
		std::vector<bool> above_threshold;
		torch::Tensor temp, scores, keep,  _, anchor_inds, anchor_inds_copy_copy;//, scores_copy, anchor_inds_copy;
		double array[] = {-1};
		torch::Tensor num_to_keep; //from nms.cpp
		for (int j = 0; j < reg_proposals.sizes()[0]; j++)
		{   torch::Tensor  proposals_copy_copy, proposals_copy, scores_copy, empty_tensor = torch::from_blob(array, {1});
		    	std::vector<torch::Tensor> empty_vec = {};
				int count=0;
				//std::vector<torch::Tensor> proposals_copy;
				//std::cout<<"Shape of proposals is : "<<reg_proposals[j].sizes();
				anchor_inds = torch::arange(reg_proposals.sizes()[1]);
				temp = torch::nn::functional::softmax(reg_proposals[j].index({Slice(), Slice(None, 2)}),torch::nn::functional::SoftmaxFuncOptions(1));
				scores = temp.index({Slice(), 1});
				if(conf_threshold != -1.0)
				{  //std::cout<<"Reaching here\n\n";
					for(int i = 0; i < scores.sizes()[0]; i++)
					{
						if (scores[i].item<float>() > conf_threshold)
						{
							above_threshold.push_back(true);
						}
						else
						{
							above_threshold.push_back(false);
							// above_threshold = torch::add(false);
						}
					}
			 }
			 for(int i =0; i < above_threshold.size(); i++)
				{  //std::cout<<"Reaching for loop for above threshold"<<std::endl;
					if(above_threshold[i] == true)
					{  //std::cout<<"reaching inside above threshold for loop  if condition\n\n"<<std::endl;
						proposals_dummy.push_back(reg_proposals[j][i]);
						scores_dummy.push_back(scores[i]);
						anchor_inds_copy.push_back(anchor_inds[i]);
					}
					else
					{
						count = count +1;
					}
				}

			 //std::cout<<"\n Proposals dummy here is  : "<<proposals_dummy;
			 //std::cout<<"The value of count is : "<<count<<std::endl;
			if (count== above_threshold.size())
				{
					proposals_list.push_back(empty_tensor);
					continue;
				}
			proposals_copy = torch::stack(proposals_dummy);
			scores_copy = torch::stack(scores_dummy);
			if(reg_proposals[j].sizes()[0] ==0)
		{ //std::cout<<"\n\n Reaching inside reg proposals if condition";
		 proposals_list.push_back(reg_proposals[j][0]);
		 continue;
		}
			 //std::cout<<"\n\n Is it reaching here? ";

			 keep_num_to_keep = nms_forward(proposals_copy, scores_copy, nms_thresh, nms_topk);
			 keep = keep_num_to_keep[0];
			 num_to_keep = keep_num_to_keep[1];
			 keep = keep.index({Slice(None, num_to_keep.item<int>())});
			 for(int i=0; i < keep.sizes()[0]; i++)
				{
					proposals_dummy_dummy.push_back(proposals_copy[keep[i].item<int>()]);
				}
			 proposals_copy_copy = torch::stack(proposals_dummy_dummy);
		proposals_list.push_back(proposals_copy_copy);
		}
return proposals_list;
}

std::vector<std::vector<torch::Tensor> > Detector::decode(const std::vector<torch::Tensor>& proposals_list, bool as_lanes = true)
{
	//std::cout<<"\n\n\n#############Reaching inside decode function##################"<<std::endl;
	std::vector<std::vector<torch::Tensor> > decoded;
	std::vector<torch::Tensor> pred, empty_pred= {};
	//std::cout<<"proposals list[0] size : "<<proposals_list[0].sizes()<<std::endl;
	//std::cout<<"proposals list : "<<proposals_list[0]<<std::endl;
 for(int i =0  ; i < proposals_list.size() ; i++ )
	{ //std::cout<<"\n\nReaching inside for loop ";
		std::vector<torch::Tensor> temp_pred;
		if (proposals_list[0].sizes()[0] ==1 && proposals_list[0].sizes()[1]!=77)
		{   
				decoded.push_back(empty_pred);
				continue;
		}
 
		proposals_list[i].index({Slice(),Slice(None, 2)}) = torch::nn::functional::softmax(proposals_list[i].index({Slice(), Slice(None, 2)}), torch::nn::functional::SoftmaxFuncOptions(1));
		proposals_list[i].index({Slice(), Slice(4)}) = round(proposals_list[i].index({Slice(), Slice(4)}));
		std::vector<torch::Tensor> empty_tensor = {};
		if (proposals_list[i].sizes()[0] == 0)
		{ 
			decoded.push_back(empty_tensor); //append null
			continue;
		}
		if (as_lanes == true)
		{ 
			pred = proposals_to_pred(proposals_list[i]);
		}
		else
		{
			temp_pred.push_back(proposals_list[i]);
			pred = temp_pred;
		}
		decoded.push_back(pred);

	}
	return decoded;
}

std::vector<torch::Tensor> Detector::proposals_to_pred(const torch::Tensor& proposals)
{   
	std::vector<torch::Tensor> lanes;//, lane_xs_dummy, lane_ys_dummy, lane_xs_dummy_dummy, lane_ys_dummy_dummy;
	int n_offsets = 72;
	torch::Tensor anchor_ys = torch::linspace(1, 0, n_offsets);
	torch::Tensor lane_xs, lane_ys, points;//, lane_xs_copy, lane_ys_copy, points, lane_xs_copy_copy, lane_ys_copy_copy;
	long int start, end, length, img_w, n_strips;
	img_w = 640;
	n_strips = n_offsets -1;
	bool temp_val ;
				//std::cout<<"\n\n shape of proposals is : "<<proposals.sizes()[0];
	for(int i= 0; i < proposals.sizes()[0]; i++)
	{
		std::vector<bool>  boolean, mask;
		std::vector<torch::Tensor> lane_ys_dummy, lane_xs_dummy, lane_ys_dummy_dummy, lane_xs_dummy_dummy, final_lane;
		torch::Tensor lane_xs_copy,lane_ys_copy,lane_xs_copy_copy, lane_ys_copy_copy;
		lane_xs = proposals[i].index({Slice(5, None)}) /  img_w;
		start = round((proposals[i][2]*n_strips).item<float>());
		//std::cout<<"\n\nstart is : "<<start; 
		length = round(proposals[i][4].item<int>());
		//std::cout<<"\n\nlength is : "<<length;
		end = start + length -1;
		end = std::min(end, anchor_ys.sizes()[0] - 1);
		for(int j = 0; j< start; j++)
		{
			if(lane_xs[j].item<float>() >= 0.0 && lane_xs[j].item<float>() <= 1.0)
				 {
				temp_val = 1;
			}
				 else
			{
				temp_val = 0;
			}
						//std::cout<<"temp val  is : "<<temp_val;
						boolean.push_back(1-temp_val);
		}
					//std::cout<<"\n\nboolean is given as "<<boolean;


		for(int j = 0; j < boolean.size(); j++)
		{
			mask.push_back(boolean[i]);
		}
		//std::cout<<"mask is "<<mask;
		lane_xs.index({Slice(end+1, None)}) = -2;
		for (int j = 0; j < mask.size(); j++)
		{
			if (mask[j]==1)
			{
				lane_xs[j] = -2;   //I'm doing this step because the length of start and mask are same.
			}
		}
		for( int j = 0; j < anchor_ys.sizes()[0]; j++)
		{
			if (lane_xs[j].item<float>() >=0.0)
			{
				lane_ys_dummy.push_back(anchor_ys[j]);
				lane_xs_dummy.push_back(lane_xs[j]);
			}
		}
		// for flipping the values
		lane_xs_copy = torch::stack(lane_xs_dummy);
		lane_ys_copy = torch::stack(lane_ys_dummy);
		for(int m = lane_xs_copy.sizes()[0]-1; m >=0 ; m--)
		{
			lane_xs_dummy_dummy.push_back(lane_xs_copy[m]);
			lane_ys_dummy_dummy.push_back(lane_ys_copy[m]);
		}

		lane_xs_copy_copy = torch::stack(lane_xs_dummy_dummy);
		lane_ys_copy_copy = torch::stack(lane_ys_dummy_dummy);
		if (lane_xs_copy_copy.sizes()[0] <=1)
		{
			//std::cout<<"inside continue function ";
			continue;
		}
		//Here we need to see what squeeze is and what is dim = 1 in python code
		lane_xs_copy_copy =lane_xs_copy_copy.view({-1, 1}).to(torch::kCUDA);
		lane_ys_copy_copy =lane_ys_copy_copy.view({-1, 1}).to(torch::kCUDA);
		final_lane.push_back(lane_xs_copy_copy);
		final_lane.push_back(lane_ys_copy_copy);
		points = torch::stack(final_lane).squeeze(2);
		points = torch::transpose(points, 1, 0);
								// lane = str(points.cpu().numpy())
		lanes.push_back(points);
	}
	return lanes;
}


class ImageConverter
{
	ros::NodeHandle nh_;
	image_transport::ImageTransport it_;
	image_transport::Subscriber image_sub_;
	// image_transport::Publisher image_pub_;
	ros::Publisher result_pub_;
	torch::DeviceType device_type = torch::kCUDA;
	std::string weights, subscribed_image, published_topic;
	
	Detector* detector;
	public:
	ImageConverter()
		: it_(nh_)
	{
		ros::param::get("/subscribed_image", subscribed_image);
		ros::param::get("/image_pub_lane_output", published_topic);
		ros::param::get("/model_path", weights);
		// Subscribe to input video feed and publish output video feed
		image_sub_ = it_.subscribe(subscribed_image, 1, &ImageConverter::imageCb, this);
		// image_pub_ = it_.advertise("/camera/output_video_lane", 1);
		result_pub_ = nh_.advertise<lane_detection::lane_detect>(published_topic, 1000);
		detector = new Detector(weights, device_type);
		//cv::namedWindow(OPENCV_WINDOW);
		//warmup();
	}

	~ImageConverter()
	{
		cv::destroyWindow(OPENCV_WINDOW);
	}

	void imageCb(const sensor_msgs::ImageConstPtr& msg)
	{
		cv_bridge::CvImagePtr cv_ptr;
		try
		{
			cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
		}
		catch (cv_bridge::Exception& e)
		{
			ROS_ERROR("cv_bridge exception: %s", e.what());
			return;
		}
		cv::Mat image_input = cv_ptr->image;
		float image_height = image_input.size().height, image_width = image_input.size().width;
		// Perform object detection on frame
		auto result = detector->run_the_code(cv_ptr->image, 0.5);
		// cv::Mat output_image = detector.generate_image(result, cv_ptr->image);
		// std::cout<<"output image is : "<<output_image;
		auto start = std::chrono::high_resolution_clock::now();
		//std::cout<<"\n\nThe type of results is : "<<typeid(result).name()<<std::endl;
		lane_detection::lane_detect msg_rec;
		//auto start = std::chrono::high_resolution_clock::now();
		msg_rec.header= msg->header;
		msg_rec.lane.clear();
		int count=0;
		for (int i=0;i < result[0].size();i++)
		{   geometry_msgs::Point point_dummy;
			if(result[0].size() ==0)
				{
					 point_dummy.x = -1;
					 point_dummy.y = -1;
					 point_dummy.z = -1;
					 msg_rec.lane.push_back(point_dummy);
					 msg_rec.num_of_points.push_back(-1);
					 continue;
				}   

			for(int j = 0; j<result[0][i].sizes()[0];j++)
			{
				point_dummy.x = result[0][i][j][0].item<float>()*image_width;
				point_dummy.y = result[0][i][j][1].item<float>()*image_height;
				point_dummy.z = 0;
				msg_rec.lane.push_back(point_dummy);
				//count++;
				//my_vector_dummy2.push_back(point);
			}
		msg_rec.num_of_points.push_back(result[0][i].sizes()[0]);


	}
		
		msg_rec.num_of_lanes = result[0].size();
		msg_rec.frame = *cv_ptr->toImageMsg();
	 	auto end = std::chrono::high_resolution_clock::now();

	 auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	 std::cout << "rostopic processing takes : " << duration.count() << " ms"<<std::endl;


		ROS_INFO("%d", msg_rec.num_of_lanes);
		result_pub_.publish(msg_rec);

		//Demo(cv_ptr->image, result, class_names);
		//sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", image_od_CV).toImageMsg();
		// Update GUI Window
		//cv::imshow(OPENCV_WINDOW, cv_ptr->image);
		//cv::waitKey(27);

		// Output modified video stream
		//image_pub.publish(msg)
		// image_pub_.publish(cv_ptr->toImageMsg());
	}

void warmup(){
		auto img = cv::imread("/home/nvidia/perception_module/src/object_detection/src/testing_lane.jpg");
		std::cout<<"Run once on an demo image"<<std::endl;
		auto temp_img = cv::Mat::zeros(img.rows, img.cols, CV_32FC3);
		//detector.run_the_code(img_img, 1.0f);
		float conf_threshold;
		ros::param::get("/confidence_threshold",conf_threshold);
	
		auto result = detector->run_the_code(img, conf_threshold);

	 }
};



int main(int argc, char **argv)
{
	// Check if video source has been passed as a parameter
	// if(argv[1] == NULL) return 1;

	ros::init(argc, argv, "image_pub_lane");
	ImageConverter ic;
	ros::spin();
	return 0;
}
