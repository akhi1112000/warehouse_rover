#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "rover_msgs/action/pick_package.hpp"
#include <thread>


using  PickPackage = rover_msgs::action::PickPackage;
using handle_goal = rclcpp_action::ServerGoalHandle<PickPackage>;


class Shelf :public rclcpp::Node
{
    public:
    Shelf() : Node("PKG_UP")
    {
             act_=rclcpp_action::create_server<PickPackage>(this,"serv",

           [this](const rclcpp_action::GoalUUID & uuid, 
               std::shared_ptr<const PickPackage::Goal> check1) {
            return goalAccept(uuid, check1);
        },


            [this](const std::shared_ptr< handle_goal> check2){return goalCancell(check2);},
            
            [this](const std::shared_ptr<handle_goal>check){handl_goal(check);}
            );

    }
    private:

    rclcpp_action::GoalResponse goalAccept
    (
        const rclcpp_action::GoalUUID& uuid,
        std::shared_ptr<const PickPackage::Goal> check
        
    )
    {
        (void)uuid;
        if(check->hight_m > 3 || check->hight_m < 0 || check->pkg=="")
        {
                RCLCPP_WARN(this->get_logger(),
                "THE HEIGHT : %f & THE ID : %s ARE NOT OK ",check->hight_m,check->pkg.c_str());
                return rclcpp_action::GoalResponse::REJECT;
        }

        else
        {
             RCLCPP_INFO(this->get_logger(),
             "THE HEIGHT : %f & THE ID : %s ARE OK ",check->hight_m,check->pkg.c_str());
            return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
        }
        
    }



    rclcpp_action::CancelResponse goalCancell(const std::shared_ptr< handle_goal> check)
    {
        (void) check;
        return rclcpp_action::CancelResponse::ACCEPT;
    }
    

    void handl_goal(const std::shared_ptr<handle_goal> check)
    {
        std::thread([this,check](){execute(check);}).detach();
    }




     void execute(const std::shared_ptr<handle_goal> execute)
    {

        auto feedBack = std::make_shared<PickPackage::Feedback>();
        auto result   = std::make_shared<PickPackage::Result>();

        
        double achieve = 0;
        
        rclcpp::Rate loop_rate(1);

       
    for (int i = 1; i <5; i++)  
     {
        
            if(execute->is_canceling())
            {
                  RCLCPP_WARN(this->get_logger(),"the server cancell the request");
                  execute->canceled(result);
                  return;
            }

        if (i==1)
        {
                feedBack->current_state = "THE ARM BEING LIFTED"; 
                feedBack->achievment_p = achieve;
                achieve =25;
                execute->publish_feedback(feedBack);
                
        }
        else if (i==2)
        {
            feedBack->current_state = "THE ARM HANDS THE PKG"; 
                feedBack->achievment_p = achieve;
                achieve = 50;
                execute->publish_feedback(feedBack);
                
        }
        else if (i==3)
        {
           feedBack->current_state = "THE PKG IS BEING WITHDRAWLED"; 
                feedBack->achievment_p = achieve;
                 achieve = 75;
                execute->publish_feedback(feedBack);
               
        }
        else if (i==4)
        {
            feedBack->current_state = "THE ARM BEING DOWN"; 
                feedBack->achievment_p = achieve;
                achieve = 100;
                execute->publish_feedback(feedBack);
                
        }

        loop_rate.sleep();
     }
             
     result->msg="SUCCEED";
     
     result->pkg_wieght = 30;
     result->succedded = true;

     execute->succeed(result);

    }

    rclcpp_action::Server<PickPackage>::SharedPtr act_;
    

    

};


int main (int argc , char * argv[])
{

    rclcpp::init(argc,argv);

    auto node = std::make_shared<Shelf>();

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