// Generated by gencpp from file lane_detection/lane_detect.msg
// DO NOT EDIT!


#ifndef LANE_DETECTION_MESSAGE_LANE_DETECT_H
#define LANE_DETECTION_MESSAGE_LANE_DETECT_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>

#include <std_msgs/Header.h>
#include <geometry_msgs/Point.h>
#include <sensor_msgs/Image.h>

namespace lane_detection
{
template <class ContainerAllocator>
struct lane_detect_
{
  typedef lane_detect_<ContainerAllocator> Type;

  lane_detect_()
    : header()
    , num_of_lanes(0)
    , lane()
    , num_of_points()
    , frame()  {
    }
  lane_detect_(const ContainerAllocator& _alloc)
    : header(_alloc)
    , num_of_lanes(0)
    , lane(_alloc)
    , num_of_points(_alloc)
    , frame(_alloc)  {
  (void)_alloc;
    }



   typedef  ::std_msgs::Header_<ContainerAllocator>  _header_type;
  _header_type header;

   typedef int32_t _num_of_lanes_type;
  _num_of_lanes_type num_of_lanes;

   typedef std::vector< ::geometry_msgs::Point_<ContainerAllocator> , typename ContainerAllocator::template rebind< ::geometry_msgs::Point_<ContainerAllocator> >::other >  _lane_type;
  _lane_type lane;

   typedef std::vector<float, typename ContainerAllocator::template rebind<float>::other >  _num_of_points_type;
  _num_of_points_type num_of_points;

   typedef  ::sensor_msgs::Image_<ContainerAllocator>  _frame_type;
  _frame_type frame;





  typedef boost::shared_ptr< ::lane_detection::lane_detect_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::lane_detection::lane_detect_<ContainerAllocator> const> ConstPtr;

}; // struct lane_detect_

typedef ::lane_detection::lane_detect_<std::allocator<void> > lane_detect;

typedef boost::shared_ptr< ::lane_detection::lane_detect > lane_detectPtr;
typedef boost::shared_ptr< ::lane_detection::lane_detect const> lane_detectConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::lane_detection::lane_detect_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::lane_detection::lane_detect_<ContainerAllocator> >::stream(s, "", v);
return s;
}


template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator==(const ::lane_detection::lane_detect_<ContainerAllocator1> & lhs, const ::lane_detection::lane_detect_<ContainerAllocator2> & rhs)
{
  return lhs.header == rhs.header &&
    lhs.num_of_lanes == rhs.num_of_lanes &&
    lhs.lane == rhs.lane &&
    lhs.num_of_points == rhs.num_of_points &&
    lhs.frame == rhs.frame;
}

template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator!=(const ::lane_detection::lane_detect_<ContainerAllocator1> & lhs, const ::lane_detection::lane_detect_<ContainerAllocator2> & rhs)
{
  return !(lhs == rhs);
}


} // namespace lane_detection

namespace ros
{
namespace message_traits
{





template <class ContainerAllocator>
struct IsFixedSize< ::lane_detection::lane_detect_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::lane_detection::lane_detect_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::lane_detection::lane_detect_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::lane_detection::lane_detect_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::lane_detection::lane_detect_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::lane_detection::lane_detect_<ContainerAllocator> const>
  : TrueType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::lane_detection::lane_detect_<ContainerAllocator> >
{
  static const char* value()
  {
    return "1ec713989d8e6daf5b7ceba45fb0a04f";
  }

  static const char* value(const ::lane_detection::lane_detect_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x1ec713989d8e6dafULL;
  static const uint64_t static_value2 = 0x5b7ceba45fb0a04fULL;
};

template<class ContainerAllocator>
struct DataType< ::lane_detection::lane_detect_<ContainerAllocator> >
{
  static const char* value()
  {
    return "lane_detection/lane_detect";
  }

  static const char* value(const ::lane_detection::lane_detect_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::lane_detection::lane_detect_<ContainerAllocator> >
{
  static const char* value()
  {
    return "std_msgs/Header header\n"
"int32 num_of_lanes\n"
"geometry_msgs/Point[] lane\n"
"float32[] num_of_points\n"
"sensor_msgs/Image frame\n"
"\n"
"================================================================================\n"
"MSG: std_msgs/Header\n"
"# Standard metadata for higher-level stamped data types.\n"
"# This is generally used to communicate timestamped data \n"
"# in a particular coordinate frame.\n"
"# \n"
"# sequence ID: consecutively increasing ID \n"
"uint32 seq\n"
"#Two-integer timestamp that is expressed as:\n"
"# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')\n"
"# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')\n"
"# time-handling sugar is provided by the client library\n"
"time stamp\n"
"#Frame this data is associated with\n"
"string frame_id\n"
"\n"
"================================================================================\n"
"MSG: geometry_msgs/Point\n"
"# This contains the position of a point in free space\n"
"float64 x\n"
"float64 y\n"
"float64 z\n"
"\n"
"================================================================================\n"
"MSG: sensor_msgs/Image\n"
"# This message contains an uncompressed image\n"
"# (0, 0) is at top-left corner of image\n"
"#\n"
"\n"
"Header header        # Header timestamp should be acquisition time of image\n"
"                     # Header frame_id should be optical frame of camera\n"
"                     # origin of frame should be optical center of camera\n"
"                     # +x should point to the right in the image\n"
"                     # +y should point down in the image\n"
"                     # +z should point into to plane of the image\n"
"                     # If the frame_id here and the frame_id of the CameraInfo\n"
"                     # message associated with the image conflict\n"
"                     # the behavior is undefined\n"
"\n"
"uint32 height         # image height, that is, number of rows\n"
"uint32 width          # image width, that is, number of columns\n"
"\n"
"# The legal values for encoding are in file src/image_encodings.cpp\n"
"# If you want to standardize a new string format, join\n"
"# ros-users@lists.sourceforge.net and send an email proposing a new encoding.\n"
"\n"
"string encoding       # Encoding of pixels -- channel meaning, ordering, size\n"
"                      # taken from the list of strings in include/sensor_msgs/image_encodings.h\n"
"\n"
"uint8 is_bigendian    # is this data bigendian?\n"
"uint32 step           # Full row length in bytes\n"
"uint8[] data          # actual matrix data, size is (step * rows)\n"
;
  }

  static const char* value(const ::lane_detection::lane_detect_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::lane_detection::lane_detect_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.header);
      stream.next(m.num_of_lanes);
      stream.next(m.lane);
      stream.next(m.num_of_points);
      stream.next(m.frame);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct lane_detect_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::lane_detection::lane_detect_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::lane_detection::lane_detect_<ContainerAllocator>& v)
  {
    s << indent << "header: ";
    s << std::endl;
    Printer< ::std_msgs::Header_<ContainerAllocator> >::stream(s, indent + "  ", v.header);
    s << indent << "num_of_lanes: ";
    Printer<int32_t>::stream(s, indent + "  ", v.num_of_lanes);
    s << indent << "lane[]" << std::endl;
    for (size_t i = 0; i < v.lane.size(); ++i)
    {
      s << indent << "  lane[" << i << "]: ";
      s << std::endl;
      s << indent;
      Printer< ::geometry_msgs::Point_<ContainerAllocator> >::stream(s, indent + "    ", v.lane[i]);
    }
    s << indent << "num_of_points[]" << std::endl;
    for (size_t i = 0; i < v.num_of_points.size(); ++i)
    {
      s << indent << "  num_of_points[" << i << "]: ";
      Printer<float>::stream(s, indent + "  ", v.num_of_points[i]);
    }
    s << indent << "frame: ";
    s << std::endl;
    Printer< ::sensor_msgs::Image_<ContainerAllocator> >::stream(s, indent + "  ", v.frame);
  }
};

} // namespace message_operations
} // namespace ros

#endif // LANE_DETECTION_MESSAGE_LANE_DETECT_H