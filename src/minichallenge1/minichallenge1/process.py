import rclpy
from rclpy.node import Node

from std_msgs.msg import Float32


class SignalProcessor(Node):

    def __init__(self):
        super().__init__('signal_process')
        self.signal_sub_ = self.create_subscription(
            Float32,
            '/signal',
            self.signal_callback,
            10)
        
        self.time_sub_ = self.create_subscription(
            Float32,
            '/time',
            self.time_callback,
            10)

        self.signal = 0.0
        self.time = 0.0

        self.int_publisher_ = self.create_publisher(Float32, '/proc_signal', 10)
        self.timer_ = self.create_timer(0.1, self.timer_callback)
    
    def signal_callback(self, msg):
        self.signal = msg.data * 2
    
    def time_callback(self, msg):
        self.time = msg.data

    def timer_callback(self):
        msg = Float32()
        msg.data = self.signal
        self.int_publisher_.publish(msg)
        self.get_logger().info('Publishing: %f' % self.signal)


def main(args=None):
    rclpy.init(args=args)

    processor_subscriber = SignalProcessor()

    rclpy.spin(processor_subscriber)

    # Destroy the node explicitly
    # (optional - otherwise it will be done automatically
    # when the garbage collector destroys the node object)
    processor_subscriber.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()