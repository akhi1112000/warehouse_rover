#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "rover_msgs/msg/battery_status.hpp"

class BatteryMonitor : public rclcpp::Node
{
   
public:
    BatteryMonitor() : Node("monitor_battery")
    {
        this->declare_parameter<std::string>("topic_name","battery_status");

        this->declare_parameter<double>("low_battery_threshold",20.00);

        this->declare_parameter<double>("critical_battery_threshold",5.00);

        this->declare_parameter<double>("full_battery_threshold",90.00);


        std::string topic = this->get_parameter("topic_name").as_string();

 low_threshold_ = this->get_parameter("low_battery_threshold").as_double();
 critical_threshold_ = this->get_parameter("critical_battery_threshold").as_double();
 full_threshold_ = this->get_parameter("full_battery_threshold").as_double();

        sub_ = this->create_subscription<rover_msgs::msg::BatteryStatus>(topic,10,[this]
(const rover_msgs::msg::BatteryStatus::SharedPtr msg_sub1)
{monitorBatteryStatus(msg_sub1);});

pub_ = this->create_publisher<std_msgs::msg::String>("command_topic",10);
        
    }


    private:

    void monitorBatteryStatus(const rover_msgs::msg::BatteryStatus::SharedPtr msg_sub)
    {
        
        std_msgs::msg::String msg_pub;

        if(msg_sub->charge_percent <= critical_threshold_ && ! msg_sub->is_charging)
        {
             msg_pub.data = "EMERGENCY_STOP";

           // RCLCPP_WARN(this->get_logger(),"EMERGENCY_STOP");
        }

       else if(msg_sub->charge_percent <=low_threshold_ && ! msg_sub->is_charging)
        {
            msg_pub.data = "RETURN_TO_DOCK";

            //RCLCPP_WARN(this->get_logger(),"RETURN_TO_DOCK");
        }

        

        else if(msg_sub->charge_percent >= full_threshold_ &&  msg_sub->is_charging)
        {
             msg_pub.data = "DOCK_RELEASE";

           // RCLCPP_WARN(this->get_logger(),"DOCK_RELEASE");
        }

       

       

        if( msg_pub.data !="")
        {
            if(sms_ != msg_pub.data )
           { 
            pub_->publish(msg_pub);
            sms_ = msg_pub.data;
           }
        }
        else
        {
            sms_="";
        }

       
    }

    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_;

    rclcpp::Subscription<rover_msgs::msg::BatteryStatus>::SharedPtr sub_;

    std::string sms_ = "Hello";

    double low_threshold_;
    double critical_threshold_;
    double full_threshold_;
    

    

    
};


int main (int argc , char * argv[])
{

    rclcpp::init(argc,argv);

    auto node = std::make_shared<BatteryMonitor>();

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



