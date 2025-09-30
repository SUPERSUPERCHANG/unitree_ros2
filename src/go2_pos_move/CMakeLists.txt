cmake_minimum_required(VERSION 3.8)
project(go2_pos_move)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(unitree_api REQUIRED)
find_package(unitree_go REQUIRED)

include_directories( include include/go2_pos_move include/nlohmann include/common)
add_library( ros2_sport_client SHARED src/ros2_sport_client.cpp)
ament_target_dependencies( ros2_sport_client rclcpp unitree_api unitree_go)

add_library( go2_pos_move SHARED src/go2_pos_move.cpp)
ament_target_dependencies( go2_pos_move rclcpp unitree_api unitree_go)
target_link_libraries( go2_pos_move ros2_sport_client)


add_executable( go2_pos_move_node src/go2_pos_move_node.cpp)
target_link_libraries( go2_pos_move_node go2_pos_move)
ament_target_dependencies( go2_pos_move_node rclcpp unitree_api)

install(DIRECTORY include/ DESTINATION include )
install(TARGETS go2_pos_move ros2_sport_client LIBRARY DESTINATION lib )
install(TARGETS go2_pos_move_node DESTINATION lib/${PROJECT_NAME})


if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
