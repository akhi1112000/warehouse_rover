#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "rover_msgs/action/move_to_shelf.hpp"
#include <thread>

using MoveToShelf = rover_msgs::action::MoveToShelf;
using GoalHandle = rclcpp_action::ServerGoalHandle<MoveToShelf>;

class actionGoal :public rclcpp::Node
{
    public:
    actionGoal() :Node("AS")
    {
        action_server_ = rclcpp_action::create_server<MoveToShelf>(this,"move_to_shelf",
        [this](const rclcpp_action::GoalUUID &uuid,
            std::shared_ptr<const MoveToShelf::Goal> goal){return handle_goal(uuid,goal);},

        [this](const std::shared_ptr<GoalHandle> goal_handle){return handle_cancel(goal_handle);},


        [this](const std::shared_ptr<GoalHandle> goal_handle){handle_accepted(goal_handle);}


            
       


        );
    }

    
    private:



    rclcpp_action::GoalResponse handle_goal(
        const rclcpp_action::GoalUUID & uuid,
        std::shared_ptr<const MoveToShelf::Goal> goal
    )
    {
        (void)uuid;
        if(goal->shelf_num =="" || (goal->speed<0 || goal->speed > 1))
        {
            if(goal->shelf_num =="")
            {
                RCLCPP_WARN(this->get_logger(),"goal has been REJECTED DUE shelf_num INC");
            }
            else if ((goal->speed<0 || goal->speed > 1))
            {
                 RCLCPP_WARN(this->get_logger(),"goal has been REJECTED DUE speed LIMIT");
            }
            return rclcpp_action::GoalResponse::REJECT;
        }
        else{
            RCLCPP_INFO(this->get_logger(),"goal has been ACCEPTED_AND_EXECUTING ");
           return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
        }
    }




    rclcpp_action::CancelResponse handle_cancel(const std::shared_ptr<GoalHandle> goal_handle)
    {
        (void)goal_handle;
         RCLCPP_INFO(this->get_logger(),"Cancel ACCEPTED");
        return rclcpp_action::CancelResponse::ACCEPT;
    }




    void handle_accepted(const std::shared_ptr<GoalHandle> goal_handle)
    {
        std::thread{[this,goal_handle](){execute(goal_handle);}}.detach();
        RCLCPP_INFO(this->get_logger(),"ok");
    }

    void execute (const std::shared_ptr<GoalHandle> goal_handle)
    {
        const auto goal = goal_handle->get_goal();

        auto feedback = std::make_shared<MoveToShelf::Feedback>();

        auto result = std::make_shared<MoveToShelf::Result>();

        

        double remaining =10.00;
        double p =remaining;

        rclcpp::Rate loop_rate(1);

        while (remaining >0)
        {
            if (goal_handle->is_canceling())
            {
                goal_handle->canceled(result);
                return;
            }
            
            remaining = remaining - goal->speed;
            if(remaining<0)
            {
                remaining = 0;
            }
            feedback->remain_dist = remaining;

           
            feedback->percent =  (p-feedback->remain_dist) / p *100;
            goal_handle->publish_feedback(feedback);
            loop_rate.sleep();

        }
        result->succeeded = true;
        result->dist =p ;
        result->msg="YOUR ROBOT 'S BEEN THERE ";

        goal_handle->succeed(result);

       

        
    }





    rclcpp_action::Server<MoveToShelf>::SharedPtr action_server_;



};


int main (int argc , char * argv[])
{

    rclcpp::init(argc,argv);

    auto node = std::make_shared<actionGoal>();

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