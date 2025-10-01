#ifndef GO2_ODOM_TF_HPP_
#define GO2_ODOM_TF_HPP_

#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "tf2_ros/transform_broadcaster.h"

namespace go2_odom_tf
{

class Go2OdomTf : public rclcpp::Node
{
public:
  Go2OdomTf();

private:
  void onOdom(const nav_msgs::msg::Odometry::SharedPtr msg);

  rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr odomSub_;
  std::unique_ptr<tf2_ros::TransformBroadcaster> tfBroadcaster_;
};

}  // namespace go2_odom_tf

#endif  // GO2_ODOM_TF_HPP_
