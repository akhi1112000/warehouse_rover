#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "rover_msgs/action/pick_package.hpp"
#include <thread>


using  PickPackage = rover_msgs::action::PickPackage;
using handle_goal = rclcpp_action::ServerGoalHandle<PickPackage>;


class Shelf_client :public rclcpp::Node
{
    public:
                Shelf_client() : Node("PKG_SEND")
                {


                    client_ = rclcpp_action::create_client<PickPackage>(this, "serv");

                    send_goal();

            

                }

                
    private:


    void send_goal()
    {
        if(!client_->wait_for_action_server(std::chrono::seconds(5)))
                {
                    RCLCPP_ERROR(this->get_logger(), "action server not available");
                    return;
                }

        auto goal = PickPackage::Goal();
        goal.hight_m=2.00;
        goal.pkg = "PKG_001";


        auto option = rclcpp_action::Client<PickPackage>::SendGoalOptions();
        
        option.goal_response_callback = 
        [this](rclcpp_action::ClientGoalHandle<PickPackage>::SharedPtr handle)
        {
            if(!handle)
            {
                RCLCPP_ERROR(this->get_logger(), "goal rejected");
            }
            else
            {
                RCLCPP_INFO(this->get_logger(), "goal accepted");
            }

        };


        option.feedback_callback = 
        [this](rclcpp_action::ClientGoalHandle<PickPackage>::SharedPtr,
        const std::shared_ptr <const PickPackage::Feedback> feed_back)
        {
             RCLCPP_INFO(this->get_logger(), "%s : %.0f%%",
                    feed_back->current_state.c_str(),
                    feed_back->achievment_p);
        };


        option.result_callback = [this]
        (const rclcpp_action::ClientGoalHandle<PickPackage>::WrappedResult & result)
            {

                if(result.code==rclcpp_action::ResultCode::SUCCEEDED )
                    {
                        RCLCPP_INFO(this->get_logger(), "done: %s, weight %.1f",
                                result.result->msg.c_str(),
                                result.result->pkg_wieght);
                    }
                else
                    {
                        RCLCPP_WARN(this->get_logger(), "goal did not succeed");
                    }
            };
           client_->async_send_goal(goal, option);
    }
     
    
 

    rclcpp_action::Client<PickPackage>::SharedPtr client_;

    
    

    

};


int main (int argc , char * argv[])
{

    rclcpp::init(argc,argv);

    auto node = std::make_shared<Shelf_client>();

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

