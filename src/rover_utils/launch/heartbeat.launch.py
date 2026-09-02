from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='rover_utils',
            executable='heart_node',
            name='heartbeat_publisher',
            parameters=[{
                'topic_name': 'rover_topic',
                'publish_rate_ms': 200,
                'message_prefix':'LAUCH'
            }]
        ),

        Node(
            package='rover_utils',
            executable='monitor_node',
            name='heartbeat_subscriber',
            parameters=[{
                            'topic_name': 'rover_topic'  
                        }]

        )
    ])