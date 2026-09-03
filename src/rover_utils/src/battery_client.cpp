#include "rclcpp/rclcpp.hpp"
#include "rover_msgs/msg/battery_status.hpp"
#include "rover_msgs/srv/reset_battery.hpp"




class BatteryCli : public rclcpp::Node
{
   
public:
    BatteryCli() : Node("battery_client")
    {
        
        client_ = this->create_client<rover_msgs::srv::ResetBattery>("reset_battery");

        send_request();
    }


   
   


    private:

     void send_request()
    {

        if (!client_->wait_for_service(std::chrono::seconds(2)))
        {
             RCLCPP_ERROR(this->get_logger(), "service not available");
    return;
        }

        auto request_ = std::make_shared<rover_msgs::srv::ResetBattery::Request>();
        request_->target_level=150;

        client_ ->async_send_request(request_ ,
             [this](rclcpp::Client<rover_msgs::srv::ResetBattery>::SharedFuture future){
                auto response = future.get();
                RCLCPP_INFO(this->get_logger(), "success: %d, msg: %s",
                    response->success, response->message.c_str());
             });
        
    }


    rclcpp::Client<rover_msgs::srv::ResetBattery>::SharedPtr client_;




    
};


int main (int argc , char * argv[])
{

    rclcpp::init(argc,argv);

    auto node = std::make_shared<BatteryCli>();

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

