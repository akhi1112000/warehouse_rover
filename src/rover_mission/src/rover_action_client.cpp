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

    bool go_to(const std::string & goal_to_go)
    {
        Move_ToShelf::Goal goal;
        goal.shelf_name = goal_to_go;

        auto goal_sender = rover_client_->async_send_goal(goal);
        auto get_goal = goal_sender.get();

        if(!get_goal)
        {
            RCLCPP_ERROR(this->get_logger(),"THE GOAL IS NOT ACCEPTED");
            return false;
        }
       
        RCLCPP_INFO(this->get_logger(),"THE GOAL IS ACCEPTED");

        auto goal_result = rover_client_->async_get_result(get_goal);
        auto get_goal_result = goal_result.get();

        if(get_goal_result.code == rclcpp_action::ResultCode::SUCCEEDED)
        {
        RCLCPP_INFO(this->get_logger(), "THE ROBOT ARRIVED !");
        return true;
        }
        else
        {
            RCLCPP_ERROR(this->get_logger(), "THE ROBOT DID NOT ARRIVE !");
            return false;
        }
    }
   
   
    void parallel()
    {
        std::thread([this](){go_to("rack_3");}).detach();
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