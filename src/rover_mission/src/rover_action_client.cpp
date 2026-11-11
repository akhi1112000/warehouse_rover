#include "rclcpp_action/rclcpp_action.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rover_interfaces/action/move_to_shelf.hpp"
#include <thread>
#include <chrono>

using Move_ToShelf = rover_interfaces::action::MoveToShelf;
using Goal_Handle = rclcpp_action::ServerGoalHandle<Move_ToShelf>;

class rover_client : public rclcpp::Node
{
    public:
    rover_client():Node("action_client")
    {
        rover_client_=rclcpp_action::create_client<Move_ToShelf>(this,"MyRover_action");

        if(rover_client_->wait_for_action_server(std::chrono::seconds(5)))
        {
            RCLCPP_INFO(this->get_logger(),"THE SERVER IS FOUND");
            parallel();
        }
        else
        {
            RCLCPP_ERROR(this->get_logger(),"THE SERVER IS NOT FOUND");
        }
       



       
        

    }

    private:

    void send_goal()
    {
        Move_ToShelf::Goal goal;
        goal.shelf_name = "rack_1";

        auto goal_sender = rover_client_->async_send_goal(goal);
        auto get_goal = goal_sender.get();

        if(!get_goal)
        {
            RCLCPP_ERROR(this->get_logger(),"THE GOAL IS NOT ACCEPTED");
            return;
        }
       
        RCLCPP_INFO(this->get_logger(),"THE GOAL IS ACCEPTED");

        auto goal_result = rover_client_->async_get_result(get_goal);
        auto get_goal_result = goal_result.get();

        if(get_goal_result.code == rclcpp_action::ResultCode::SUCCEEDED)
        {
        RCLCPP_INFO(this->get_logger(), "THE ROBOT ARRIVED !");
        }
        else
        {
            RCLCPP_ERROR(this->get_logger(), "THE ROBOT DID NOT ARRIVE !");
        }
    }
    void parallel()
    {
        std::thread([this](){send_goal();}).detach();
    }



    rclcpp_action::Client<Move_ToShelf>::SharedPtr rover_client_;

    


};

int main(int argc,char ** argv)
{
   rclcpp::init(argc,argv);
  
   auto node = std::make_shared<rover_client>();


  
   rclcpp::spin(node);


   rclcpp::shutdown();


   return 0;


}