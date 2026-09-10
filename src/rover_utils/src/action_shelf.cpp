#include "rclcpp/rclcpp.hpp"
#include "rover_msgs/action/move_to_shelf.hpp"

#include "rclcpp_action/rclcpp_action.hpp"


using MoveToShelf = rover_msgs::action::MoveToShelf;
using GoalHandle = rclcpp_action::ServerGoalHandle<MoveToShelf>;

class Shelf : public rclcpp::Node
{
    public:
    Shelf():Node("review")
    {

    }

    rclcpp_action::GoalResponse goal_response(const rclcpp_action::GoalUUID  &,
    std::shared_ptr<const rover_msgs::action::MoveToShelf::Goal>goal)
    {
        
        if(goal->speed==0 || goal->speed >=1)
        {
            return rclcpp_action::GoalResponse::REJECT;
        }
        else
        {
            return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
        }
    }

    rclcpp_action::CancelResponse handle_cancel(const std::shared_ptr
        <rclcpp_action::ServerGoalHandle<rover_msgs::action::MoveToShelf>>cancel_goal)
    {
        return rclcpp_action::CancelResponse::ACCEPT;
    }


    void handle_accept(const std::shared_ptr<GoalHandle> goal_handle)
    {

    }

    private:

    rclcpp_action::Server<rover_msgs::action::MoveToShelf>::SharedPtr action_;

    
}