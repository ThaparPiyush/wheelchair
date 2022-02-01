#!/usr/bin/env python3

# Module to control the robot by manually sending commands to wheel1 and wheel2
# Use this module with care of the robot might have an accident
import time
import sys
import rospy

from std_msgs.msg import String
from geometry_msgs.msg import Twist
# Sabertooth connections
from pysabertooth import Sabertooth
import serial.tools.list_ports as port


MAX_SPEED = 30.0
class MoterControl:
	"""
		We have a direct control to the motors using sabertooth
		We send the velocity parameter to both wheel1 and wheel2 
		max vel: 100(No not try this is will break the robot)
		min vel: 0
		For safety the velocity is set to 30.0
	"""

	def __init__(self):
		self.wheel1 = 0
		self.wheel2 = 0
		self.ROBOT_MODE = "TRACK"

		port = self._find_sabertooth_port()
		self.saber = Sabertooth(port, baudrate=115200, address=128, timeout=0.1)

		rospy.Subscriber("/cmd_vel", Twist, self.callback)
		# rospy.Subscriber("/robot_mode", String, self.mode_callback)

	def _find_sabertooth_port(self):
		"""
			Find the port sabertooth is connected to
			generally - /dev/ACM0
			somtimes  - /dev/ACM1
		"""
		print("\nDetecting sabertooth....\n")
		pl = list(port.comports())
		address = ''
		for p in pl:
		  if 'Sabertooth' in str(p):
		      address = str(p).split(" ")
		print("\nAddress found @")
		if not address:
			print("Port Not Found Exiting")
			sys.exit(0)
		else:
			print(address[0])
		
		return address[0]

	def __move_robot(self,wheel1,wheel2):	
		if wheel1  > MAX_SPEED: 
			wheel1 = MAX_SPEED

		if wheel1 < -MAX_SPEED:
			wheel1 = -MAX_SPEED

		if wheel2  > MAX_SPEED: 
			wheel2 = MAX_SPEED

		if wheel2 < -MAX_SPEED:
			wheel2 = -MAX_SPEED
		self.saber.drive(1, wheel1)
		self.saber.drive(2, wheel2)
		print("wheel1:", wheel1, "wheel2",wheel2)
				

	def callback(self,data):
		
		speed = data.linear.x
		angle = data.angular.z

		print("speed", speed, "angle", angle)

		self.wheel1 = speed + angle
		self.wheel2 = -speed + angle

		self.__move_robot(self.wheel1,self.wheel2)
				


if __name__ == '__main__':

	rospy.init_node('motor_movement', anonymous=True)

	mover = MoterControl()
	rospy.spin()
