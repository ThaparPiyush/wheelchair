# wheelchair
=========================================================================
SLAM using Cartographer, Navigation Stack and gazebo

visit following for detailed elaboration 

[gazebo](http://gazebosim.org/)

[Navigation stack](http://wiki.ros.org/navigation)

[ros-cartographer](https://google-cartographer-ros.readthedocs.io/en/latest/)

Packages
--------------------------------------------------------------------------

* wheelchair_description: consist of robot model and configurations and launch files

* wheelchair_simulation: consist of all gazebo worlds and launch files

* wheelchair_cartographer: consist of cartographer configurations and launch files

* wheelchair_navigation: consist of navigation parameters and launch files

 Robomuse_description
--------------------------------------------------------------------------

I.To view robot model in Rviz use

	$ roslaunch wheelchair_description display.launch

II.To view robot model in Gazebo use

	$ roslaunch wheelchair_description gazebo.launch


 Robomuse_simulation
--------------------------------------------------------------------------

I.To launch any of the gazebo world use 

	$ roslaunch wheelchair_simulation robomuse5_house.launch
	
	$ roslaunch robomuse_simulation robomuse_workhouse.launch
	
	$ roslaunch robomuse_simulation robomuse5_world.launch

	$ roslaunch robomuse_simulation try_world.launch

II. To perform teleop operation on robomuse in gazebo use

	$ roslaunch robomuse_simulation robomuse5_teleop_key.launch


 Robomuse_cartographer
--------------------------------------------------------------------------

I. To make map of the world using cartographer use
	
	$ roslaunch robomuse_cartographer cartographer.launch

II. To save the map of the world use
	
	$ rosrun map_server map_saver -f ~/map_name


 Robomuse_navigation
--------------------------------------------------------------------------

I. To perform navigation in any of the world launched and mapped earlier use
 
	$ roslaunch robomuse_navigation robomuse5_navigation.launch

-------------------------------------------------------------------------
