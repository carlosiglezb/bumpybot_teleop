#!/usr/bin/env python3
import rospy
from geometry_msgs.msg import Twist, Vector3
import lpf

velocity_linear = Vector3(0,0,0)
velocity_angular = Vector3(0,0,0)

def callback(msg):
    global velocity_linear, velocity_angular 
    velocity_linear, velocity_angular = msg.linear, msg.angular

if __name__ == '__main__':
    rospy.init_node("velocity_smoother")
    output = rospy.Publisher("/trikey/base_controller/cmd_vel", Twist, queue_size=10)
    input = rospy.Subscriber("cmd_vel", Twist, callback)

    rate = rospy.Rate(1000)
    low_pass = lpf.vel_smoother()
    msg = Twist()

    
    while not rospy.is_shutdown():
        msg.linear, msg.angular = low_pass.lpf(velocity_linear, velocity_angular)
        output.publish(msg)
        rate.sleep()
