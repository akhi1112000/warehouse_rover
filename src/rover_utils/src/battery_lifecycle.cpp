#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "rover_msgs/msg/battery_status.hpp"
#include "rover_msgs/srv/reset_battery.hpp"
#include "rclcpp/qos.hpp"
#include "rclcpp/rclcpp.hpp"



using CallbackReturn = rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;
using BatteryStatus  =rover_msgs::msg::BatteryStatus;
class BatteryPublisherlifecycle : public rclcpp_lifecycle::LifecycleNode
{
   
public:
    BatteryPublisherlifecycle() : LifecycleNode("rover_battery")
    {
        
        
    }

 CallbackReturn on_configure(const rclcpp_lifecycle::State &)
    {
        RCLCPP_INFO(this->get_logger(), "on_configure");
        rclcpp::QoS qos(10);
        qos.reliable();
        qos.transient_local();
        this->declare_parameter<std::string>("topic_name","battery_status");

        this->declare_parameter<int>("publish_rate_ms",100);
               

        this->declare_parameter<std::string>("message_prefix","the battery_statues");




        std::string topic = this->get_parameter("topic_name").as_string();

        
        rate = this->get_parameter("publish_rate_ms").as_int();
        prefix_ = this->get_parameter("message_prefix").as_string();

        pub_ = this->create_publisher<rover_msgs::msg::BatteryStatus>(topic,qos);

        return CallbackReturn::SUCCESS;
    }
    CallbackReturn on_activate(const rclcpp_lifecycle::State &)
    {
               

        RCLCPP_INFO(this->get_logger(), "on_activate");
        time_ = this->create_wall_timer(
        std::chrono::milliseconds(rate),
        [this](){ publishBatteryStatus(); });
        return CallbackReturn::SUCCESS;
    }

    CallbackReturn on_deactivate(const rclcpp_lifecycle::State &)
    {
        time_.reset();
        RCLCPP_INFO(this->get_logger(), "on_deactivate");
        return CallbackReturn::SUCCESS;
    }
    CallbackReturn on_cleanup(const rclcpp_lifecycle::State &)
    {
        pub_.reset();
        reset_serv_.reset();
        RCLCPP_INFO(this->get_logger(), "on_cleanup");
        return CallbackReturn::SUCCESS;
    }
   
    private:


 void publishBatteryStatus()
    {
        if (is_charging_) {
            counter_ = counter_ + 0.5;
            if (counter_ >= 100.0) { is_charging_ = false; }
        } else {
            counter_ = counter_ - 0.5;
            if (counter_ <= 0.0) { is_charging_ = true; }
        }

        rover_msgs::msg::BatteryStatus msg;
        msg.charge_percent = counter_;
        msg.voltage = 22.0 + (0.03 * counter_);
        msg.is_charging = is_charging_;
        msg.time_remaining_min = static_cast<int>(counter_ * 2);

        if (msg.charge_percent < 20.0) {
            RCLCPP_WARN(this->get_logger(), "battery low: %.1f%%", msg.charge_percent);
        } else {
            RCLCPP_INFO(this->get_logger(), "battery: %.1f%% | %.2fV | charging: %s",
                        msg.charge_percent, msg.voltage,
                        msg.is_charging ? "yes" : "no");
        }

        pub_->publish(msg);
    }


 void onReset(const rover_msgs::srv::ResetBattery::Request::SharedPtr request,
                       rover_msgs::srv::ResetBattery::Response::SharedPtr response)
    {
          if(request->target_level<0 || request->target_level>100)
          {
            response->success=false;
            RCLCPP_WARN(this->get_logger(), "invalid target: %.1f", request->target_level); 
            response->message = "target_level must be between 0 and 100";
            RCLCPP_INFO(this->get_logger(), "%s", response->message.c_str());         
          }
          else
          {
            response->success = true;
            counter_ = request->target_level;
            response->message = "target_level is between 0 and 100";
            RCLCPP_INFO(this->get_logger(),"%s",response->message.c_str());
          }
    }
    
   



    double counter_ =100.00; 
    bool is_charging_ = false;
    int rate;
    
    rclcpp_lifecycle::LifecyclePublisher<BatteryStatus>::SharedPtr pub_;
    rclcpp::TimerBase::SharedPtr time_;
    std::string prefix_;


    rclcpp::Service<rover_msgs::srv::ResetBattery>::SharedPtr reset_serv_;

    
};


int main (int argc , char * argv[])
{

    rclcpp::init(argc,argv);

    auto node = std::make_shared<BatteryPublisherlifecycle>();

    try
    {
        rclcpp::executors::SingleThreadedExecutor executor;
        executor.add_node(node->get_node_base_interface());
        executor.spin();
    }
    catch(const std::exception& e)
    {
    RCLCPP_FATAL(rclcpp::get_logger("main"), "fatal: %s", e.what());
    }
    
    rclcpp::shutdown();

    return 0;


}



