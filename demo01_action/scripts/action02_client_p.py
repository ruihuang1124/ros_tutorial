#! /usr/bin/env python

import rospy
import actionlib
from demo01_action.msg import *



def done_cb(state,result):
    if state == actionlib.GoalStatus.SUCCEEDED:
        rospy.loginfo("Response result:%d",result.result)

def active_cb():
    rospy.loginfo("Server is activated")


def fb_cb(fb):
    rospy.loginfo("Current progress:%.2f",fb.progress_bar)

if __name__ == "__main__":
    # 2.Init ROS Node
    rospy.init_node("action_client_p")
    # 3.Creat action Client Object
    client = actionlib.SimpleActionClient("addInts",AddIntsAction)
    # 4.Waiting for the server
    client.wait_for_server()
    # 5.Organized the object and sendGoal
    goal_obj = AddIntsGoal()
    goal_obj.num = 10
    client.send_goal(goal_obj,done_cb,active_cb,fb_cb)
    # 6.create callback function, done_cb, active_cb, fb_cb
    # 7.spin
    rospy.spin()
