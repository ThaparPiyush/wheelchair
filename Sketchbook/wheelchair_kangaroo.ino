#include <ros.h>
#include <Kangaroo.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Empty.h>
#include <ros/time.h>
#include <tf/tf.h>
#include <tf/transform_broadcaster.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Point.h>
#include <nav_msgs/Odometry.h>

// Independent mode channels on Kangaroo are, by default, '1' and '2'.
KangarooSerial  K(Serial2);
KangarooChannel Drive(K, 'D');
KangarooChannel Turn(K, 'T');

unsigned long timer;
unsigned long currentMillis;
float radian = 3.14159265359/180;
float angle_error_factor = 0.9;

float oldPosition_z=0;
float newPosition_z;
float oldPosition_x=0;
float newPosition_x;

long vx = 0;
long vth = 0;
double dx = 0;
double dtheta = 0;

ros::NodeHandle  nh;

  void cmd_velCallback(const geometry_msgs::Twist& cmd_vel)
  {
    vx = cmd_vel.linear.x;
    vth = cmd_vel.angular.z;
    long vel_x = (cmd_vel.linear.x)*1000;
    long vel_th = (cmd_vel.angular.z)*57;

    if(int(vel_x) == 0 && int(vel_th)==0)
    {
      Drive.s(0);
      Turn.s(0);
    }
    else
    {
      Drive.s(vel_x);
      Turn.s(vel_th);
    } 

  }


    
  //Motor Command Subscribers
  ros::Subscriber<geometry_msgs::Twist> Twist_Sub("wheelchair_diff/cmd_vel", &cmd_velCallback );
  geometry_msgs::Point position_msg;
//  nav_msgs::Odometry odom_msg;
//  Encoder Data Publishers
//  ros::Publisher odom_pub("odom", &odom_msg);
  ros::Publisher pos_pub("position", &position_msg);
  
  geometry_msgs::TransformStamped t;
  tf::TransformBroadcaster broadcaster;

  double x = 0.0;
  double y = 0.0;
  double theta = 0.0;
  char base_link[] = "base_link";
  char odom[] = "odom";

  void odom_publisher()
  {  

    newPosition_x=float(Drive.getP().value());
    newPosition_z=float(Turn.getP().value());
    position_msg.x = newPosition_x;
    position_msg.z = newPosition_z;
    pos_pub.publish(&position_msg);
//
//    dx = (newPosition_x-oldPosition_x);
////    dtheta = newPosition_z;
//    
//    theta = newPosition_z * angle_error_factor * radian; //in radian
//    x = x + dx*cos(theta);
//    y = y + dx*sin(theta);
////    if(theta > 3.14)
////      theta=-3.14;
//    oldPosition_x = newPosition_x;
//    // tf odom->base_link
//    t.header.frame_id = odom;
//    t.child_frame_id = base_link;
//    
//    t.transform.translation.x = x/1000; //in meters
//    t.transform.translation.y = y/1000;
//    
//    t.transform.rotation = tf::createQuaternionFromYaw(theta);
//    t.header.stamp = nh.now();
//    
//    broadcaster.sendTransform(t);
//
//    odom_msg.header.frame_id = odom;
//    odom_msg.header.stamp = nh.now();
////    odom_msg.header.seq++;
//    odom_msg.child_frame_id = base_link;
//    //set the position
//    odom_msg.pose.pose.position.x = x/1000;
//    odom_msg.pose.pose.position.y = y/1000;
//    odom_msg.pose.pose.position.z = 0.0;
//    odom_msg.pose.pose.orientation = tf::createQuaternionFromYaw(theta);
//
//    //set the velocity
//    odom_msg.twist.twist.linear.x = vx;
//    odom_msg.twist.twist.linear.y = 0.0;
//    odom_msg.twist.twist.angular.z = vth;
//
//    odom_pub.publish(&odom_msg);
    
  }

void setup()
{
  
//**********Kangaroo Setup**********//  
  Serial2.begin(9600);
  Drive.start();
  Turn.start();
  Drive.units(974, 16384);  //calculated by encoder calculator on kangaroo website
  Turn.units(360, 25369);   //calculated by encoder calculator on kangaroo website
  Drive.home().wait();
  Turn.home().wait();

  nh.getHardware()->setBaud(115200);
  
  //Publishers & Subscribers Initialise  
  nh.initNode();
  broadcaster.init(nh);
//  nh.advertise(odom_pub);
  nh.advertise(pos_pub);
  nh.subscribe(Twist_Sub);
  currentMillis = millis();
}

void loop()
{
  
  if (currentMillis-timer >= 50) //publish every 50 milliseconds
  {
    timer = currentMillis + 50;
    odom_publisher();
  }
  currentMillis=millis();
  nh.spinOnce();  
}
