#include "tf2_ros/transform_listener.h"
#include "tf2_ros/buffer.h"
#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/static_transform_broadcaster.h"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "tf2/LinearMath/Quaternion.hpp"

class TfStaticListnerDemo : public rclcpp::Node
{
    public:
    TfStaticListnerDemo() : Node("tf_static_listner_demo")
    {
       tf_buffer_ = std::make_shared<tf2_ros::Buffer>(this->get_clock());
       tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);
       timer_=this->create_wall_timer(std::chrono::seconds(1),
        [this](){ lookupTransform(); });
    

    }


    private:
    void lookupTransform()
    {
            try {
                auto t = tf_buffer_->lookupTransform("base_link", "laser", tf2::TimePointZero);
                RCLCPP_INFO(this->get_logger(), "x=%.2f z=%.2f",
                t.transform.translation.x,
                t.transform.translation.z);
                }
            catch (const tf2::TransformException & e) 
                {
                RCLCPP_WARN(this->get_logger(), "could not transform: %s", e.what());
                }

    }


    std::shared_ptr<tf2_ros::Buffer> tf_buffer_;
    std::shared_ptr<tf2_ros::TransformListener> tf_listener_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main (int argc , char * argv[])
{

    rclcpp::init(argc,argv);

    auto node = std::make_shared<TfStaticListnerDemo>();

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