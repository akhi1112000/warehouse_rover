cmake_minimum_required(VERSION 3.8)
project(rover_utils)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(rover_msgs REQUIRED)

add_executable(heart_node src/heartbeat_node.cpp)
ament_target_dependencies(heart_node rclcpp std_msgs)

add_executable(monitor_node src/heartbeat_monitor.cpp)
ament_target_dependencies(monitor_node rclcpp std_msgs)

add_executable(battery_node src/battery_publisher.cpp)
ament_target_dependencies(battery_node rclcpp rover_msgs)

add_executable(battery_monitor src/battery_monitor.cpp)
ament_target_dependencies(battery_monitor rclcpp rover_msgs std_msgs)

install(TARGETS heart_node monitor_node battery_node battery_monitor
  DESTINATION lib/${PROJECT_NAME})

install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME})

ament_package()
