#include <cstdlib>
#include <iostream>
#include <memory>

#include <rclcpp/executors/multi_threaded_executor.hpp>
#include <rclcpp/rclcpp.hpp>

#include <standup_crouchdown.hpp>

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);

  int test_mode = go2_h_level_standup::MOVE;
  
  // if (argc > 1) {
  //   test_mode = std::atoi(argv[1]);  // NOLINT
  // } else {
  //   std::cerr << "Usage: " << argv[0] << " <test_mode>" << std::endl;
  //   std::cerr << "Defaulting to NORMAL_STAND (0)." << std::endl;
  // }

  auto node = std::make_shared<go2_h_level_standup::Go2SportClientNode>(test_mode);
  
  node->GetInitState();

  rclcpp::executors::MultiThreadedExecutor executor;
  executor.add_node(node);
  executor.spin();

  rclcpp::shutdown();
  return 0;
}
