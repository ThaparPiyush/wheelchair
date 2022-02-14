#pragma once
#include<ros/ros.h>
#include<ros/console.h>
#include<nav_msgs/Odometry.h>
#include<geometry_msgs/Pose.h>

class MaxVelocity
{
public:
MaxVelocity();
~MaxVelocity();
void odomCallback(const nav_msgs::Odometry& odom);
float maxVelocity(const geometry_msgs::Pose& current_pose, const geometry_msgs::Pose& previous_pose);
// ros::Subscriber
ros::Subscriber m_Odom_sub_;

// Variables
geometry_msgs::Pose m_gPose;
geometry_msgs::Pose m_gPrev_pose;
float m_fMax_velocity;

};
