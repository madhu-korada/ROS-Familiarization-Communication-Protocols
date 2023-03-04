#include <ros/ros.h>
#include <arithmetic_node/arithmetic_reply.h>
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
    arithmetic_node::arithmetic_reply publishMsg;
public:
    RosClass();
    ~RosClass();
    void chatterCallback(const message_ui::sent_msg::ConstPtr& msg);

};
RosClass::RosClass() {

    chatter_sub = n.subscribe("sent_msg", 1000, &RosClass::chatterCallback, this);
    chatter_pub = n.advertise<arithmetic_node::arithmetic_reply>("arithmetic_reply", 1000);
    ros::spin();
}

RosClass::~RosClass() {
    ROS_INFO("Destroying the object");
}

void RosClass::chatterCallback(const message_ui::sent_msg::ConstPtr& msg)
{ 
    string equation = msg->message;
    std::cout << "Received message: " << equation << std::endl;
    double result = 0, num1 = 0, num2 = 0;
    char op = ' ';
    for (int i = 0; i < equation.length(); i++) {
        if (equation[i] == '+' || equation[i] == '-' || equation[i] == '*' || equation[i] == '/') {
            op = equation[i];
            num1 = std::stod(equation.substr(0, i));
            num2 = std::stod(equation.substr(i + 1, equation.length() - i));
            result = op == '+' ? num1 + num2 : op == '-' ? num1 - num2 : op == '*' ? \
                                 num1 * num2 : op == '/' ? num1 / num2 : 0;
            std::cout << "Result: " << result << std::endl;
            publishMsg.oper_type = op == '+' ? "Add" : op == '-' ? "Subtract" : op == '*' ? 
                                                "Multiply" : op == '/' ? "Devide" : "None"; 
            
            publishMsg.answer = result;
            publishMsg.time_received = msg->header.stamp.toSec(); 
            publishMsg.time_answered = ros::Time::now().toSec(); 
            publishMsg.process_time = ros::Time::now().toSec() - msg->header.stamp.toSec();
            chatter_pub.publish(publishMsg);    
            break;
        }
    }
}

int main(int argc, char **argv) {

  ros::init(argc, argv, "arithmetic_node");
  std::cout << "arithmetic node started" << std::endl;
  RosClass rosClass;

  return 0;
}