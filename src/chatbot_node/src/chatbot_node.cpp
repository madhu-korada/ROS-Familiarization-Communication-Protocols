#include <ros/ros.h>
#include <chatbot_node/reply_msg.h>
#include <message_ui/sent_msg.h>
#include <string>
#include <iostream>

using namespace std;

//Add your code here
/**
 * This tutorial demonstrates simple receipt of messages over the ROS system.
 */
void chatterCallback(const message_ui::sent_msg::ConstPtr& msg)
{
  ROS_INFO("I heard: [%s]", msg->message.c_str());
}

int main(int argc, char **argv) {

  ros::init(argc, argv, "chatbot_node");
  ros::NodeHandle n;

  std::cout << "Chatbot node started" << std::endl;
  //Add your code here
  ros::Subscriber sub = n.subscribe("chatter", 1000, chatterCallback);

  ros::spin();


  ros::Rate loop_rate(20);

  while(ros::ok()) {
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}