cmake_minimum_required(VERSION 3.8)
project(go2_odom_tf)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(tf2_ros REQUIRED)

include_directories( include include/go2_odom_tf )

# build library (class implementation)
add_library( go2_odom_tf SHARED src/go2_odom_tf.cpp )
ament_target_dependencies( go2_odom_tf rclcpp nav_msgs geometry_msgs tf2_ros )

# build node executable (main)
add_executable( go2_odom_tf_node src/go2_odom_tf_node.cpp )
target_link_libraries( go2_odom_tf_node go2_odom_tf )
ament_target_dependencies( go2_odom_tf_node rclcpp nav_msgs geometry_msgs tf2_ros )

# install headers, library, and node
install(DIRECTORY include/ DESTINATION include )
install(TARGETS go2_odom_tf LIBRARY DESTINATION lib )
install(TARGETS go2_odom_tf_node DESTINATION lib/${PROJECT_NAME} )


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
