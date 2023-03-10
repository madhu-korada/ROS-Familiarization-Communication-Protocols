#include <ros/ros.h>
#include <chatbot_node/reply_msg.h>
#include <message_ui/sent_msg.h>
#include <arithmetic_node/arithmetic_reply.h>
#include <counter_node/counter.h>

int num_reply_msg = 0;
int num_sent_msg = 0;

ros::Time last_sent_msg_time;
ros::Time last_reply_msg_time;

ros::Subscriber reply_msg_sub;
ros::Subscriber arithmetic_reply_msg_sub;
ros::Subscriber sent_msg_sub;

void sent_msg_callback(const message_ui::sent_msg msg)
{
	num_sent_msg++;
	last_sent_msg_time = msg.header.stamp;
}

void reply_msg_callback(const chatbot_node::reply_msg msg)
{
	num_reply_msg++;
	last_reply_msg_time = msg.header.stamp;
}

void arithmetic_reply_msg_callback(const arithmetic_node::arithmetic_reply msg)
{
	num_reply_msg++;
	last_reply_msg_time = msg.header.stamp;
}

bool callback(counter_node::counter::Request& request, counter_node::counter::Response& response)
{
  ROS_INFO("Service Requested ...");
  int request_code = request.req_id;
  switch (request_code)
  {
  case 0:
    response.reply = num_sent_msg + num_reply_msg;
    break;
  case 1:
    response.reply = num_reply_msg;
    break;
  case 2:
    response.reply = num_sent_msg;
    break;
  case 3:
    response.reply = ros::Time::now().toSec() - last_reply_msg_time.toSec();
    break;
  case 4:
    response.reply = ros::Time::now().toSec() - last_sent_msg_time.toSec();
    break;
  default:
    break;
  }
  return true;
}


int main(int argc, char **argv) {

  ros::init(argc, argv, "counter_node");
  ros::NodeHandle n;

  reply_msg_sub = n.subscribe("reply_msg", 1000, reply_msg_callback);
  sent_msg_sub = n.subscribe("sent_msg", 1000, sent_msg_callback);
  
  arithmetic_reply_msg_sub = n.subscribe("arithmetic_reply", 1000, arithmetic_reply_msg_callback);

  ros::Time begin = ros::Time::now();
  ros::ServiceServer service = n.advertiseService("message_counter", callback);


  ros::Rate loop_rate(20);

  while(ros::ok()) {
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}