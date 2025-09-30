#ifndef GO2_POS_MOVE_HPP
#define GO2_POS_MOVE_HPP

#include <thread>

#include <rclcpp/rclcpp.hpp>
#include <unitree_api/msg/request.hpp>
#include <unitree_api/msg/response.hpp>
#include <unitree_go/msg/sport_mode_state.hpp>

#include "ros2_sport_client.h"

namespace go2_pos_move
{

constexpr char TOPIC_HIGHSTATE[] = "lf/sportmodestate";

enum TestMode
{
  NORMAL_STAND,
  BALANCE_STAND,
  VELOCITY_MOVE,
  STAND_DOWN,
  STAND_UP,
  DAMP,
  RECOVERY_STAND,
  SIT,
  RISE_SIT,
  MOVE,
  STOP_MOVE
};

class Go2SportClientNode : public rclcpp::Node
{
public:
  Go2SportClientNode();
  ~Go2SportClientNode() override;

  void RobotTestControl(TestMode test_mode);
  void GetInitState();
  void HighStateHandler(const unitree_go::msg::SportModeState::SharedPtr msg);

private:
  unitree_go::msg::SportModeState state_{};
  SportClient sport_client_;
  rclcpp::Subscription<unitree_go::msg::SportModeState>::SharedPtr suber_;
  rclcpp::Subscription<unitree_api::msg::Response>::SharedPtr req_suber_;
  rclcpp::TimerBase::SharedPtr timer_;
  unitree_api::msg::Request req_{};
  double px0_{};
  double py0_{};
  double yaw0_{};
  // double ct_{};s
  int flag_{};
  int isPrintHighState_{1};
  // float dt_ = 0.1F;
  std::thread t1_;
};

}  // namespace go2_pos_move

#endif  // GO2_POS_MOVE_HPP
