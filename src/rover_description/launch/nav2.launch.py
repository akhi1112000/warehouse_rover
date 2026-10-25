import os
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription


def generate_launch_description():
    rover_dir=get_package_share_directory('rover_description')
    nav_dir=get_package_share_directory('nav2_bringup')

    config_file=os.path.join(rover_dir,'config','nav2_params.yaml')
    launch_file=os.path.join(nav_dir,'launch','bringup_launch.py')


    map_dir=os.path.join(rover_dir,'maps','warehouse.yaml')

    nav2 = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(launch_file),
        launch_arguments={
        'params_file': config_file,
        'map': map_dir,
        'use_sim_time' : 'true',
        }.items(),

    )

    return LaunchDescription([
        nav2
    ])


 

    
