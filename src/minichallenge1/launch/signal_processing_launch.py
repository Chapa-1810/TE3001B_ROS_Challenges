from launch import LaunchDescription
from launch_ros.actions import Node 

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='minichallenge1',
            namespace='minichallenge1',
            executable='process',
            name='process'
        ),
        Node(
            package='minichallenge1',
            namespace='minichallenge1',
            executable='signal',
            name='signal'
        ),

        Node(
            package='rqt_plot', executable='rqt_plot', output='screen', emulate_tty=True,
            arguments=["/proc_signal/data", "/signal/data"]
        ),
    ])