#include "rover_hardware/rover_system.hpp"



namespace rover_hardware
{
 
 
 
hardware_interface::CallbackReturn RoverSystem::on_init
(const hardware_interface::HardwareInfo & info)

{
    if(hardware_interface::SystemInterface::on_init(info) 
    != hardware_interface::CallbackReturn::SUCCESS)
    {

         return hardware_interface::CallbackReturn::ERROR;
 
    }
      hw_commands_.resize(info_.joints.size(),0.0);
      hw_positions_.resize(info_.joints.size(),0.0);
      hw_velocities_.resize(info_.joints.size(),0.0);

      for(const auto & joint : info_.joints )
      {
        if(joint.command_interfaces != 1)
        {
            RCLCPP_ERROR(rclcpp::get_logger("RoverSystem"), "Joint %s must have exactly one command interface", joint.name.c_str());
            return hardware_interface::CallbackReturn::ERROR;
        }
      }
      return hardware_interface::CallbackReturn::SUCCESS;
} 
  

}


