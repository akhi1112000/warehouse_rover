#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "rover_msgs/msg/battery_status.hpp"
#include "rover_msgs/srv/reset_battery.hpp"
#include "rover_msgs/srv/get_last_command.hpp"




class CollectionNode : public rclcpp::Node
{
   
    public:
    CollectionNode():Node("collect")
    {
        this->declare_parameter<double>("high_level",90.00);
        this->declare_parameter<double>("low_level",20.00);
        this->declare_parameter<double>("critical_level",5.00);

        high_level=this->get_parameter("high_level").as_double();
         low_level=this->get_parameter("low_level").as_double();
          critical_level=this->get_parameter("critical_level").as_double();


        pub_ = this->create_publisher<std_msgs::msg::String>("rover_command",10);

        sub_=this->create_subscription<rover_msgs::msg::BatteryStatus>("battery_status",10,
        [this](rover_msgs::msg::BatteryStatus::SharedPtr msg_rev_1){call_back_sub(msg_rev_1);});

        serv_=this->create_service<rover_msgs::srv::GetLastCommand>("my_server",[this](
            const rover_msgs::srv::GetLastCommand::Request::SharedPtr req_1,
                             rover_msgs::srv::GetLastCommand::Response::SharedPtr res_1
        ){call_back_srv(req_1,res_1);});


        client_ = this->create_client<rover_msgs::srv::ResetBattery>("reset_battery");
    }


   
   


    private:

    void call_back_sub(rover_msgs::msg::BatteryStatus::SharedPtr msg_rev)
    {
        std_msgs::msg::String info;

        if (msg_rev->charge_percent <critical_level && !msg_rev->is_charging)
        {
            info.data = "EMERGENCY_STOP";
        }
        else if(msg_rev->charge_percent <=low_level && ! msg_rev->is_charging)
        {
            info.data = "RETURN_TO_DOCK";
        }
        else if(msg_rev->charge_percent >= high_level &&  msg_rev->is_charging)
        {
             info.data = "DOCK_RELEASE";
        }


         if(msg_rev->charge_percent <critical_level && reset_sent_ ==false)
           {
            sendReset();
            reset_sent_ =true;
           }

           else if(msg_rev->charge_percent >critical_level )
           {
           reset_sent_ =false;
           }

         if( info.data !="")
        {
            if(sms_ != info.data )
           { 
            pub_->publish(info);
            c_count_ = c_count_+1;
            sms_ = info.data;
            last_command_ = info.data;
           }
          
        }
        else
        {
            sms_="";
        }
        
        

       
        

    }

    void call_back_srv(const rover_msgs::srv::GetLastCommand::Request::SharedPtr req_,
                             rover_msgs::srv::GetLastCommand::Response::SharedPtr res_)
        {

            (void)req_;

            res_->last_command = last_command_;
            res_->command_count = c_count_;
            
            
        }


     void sendReset()
        {
           if(!client_->service_is_ready())
           {
            return;
           }

           auto request = std::make_shared<rover_msgs::srv::ResetBattery::Request>();

           request->target_level=100;


           client_->async_send_request(request, [this](rclcpp::Client<rover_msgs::srv::ResetBattery>::SharedFuture future){
            auto response = future.get();
             RCLCPP_INFO(this->get_logger(), "success: %d, msg: %s",
                    response->success, response->message.c_str());
           });
        }

    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_;
    rclcpp::Subscription<rover_msgs::msg::BatteryStatus>::SharedPtr sub_;
    rclcpp::Service<rover_msgs::srv::GetLastCommand>::SharedPtr serv_;
    rclcpp::Client<rover_msgs::srv::ResetBattery>::SharedPtr client_;
    std::string sms_ ;
    std::string last_command_;
    double high_level;
    double low_level;
    double critical_level;
    int c_count_ = 0;
    bool reset_sent_ =false;
    




    
};


int main (int argc , char * argv[])
{

    rclcpp::init(argc,argv);

    auto node = std::make_shared<CollectionNode>();

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

