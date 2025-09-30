#include <chrono>
#include <memory>
#include <thread>

#include <standup_crouchdown.hpp>

namespace go2_h_level_standup
{

Go2SportClientNode::Go2SportClientNode(int test_mode)
  : Node("go2_sport_client_node"),
    sport_client_(this),
    test_mode_(test_mode)
{
  suber_ = this->create_subscription<unitree_go::msg::SportModeState>(
    TOPIC_HIGHSTATE, 1,
    [this](const unitree_go::msg::SportModeState::SharedPtr data)
    {
      HighStateHandler(data);
    });

  t1_ = std::thread([this]
  {
    // Wait for ROS 2 spin to start before issuing commands.
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    RobotControl();
  });
}

Go2SportClientNode::~Go2SportClientNode()
{
  if (t1_.joinable()) {
    t1_.join();
  }
}

void Go2SportClientNode::RobotControl()
{
  ct_ += dt_;
  switch (test_mode_) {
    case NORMAL_STAND:
      sport_client_.StandUp(req_);
      break;
    case BALANCE_STAND:
      sport_client_.BalanceStand(req_);
      break;
    case VELOCITY_MOVE:
      sport_client_.Move(req_, 0.3F, 0.0F, 0.3F);
      break;
    case STAND_DOWN:
      sport_client_.StandDown(req_);
      break;
    case STAND_UP:
      sport_client_.StandUp(req_);
      break;
    case DAMP:
      sport_client_.Damp(req_);
      break;
    case RECOVERY_STAND:
      sport_client_.RecoveryStand(req_);
      break;
    case SIT:
      if (flag_ == 0) {
        sport_client_.Sit(req_);
        flag_ = 1;
      }
      break;
    case RISE_SIT:
      if (flag_ == 0) {
        sport_client_.RiseSit(req_);
        flag_ = 1;
      }
      break;
    case MOVE:
      sport_client_.Move(req_, 0.3F, 0.0F, 0.0F);
      break;
    case STOP_MOVE:
      sport_client_.StopMove(req_);
      break;
    default:
      sport_client_.StopMove(req_);
      break;
  }
}

void Go2SportClientNode::GetInitState()
{
  px0_ = state_.position[0];
  py0_ = state_.position[1];
  yaw0_ = state_.imu_state.rpy[2];
  RCLCPP_INFO(this->get_logger(),
    "initial position: x0: %f, y0: %f, yaw0: %f", px0_, py0_, yaw0_);
}

void Go2SportClientNode::HighStateHandler(
  const unitree_go::msg::SportModeState::SharedPtr msg)
{
  state_ = *msg;
  RCLCPP_INFO(this->get_logger(),
    "Position: %f, %f, %f", state_.position[0], state_.position[1], state_.position[2]);
  RCLCPP_INFO(this->get_logger(),
    "IMU rpy: %f, %f, %f", state_.imu_state.rpy[0], state_.imu_state.rpy[1], state_.imu_state.rpy[2]);
}

}  // namespace go2_h_level_standup
