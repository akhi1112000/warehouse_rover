#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "rover_interfaces/action/move_to_shelf.hpp"
#include "nav2_msgs/action/navigate_to_pose.hpp"
#include <string>
#include <thread>




using MoveToShelf = rover_interfaces::action::MoveToShelf;
using Goal_Handle = rclcpp_action::ServerGoalHandle<MoveToShelf>;
using nav_2       = nav2_msgs::action::NavigateToPose;  



class MoveToShelfServer :public rclcpp::Node
{
   public:
   MoveToShelfServer():Node("move_to_shelf_server")
   {
       RCLCPP_INFO(this->get_logger(),"it is created !");


       action_server_= rclcpp_action::create_server<MoveToShelf>(this,"MyRover_action",




           [this](const rclcpp_action::GoalUUID &uuid,  std::shared_ptr<const MoveToShelf::Goal> goal){return Server_answer(uuid,goal);},
           [this](const std::shared_ptr<Goal_Handle> goal_cancel){return Cancel(goal_cancel);},
           [this](const std::shared_ptr<Goal_Handle> goal_start){return start(goal_start);}
      
       );

       nav2_client_= rclcpp_action::create_client<nav_2>(this,"navigate_to_pose");

       if(nav2_client_->wait_for_action_server(std::chrono::seconds(5)))
       {
               RCLCPP_INFO(this->get_logger(),"IS FOUND !");

       }
       else
       {
        RCLCPP_INFO(this->get_logger(),"IS NOT FOUND !");
       }

       


   }


   private:
   rclcpp_action::GoalResponse Server_answer(const rclcpp_action::GoalUUID &uuid,
                               std::shared_ptr<const MoveToShelf::Goal> goal)
       {
           (void)uuid;


           int shelf_num =5;
           


           for (int i = 1; i <= shelf_num; i++)
           {
               if(goal->shelf_name =="rack_" + std::to_string(i))
               {
                  
                   RCLCPP_INFO(this->get_logger(),"GOAL WAS FOUND");
                   return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
               }
              
           }
          
               RCLCPP_INFO(this->get_logger(),"GOAL WAS NOT FOUND");
               return rclcpp_action::GoalResponse::REJECT;
       }
  
       
  
   rclcpp_action::CancelResponse Cancel(const std::shared_ptr<Goal_Handle> goal_cancel)
   {
       (void)goal_cancel;
       RCLCPP_INFO(this->get_logger(),"THE SERVER WILL CANCEL THE GOAL ");
       return rclcpp_action::CancelResponse::ACCEPT;
   }


   void execute(const std::shared_ptr<Goal_Handle> goal_start)
   {
      
       const auto goal = goal_start->get_goal();
       RCLCPP_INFO(this->get_logger(), "SHELF name is %s", goal->shelf_name.c_str());
       //int dist =10;
       auto feedback = std::make_shared<MoveToShelf::Feedback>();
       auto result = std::make_shared<MoveToShelf::Result>();
       rclcpp::Rate loop_rate(1);


       double x = 2.55;
       std::string text = goal->shelf_name;
       int i = std::stoi(text.substr(5));
       double y = (-1.30 + ((i-1)*2.15))-1.075;
       RCLCPP_INFO(this->get_logger(), "G0 TO  X:'%.3f' & Y:'%.3f'", x,y);



       nav_2::Goal nav_goal;
       nav_goal.pose.header.frame_id = "map";
       nav_goal.pose.header.stamp= this->now();
       nav_goal.pose.pose.position.x = x;
       nav_goal.pose.pose.position.y = y;
       nav_goal.pose.pose.orientation.w=1;


       auto options = rclcpp_action::Client<nav_2>::SendGoalOptions();

       options.feedback_callback = 
       [goal_start](rclcpp_action::ClientGoalHandle<nav_2>::SharedPtr,
                    const std::shared_ptr<const nav_2::Feedback> nav_feedback )

                    {
                        auto feedback = std::make_shared<MoveToShelf::Feedback>();
                        feedback->rem_dist = nav_feedback->distance_remaining;
                        goal_start->publish_feedback(feedback);
                    };

       auto handle_future = nav2_client_->async_send_goal(nav_goal,options);
       
       auto nav_handle = handle_future.get();

       if(!nav_handle)
       {
                RCLCPP_ERROR(this->get_logger(), "Nav2 rejected the goal");
                result->state_result= false;
                result->is_shelf = true;
                goal_start->abort(result);
                return;


       }
       
       auto result_future = nav2_client_->async_get_result(nav_handle);


        while (result_future.wait_for(std::chrono::milliseconds(100)) != std::future_status::ready)
    {
        if (goal_start->is_canceling())
        {
            nav2_client_->async_cancel_goal(nav_handle);
            result->state_result = false;
            result->is_shelf = true;
            goal_start->canceled(result);
            RCLCPP_INFO(this->get_logger(), "Canceled, Nav2 goal canceled too");
            return;
        }
    }

    auto nav_result = result_future.get();

        if (nav_result.code == rclcpp_action::ResultCode::SUCCEEDED)
    {
        RCLCPP_INFO(this->get_logger(), "Arrived at %s", goal->shelf_name.c_str());
        result->state_result = true;
        result->is_shelf = true;
        goal_start->succeed(result);
    }
    else
    {
        RCLCPP_ERROR(this->get_logger(), "Nav2 failed, error code %d",
                     nav_result.result->error_code);
        result->state_result = false;
        result->is_shelf = true;
        goal_start->abort(result);
    }

       
   }


   void start(const std::shared_ptr<Goal_Handle> goal_start)
   {
       std::thread{[this,goal_start]() {execute(goal_start);}}.detach();
   }
  
       rclcpp_action::Server<MoveToShelf>::SharedPtr action_server_;
       rclcpp_action::Client<nav_2>::SharedPtr nav2_client_;






};


int main(int argc,char ** argv)
{
   rclcpp::init(argc,argv);
  
   auto node = std::make_shared<MoveToShelfServer>();


  
   rclcpp::spin(node);


   rclcpp::shutdown();


   return 0;


}

