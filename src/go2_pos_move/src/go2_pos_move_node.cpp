#include <go2_pos_move.hpp>

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<go2_pos_move::Go2SportClientNode>();
    node->GetInitState();
    // node->RobotTestControl(go2_pos_move::MOVE);
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}