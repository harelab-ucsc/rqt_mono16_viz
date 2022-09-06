#include "ros/ros.h"
#include "sensor_msgs/Image.h"

#include <sstream>
#include <stdlib.h>
#include <vector>

using namespace std;
ros::Publisher pub;

int bit_shift;

string new_encoding; // TODO: make it so that new_encoding automatically determines bit_shift, relative to msg->encoding?
string node_name;
string default_node_name;
string listen_topic;
string pub_topic;


void downsampleCallback(const sensor_msgs::Image& msg) {
    sensor_msgs::Image img_out;
    //on creation, the img_out.data vector<uint8_t> is size 0, need STL vector resize()
    //to allocate storage for the new image

    //resize img_out.data appropriately based on output encoding type

    img_out.data.resize(msg.width*msg.height);

    uint8_t  shifted_value;
    for(int i=0;i < msg.height*msg.width;i++) {
        shifted_value = (uint8_t)(msg.data[i] >> bit_shift);
        if (shifted_value > (1 << 8*sizeof(img_out.data[0]))) {
            //print(Data truncation, not enough shift: shifted_val, bit_shift);
            cout << "Data truncation error, not enough shift." << endl;
            break;
        } else {
            img_out.data[i]=shifted_value;
        }
    }
    img_out.height = msg.height;
    img_out.width = msg.width;
    img_out.encoding = new_encoding;
    img_out.is_bigendian = msg.is_bigendian;
    img_out.step = msg.width; 
    pub.publish(img_out);
}


int main(int argc, char **argv) {
    ros::param::get("~bit_shift", bit_shift);
    ros::param::get("~new encoding", new_encoding);
    ros::param::get("~listen_topic", listen_topic);
    ros::param::get("~pub_topic", pub_topic);
    ros::param::param<std::string>("~node name", default_node_name, "mono_downsampler");
    ros::param::param<std::string>("~pub_topic", default_node_name, listen_topic + "/8bit");

    ros::init(argc, argv, node_name);
    ros::NodeHandle nh;

    //Print/log values of parameters that the node is starting up with to help with debug

    ros::Subscriber sub = nh.subscribe(listen_topic, 100, downsampleCallback);
    pub = nh.advertise<sensor_msgs::Image>(pub_topic, 100);
    ros::spin();
}