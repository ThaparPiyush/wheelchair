# wheelchair
=========================================================================

SLAM using Gmapping, Navigation Stack and gazebo

visit following for detailed elaboration 

[gazebo](http://gazebosim.org/)

[Navigation stack](http://wiki.ros.org/navigation)

[gmapping](http://wiki.ros.org/gmapping)

Packages
--------------------------------------------------------------------------

* wheelchair_description: consist of wheelchair model and launch files

* wheelchair_simulation: consist of all gazebo worlds and launch files

* wheelchair_cartographer: consist of mapping configurations and launch files

* wheelchair_navigation: consist of navigation parameters and launch files

 wheelchair_description
--------------------------------------------------------------------------

I.To view robot model in Rviz use

	$ roslaunch wheelchair_description display.launch

II.To view robot model in Gazebo use

	$ roslaunch wheelchair_description gazebo.launch


 wheelchair_simulation
--------------------------------------------------------------------------

I.To launch any of the gazebo world use 

	$ roslaunch wheelchair_simulation hospital_world.launch
	

II. To perform teleop operation on robomuse in gazebo use

	$ rosrun wheelchair_simulation robot_teleop_key


 wheelchair_cartographer
--------------------------------------------------------------------------

I. To make map of the world using cartographer use
	
	$ roslaunch wheelchair_cartographer gampping.launch

II. To save the map of the world use
	
	$ rosrun map_server map_saver -f ~/map_name


 wheelchair_navigation
--------------------------------------------------------------------------

I. To perform navigation in any of the world launched and mapped earlier use
 
	$ roslaunch wheelchair_navigation hw_robot_navigation.launch

-------------------------------------------------------------------------
