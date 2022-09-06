#!/usr/bin/env python3

from math import atan2
import os
import queue
import tf
from sqlalchemy import true
import rospy
from std_msgs.msg import Bool,String
from geometry_msgs.msg import PoseStamped,Pose,Point,Twist
from  visualization_msgs.msg import MarkerArray
# from human_detection.msg import box_list
import time
from nav_msgs.msg import Odometry
from move_base_msgs.msg import MoveBaseAction, MoveBaseGoal
import actionlib

global client

def face_callback(msg):
  global face_list
  face_list1=[]
  face_string_list=msg.data.split("/")
  for face_string1 in face_string_list:
    if len(face_string1)>1:
      face_string=face_string1.split(",") 
      face_list1.append([float(face_string[0]),float(face_string[1]),face_string[2]])
  face_list=face_list1 
  print(face_list)

# def box_callback(msg):
#   global cx_list
#   cx_list1=[]
#   for people in msg.people_list:
#     cx_list1.append((float(people.xmin)+float(people.xmax))/2)
#     # print(cx_list1)
#   cx_list=cx_list1  

def name_callback(msg):
  global cx_list,face_list,human_list,face_pos_ind,face_neg_ind
  # print("cx_list",cx_list,"face_list",face_list,"human_list",human_list)

  face_cx=-1000
  try:
    if any(msg.data in sublist for sublist in face_list):
      face_pos_ind+=1
    else:
      face_neg_ind+=1 
      if face_neg_ind>3:
        face_neg_ind=0
        face_pos_ind=0 
    face_ind=-1    
    for face in face_list:
      face_ind+=1
      if face_pos_ind>4 and msg.data==face[2]:
        face_pos_ind=0
        face_neg_ind=0  
        command=Twist()
        command_pub.publish(command) 

        face_cx=face[0]
        if true:
          # near_index=min(range(len(cx_list)), key = lambda i: abs(cx_list[i]-face_cx))
          req_x=human_list[face_ind][1]
          req_y=human_list[face_ind][2]
          
          print(req_x,req_y)
          if req_x**2+req_y**2>0.4:
            br.sendTransform((req_y-0.3,req_x-0.3, 0),
                      tf.transformations.quaternion_from_euler(0, 0,0),
                      rospy.Time.now(),
                      "goal",
                      "base_link")
            
            (trans,rot) = listener.lookupTransform('/map', '/goal', rospy.Time(0))
            # os.system("rosnode kill /camera/color_rectify_color /camera/points_xyzrgb_hw_registered /camera/realsense2_camera_manager")
            # rospy.sleep(3)


            # goal=PoseStamped()
            # goal.header.stamp=rospy.Time.now()
            # goal.header.frame_id="base_link"
            # goal.pose.position.x=req_y
            # goal.pose.position.y=req_x
            # goal.pose.orientation.w=1
            # for i in range(0,100):
            #   goal_pub.publish(goal)
            print("gi")
            goal = MoveBaseGoal()
            goal.target_pose.header.frame_id = "map"
            goal.target_pose.header.stamp = rospy.Time.now()
            goal.target_pose.pose.position.x = trans[0]
            goal.target_pose.pose.position.y = trans[1]
            goal.target_pose.pose.orientation.x = 0
            goal.target_pose.pose.orientation.y = 0
            goal.target_pose.pose.orientation.z = 0
            goal.target_pose.pose.orientation.w = 1
            
            client.send_goal(goal)
            wait = client.wait_for_result()
            command=Twist()
            command_pub.publish(command) 
            # command=Twist()
            # command.linear.x=0.2
            # angle=atan2(req_x,req_y)
            # if angle>0.5:
            #   command.angular.z=0.1
            # elif angle<0.5:
            #   command.angular.z=-0.1
            # else:
            #   command.angular.z=0
            # command_pub.publish(command)  


          
            # command=Twist()
            # command_pub.publish(command)  
            reach_status=Bool()
            reach_status.data=True
            # stop_pub.publish(reach_status)
            reach_pub.publish(reach_status)
      else:
        # command=Twist()
        # command.angular.z=-0.2
        # command_pub.publish(command) 
        pass

  except:

      # command=Twist()
      # command.angular.z=-0.2
      # command_pub.publish(command) 
      pass
    

  

def Object_callback(object_message):
 global human_list
 human_list1=[]
 for object in object_message.markers:
   oid=object.id
   ox=object.pose.position.x
   oz=object.pose.position.z
   oy=object.pose.position.y
   human_list1.append([oid,ox,oz])
 human_list=human_list1  




if __name__=="__main__":
  global cx_list,face_list,human_list,face_pos_ind,face_neg_ind
  face_pos_ind=face_neg_ind=0
  client = actionlib.SimpleActionClient('move_base',MoveBaseAction)
  cx_list=face_list=human_list=[]
  rospy.init_node('humna_detect_follow')
  br = tf.TransformBroadcaster()

  name_sub=rospy.Subscriber("human_name",String,name_callback)
  face_sub=rospy.Subscriber("recognized_faces",String,face_callback)
  object_sub=rospy.Subscriber('obj_to_dist/show_people_marker_array', MarkerArray , Object_callback)
  # box_sub=rospy.Subscriber('human_detected_image/bounding_box', box_list , box_callback)
  reach_pub=rospy.Publisher("reached",Bool,queue_size=10,latch=True)
  goal_pub=rospy.Publisher("move_base_simple/goal",PoseStamped,queue_size=10,latch=True)
  stop_pub = rospy.Publisher('/move_base_simple/stop', Bool , queue_size=11,latch=True)
  command_pub=rospy.Publisher("wheelchair_diff/cmd_vel",Twist,queue_size=11,latch=True)
  listener = tf.TransformListener() 

  rospy.spin()
