#! /usr/bin/env python
import rospy
import actionlib
from demo01_action.msg import *
import threading
import time


class MyActionServer:
    def __init__(self):
        #SimpleActionServer(name, ActionSpec, execute_cb=None, auto_start=True)
        self.RATE = 0.1
        self.server = actionlib.ActionServer("addInts",AddIntsAction,self.on_goal_cb, self.on_cancel_cb, auto_start = False)
        self.follwoing_lock = threading.Lock()
        self.time_start_ = time.time()
        self.goal_handle = None
        self.update_timer = rospy.Timer(rospy.Duration(self.RATE),self._update_timer_cb)
        self.time_duraction_ = time.time()
        self.sum = 0


    def start(self):
        self.server.start()
        rospy.loginfo("Server starting")




    def on_goal_cb(self,goal_handle):

        # Reject if we are following already
        if self.goal_handle:
            rospy.logerr("Received a goal while executing a trajectory")
            goal_handle.set_rejected(text="Received a goal while we are still executing a trajectory")
            return
        num = goal_handle.get_goal().num
        with self.follwoing_lock:
            self.goal_handle = goal_handle
            self.time_start_ = time.time()
            self.time_duraction_ = 10
            rate = rospy.Rate(5)
            self.goal_handle.set_accepted()
            rospy.loginfo("Server handle the request:")
            self.sum = 0
            for i in range(1,num + 1):
                self.sum = self.sum + i
                rate.sleep()
            rospy.loginfo("Finish handled the request!!!!!!!")

    def on_cancel_cb(self, goal_handle):
        with self.following_lock:
            rospy.loginfo("CURI Torso action canceled: Goal id: "+str( goal_handle.get_goal_id().id))


    def _update_timer_cb(self, event):
        """
            Check for success
        """
        now = rospy.get_rostime()
        time_now = time.time()
        time_used = time_now - self.time_start_

        if self.goal_handle and time_used >(self.time_duraction_+1):
            # self.goal_handle.set_succeeded()
            result = AddIntsResult()
            result.result = self.sum        
            # self.server.set_succeeded(result)
            rospy.loginfo("result response:%d",self.sum)
            rospy.loginfo("Torso action completed: Goal id: "+str( self.goal_handle.get_goal_id().id))
            self.goal_handle = None

        elif self.goal_handle:
            rospy.loginfo("Start update msg!!!!!!!!!")
            msg = AddIntsFeedback()
            msg.progress_bar = 0.99
            self.goal_handle.publish_feedback(msg)

    
if __name__ == "__main__":
    rospy.init_node("action_server_p")
    server = MyActionServer()
    server.start()
    rospy.spin()
