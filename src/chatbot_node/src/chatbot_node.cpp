#include <ros/ros.h>
#include <chatbot_node/reply_msg.h>
#include <message_ui/sent_msg.h>
#include <string>
#include <iostream>

#include "std_msgs/String.h"
#include <sstream>

using namespace std;

//Add your code here
/**
 * This tutorial demonstrates simple receipt of messages over the ROS system.
 */

class RosClass
{
private:
    ros::NodeHandle n;
    ros::Subscriber chatter_sub;
    ros::Publisher chatter_pub;
    message_ui::sent_msg sendMsg;
    std::string myName;
    std::stringstream publishMsg;
public:
    RosClass();
    ~RosClass();
    void chatterCallback(const message_ui::sent_msg::ConstPtr& msg);

};
RosClass::RosClass() {
    if(n.getParam("/name", myName)){
        ROS_INFO("Got name from param: %f", myName);
    } else {
        ROS_INFO("Failed to get param 'name'");
        // n.setParam(myName, "DefaultName");
        n.param<std::string>("name", myName, "DefaultName");
    }

    chatter_sub = n.subscribe("sent_msg", 1000, &RosClass::chatterCallback, this);
    chatter_pub = n.advertise<chatbot_node::reply_msg>("reply_msg", 1000);
    ros::spin();
}

RosClass::~RosClass() {
    ROS_INFO("Destroying the object");
}

void RosClass::chatterCallback(const message_ui::sent_msg::ConstPtr& msg)
{
  publishMsg.str(""); 
  if (msg->message == "Hello") {
      publishMsg << "Hello, " << myName;
  }
  else if (msg->message == "What is your name?") {
      publishMsg << "My name is MRSD Siri";
  }
  else if (msg->message == "How are you?") {
      publishMsg << "I am fine, thank you";
  }
  else{
    return;
  }

  sendMsg.message = publishMsg.str();
//   ROS_INFO("Need to reply: [%s]", sendMsg.message);
  cout << "Need to reply: " << sendMsg.message << endl;
  chatter_pub.publish(sendMsg);
  sendMsg.message = "";
//   publishMsg.clear();    
}

int main(int argc, char **argv) {

  ros::init(argc, argv, "chatbot_node");
  std::cout << "Chatbot node started" << std::endl;
  RosClass rosClass;
  // ros::NodeHandle n;

  // //Add your code here
  // ros::Subscriber chatter_sub = n.subscribe("sent_msg", 1000, chatterCallback);
  // ros::Publisher chatter_pub = n.advertise<message_ui::sent_msg>("reply_msg", 1000);
  // ros::Publisher chatter_pub_dummy = n.advertise<std_msgs::String>("reply_msg_std", 1000);
  // ros::spin();


  // ros::Rate loop_rate(20);

  // while(ros::ok()) {
  //   std_msgs::String msg;

  //   std::stringstream ss;
  //   ss << "hello world ";// << count;
  //   message_ui::sent_msg msg2;
  //   msg2.message = ss.str();
  //   chatter_pub.publish(msg2);
  //   chatter_pub_dummy.publish(msg);
  //   msg.data = ss.str();
  //   ROS_INFO("%s", msg.data.c_str());

  //   ros::spinOnce();
  //   loop_rate.sleep();
  // }

  return 0;
}