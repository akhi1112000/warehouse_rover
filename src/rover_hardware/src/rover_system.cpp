#include "rover_hardware/rover_system.hpp"
#include <pluginlib/class_list_macros.hpp>
namespace rover_hardware

{
     hardware_interface::CallbackReturn RoverSystem::
     on_init(const hardware_interface::HardwareInfo & info)
    {
       


        if(hardware_interface::SystemInterface::on_init(info) != hardware_interface::CallbackReturn::SUCCESS)
        {

            return hardware_interface::CallbackReturn::ERROR;
        }
        hw_commands_.resize(info_.joints.size(),0.0);
        hw_positions_.resize(info_.joints.size(),0.0);
        hw_velocities_.resize(info_.joints.size(),0.0);

         for(const auto & joint : info_.joints)
        {
            if(joint.command_interfaces.size()!=1)
            {
                RCLCPP_ERROR(rclcpp::get_logger("RoverSystem"),
                 "Joint %s must have exactly one command interface", joint.name.c_str());
                 return hardware_interface::CallbackReturn::ERROR;
            }
            if(joint.command_interfaces[0].name !="velocity")
            {
                RCLCPP_ERROR(rclcpp::get_logger("RoverSystem"),
                 "Joint %s command interface must be velocity", joint.name.c_str());
                 return hardware_interface::CallbackReturn::ERROR;
            }
            if(joint.state_interfaces.size()!=2)
            {
                RCLCPP_ERROR(rclcpp::get_logger("RoverSystem"),
                 "Joint %s must have exactly two state interfaces", joint.name.c_str());
                 return hardware_interface::CallbackReturn::ERROR;
            }
        }
        return hardware_interface::CallbackReturn::SUCCESS;
    }

    
    std::vector<hardware_interface::StateInterface> RoverSystem::export_state_interfaces()
    {
        std::vector<hardware_interface::StateInterface> state_interface;
        for (size_t i = 0; i < info_.joints.size(); i++)
        {
            state_interface.emplace_back(hardware_interface::StateInterface
            (info_.joints[i].name,"position",&hw_positions_[i]));

            state_interface.emplace_back(hardware_interface::StateInterface
            (info_.joints[i].name,"velocity",&hw_velocities_[i]));


        }
        
        return state_interface;
    }

    std::vector<hardware_interface::CommandInterface> RoverSystem::export_command_interfaces()
    {
        std::vector<hardware_interface::CommandInterface> command_interface;
        for (size_t i = 0; i < info_.joints.size(); i++)
        {
            command_interface.emplace_back(hardware_interface::CommandInterface
            (info_.joints[i].name,"velocity",&hw_commands_[i]));

        }
        
        return command_interface;
    }

    hardware_interface::CallbackReturn RoverSystem::on_activate(const rclcpp_lifecycle::State & /*previous_state*/)
    {
        RCLCPP_INFO(rclcpp::get_logger("RoverSystem"), "Activating hardware");
        for (size_t i = 0; i < info_.joints.size(); i++)
        {
            hw_commands_[i] = 0.0;
            hw_velocities_[i] = 0.0;
        }
        return hardware_interface::CallbackReturn::SUCCESS;
        
    }

    hardware_interface::CallbackReturn RoverSystem::on_deactivate(const rclcpp_lifecycle::State & /*previous_state*/)
    {
        RCLCPP_INFO(rclcpp::get_logger("RoverSystem"), "Deactivating hardware");
        for (size_t i = 0; i < info_.joints.size(); i++)
        {
            hw_commands_[i] = 0.0;
            
        }
        return hardware_interface::CallbackReturn::SUCCESS;
        
    }

    hardware_interface::return_type RoverSystem::read(const rclcpp::Time &/*time*/ ,const rclcpp::Duration & period)
    {
        for (size_t i = 0; i < info_.joints.size(); i++)
        {
            hw_positions_[i] = hw_positions_[i] + hw_velocities_[i] * period.seconds();
        }
        
        return hardware_interface::return_type::OK;
       
    }

     hardware_interface::return_type RoverSystem::write(const rclcpp::Time &/*time*/ ,const rclcpp::Duration & /*period*/)
    {
        for (size_t i = 0; i < info_.joints.size(); i++)
        {
            hw_velocities_[i]= hw_commands_[i];
        }
        
        return hardware_interface::return_type::OK;
       
    }


}

PLUGINLIB_EXPORT_CLASS(rover_hardware::RoverSystem, hardware_interface::SystemInterface)