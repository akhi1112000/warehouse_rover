cmake_minimum_required(VERSION 3.8)
project(rover_hardware)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(hardware_interface REQUIRED)
find_package(pluginlib REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)



add_library(rover_hardware SHARED src/rover_system.cpp)

target_include_directories(rover_hardware PUBLIC 
 $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
$<INSTALL_INTERFACE:include>

)




ament_target_dependencies(rover_hardware
  hardware_interface
  pluginlib
  rclcpp
  rclcpp_lifecycle
)


pluginlib_export_plugin_description_file(hardware_interface rover_hardware.xml)

install(TARGETS rover_hardware
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)


install(DIRECTORY include/
  DESTINATION include
)

ament_package()
