import os

from launch import LaunchDescription

from launch_ros.actions import Node

from launch.actions import IncludeLaunchDescription

from ament_index_python.packages import get_package_share_directory

from launch.substitutions import Command

from launch_ros.parameter_descriptions import ParameterValue

from launch.launch_description_sources import PythonLaunchDescriptionSource

def generate_launch_description():

    pkg_dir=get_package_share_directory('rover_description')

    xacro_file = os.path.join(pkg_dir,'urdf','rover.urdf.xacro')

    sdf_file= os.path.join(pkg_dir,'worlds','warehouse.sdf')

    robot_description  =Command(['xacro ',xacro_file,' use_gazebo:=true'])

    ros_gz_sim=get_package_share_directory('ros_gz_sim')

    gazebo = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(ros_gz_sim,'launch','gz_sim.launch.py')
        ),
        launch_arguments={'gz_args': '-r ' + sdf_file}.items()
    )

    bridge=IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(pkg_dir, 'launch', 'bridge.launch.py')
        )
    )

    spwan=Node(
        package='ros_gz_sim',
        executable='create',
        arguments=['-topic','robot_description','-name','warehouse_rover',
               '-x', '-3',
               '-y', '-3',
               '-z', '0.5'],
        output='screen'
    )



    rsp = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        parameters=[{'robot_description':ParameterValue(robot_description,value_type=str),'use_sim_time': False}]

    )

    

    rviz=Node(
        package='rviz2',
        executable='rviz2',
        arguments=['-d',os.path.join(pkg_dir,'rviz','rover.rviz')],
        parameters=[{'use_sim_time': True}]
    )

    jsb_spawner=Node(
        package='controller_manager',
        executable='spawner',
        arguments=['joint_state_broadcaster']

    )

    diff_drive_spawner=Node(
        package='controller_manager',
        executable='spawner',
        arguments=['diff_drive_controller']
    
        )



    return LaunchDescription([
        rsp,
        gazebo,
        spwan,
        bridge,
        rviz,
        jsb_spawner,
        diff_drive_spawner
        
        
        
    ])