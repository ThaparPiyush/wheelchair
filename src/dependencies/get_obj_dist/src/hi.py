#!/usr/bin/env python3


import rospy




def hi():
    while True:
      print(rospy.Time.now())
if __name__=="__main__":

  rospy.init_node('humna_detect_follow')
  hi()
