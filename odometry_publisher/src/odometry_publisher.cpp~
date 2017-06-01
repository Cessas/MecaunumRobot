
#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include <encoders_msgs/Encoders.h>
#include<boost/assign.hpp>

#define PI 3.14159265
#define COUNT_OVERFLOW 2147483647
#define COUNT_RESET 200

// total size of int
//#define INT_MAX 65535
int max(int a,int b){
if(a>b) return a;
else return b;
}
int min(int a,int b){
if(a>b) return b;
else return a;
}
// get sign of a number
#define sign(a) (min(1, max(-1, a)))

class OdometryPublisher
{
	public:
		OdometryPublisher();

	private:
		int lastEncoderCounts[4];
		void encoderCallback(const encoders_msgs::Encoders::ConstPtr& encoders);

		ros::NodeHandle nh;
		ros::Subscriber enc_sub;
		ros::Publisher odom_pub;
               // ros::Publisher odom_combined_pub;
		tf::TransformBroadcaster odom_broadcaster;
		
		double x, y, th;
		double scale_x, scale_y, scale_th;
		bool calibration_mode;
};


OdometryPublisher::OdometryPublisher()
{
	for(int i = 0; i < 4; i++)
		this->lastEncoderCounts[i] = 0;	
// intialize integrators
	x = y = th = 0;
	
	// load parameters
    ros::NodeHandle nh_priv("~");
	nh_priv.param("scale_x", scale_x, 0.6283185 / 1660 / 2.15); // loosely base off of 360 counts per rev, 100mm wheels (which is 0.6283185 / 360.0)
	nh_priv.param("scale_y", scale_y, 0.6283185 / 1660 / 2.7);
	nh_priv.param("scale_th", scale_th, 0.0012);
	nh_priv.param("calibration_mode", calibration_mode, false);
	
	// lets show em what we got
	ROS_INFO_STREAM("param scale_x: " << scale_x);
	ROS_INFO_STREAM("param scale_y: " << scale_y);
	ROS_INFO_STREAM("param scale_th: " << scale_th);
	ROS_INFO_STREAM("param calibration_mode: " << calibration_mode);

	// connects subs and pubs
	enc_sub = nh.subscribe<encoders_msgs::Encoders>("Encoders", 10, &OdometryPublisher::encoderCallback, this);
	odom_pub = nh.advertise<nav_msgs::Odometry>("odom", 50);
        //odom_combined_pub = nh.advertise<nav_msgs::Odometry>("odom_combined", 50);
}



void OdometryPublisher::encoderCallback(const encoders_msgs::Encoders::ConstPtr& encoders)
{
	int newEncoderCounts[5];
	newEncoderCounts[1] = encoders->encoderA; // M_fl
	newEncoderCounts[0] = encoders->encoderB; // M_fr
	newEncoderCounts[3] = encoders->encoderC; // M_bl
	newEncoderCounts[2] = encoders->encoderD; // M_br

	newEncoderCounts[4]=encoders->header.stamp.toSec();//保存当前时间

	
	// find deltas
	int deltaEncoderCounts[5];
	for(int i = 0; i < 4; i++) {
		// check for overflow
		if(abs(this->lastEncoderCounts[i]) > COUNT_OVERFLOW && abs(newEncoderCounts[i]) > COUNT_OVERFLOW && sign(this->lastEncoderCounts[i]) != sign(newEncoderCounts[i])) {
			if(sign(this->lastEncoderCounts[i]) > 0)
				deltaEncoderCounts[i] = newEncoderCounts[i] - this->lastEncoderCounts[i] + INT_MAX;
			else
				deltaEncoderCounts[i] = newEncoderCounts[i] - this->lastEncoderCounts[i] - INT_MAX;
		}
		else deltaEncoderCounts[i] = newEncoderCounts[i] - this->lastEncoderCounts[i];
		
		// check for gross change -> MD25 board power cycled
		if(abs(deltaEncoderCounts[i]) > COUNT_RESET) deltaEncoderCounts[i] = 0;
		// no idea what the real value is in this case
		
		// save encoder counts
		this->lastEncoderCounts[i] = newEncoderCounts[i]; // i should grow up and just use a point after the for loop
	}
	
	// convert the motor counts into x, y, theta counts
int xytCounts[3];
        xytCounts[0] = (deltaEncoderCounts[0] + deltaEncoderCounts[1] + deltaEncoderCounts[2] + deltaEncoderCounts[3]) / 4;
	xytCounts[1] = (0 - deltaEncoderCounts[0] + deltaEncoderCounts[1] + deltaEncoderCounts[2] - deltaEncoderCounts[3]) / 4;
	xytCounts[2] = (0 - deltaEncoderCounts[0] + deltaEncoderCounts[1] - deltaEncoderCounts[2] + deltaEncoderCounts[3]) / 4;








	
// unpack the encoder message in base_link frame
	ros::Time current_time = ros::Time::now();
	//double dx =xytCounts[0] * scale_x;
	//double dy = xytCounts[1] * scale_y;
	//double dth = xytCounts[2] * scale_th;
double dx =xytCounts[0] * scale_x;
	double dy = xytCounts[1] * scale_y;
	double dth = xytCounts[2] * scale_th;

	double dt = newEncoderCounts[4]-lastEncoderCounts[4];
	
	// convert to movements in the odom frame
	if(calibration_mode) {
		x += dx;
		y += dy;
		th += dth;
		//th = fmod(th, 2*PI);
	}
    else {
		x += dx * cos(th) - dy * sin(th);
		y += dx * sin(th) + dy * cos(th);
		th += dth;
		th = fmod(th, 2*PI);
    }

	// since all odometry is 6DOF we'll need a quaternion created from yaw
	geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(th);

	// first, we'll publish the transform over tf
	//geometry_msgs::TransformStamped odom_trans;
	//odom_trans.header.stamp = current_time;
	//odom_trans.header.frame_id = "odom_combined";
	//odom_trans.child_frame_id = "base_footprint";

	//odom_trans.transform.translation.x = x;
	//odom_trans.transform.translation.y = y;
	//if(calibration_mode) odom_trans.transform.translation.z = th; // need some way to check this w/o the quaternion
	//else odom_trans.transform.translation.z = 0.0;
	//odom_trans.transform.rotation = odom_quat;

	// send the transform
	//odom_broadcaster.sendTransform(odom_trans);

	// next, we'll publish the odometry message over ROS
	nav_msgs::Odometry odom;
	odom.header.stamp = current_time;
	odom.header.frame_id = "odom_combined";

	// set the position
	odom.pose.pose.position.x = x;
	odom.pose.pose.position.y = y;
	odom.pose.pose.position.z = 0.0;
	odom.pose.pose.orientation = odom_quat;
        odom.pose.covariance =  boost::assign::list_of(1e-3) (0) (0)  (0)  (0)  (0)
                                                       (0) (1e-3)  (0)  (0)  (0)  (0)
                                                       (0)   (0)  (1e6) (0)  (0)  (0)
                                                       (0)   (0)   (0) (1e6) (0)  (0)
                                                       (0)   (0)   (0)  (0) (1e6) (0)
                                                       (0)   (0)   (0)  (0)  (0)  (1e3) ;


	// set the velocity
	odom.child_frame_id = "base_footprint";
	odom.twist.twist.linear.x = dx / dt;
	odom.twist.twist.linear.y = dy / dt;
	odom.twist.twist.angular.z = dth / dt;
        odom.twist.covariance =  boost::assign::list_of(1e-3) (0)   (0)  (0)  (0)  (0)
                                                      (0) (1e-3)  (0)  (0)  (0)  (0)
                                                      (0)   (0)  (1e6) (0)  (0)  (0)
                                                      (0)   (0)   (0) (1e6) (0)  (0)
                                                      (0)   (0)   (0)  (0) (1e6) (0)
                                                      (0)   (0)   (0)  (0)  (0)  (1e3) ; 

	// publish the message
	odom_pub.publish(odom);
}


int main(int argc, char** argv)
{
	ros::init(argc, argv, "odometry_publisher");
	OdometryPublisher odom;

	ros::spin();
}
