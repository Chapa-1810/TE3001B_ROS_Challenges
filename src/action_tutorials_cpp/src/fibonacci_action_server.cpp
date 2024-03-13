#include <functional>
#include <memory>
#include <thread>

#include "action_tutorials_interfaces/action/fibonacci.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "rclcpp_components/register_node_macro.hpp"

#include "action_tutorials_cpp/visibility_control.h"


using Fibonacci = action_tutorials_interfaces::action::Fibonacci;
using GoalHandleFibonacci = rclcpp_action::ServerGoalHandle<Fibonacci>;

rclcpp_action::GoalResponse handle_goal(
const rclcpp_action::GoalUUID & uuid,
std::shared_ptr<const Fibonacci::Goal> goal)
{
RCLCPP_INFO(node_->get_logger(), "Received goal request with order %d", goal->order);
    (void)uuid;
    return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
}

rclcpp_action::CancelResponse handle_cancel(
const std::shared_ptr<GoalHandleFibonacci> goal_handle)
{
RCLCPP_INFO(node_->get_logger(), "Received request to cancel goal");
    (void)goal_handle;
    return rclcpp_action::CancelResponse::ACCEPT;
}

void handle_accepted(const std::shared_ptr<GoalHandleFibonacci> goal_handle)
{
    execute(goal_handle);
}

void execute(const std::shared_ptr<GoalHandleFibonacci> goal_handle)
{
    RCLCPP_INFO(node_->get_logger(), "Executing goal");
    rclcpp::Rate loop_rate(1);
    const auto goal = goal_handle->get_goal();
    auto feedback = std::make_shared<Fibonacci::Feedback>();
    auto & sequence = feedback->partial_sequence;
    sequence.push_back(0);
    sequence.push_back(1);
    auto result = std::make_shared<Fibonacci::Result>();

    for (int i = 1; (i < goal->order) && rclcpp::ok(); ++i) {
        // Check if there is a cancel request
        if (goal_handle->is_canceling()) {
        result->sequence = sequence;
        goal_handle->canceled(result);
        RCLCPP_INFO(node_->get_logger(), "Goal canceled");
        return;
        }
        // Update sequence
        sequence.push_back(sequence[i] + sequence[i - 1]);
        // Publish feedback
        goal_handle->publish_feedback(feedback);
        RCLCPP_INFO(node_->get_logger(), "Publish feedback");

        loop_rate.sleep();
    }

    // Check if goal is done
    if (rclcpp::ok()) {
        result->sequence = sequence;
        goal_handle->succeed(result);
        RCLCPP_INFO(node_->get_logger(), "Goal succeeded");
    }
}

rclcpp_action::Server<Fibonacci>::SharedPtr action_server_;
std::shared_ptr<rclcpp::Node> node_;

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  node_ = rclcpp::Node::make_shared("fibonacci_server");

  action_server_ = rclcpp_action::create_server<Fibonacci>(
      node_,
      "fibonacci",
      std::bind(handle_goal, node_, std::placeholders::_1, std::placeholders::_2),
      std::bind(handle_cancel, node_, std::placeholders::_1),
      std::bind(handle_accepted, node_, std::placeholders::_1));

  rclcpp::spin(std::make_shared<action_tutorials_cpp::FibonacciActionServer>());
  rclcpp::shutdown();
  return 0;
}