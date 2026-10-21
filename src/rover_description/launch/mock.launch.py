import os
from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
from launch.substitutions import Command
from launch_ros.parameter_descriptions import ParameterValue

def generate_launch_description():
    pkg_dir= get_package_share_directory('rover_description')
    xacro_file = os.path.join(pkg_dir,'urdf','rover.urdf.xacro')
    robot_description = Command(['xacro ',xacro_file,' use_gazebo:=false'])
    controllers_yaml = os.path.join(pkg_dir,'config','controllers.yaml')
    rviz_config = os.path.join(pkg_dir, 'rviz', 'rover.rviz')

    rsp=Node(
        package="robot_state_publisher",
        executable="robot_state_publisher",
         parameters=[{
        'robot_description': ParameterValue(robot_description, value_type=str),
        'use_sim_time': False
    }],
    output='screen'
    )

    controller_manager=Node(
        package="controller_manager",
        executable="ros2_control_node",
        parameters=[controllers_yaml,{'use_sim_time': False}],
        remappings=[('~/robot_description', '/robot_description')],
        output='screen'

    )

    jsp_spawner = Node(
        package='controller_manager',
        executable='spawner',
        arguments=['joint_state_broadcaster'],
        output='screen'
    )

    diff_drive_spawner=Node(
        package='controller_manager',
        executable='spawner',
        arguments=['diff_drive_controller'],
        output='screen'
    )

    rviz = Node(
        package='rviz2',
        executable='rviz2',
        arguments=['-d', rviz_config],
        parameters=[{'use_sim_time': False}],
        output='screen'
    )
    return LaunchDescription([
        rsp,
        controller_manager,
        jsp_spawner,
        diff_drive_spawner,
        rviz
    ])