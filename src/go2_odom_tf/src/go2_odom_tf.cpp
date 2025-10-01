#include "go2_odom_tf/go2_odom_tf.hpp"

namespace go2_odom_tf
{

Go2OdomTf::Go2OdomTf()
: rclcpp::Node("go2_odom_tf_node"),
  tfBroadcaster_(std::make_unique<tf2_ros::TransformBroadcaster>(*this))
{
  // Hardcode topic and QoS
  std::string odomTopic = "/utlidar/robot_odom";   // subscribe to this odometry topic
  auto qos = rclcpp::SystemDefaultsQoS();

  // Subscribe using lambda
  odomSub_ = this->create_subscription<nav_msgs::msg::Odometry>(
    odomTopic, qos,
    [this](const nav_msgs::msg::Odometry::SharedPtr msg) {
      this->onOdom(msg);
    });

  RCLCPP_INFO(get_logger(), "Subscribed to odometry topic: %s", odomTopic.c_str());
}

void Go2OdomTf::onOdom(const nav_msgs::msg::Odometry::SharedPtr msg)
{
  // Parent frame = header.frame_id (default to "odom" if empty)
  std::string parentFrame = msg->header.frame_id.empty() ? "odom" : msg->header.frame_id;

  // Child frame = child_frame_id (fallback "base_link" if empty)
  std::string childFrame = msg->child_frame_id.empty() ? "base_link" : msg->child_frame_id;

  geometry_msgs::msg::TransformStamped tfMsg;
  tfMsg.header.stamp = msg->header.stamp;
  tfMsg.header.frame_id = parentFrame;
  tfMsg.child_frame_id = childFrame;

  // Copy pose -> transform
  tfMsg.transform.translation.x = msg->pose.pose.position.x;
  tfMsg.transform.translation.y = msg->pose.pose.position.y;
  tfMsg.transform.translation.z = msg->pose.pose.position.z;

  tfMsg.transform.rotation = msg->pose.pose.orientation;

  // Broadcast
  tfBroadcaster_->sendTransform(tfMsg);
  //RCLCPP_INFO(get_logger(), "Sent TF from %s to %s", parentFrame.c_str(), childFrame.c_str());

}
}  // namespace go2_odom_tf
