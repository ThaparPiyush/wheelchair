#pragma once
#include<ros/ros.h>
#include<ros/console.h>
#include<nav_msgs/Odometry.h>
#include <tf/transform_broadcaster.h>
#include<geometry_msgs/Point.h>
#include<geometry_msgs/Twist.h>

#define angle_error_factor 0.9
#define radian 3.14159265359/180
class OdomPublisher
{
public:
OdomPublisher();
~OdomPublisher();
void positionCallback(const geometry_msgs::Point& position);
void twistCallback(const geometry_msgs::Twist& twist);

// ros::Subscriber
ros::Subscriber m_Position_sub_;
ros::Subscriber m_Velocity_sub_;
// ros::Publisher
ros::Publisher m_Odom_pub_;

// Variables
geometry_msgs::Point m_gPose;
geometry_msgs::Point m_gPrevPose;
geometry_msgs::Twist m_gTwist;
double x;
double y;
//TF
tf::TransformBroadcaster odom_broadcaster;
// tf::Transform tr;
geometry_msgs::TransformStamped odom_trans;
geometry_msgs::Quaternion odom_quat;
};
