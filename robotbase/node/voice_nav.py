#!/usr/bin/env python
#-*- coding:utf-8 -*-  
import re
import rospy
import actionlib
from actionlib_msgs.msg import *
from geometry_msgs.msg import Pose, Point, Quaternion, Twist
from move_base_msgs.msg import MoveBaseAction, MoveBaseGoal
from tf.transformations import quaternion_from_euler

class voice_nav():
    def __init__(self):
        rospy.init_node('nav_test', anonymous=False)
        
        rospy.on_shutdown(self.shutdown)
	r = rospy.Rate(self.rate)
	rospy.Subscriber('/recognizer/output', String, self.speech_callback)
	firstgoal=MoveBaseGoal()
	secondgoal=MoveBaseGoal()
        # Subscribe to the move_base action server
        self.move_base = actionlib.SimpleActionClient("move_base", MoveBaseAction)
        
        rospy.loginfo("Waiting for move_base action server...")
        
        # Wait 60 seconds for the action server to become available
        self.move_base.wait_for_server(rospy.Duration(60))

            # Intialize the waypoint goal
         kitchengoal=MoveBaseGoal()
            # Use the map frame to define goal poses
         kitchengoal.target_pose.header.frame_id = 'map'
            # Set the time stamp to "now"
         kitchengoal.target_pose.header.stamp = rospy.Time.now()
	 kitchengoal.target_pose.Pose(Pose(Point(0.0, 0.0, 0.0),Quaternion(0,0,0,1)))
            
 
            


        
    def move(self, goal):
            # Send the goal pose to the MoveBaseAction server
            self.move_base.send_goal(goal)
            
            # Allow 1 minute to get there
            finished_within_time = self.move_base.wait_for_result(rospy.Duration(60)) 
            
            # If we don't get there in time, abort the goal
            if not finished_within_time:
                self.move_base.cancel_goal()
                rospy.loginfo("Timed out achieving goal")
            else:
                # We made it!
                state = self.move_base.get_state()
                if state == GoalStatus.SUCCEEDED:
                    rospy.loginfo("Goal succeeded!")


    def get_goal(self,command):
	if(command[0][0]=="厨房"):
		firstgoal=kitchengoal
	elif(command[0][0]=="餐厅"):
		firstgoal=hallgoal
	elif(command[0][0]=="卧室"):
		firstgoal=bedroomgoal
	elif(command[0][0]=="客厅"):
		firstgoal=livingroomgoal

	if(command[0][1]=="厨房"):
		secondgoal=kitchengoal
	elif(command[0][1]=="餐厅"):
		secondgoal=hallgoal
	elif(command[0][1]=="卧室"):
		secondgoal=bedroomgoal
	elif(command[0][1]=="客厅"):
		secondgoal=livingroomgoal




    def speech_callback(self, msg):
        # Get the motion command from the recognized phrase
        line =msg.data
	p = re.compile( r'(厨房).*?(餐厅)' or r'(餐厅).*?(厨房)' and r'(厨房).*?(客厅)' or r'(客厅).*?(厨房)' and r'(厨房).*?(卧室)' 				or r'(卧室).*?(厨房)' and
			r'(客厅).*?(餐厅)' or r'(餐厅).*?(客厅)' and r'(卧室).*?(餐厅)' or r'(餐厅).*?(卧室)' and
			r'(客厅).*?(卧室)' or r'(卧室).*?(客厅)' )
	
	command=p.findall(line)
	get_goal()
	move(firstgoal)
	move(secondgoal)
        
                

if __name__ == '__main__':
    try:
        voice_nav()
    except rospy.ROSInterruptException:
        rospy.loginfo("voice test finished.")