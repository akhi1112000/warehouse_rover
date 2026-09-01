#include "rclcpp/rclcpp.hpp"

#include "std_msgs/msg/string.hpp"



class HeartbeatMonitor : public rclcpp::Node
{
    public:
    
    HeartbeatMonitor(): Node("heartbeat_monitor")
    {
        this->declare_parameter<std::string>("topic_name","rover_topic");

        std::string topic = this->get_parameter("topic_name").as_string();

        sub_=this->create_subscription<std_msgs::msg::String>(topic,10,
        [this](const  std_msgs::msg::String::SharedPtr msg_ptr){call_back(msg_ptr);});

    }

    private:

    void call_back(const  std_msgs::msg::String::SharedPtr  msg_ptr)
    {
        RCLCPP_INFO(this->get_logger(),"i heared that : %s",msg_ptr->data.c_str());
    }

    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub_;
};


int main (int argc , char * argv[])
{

    rclcpp::init(argc,argv);

    auto node = std::make_shared<HeartbeatMonitor>();

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
