#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "rover_msgs/action/dock_to_charger.hpp"
#include <thread>


using  DockToCharger = rover_msgs::action::DockToCharger;
using handle_target = rclcpp_action::ServerGoalHandle<DockToCharger>;


class dock :public rclcpp::Node
{
    public:
    dock() : Node("dock_serv")
    {
        act_=rclcpp_action::create_server<DockToCharger>(this,"servo",
        
        
        
[this](const rclcpp_action::GoalUUID &uuid1,
std::shared_ptr<const DockToCharger::Goal> check_request_)
{return check_request(uuid1,check_request_);},
        
    
[this](const std::shared_ptr<handle_target> cancell)
{ return cancell_request(cancell);},
        

[this](const std::shared_ptr<handle_target> execute_)
{accept_request(execute_);});

    }
    private:


    rclcpp_action::GoalResponse check_request(
        const rclcpp_action::GoalUUID &uuid,
        std::shared_ptr<const DockToCharger::Goal> check_request_
    )
    {
        (void)uuid;
        if(check_request_->charge_station =="")
        {
            RCLCPP_WARN(this->get_logger(),"THE CHARGE STATION IS EMPTY");
            return rclcpp_action::GoalResponse::REJECT;

        }
        else
        {
           
            RCLCPP_INFO(this->get_logger(),"goal accepted for station X");
            return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;

        }

        

    }

    rclcpp_action::CancelResponse cancell_request(const std::shared_ptr<handle_target> cancell)
    {
        (void)cancell;
        return rclcpp_action::CancelResponse::ACCEPT;

    }

    void accept_request(const std::shared_ptr<handle_target> hand_accept)
    {
        std::thread([this,hand_accept](){execute(hand_accept);}).detach();
        RCLCPP_INFO(this->get_logger(),"TARGET ACCEPTED");
    }

    void execute(const std::shared_ptr<handle_target> execute)
    {
        const auto exec = execute->get_goal();
        auto feedback = std::make_shared<DockToCharger::Feedback>();
        auto result = std::make_shared<DockToCharger::Result>();
        double dist = 5, digree=90;

        rclcpp::Rate loop_rate(1);


        while (dist > 0 || digree > 0)
        {
              if(dist >=0)
            {
                if(dist==0)
                {
                    dist =0;
                }
                else
                {
                     dist = dist - 0.5;
                }
               
                
            }
             if(digree >=0)
            {
                if(digree==0)
                {
                    digree =0;
                }
                else
                {
                     digree = digree-5;
                }
               
                
            }


            if(execute->is_canceling())
            {
                  RCLCPP_WARN(this->get_logger(),"the server cancell the request");
                  execute->canceled(result);
                  return;
            }

            
            

            feedback->remain_dist =dist;
            feedback->remian_angle = digree;

            execute->publish_feedback(feedback);

            loop_rate.sleep();

        }

        result->msg = "finished";
        result->succeeded = true;
        execute->succeed(result);
        

    }

    rclcpp_action::Server<DockToCharger>::SharedPtr act_;

    

};


int main (int argc , char * argv[])
{

    rclcpp::init(argc,argv);

    auto node = std::make_shared<dock>();

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