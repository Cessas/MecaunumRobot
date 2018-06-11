# MecaunumRobot
we simplify the underlying system,reduce one sheild to hold on the whole functions.
Added power module
# How 2 use
see http://wiki.ros.org/kinetic/Installation/Ubuntu to install ROS
	1.git clone this rep,kinetic branch into $YOUR_CATKIN_WS/SRC
	2.catkin_make
	3.sudo apt-get install ros-kinetic-ecl-threads && ros-kinetic-move-base-msgs
	4.fire up the terminal then # roslaunch bringup bringup_with_smoother.launch to launch the robot and joy controller
	5.# roslaunch bringup bringup_without_map_and_navigation.launch to launch navigation and gmapping simultaneously
	6.to use voice command,roslaunch bringup voice_command.launch .you SHOULD add your rules in convert.py.now ONLY support 4 scenes.
#EMAIL:yuguantoumx@icloud.com
#FROM AHJZU ROBOT INNOVATION LAB
