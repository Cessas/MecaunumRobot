/*
 * make_plan.cpp
 *
 *  Created on: Aug 10, 2016
 *      Author: unicorn
 */
#include <ros/ros.h>
#include <nav_msgs/GetPlan.h>
#include <geometry_msgs/PoseStamped.h>
#include <string>
#include <boost/foreach.hpp>
#define forEach BOOST_FOREACH
void fillPathRequest(nav_msgs::GetPlan::Request &request)
{
request.start.header.frame_id ="map";
request.start.pose.position.x = 12.378;
request.start.pose.position.y = 28.638;
request.start.pose.orientation.w = 1.0;
request.goal.header.frame_id = "map";
request.goal.pose.position.x = 18.792;
request.goal.pose.position.y = 29.544;
request.goal.pose.orientation.w = 1.0;
request.tolerance = 0.5;
}

void callPlanningService(ros::ServiceClient &serviceClient, nav_msgs::GetPlan &srv)
{
// Perform the actual path planner call
if (serviceClient.call(srv)) {
if (!srv.response.plan.poses.empty()) {
forEach(const geometry_msgs::PoseStamped &p, srv.response.plan.poses) {
ROS_INFO("x = %f, y = %f", p.pose.position.x, p.pose.position.y);
}
}
else {
ROS_WARN("Got empty plan");
}
}
else {
ROS_ERROR("Failed to call service %s - is the robot moving?",
serviceClient.getService().c_str());
}
}

int main(int argc, char** argv)
{
ros::init(argc, argv, "make_plan_node");
ros::NodeHandle nh;
// Init service query for make plan
std::string service_name = "move_base_node/make_plan";
while (!ros::service::waitForService(service_name, ros::Duration(3.0))) {
ROS_INFO("Waiting for service move_base/make_plan to become available");
}
ros::ServiceClient serviceClient = nh.serviceClient<nav_msgs::GetPlan>(service_name, true);
if (!serviceClient) {
ROS_FATAL("Could not initialize get plan service from %s",
serviceClient.getService().c_str());
return -1;
}
nav_msgs::GetPlan srv;
fillPathRequest(srv.request);
if (!serviceClient) {
ROS_FATAL("Persistent service connection to %s failed",
serviceClient.getService().c_str());
return -1;
}
callPlanningService(serviceClient, srv);
}




