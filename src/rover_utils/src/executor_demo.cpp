#include "rclcpp/rclcpp.hpp"
#include <thread>

class executorDemo : public rclcpp:: Node
{
    public:
    executorDemo() : Node("executor_demo")
    {
        group_1 = this->create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);

        group_2 = this->create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);

        timr1_ = this->create_wall_timer(std::chrono::seconds(1),[this](){fastTask();},group_1);
        timr2_ = this->create_wall_timer(std::chrono::seconds(1),[this](){slowTask();},group_2);
    }

    private:

    
    void slowTask()
        {
            RCLCPP_INFO(this->get_logger(), "slow task started");
            std::this_thread::sleep_for(std::chrono::seconds(3));
            RCLCPP_INFO(this->get_logger(), "slow task done");
        }

     void fastTask()
        {
            RCLCPP_INFO(this->get_logger(), "fast task started");

        }

    rclcpp::TimerBase::SharedPtr timr1_ ,timr2_;
    rclcpp::CallbackGroup::SharedPtr group_1;
    rclcpp::CallbackGroup::SharedPtr group_2;
};

int main (int argc , char * argv[])
{

    rclcpp::init(argc,argv);

    auto node = std::make_shared<executorDemo>();

    try
    {
        rclcpp::executors::MultiThreadedExecutor executor;
        executor.add_node(node);
        executor.spin();
    }
    catch(const std::exception& e)
    {
    RCLCPP_FATAL(rclcpp::get_logger("main"), "fatal: %s", e.what());
    }
    
    rclcpp::shutdown();

    return 0;


}
