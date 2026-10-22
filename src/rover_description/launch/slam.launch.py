import os
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription


def generate_launch_description():
    rover_dir=get_package_share_directory('rover_description')
    slam_dir=get_package_share_directory('slam_toolbox')

    config_file=os.path.join(rover_dir,'config','slam_params.yaml')
    launch_file=os.path.join(slam_dir,'launch','online_async_launch.py')

    slam = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(launch_file),
        launch_arguments={
        'slam_params_file': config_file,
        'use_sim_time' : 'true',
        }.items(),

    )

    return LaunchDescription([
        slam
    ])


 

    
