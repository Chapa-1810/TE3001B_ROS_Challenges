import rclpy
import math
from rclpy.node import Node

from std_msgs.msg import String
from std_msgs.msg import Float32

class SignalPublisher(Node):
    def __init__(self):
        super().__init__('signal_publisher')
        self.time_pub_ = self.create_publisher(Float32, '/time', 10)
        self.signal_pub_ = self.create_publisher(Float32, '/signal', 10)
        self.timer_ = self.create_timer(0.1, self.timer_callback)
        self.time = 0

    def timer_callback(self):
        msg = Float32()
        #print (float(10 * math.sin(self.time)))
        msg.data = float(10 * math.sin(self.time))
        self.signal_pub_.publish(msg)
        self.get_logger().info('Publishing: %f' % msg.data)
        self.time += 0.1
        msg.data = float(self.time)
        self.time_pub_.publish(msg)
        self.get_logger().info('Publishing: %f' % msg.data)


def main(args=None):
    rclpy.init(args=args)

    signal_publisher_ = SignalPublisher()

    rclpy.spin(signal_publisher_)

    signal_publisher_.destroy_node()
    rclpy.shutdown()