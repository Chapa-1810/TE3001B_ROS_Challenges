from launch import LaunchDescription
from launch_ros.actions import Node 

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='tutorials',
            namespace='tutorials',
            executable='process',
            name='process'
        ),
        Node(
            package='tutorials',
            namespace='tutorials',
            executable='signal',
            name='signal'
        ),
    ])