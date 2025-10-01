from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():
    rviz_config_path = os.path.join("config",
        "config.rviz"
    )

    return LaunchDescription([
        
        Node(
            package="go2_odom_tf",
            executable="go2_odom_tf_node",
            name="go2_odom_tf_node",
            output="screen"
        ),

        
        Node(
            package="go2_pos_move",
            executable="go2_pos_move_node",
            name="go2_pos_move_node",
            output="screen"
        ),

        # rviz2
        Node(
            package="rviz2",
            executable="rviz2",
            name="rviz2",
            output="screen",
            #arguments=["-d", rviz_config_path]
        ),
    ])
