#!/usr/bin/env python3
import os
from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='minichallenge2', executable='signal_generator', output='screen', emulate_tty=True,
            parameters=[os.path.join(get_package_share_directory('minichallenge2'), 'config', 'signal_parameters.yaml')]
        ),
        Node(
            package='minichallenge2', executable='signal_reconstruction', output='screen', emulate_tty=True
        ),
        Node(
            package='rqt_plot', executable='rqt_plot', output='screen', emulate_tty=True,
            arguments=["/signal/data",  "/signal_reconstructed/data"]
        ),
        
    ])