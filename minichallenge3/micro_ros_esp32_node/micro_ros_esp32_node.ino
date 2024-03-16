#include <micro_ros_arduino.h>

#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <std_msgs/msg/int32.h>
#include <std_msgs/msg/float32.h>

rcl_publisher_t publisher_raw;
rcl_publisher_t publisher_vol;
rcl_subscription_t subscriber_freq;
std_msgs__msg__Int32 pub_msg_raw;
std_msgs__msg__Float32 pub_msg_vol;
std_msgs__msg__Float32 sub_msg_freq;
rclc_executor_t executor;
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;
rcl_timer_t timer_raw;
rcl_timer_t timer_vol;

int potPin = 34;   // select the input pin for the potentiometer
int potValue = 0;
float voltage = 0;

int pwm_pin = 16;
int freq = 5000;
int led_cha = 0;
int resulution = 8;


#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}


void error_loop(){
  while(1){
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(100);
  }
}

void timer_callback_raw(rcl_timer_t * timer, int64_t last_call_time)
{  
  RCLC_UNUSED(last_call_time);
  if
   (timer != NULL) {
    potValue = analogRead(potPin);
    voltage = potValue * (3.3 / 4095.0);
  }


}

void timer_callback_vol(rcl_timer_t * timer, int64_t last_call_time)
{  
  RCLC_UNUSED(last_call_time);
  if (timer != NULL) {
    pub_msg_vol.data = voltage;
    
    pub_msg_vol.data = voltage;
    pub_msg_raw.data = potValue;
    RCSOFTCHECK(rcl_publish(&publisher_vol, &pub_msg_vol, NULL));
    RCSOFTCHECK(rcl_publish(&publisher_raw, &pub_msg_raw, NULL));
    //msg.data++;
  }
}


void frequency_callback(const void * msgin){
  const std_msgs__msg__Float32 * msg = (const std_msgs__msg__Float32 *)msgin;
  float duty_cycle = max(0, min(msg->data,100));
  ledcWrite(led_cha, duty_cycle * 255);
}

void setup() {
  set_microros_transports();
  
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);  
  ledcSetup(led_cha, freq, resulution);
  ledcAttachPin(pwm_pin, led_cha);
  ledcWrite(led_cha, 0);
  
  delay(2000);

  allocator = rcl_get_default_allocator();

  //create init_options
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

  // create node
  RCCHECK(rclc_node_init_default(&node, "micro_ros_esp32_node", "", &support));

  // create publisher
  RCCHECK(rclc_publisher_init_default(
    &publisher_raw,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
    "micro_ros_esp32/raw_pot"));

  RCCHECK(rclc_publisher_init_default(
    &publisher_vol,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32),
    "micro_ros_esp32/voltage"));

  RCCHECK(rclc_subscription_init_default(
    &subscriber_freq,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32),
    "micro_ros_esp32/pwm_duty_cycle"));

  // create timer,
  const unsigned int timer_timeout_raw = 100;
  const unsigned int timer_timeout_vol = 10;
  RCCHECK(rclc_timer_init_default(
    &timer_raw,
    &support,
    RCL_MS_TO_NS(timer_timeout_raw),
    timer_callback_raw));

  RCCHECK(rclc_timer_init_default(
    &timer_vol,
    &support,
    RCL_MS_TO_NS(timer_timeout_vol),
    timer_callback_vol));

  // create executor
  RCCHECK(rclc_executor_init(&executor, &support.context, 3, &allocator));
  RCCHECK(rclc_executor_add_timer(&executor, &timer_raw));
  RCCHECK(rclc_executor_add_timer(&executor, &timer_vol));
  RCCHECK(rclc_executor_add_subscription(&executor, &subscriber_freq, &sub_msg_freq, &frequency_callback, ON_NEW_DATA));

  pub_msg_raw.data = 0;
  pub_msg_vol.data = 0;
}

void loop() {
  delay(5);
  RCSOFTCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
}
