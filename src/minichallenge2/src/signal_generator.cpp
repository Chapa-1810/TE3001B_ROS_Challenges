#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <math.h>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32.hpp"
#include "ros_msgs/msg/signal_params.hpp"
// #include "ros_msgs/msg/signalparams.hpp"

using namespace std::chrono_literals;

rclcpp::TimerBase::SharedPtr timer_;
rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr publisher_signal_;
rclcpp::Publisher<ros_msgs::msg::SignalParams>::SharedPtr publisher_params_;
long long time_;
std::shared_ptr<rclcpp::Node> node_;

void timer_callback()
{
  // Create a 2hz Sine signal with amplitude 0.5 and send it down the topic
  double amplitude = node_->get_parameter("amplitude").as_double();
  double frequency = node_->get_parameter("frequency").as_double();
  double offset = node_->get_parameter("offset").as_double();
  double time = node_->get_parameter("time").as_double();
  int type = node_->get_parameter("type").as_int();

  float curr_time = (time_ + time);
  auto message = std_msgs::msg::Float32();
  if (type == 1)
    message.data = amplitude * sin(2 * M_PI * frequency * (curr_time)/10) + offset;
  else if (type == 2) // Create Square signal
    message.data = amplitude * sin(2 * M_PI * frequency * (curr_time)/10) > 0 ? amplitude + offset : -amplitude + offset;
  else if (type == 3) // Create Sawtooth signal
    message.data = (fmod((curr_time) * frequency, 1) * 2 * amplitude) + offset;
  else 
    message.data = 0;
  RCLCPP_INFO(node_->get_logger(), "Publishing: '%f'", message.data);
  publisher_signal_->publish(message);

  auto params = ros_msgs::msg::SignalParams();
  params.amplitude = amplitude;
  params.frequency = frequency;
  params.offset = offset;
  params.type = type;
  params.time = curr_time;

  publisher_params_->publish(params);
  
  RCLCPP_INFO(node_->get_logger(), "Publishing signal params");
  
  time_++;
}

int main(int argc, char * argv[])
{ 
  rclcpp::init(argc, argv);
  node_ = rclcpp::Node::make_shared("signal_generator");
  node_->declare_parameter("type", 1);
  node_->declare_parameter("amplitude", 0.5);
  node_->declare_parameter("frequency", 2.0);
  node_->declare_parameter("offset", 0.0);
  node_->declare_parameter("time", 0.0);
  publisher_signal_ = node_->create_publisher<std_msgs::msg::Float32>("signal", 10);
  publisher_params_ = node_->create_publisher<ros_msgs::msg::SignalParams>("signal_params", 10);
  timer_ = node_->create_wall_timer(50ms, timer_callback);

  rclcpp::spin(node_);
  rclcpp::shutdown();
  return 0;
}