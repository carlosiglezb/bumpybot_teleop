#!/usr/bin/env python2
import rospy
from geometry_msgs.msg import Twist, Vector3
from std_msgs.msg import Time
import lpf

velocity_linear = Vector3(0,0,0)
velocity_angular = Vector3(0,0,0)

def callback(msg):
    global velocity_linear, velocity_angular 
    velocity_linear, velocity_angular = msg.linear, msg.angular
    t_in.data = rospy.Time.now()

if __name__ == '__main__':
    rospy.init_node("velocity_smoother")
    output = rospy.Publisher("/trikey/base_controller/cmd_vel", Twist, queue_size=10)
    t_call = rospy.Publisher("/trikey/base_controller/t_call", Time, queue_size=10)
    t_pub = rospy.Publisher("/trikey/base_controller/t_pub", Time, queue_size=10)
    input = rospy.Subscriber("cmd_vel", Twist, callback)

    rate = rospy.Rate(1000)
    low_pass = lpf.vel_smoother()
    msg = Twist()
    t_in = Time()
    t_out= Time()
    prev_lin_vel = msg.linear
    
    while not rospy.is_shutdown():
        # if (last_time - new_time) < servo_dt:
            # msg = last_msg 
        # if velocity_linear.x == 0:
            # velocity_linear = prev_lin_vel
        msg.linear, msg.angular = low_pass.lpf(velocity_linear, velocity_angular)
        # prev_lin_vel = msg.linear
        # velocity_linear = msg.linear
        output.publish(msg)
        t_out.data = rospy.Time.now()
        t_call.publish(t_in)
        t_pub.publish(t_out)
        rate.sleep()


    # rospy.spin()