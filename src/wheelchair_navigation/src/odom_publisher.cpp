#include <wheelchair_navigation/odom_publisher.h>

OdomPublisher::OdomPublisher() : x(0.0), y(0.0)
{

}

OdomPublisher::~OdomPublisher() 
{

}

void OdomPublisher::twistCallback(const geometry_msgs::Twist& twist)
{
    m_gTwist = twist;
}
void OdomPublisher::positionCallback(const geometry_msgs::Point& position)
{
    m_gPose = position;
    double distance = (m_gPose.x - m_gPrevPose.x);
    double theta = m_gPose.z * angle_error_factor * radian;
    x = x + distance*cos(theta);
    y = y + distance*sin(theta);
    //since all odometry is 6DOF we'll need a quaternion created from yaw
    geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(theta);

    //first, we'll publish the transform over tf
    // geometry_msgs::TransformStamped odom_trans;
    odom_trans.header.stamp = ros::Time::now();
    odom_trans.header.frame_id = "odom";
    odom_trans.child_frame_id = "base_link";

    odom_trans.transform.translation.x = x/1000;
    odom_trans.transform.translation.y = y/1000;
    odom_trans.transform.translation.z = 0.0;
    odom_trans.transform.rotation = odom_quat;

    //send the transform

    //next, we'll publish the odometry message over ROS
    nav_msgs::Odometry odom;
    odom.header.stamp = ros::Time::now();
    odom.header.frame_id = "odom";
    odom.child_frame_id = "base_link";

    //set the position
    odom.pose.pose.position.x = x/1000;
    odom.pose.pose.position.y = y/1000;
    odom.pose.pose.position.z = 0.0;
    odom.pose.pose.orientation = odom_quat;

    //set the velocity
    odom.twist.twist.linear.x = 0;
    odom.twist.twist.linear.y =  0;
    odom.twist.twist.angular.z =  0;

     m_gPrevPose.x = m_gPose.x;
    //publish the message
    odom_broadcaster.sendTransform(odom_trans);
    m_Odom_pub_.publish(odom);
}

int main(int argc, char** argv)
{
        ros::init(argc, argv, "odom_publisher");
        ros::NodeHandle n;
        OdomPublisher m_obj;
        ros::Rate rate(20);
        m_obj.m_Position_sub_ = n.subscribe("position", 10, &OdomPublisher::positionCallback, &m_obj);
        m_obj.m_Velocity_sub_ = n.subscribe("wheelchair_diff/cmd_vel", 10, &OdomPublisher::twistCallback, &m_obj);
        m_obj.m_Odom_pub_ = n.advertise<nav_msgs::Odometry>("odom", 1000);
        while (ros::ok())
        {
                ros::spinOnce();
                rate.sleep();
        }
        return 0;
}