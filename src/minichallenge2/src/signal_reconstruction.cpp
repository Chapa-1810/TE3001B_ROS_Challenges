#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <math.h>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32.hpp"
#include "ros_msgs/msg/signal_params.hpp"


using namespace std::chrono_literals;

rclcpp::TimerBase::SharedPtr timer_;
rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr publisher_signal_;
rclcpp::Subscription<ros_msgs::msg::SignalParams>::SharedPtr subscriber_params_;
std::shared_ptr<rclcpp::Node> node_;

void topic_callback(const ros_msgs::msg::SignalParams::SharedPtr msg)
{
    double amplitude = msg->amplitude;
    double frequency = msg->frequency;
    double offset = msg->offset;
    double time = msg->time;
    int type = msg->type;

    auto message = std_msgs::msg::Float32();

    if (type == 1)
      message.data = amplitude * sin(2 * M_PI * frequency * (time)/10) + offset;
    else if (type == 2) // Create Square signal
      message.data = amplitude * sin(2 * M_PI * frequency * (time)/10) > 0 ? amplitude + offset : -amplitude + offset;
    else if (type == 3) // Create Sawtooth signal
      message.data = (fmod((time) * frequency, 1) * 2 * amplitude) + offset;
    else 
      message.data = 0;

    RCLCPP_INFO(node_->get_logger(), "Publishing: '%f'", message.data);

    publisher_signal_->publish(message);
}

int main(int argc, char * argv[])
{ 
  rclcpp::init(argc, argv);
  node_ = rclcpp::Node::make_shared("signal_recontruction");
  publisher_signal_ = node_->create_publisher<std_msgs::msg::Float32>("signal_reconstructed", 1000);
  subscriber_params_ = node_->create_subscription<ros_msgs::msg::SignalParams>("signal_params", 10, std::bind(topic_callback, std::placeholders::_1));
  rclcpp::spin(node_);
  rclcpp::shutdown();
  return 0;
}