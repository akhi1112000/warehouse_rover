from launch import LaunchDescription
from launch_ros.actions import Node



def generate_launch_description():
        rover_server = Node(
            package='rover_mission',
            executable='rover_action',
            parameters=[{'use_sim_time':True}],
            output='screen',
        )


        rover_client = Node(
            package='rover_mission',
            executable='rover_client',
            output='screen',

        )
                


        return LaunchDescription([
            rover_server,
            rover_client
        ])