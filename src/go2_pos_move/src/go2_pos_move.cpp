#include <go2_pos_move.hpp>

namespace go2_pos_move
{

Go2SportClientNode::Go2SportClientNode()
  : Node("go2_pos_move_node"),
    sport_client_(this)
{
  suber_ = this->create_subscription<unitree_go::msg::SportModeState>(
    TOPIC_HIGHSTATE, 1,
    [this](const unitree_go::msg::SportModeState::SharedPtr data)
    {
      HighStateHandler(data);
    });
    
  posMoveSub_ = this->create_subscription<geometry_msgs::msg::Pose2D>(
    "go2_pos_move",                                            
    10,                                                        
    [this](const geometry_msgs::msg::Pose2D::SharedPtr pose) {
      go2PosMoveHandler(pose);
    });

  tfBuffer_   = std::make_unique<tf2_ros::Buffer>(this->get_clock());
  tfListener_ = std::make_shared<tf2_ros::TransformListener>(*tfBuffer_);

  //   t1_ = std::thread([this]
  // {
  //   // Wait for ROS 2 spin to start before issuing commands.
  //   std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  //   RobotTestControl(MOVE);
  // });
}

Go2SportClientNode::~Go2SportClientNode(){
  if (t1_.joinable()) {
    t1_.join();
  }
};

void Go2SportClientNode::RobotTestControl(TestMode test_mode)
{
  switch (test_mode) {
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
  if (isPrintHighState_) {
    state_ = *msg;
  RCLCPP_INFO(this->get_logger(),
    "Position: %f, %f, %f", state_.position[0], state_.position[1], state_.position[2]);
  RCLCPP_INFO(this->get_logger(),
    "IMU rpy: %f, %f, %f", state_.imu_state.rpy[0], state_.imu_state.rpy[1], state_.imu_state.rpy[2]);
  }

}

void Go2SportClientNode::go2PosMoveHandler(
    const geometry_msgs::msg::Pose2D::SharedPtr pose)
{
  RCLCPP_INFO(this->get_logger(),
              "Received target pose: x=%.3f, y=%.3f, theta=%.3f",
              pose->x, pose->y, pose->theta);

  // 1) Read current pose from TF
  double curX = 0.0, curY = 0.0, curYaw = 0.0;
  if (!getCurrentPoseFromTf(curX, curY, curYaw)) {
    // If TF not ready, we can decide to skip or just pass through the target as-is.
    RCLCPP_WARN(this->get_logger(),
                "TF pose is unavailable. Please ensure TF tree is online.");
    // sport_client_.Move(req_, pose->x, pose->y, pose->theta);
    return;
  }

  // 2) Compute deltas in the same frame of Pose2D (assumed odom frame)
  const double dx     = pose->x - curX;
  const double dy     = pose->y - curY;
  const double dtheta = normalizeAngle(pose->theta - curYaw);

  RCLCPP_INFO(this->get_logger(),
              "Target(odom): x=%.3f, y=%.3f, yaw=%.3f | Current: x=%.3f, y=%.3f, yaw=%.3f | Delta: dx=%.3f, dy=%.3f, dθ=%.3f",
              pose->x, pose->y, pose->theta, curX, curY, curYaw, dx, dy, dtheta);

  // 3) Send incremental command to robot
  // NOTE: If SportClient::Move expects velocities (vx, vy, yaw_rate),
  //       you may need a controller to convert (dx,dy,dθ) into velocity setpoints over time.
  //       Here we follow your original usage and pass the deltas directly.
  sport_client_.Move(req_, static_cast<float>(dx),
                          static_cast<float>(dy),
                          static_cast<float>(dtheta));

  RCLCPP_INFO(this->get_logger(), "Sent incremental Move: (%.3f, %.3f, %.3f).",
              dx, dy, dtheta);
  
}

bool Go2SportClientNode::getCurrentPoseFromTf(double &x, double &y, double &yaw)
{
  // We assume the target Pose2D is expressed in the "odom" frame,
  // and robot base is "base_link". Change names here if your frames differ.
  static const std::string parent = "odom";
  static const std::string child  = "base_link";

  try {
    // Use the latest available transform
    geometry_msgs::msg::TransformStamped tfMsg =
        tfBuffer_->lookupTransform(parent, child, tf2::TimePointZero);

    x = tfMsg.transform.translation.x;
    y = tfMsg.transform.translation.y;

    // Extract yaw from quaternion
    yaw = tf2::getYaw(tfMsg.transform.rotation);
    return true;
  } catch (const tf2::TransformException &ex) {
    RCLCPP_WARN(this->get_logger(), "TF lookup failed: %s", ex.what());
    return false;
  }
}

double Go2SportClientNode::normalizeAngle(double a)
{
  // Wrap to [-pi, pi]
  while (a > M_PI)  a -= 2.0 * M_PI;
  while (a < -M_PI) a += 2.0 * M_PI;
  return a;
}

}  // namespace go2_pos_move
