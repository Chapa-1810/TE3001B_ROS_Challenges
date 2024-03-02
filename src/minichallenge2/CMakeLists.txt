cmake_minimum_required(VERSION 3.8)
project(minichallenge2)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(ros_msgs REQUIRED)

add_executable(signal_generator src/signal_generator.cpp)
ament_target_dependencies(signal_generator rclcpp std_msgs ros_msgs)
add_executable(signal_reconstruction src/signal_reconstruction.cpp)
ament_target_dependencies(signal_reconstruction rclcpp std_msgs ros_msgs)
# ament_target_dependencies(signal_generator rclcpp std_msgs)

install(TARGETS
  signal_generator
  signal_reconstruction
  DESTINATION lib/${PROJECT_NAME})

install(
    DIRECTORY config
    DESTINATION share/${PROJECT_NAME}
  )

ament_package()
