cmake_minimum_required(VERSION 3.8)
project(go2_h_level_standup)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(unitree_api REQUIRED)
find_package(unitree_go REQUIRED)

include_directories( include include/go2_h_level_standup include/nlohmann include/common)
add_library( ros2_sport_client SHARED src/ros2_sport_client.cpp)
ament_target_dependencies( ros2_sport_client rclcpp unitree_api unitree_go)

add_library( standup_crouchdown SHARED src/standup_crouchdown.cpp)
ament_target_dependencies( standup_crouchdown rclcpp unitree_api unitree_go)
target_link_libraries( standup_crouchdown ros2_sport_client)


add_executable( standup_crouchdown_node src/standup_crouchdown_node.cpp)
target_link_libraries( standup_crouchdown_node standup_crouchdown)
ament_target_dependencies( standup_crouchdown_node rclcpp unitree_api)

install(DIRECTORY include/ DESTINATION include )
install(TARGETS standup_crouchdown ros2_sport_client DESTINATION LIBRARY DESTINATION lib )
install(TARGETS standup_crouchdown_node DESTINATION lib/${PROJECT_NAME})




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
