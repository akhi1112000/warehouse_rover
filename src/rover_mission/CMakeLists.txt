cmake_minimum_required(VERSION 3.8)
project(rover_mission)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(nav2_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(rover_interfaces REQUIRED)

add_executable(rover_action src/move_to_shelf_server.cpp)
ament_target_dependencies(rover_action rclcpp rclcpp_action nav2_msgs geometry_msgs rover_interfaces)


install(TARGETS rover_action

  DESTINATION lib/${PROJECT_NAME}
)


ament_package()
