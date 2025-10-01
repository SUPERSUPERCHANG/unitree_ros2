#include "rclcpp/rclcpp.hpp"
#include "go2_odom_tf/go2_odom_tf.hpp"

int main(int argc, char ** argv)
{
  // Initialize ROS 2, spin the node, then cleanly shutdown.
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<go2_odom_tf::Go2OdomTf>());
  rclcpp::shutdown();
  return 0;
}


