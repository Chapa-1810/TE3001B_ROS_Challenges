import rclpy
from rclpy.node import Node

from std_msgs.msg import String

class MinimalPublisher(Node):
    def __init__(self):
        super().__init__('HW_publisher')
        self.publisher_ = self.create_publisher(String, '/test_publisher', 10)
        self.timer_ = self.create_timer(0.5, self.timer_callbak)
        self.i = 0

    def timer_callback(self):
        msg = String()
        msg.data = 'Hello World'
        self.publisher_.publish(msg)
        self.get_logger().info('Publishing: %s' % msg.data)

def main(args=None):
    rclpy.init(args=args)

    min_publisher = MinimalPublisher()

    rclpy.spin(min_publisher)

    min_publisher.destroy_node()
    rclpy.shutdown()