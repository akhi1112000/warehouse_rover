#include "rclcpp/rclcpp.hpp"
#include "rover_msgs/msg/battery_status.hpp"

class BatteryPublisher : public rclcpp::Node
{
   
public:
    BatteryPublisher() : Node("rover_battery")
    {
        this->declare_parameter<std::string>("topic_name","battery_status");

        this->declare_parameter<int>("publish_rate_ms",100);

        this->declare_parameter<std::string>("message_prefix","the battery_statues");




        std::string topic = this->get_parameter("topic_name").as_string();

        int rate = this->get_parameter("publish_rate_ms").as_int();

        prefix_ = this->get_parameter("message_prefix").as_string();

        pub_ = this->create_publisher<rover_msgs::msg::BatteryStatus>(topic,10);

        time_= this->create_wall_timer(
            std::chrono::milliseconds(rate),[this](){publishBatteryStatus();});
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

    double counter_ =100.00; 
    bool is_charging_ = false;
    rclcpp::Publisher<rover_msgs::msg::BatteryStatus>::SharedPtr pub_;
    rclcpp::TimerBase::SharedPtr time_;
    std::string prefix_;

    
};


int main (int argc , char * argv[])
{

    rclcpp::init(argc,argv);

    auto node = std::make_shared<BatteryPublisher>();

    try
    {
            rclcpp::spin(node);

    }
    catch(const std::exception& e)
    {
    RCLCPP_FATAL(rclcpp::get_logger("main"), "fatal: %s", e.what());
    }
    
    rclcpp::shutdown();

    return 0;


}



