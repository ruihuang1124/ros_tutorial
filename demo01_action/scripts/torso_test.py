#! /usr/bin/env python3
"""torso_manager
A ros node that
    - establishes communication with the torso's rqt process (without changing the state of the system ) and
    - reads the current joint positions and then publisher the position as a ros topic
    - create a listener to accept the control msg (position/velocity) and then activate 
        the torso to execute that command  
"""
from scipy.spatial.transform import Rotation as R
from everest_pysoem import *
import numpy as np 
import rospy
from std_msgs.msg import String
from sensor_msgs.msg import JointState
import threading
import actionlib
from control_msgs.msg import FollowJointTrajectoryAction, FollowJointTrajectoryFeedback
from trajectory_msgs.msg import JointTrajectory, JointTrajectoryPoint
import time, sys, math
import datetime
from everest_pysoem.CiA402 import is_target_reached
import math

from nav_msgs.msg import Odometry
from geometry_msgs.msg import Point, Pose, Quaternion, Twist, Vector3
from curi_torso_interface.msg import TorsoStates

JOINT_NAMES = ['J1', 'J2', 'J3', 'J4', 'J5', 'J6', 'J7']
max_velocity = 0.5
import actionlib
from demo01_action.msg import *

class MyActionServer:
    def __init__(self):
        #SimpleActionServer(name, ActionSpec, execute_cb=None, auto_start=True)
        self.server = actionlib.SimpleActionServer("addInts",AddIntsAction,self.cb,False)
        self.server.start()
        rospy.loginfo("Server starting")


    def cb(self,goal):
        rospy.loginfo("Server handle the request:")
        num = goal.num
        rate = rospy.Rate(10)
        sum = 0
        for i in range(1,num + 1):

            sum = sum + i

            feedBack = i / num
            rospy.loginfo("current progress:%.2f",feedBack)

            feedBack_obj = AddIntsFeedback()
            feedBack_obj.progress_bar = feedBack
            self.server.publish_feedback(feedBack_obj)
            rate.sleep()

        result = AddIntsResult()
        result.result = sum        
        self.server.set_succeeded(result)
        rospy.loginfo("result response:%d",sum)
if __name__ == "__main__":
    rospy.init_node("action_server_p")
    server = MyActionServer()
    rospy.spin()
