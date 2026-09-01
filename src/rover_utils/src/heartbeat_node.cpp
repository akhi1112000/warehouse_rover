#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class heartbeat_node : public rclcpp::Node
{
   
public:
    heartbeat_node() : Node("rover")
    {
        this->declare_parameter<std::string>("topic_name","rover_topic");
        this->declare_parameter<int>("publish_rate_ms",500);
        this->declare_parameter<std::string>("message_prefix","hello");

        std::string topic = this->get_parameter("topic_name").as_string();
        int rate = this->get_parameter("publish_rate_ms").as_int();
        prefix_ = this->get_parameter("message_prefix").as_string();

        counter_ = 0 ; 
        pub_ = this->create_publisher<std_msgs::msg::String>(topic,10);

        time_= this->create_wall_timer(
            std::chrono::milliseconds(rate),[this](){publisher_heartbeat();});
    }


    private:

    void publisher_heartbeat()
    {
        std_msgs::msg::String msg;

        msg.data = prefix_ + std::to_string(counter_);

        pub_->publish(msg);

        RCLCPP_INFO(this->get_logger(), "%s", msg.data.c_str());

        counter_ =counter_+1;

    }

    int counter_; 
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_;
    rclcpp::TimerBase::SharedPtr time_;
    std::string prefix_;

    
};


int main (int argc , char * argv[])
{

    rclcpp::init(argc,argv);

    auto node = std::make_shared<heartbeat_node>();

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



