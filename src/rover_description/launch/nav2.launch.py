import os
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    rover_dir=get_package_share_directory('rover_description')
    nav_dir=get_package_share_directory('nav2_bringup')

    config_file=os.path.join(rover_dir,'config','nav2_params.yaml')
    launch_file=os.path.join(nav_dir,'launch','bringup_launch.py')


    map_dir=os.path.join(rover_dir,'maps','warehouse.yaml')
    mask_file=os.path.join(rover_dir,'maps','keepout_mask.yaml')

    nav2 = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(launch_file),
        launch_arguments={
        'params_file': config_file,
        'map': map_dir,
        'use_sim_time' : 'true',
        }.items(),

    )

    mask_server = Node(
        package='nav2_map_server',
        executable='map_server',
        name='filter_mask_server',
        parameters=[config_file, {'yaml_filename': mask_file, 'use_sim_time': True}],
        output='screen')

    info_server = Node(
        package='nav2_map_server',
        executable='costmap_filter_info_server',
        name='costmap_filter_info_server',
        parameters=[config_file, {'use_sim_time': True}],
        output='screen')

    filters_manager = Node(
        package='nav2_lifecycle_manager',
        executable='lifecycle_manager',
        name='lifecycle_manager_costmap_filters',
        parameters=[{'use_sim_time': True,
                     'autostart': True,
                     'node_names': ['filter_mask_server', 'costmap_filter_info_server']}],
        output='screen')

    return LaunchDescription([
         nav2, mask_server, info_server, filters_manager
    ])


 

    
