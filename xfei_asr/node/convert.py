#!/usr/bin/python
# -*- coding: utf-8 -*-
import rospy
import re
from std_msgs.msg import String
class DealCmd():
    def __init__(self):
        rospy.init_node('dealcmd',anonymous=False)
        self.cmd_pub=rospy.Publisher('cmd_output',String,queue_size=10)
        rospy.wait_for_message('xf_voice',String)
	rospy.Subscriber('xf_voice', String, self.simplify_cmd)  
	rospy.loginfo("staring simlify words")
    def simplify_cmd(self,msg):
        string=msg.data
        a = re.compile(r'(厨房).*?(客厅)' and r'(客厅).*?(厨房)')
        b = re.compile(r'(厨房).*?(卧室)' and r'(卧室).*?(厨房)')
        c = re.compile(r'(厨房).*?(餐厅)' and r'(餐厅).*?(厨房)')
        d = re.compile(r'(卧室).*?(客厅)' and r'(客厅).*?(卧室)')
        e = re.compile(r'(餐厅).*?(客厅)' and r'(客厅).*?(餐厅)')
        f = re.compile(r'(卧室).*?(餐厅)' and r'(餐厅).*?(卧室)')
        	        	# line = '去餐厅拿一个杯子给在的卧室志愿者'
        line = string
        m1 = a.findall(line)
        m2 = b.findall(line)
        m3 = c.findall(line)
        m4 = d.findall(line)
        m5 = e.findall(line)
        m6 = f.findall(line)
        target = (len(m1), len(m2), len(m3), len(m4), len(m5), len(m6))
	try:
        	pos = target.index(1) + 1
        	if (pos == 1): m = m1
        	if (pos == 2): m = m2
        	if (pos == 3): m = m3
        	if (pos == 4): m = m4
        	if (pos == 5): m = m5
        	if (pos == 6): m = m6
		if(m[0][0]=="厨房" and m[0][1]=="客厅" ):self.cmd_pub.publish("kit liv")
		if(m[0][0]=="厨房" and m[0][1]=="卧室" ):self.cmd_pub.publish("kit bed")
		if(m[0][0]=="厨房" and m[0][1]=="餐厅" ):self.cmd_pub.publish("kit din")
		if(m[0][1]=="厨房" and m[0][0]=="客厅" ):self.cmd_pub.publish("liv kit")
		if(m[0][1]=="厨房" and m[0][0]=="卧室" ):self.cmd_pub.publish("bed kit")
		if(m[0][1]=="厨房" and m[0][0]=="餐厅" ):self.cmd_pub.publish("din kit")

		if(m[0][0]=="客厅" and m[0][1]=="卧室" ):self.cmd_pub.publish("liv bed")
		if(m[0][0]=="客厅" and m[0][1]=="餐厅" ):self.cmd_pub.publish("liv din")
		if(m[0][1]=="客厅" and m[0][0]=="卧室" ):self.cmd_pub.publish("bed liv")
		if(m[0][1]=="客厅" and m[0][0]=="餐厅" ):self.cmd_pub.publish("din liv")

		if(m[0][0]=="餐厅" and m[0][1]=="卧室" ):self.cmd_pub.publish("din bed")
		if(m[0][1]=="餐厅" and m[0][0]=="卧室" ):self.cmd_pub.publish("bed din")
		
	except:
		self.cmd_pub.publish("FAILED")


if __name__ == '__main__': 
     try:  
	DealCmd()  
        rospy.spin() 
     except :  
         #self.cmd_pub("FAILED") 
	pass

 


