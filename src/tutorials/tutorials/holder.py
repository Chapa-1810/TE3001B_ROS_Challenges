import rclpy
import math
import numpy as np
from rclpy.node import Node
from std_msgs.msg import Float32

class signalPub(Node):
    def __init__(self):
        super().__init__('signal_publ')
        self.signal = self.create_publisher(Float32, '/signal', 10)
        self.time = self.create_publisher(Float32, '/time', 10)
        timer_period = 0.1
        self.timer = self.create_timer(timer_period, self.timer_callback)
        self.get_logger().info('|signal_generator node successfully initialized|')
        self.msg_signal = Float32()
        self.msg_time = Float32()
        self.counter = 0.0

    def timer_callback(self):
        self.msg_signal.data = float(10 * math.sin(self.counter))
        self.msg_time.data = float(self.counter)
        self.signal.publish(self.msg_signal.data)
        self.time.publish(self.msg_time.data)
        self.counter += 0.1

def main(args=None):
    rclpy.init(args=args)
    m_p = signalPub()
    rclpy.spin(m_p)
    m_p.destroy_node()
    rclpy.shutdown()

if __name__ == '_main_':
    main()