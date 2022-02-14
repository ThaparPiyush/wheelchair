#include <wheelchair_navigation/max_velocity_calculator.h>

MaxVelocity::MaxVelocity() : m_fMax_velocity(0.0)
{

}

MaxVelocity::~MaxVelocity() 
{

}
void MaxVelocity::odomCallback(const nav_msgs::Odometry& odom)
{
    m_gPose = odom.pose.pose;
    m_fMax_velocity = maxVelocity(m_gPose, m_gPrev_pose);
    if (m_fMax_velocity > 0.08)
        ROS_INFO("MAX VELOCITY: %f", m_fMax_velocity);
    // ROS_INFO("MAX VELOCITY: %f", m_fMax_velocity);
    
    m_gPrev_pose = m_gPose;
}

float MaxVelocity::maxVelocity(const geometry_msgs::Pose& current_pose, const geometry_msgs::Pose& previous_pose)
{
    float max_vel = (current_pose.position.x - previous_pose.position.x)* 200;
    
    return max_vel;
}
int main(int argc, char** argv)
{
        ros::init(argc, argv, "max_velocity_calculator");
        ros::NodeHandle n;
        MaxVelocity m_obj;
        ros::Rate rate(10);
        m_obj.m_Odom_sub_ = n.subscribe("/camera/odom/sample", 10, &MaxVelocity::odomCallback, &m_obj);
        while (ros::ok())
        {
                rate.sleep();
                ros::spinOnce();
        }
        return 0;
}