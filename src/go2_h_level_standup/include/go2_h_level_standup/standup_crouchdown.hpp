#ifndef GO2_H_LEVEL_STANDUP_STANDUP_CROUCHDOWN_HPP
#define GO2_H_LEVEL_STANDUP_STANDUP_CROUCHDOWN_HPP

#include <thread>

#include <rclcpp/rclcpp.hpp>
#include <unitree_api/msg/request.hpp>
#include <unitree_api/msg/response.hpp>
#include <unitree_go/msg/sport_mode_state.hpp>

#include "ros2_sport_client.h"

namespace go2_h_level_standup
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
  explicit Go2SportClientNode(int test_mode);
  ~Go2SportClientNode() override;

  void RobotControl();
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
  double ct_{};
  int flag_{};
  float dt_ = 0.1F;
  int test_mode_{};
  std::thread t1_;
};

}  // namespace go2_h_level_standup

#endif  // GO2_H_LEVEL_STANDUP_STANDUP_CROUCHDOWN_HPP

