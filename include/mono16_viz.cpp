#include "ros/ros.h"
#include "sensor_msgs/Image.h"

#include <sstream>
#include <stdlib.h>

// namespace std;
ros::Publisher pub;

int bit_shift;
std::string new_encoding; // TODO: make it so that new_encoding automatically determines bit_shift, relative to msg->encoding?
std::string node_name;
std::string default_node_name;
std::string listen_topic;
std::string pub_topic;


void downsampleCallback(Image* msg) {
    Image img_out;
    for(i=0;i < msg->height*msg->width;i++) {
        img_out[i] = msg->data >> bit_shift;
    }
    img_out.height = msg->height;
    img_out.width = msg->width;
    img_out.encoding = new_encoding;
    img_out.is_bigendian = msg->is_bigendian;
    img_out.atep = msg->step;
    pub.publish(img_out);
}


int main(int argc, char **argv) {

    ros::param::get("~bit_shift", shift);
    ros::param::get("~new encoding", new_encoding);
    ros::param::get("~listen_topic", listen_topic);
    ros::param::get("~pub_topic", pub_topic);
    ros::param::param<std::string>("~node name", default_param, "mono_downsampler");
    ros::param::param<std::string>("~pub_topic", default_param, listen_topic + "/8bit");

    ros::init(argc, argv, node_name);
    ros::NodeHandle nh;
    ros::Subscriber sub = nh.subscribe(listen_topic, 100, downsampleCallback);
    pub = nh.advertise<sensor_msgs::Image>(pub_topic, 100);
}