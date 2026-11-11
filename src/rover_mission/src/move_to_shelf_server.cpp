#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "rover_interfaces/action/move_to_shelf.hpp"
#include "nav2_msgs/action/navigate_to_pose.hpp"
#include <string>
#include <thread>
#include <algorithm>
#include <cctype>




using Move_ToShelf = rover_interfaces::action::MoveToShelf;
using Goal_Handle = rclcpp_action::ServerGoalHandle<Move_ToShelf>;
using nav_2       = nav2_msgs::action::NavigateToPose;  



class MoveToShelfServer :public rclcpp::Node
{

   public:
   MoveToShelfServer():Node("move_to_shelf_server")
   {
       RCLCPP_INFO(this->get_logger(),"it is created !");


       action_server_= rclcpp_action::create_server<Move_ToShelf>(this,"MyRover_action",




           [this](const rclcpp_action::GoalUUID &uuid,  std::shared_ptr<const Move_ToShelf::Goal> goal){return still_need_torespond(uuid,goal);},
           [this](const std::shared_ptr<Goal_Handle> goal_cancel){return now_it_is_cancelled(goal_cancel);},
           [this](const std::shared_ptr<Goal_Handle> goal_start){return call_thread(goal_start);}
      
       );

       nav2_client_=rclcpp_action::create_client<nav_2>(this,"navigate_to_pose");

         if(nav2_client_->wait_for_action_server(std::chrono::seconds(5)))
         {
            RCLCPP_INFO(this->get_logger(), "THE CLIENT Nav2 server is found!");
         }
         else
         {
            RCLCPP_ERROR(this->get_logger(), "THE CLIENT NAV2 IS NOT FOUND YET !");
         }

   }




   private:
  

   rclcpp_action::GoalResponse still_need_torespond(const rclcpp_action::GoalUUID &UUID,
std::shared_ptr<const Move_ToShelf::Goal>&goal)
   {
    (void)UUID;

    int shelf_num = 5;


   
    if(goal->shelf_name == "home" || goal->shelf_name == "loading" )
    {
      RCLCPP_INFO(this->get_logger(),"THE GOAL '%s' IS FOUND",goal->shelf_name.c_str());
      return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
    }

    for (int i = 1; i <= shelf_num; i++)
    {
        std::string shelf= "RACK_" + std::to_string(i);
        std::transform(shelf.begin() ,shelf.end(), shelf.begin() ,::tolower);
        if(goal->shelf_name ==shelf )
        {
            RCLCPP_INFO(this->get_logger(),"THE GOAL '%s' IS FOUND",goal->shelf_name.c_str());
            return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
        }
       
    }

     RCLCPP_ERROR(this->get_logger(),"THE GOAL '%s' IS NOT FOUND",goal->shelf_name.c_str());
    return rclcpp_action::GoalResponse::REJECT;
   }


rclcpp_action::CancelResponse now_it_is_cancelled (const std::shared_ptr<Goal_Handle>& goal )
   {
    (void)goal;
    RCLCPP_ERROR(this->get_logger(),"THE GOAL IT IS CANCELLED");
    return rclcpp_action::CancelResponse::ACCEPT;

   }



    

   void execute(const std::shared_ptr<Goal_Handle> &goal_handle)
   {
      auto goal = goal_handle->get_goal();
      auto result = std::make_shared<Move_ToShelf::Result>();

      double x;
      double y;
      if(goal->shelf_name == "home")
      {
         x = 0;
         y = 0;
      }

      else if (goal->shelf_name == "loading")
      {
         x = -5.8;
         y = 4.3;
      }

      else
      {
         x = 2.55;
      double i  = std::stoi(goal->shelf_name.substr(5));

       y = -1.30 + (i  - 1) * 2.15 - 1.075;

      }
      

      

      nav_2::Goal nav2_goal;
      nav2_goal.pose.header.frame_id = "map";
      nav2_goal.pose.header.stamp=this->now();
      nav2_goal.pose.pose.position.x=x;
      nav2_goal.pose.pose.position.y=y;
      nav2_goal.pose.pose.orientation.w=1;

      auto PHONE = rclcpp_action::Client<nav_2>::SendGoalOptions();

      PHONE.feedback_callback = [this](auto, auto nav_feedback)
      {
         RCLCPP_INFO(this->get_logger(), "Nav2 says: %.2f m left", nav_feedback->distance_remaining);
      };


      auto OTLOP = nav2_client_->async_send_goal(nav2_goal,PHONE);
      auto safhet_eltalap = OTLOP.get();

      if(!safhet_eltalap)
      {
         RCLCPP_ERROR(this->get_logger(),"THE RESTAURANT DOESN'T ACCEPT THE GOAL !");
         result->state_result=false;
         result->is_shelf=true;
         goal_handle->abort(result);
         return;
      }


      auto result_future = nav2_client_->async_get_result(safhet_eltalap);

      while (result_future.wait_for(std::chrono::milliseconds(100)) !=
   std::future_status::ready)
      {
         if (goal_handle->is_canceling())
         {
            nav2_client_->async_cancel_goal(safhet_eltalap);
            result->state_result = false;
            result->is_shelf = true;
            goal_handle->canceled(result);
           RCLCPP_INFO(this->get_logger(), "Canceled, told the restaurant too");
        return;
    }
}

auto nav_result = result_future.get();

      if(nav_result.code == rclcpp_action::ResultCode::SUCCEEDED)
      {
      RCLCPP_INFO(this->get_logger(),"THE RESTAURANT DELIVERED !");
      result->state_result=true;
      result->is_shelf=true;
      goal_handle->succeed(result);


      }
      else
      {
      RCLCPP_WARN(this->get_logger(),
      "THE RESTAURANT COULDN'T DELIVER !%d",nav_result.result->error_code);
      result->state_result=false;
      result->is_shelf=true;
      goal_handle->abort(result);
      }


      

   }
   
   void call_thread(const std::shared_ptr<Goal_Handle> goal)
   {
     std::thread([this,goal](){execute(goal);}).detach();
   }


   double shelf_y(const std::string & name)
{
    int number = std::stoi(name.substr(5));
    double shelf_center = FIRST_SHELF_Y + (number - 1) * SHELF_SPACING;
    return shelf_center - SHELF_SPACING / 2;
}
  
       rclcpp_action::Server<Move_ToShelf>::SharedPtr action_server_;
       rclcpp_action::Client<nav_2>::SharedPtr nav2_client_;

       static constexpr double SHELF_X = 2.55;
       static constexpr double FIRST_SHELF_Y = -1.30;
       static constexpr double SHELF_SPACING = 2.15;






};


int main(int argc,char ** argv)
{
   rclcpp::init(argc,argv);
  
   auto node = std::make_shared<MoveToShelfServer>();


  
   rclcpp::spin(node);


   rclcpp::shutdown();


   return 0;


}

