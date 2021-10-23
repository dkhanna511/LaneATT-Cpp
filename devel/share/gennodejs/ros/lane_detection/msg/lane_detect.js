// Auto-generated. Do not edit!

// (in-package lane_detection.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;
let sensor_msgs = _finder('sensor_msgs');
let geometry_msgs = _finder('geometry_msgs');
let std_msgs = _finder('std_msgs');

//-----------------------------------------------------------

class lane_detect {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.header = null;
      this.num_of_lanes = null;
      this.lane = null;
      this.num_of_points = null;
      this.frame = null;
    }
    else {
      if (initObj.hasOwnProperty('header')) {
        this.header = initObj.header
      }
      else {
        this.header = new std_msgs.msg.Header();
      }
      if (initObj.hasOwnProperty('num_of_lanes')) {
        this.num_of_lanes = initObj.num_of_lanes
      }
      else {
        this.num_of_lanes = 0;
      }
      if (initObj.hasOwnProperty('lane')) {
        this.lane = initObj.lane
      }
      else {
        this.lane = [];
      }
      if (initObj.hasOwnProperty('num_of_points')) {
        this.num_of_points = initObj.num_of_points
      }
      else {
        this.num_of_points = [];
      }
      if (initObj.hasOwnProperty('frame')) {
        this.frame = initObj.frame
      }
      else {
        this.frame = new sensor_msgs.msg.Image();
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type lane_detect
    // Serialize message field [header]
    bufferOffset = std_msgs.msg.Header.serialize(obj.header, buffer, bufferOffset);
    // Serialize message field [num_of_lanes]
    bufferOffset = _serializer.int32(obj.num_of_lanes, buffer, bufferOffset);
    // Serialize message field [lane]
    // Serialize the length for message field [lane]
    bufferOffset = _serializer.uint32(obj.lane.length, buffer, bufferOffset);
    obj.lane.forEach((val) => {
      bufferOffset = geometry_msgs.msg.Point.serialize(val, buffer, bufferOffset);
    });
    // Serialize message field [num_of_points]
    bufferOffset = _arraySerializer.float32(obj.num_of_points, buffer, bufferOffset, null);
    // Serialize message field [frame]
    bufferOffset = sensor_msgs.msg.Image.serialize(obj.frame, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type lane_detect
    let len;
    let data = new lane_detect(null);
    // Deserialize message field [header]
    data.header = std_msgs.msg.Header.deserialize(buffer, bufferOffset);
    // Deserialize message field [num_of_lanes]
    data.num_of_lanes = _deserializer.int32(buffer, bufferOffset);
    // Deserialize message field [lane]
    // Deserialize array length for message field [lane]
    len = _deserializer.uint32(buffer, bufferOffset);
    data.lane = new Array(len);
    for (let i = 0; i < len; ++i) {
      data.lane[i] = geometry_msgs.msg.Point.deserialize(buffer, bufferOffset)
    }
    // Deserialize message field [num_of_points]
    data.num_of_points = _arrayDeserializer.float32(buffer, bufferOffset, null)
    // Deserialize message field [frame]
    data.frame = sensor_msgs.msg.Image.deserialize(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += std_msgs.msg.Header.getMessageSize(object.header);
    length += 24 * object.lane.length;
    length += 4 * object.num_of_points.length;
    length += sensor_msgs.msg.Image.getMessageSize(object.frame);
    return length + 12;
  }

  static datatype() {
    // Returns string type for a message object
    return 'lane_detection/lane_detect';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '1ec713989d8e6daf5b7ceba45fb0a04f';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    std_msgs/Header header
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
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new lane_detect(null);
    if (msg.header !== undefined) {
      resolved.header = std_msgs.msg.Header.Resolve(msg.header)
    }
    else {
      resolved.header = new std_msgs.msg.Header()
    }

    if (msg.num_of_lanes !== undefined) {
      resolved.num_of_lanes = msg.num_of_lanes;
    }
    else {
      resolved.num_of_lanes = 0
    }

    if (msg.lane !== undefined) {
      resolved.lane = new Array(msg.lane.length);
      for (let i = 0; i < resolved.lane.length; ++i) {
        resolved.lane[i] = geometry_msgs.msg.Point.Resolve(msg.lane[i]);
      }
    }
    else {
      resolved.lane = []
    }

    if (msg.num_of_points !== undefined) {
      resolved.num_of_points = msg.num_of_points;
    }
    else {
      resolved.num_of_points = []
    }

    if (msg.frame !== undefined) {
      resolved.frame = sensor_msgs.msg.Image.Resolve(msg.frame)
    }
    else {
      resolved.frame = new sensor_msgs.msg.Image()
    }

    return resolved;
    }
};

module.exports = lane_detect;
