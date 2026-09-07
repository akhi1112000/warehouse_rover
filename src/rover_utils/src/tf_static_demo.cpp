#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/static_transform_broadcaster.h"

#include "geometry_msgs/msg/transform_stamped.hpp"

#include "tf2/LinearMath/Quaternion.hpp"

class TfStaticDemo : public rclcpp::Node
{
    public:
    TfStaticDemo() : Node("tf_static_demo")
    {
        broadcaster_ = std::make_shared<tf2_ros::StaticTransformBroadcaster>(this);
        publishStaticTransform();
    }


    private:
    void publishStaticTransform()
    {
        geometry_msgs::msg::TransformStamped t;
        t.header.stamp = this->get_clock()->now();
        t.header.frame_id="base_link";
        t.child_frame_id = "laser";

        t.transform.translation.x = 0.3;
        t.transform.translation.y = 0.0;
        t.transform.translation.z = 0.2;

        tf2::Quaternion q;

        q.setRPY(0,0,0);

        t.transform.rotation.x = q.x();
        t.transform.rotation.y = q.y();
        t.transform.rotation.z = q.z();
        t.transform.rotation.w = q.w();

        broadcaster_->sendTransform(t);

    }

    std::shared_ptr<tf2_ros::StaticTransformBroadcaster> broadcaster_;
};

int main (int argc , char * argv[])
{

    rclcpp::init(argc,argv);

    auto node = std::make_shared<TfStaticDemo>();

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