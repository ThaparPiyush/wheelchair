
-- Code for getting namespace param ---
-- local getenv = os.getenv
---------------------------------------
include "map_builder.lua"
include "trajectory_builder.lua"

options = {
  map_builder = MAP_BUILDER,
  trajectory_builder = TRAJECTORY_BUILDER,
  map_frame = "map",
  tracking_frame = "base_link", -- imu_link, If you are using gazebo, use 'base_link' (libgazebo_ros_imu's bug)
  published_frame = "odom",
  odom_frame = "odom",
  provide_odom_frame = false,
  publish_frame_projected_to_2d = false,
--  use_pose_extrapolator = false,
  use_odometry = true,
  use_nav_sat = false,
  use_landmarks = false,
  num_laser_scans = 1,
  num_multi_echo_laser_scans = 0,
  num_subdivisions_per_laser_scan = 1,
  num_point_clouds = 0,
  lookup_transform_timeout_sec = 0.2,
  submap_publish_period_sec = 0.6,
  pose_publish_period_sec = 5e-3,
  trajectory_publish_period_sec = 30e-3,
  rangefinder_sampling_ratio = 1.,
  odometry_sampling_ratio = 0.1,
  fixed_frame_pose_sampling_ratio = 1.0,
  imu_sampling_ratio = 0.1,
  landmarks_sampling_ratio = 1.0,
}

--TRAJECTORY_BUILDER.collate_landmarks = on  --  Landmarks will be included in the SLAM accoding to the ID of the `cartographer_ros_msgs/LandmarkEntry`_. The `cartographer_ros_msgs/LandmarkList`_ should be provided at a sample rate comparable to the other sensors.  The list can be empty but has to be provided because Cartographer strictly time orders sensor data in order to make the landmarks deterministic. However it is possible to set the trajectory builder option "collate_landmarks" to false and allow for a non-deterministic but also non-blocking approach.
MAP_BUILDER.use_trajectory_builder_2d = true


TRAJECTORY_BUILDER_2D.num_accumulated_range_data = 1

TRAJECTORY_BUILDER_2D.min_range = 0.15
TRAJECTORY_BUILDER_2D.max_range = 8.0 
TRAJECTORY_BUILDER_2D.missing_data_ray_length = 9.0
TRAJECTORY_BUILDER_2D.use_imu_data = false




----------------PARAMETERS FOR LOCAL SLAM----------------------------

--Prior pose weights given to ceres scan matcher.
TRAJECTORY_BUILDER_2D.ceres_scan_matcher.translation_weight = 1e1		--1*10
TRAJECTORY_BUILDER_2D.ceres_scan_matcher.rotation_weight = 0.001		--0.001*15
TRAJECTORY_BUILDER_2D.ceres_scan_matcher.occupied_space_weight = 1 

--Submap size 
TRAJECTORY_BUILDER_2D.submaps.num_range_data = 45 			--Number of range data added before adding a new submap; determines the size of submap; Small enough to contain local SLAM error (lower than the resolution)  and large enough to be distinct for loop closure





---------------------USE THIS PARAMETER FOR REMOVING GSLAMMING EFFECTS-------------------
POSE_GRAPH.optimize_every_n_nodes = 90  				       --Used for online-loop closing; zero value removes the effect of SLAM
---------------------XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX---------------------




-----------------------------------------
------------Global SLAM------------
POSE_GRAPH.global_sampling_ratio = 0.003 -- Decrease[DEFAULT]
POSE_GRAPH.constraint_builder.sampling_ratio = 0.4 -- Decrease
--POSE_GRAPH.constraint_builder.min_score = 0.85 -- Increase
POSE_GRAPH.global_constraint_search_after_n_seconds = 30 -- Increase

---------Global/Local SLAM---------



MAP_BUILDER.num_background_threads = 8 					--Added background threads as the number of cores of laptop
POSE_GRAPH.optimization_problem.log_solver_summary = true
POSE_GRAPH.constraint_builder.fast_correlative_scan_matcher.angular_search_window = math.rad(15.)
POSE_GRAPH.constraint_builder.fast_correlative_scan_matcher.linear_search_window = 5.0
POSE_GRAPH.optimization_problem.huber_scale = 1e2


-----Added Changes---                      
POSE_GRAPH.constraint_builder.min_score = 0.55				--[DEFAULT]Threshold for the scan match score below which a match is not considered. Low scores indicate that the scan and map do not look similar.
POSE_GRAPH.constraint_builder.global_localization_min_score = 0.6       --[DEFAULT]Threshold below which global localizations are not trusted.
POSE_GRAPH.constraint_builder.max_constraint_distance = 4    	        --Threshold for poses to be considered near a submap. Value 1 made correct map.	
POSE_GRAPH.constraint_builder.fast_correlative_scan_matcher.angular_search_window = math.rad(30.)
POSE_GRAPH.constraint_builder.fast_correlative_scan_matcher.linear_search_window = 5.0


---About to be added changes------------
POSE_GRAPH.constraint_builder.loop_closure_translation_weight = 0.3*1e4  	--Weight used in the optimization problem for the translational component of loop closure constraints.
POSE_GRAPH.constraint_builder.loop_closure_rotation_weight = 0.3*1e5		--Weight used in the optimization problem for the rotational component of loop closure constraints.
POSE_GRAPH.matcher_translation_weight = 10*5e2					--Weight used in the optimization problem for the translational component of non-loop-closure scan matcher constraints.
POSE_GRAPH.matcher_rotation_weight = 10*1.6e3					--Weight used in the optimization problem for the rotational component of non-loop-closure scan matcher constraints.
--POSE_GRAPH.optimization_problem.local_slam_pose_translation_weight = 0.0*1e5	--Scaling parameter for translation between consecutive nodes based on the local SLAM pose. 
--POSE_GRAPH.optimization_problem.local_slam_pose_rotation_weight = 0.0*1e5	--Scaling parameter for rotation between consecutive nodes based on the local SLAM pose.

--POSE_GRAPH.constraint_builder.min_score = 0.85			--0.1 made scans matching with the map neglible, very bad behaviour; 0.85 leads to too much credibility to map and scan matching, map drifted during rotation; 
--POSE_GRAPH.constraint_builder.log_matches = true


--Changes for IMU--
POSE_GRAPH.optimization_problem.odometry_translation_weight = 5*1e5     	--Scaling parameter for translation between consecutive nodes based on the odometry.
POSE_GRAPH.optimization_problem.odometry_rotation_weight = 0.001*0e5		--Scaling parameter for rotation between consecutive nodes based on the odometry.
POSE_GRAPH.optimization_problem.acceleration_weight = 1*1e3    			--Scaling parameter for the IMU acceleration term.
POSE_GRAPH.optimization_problem.rotation_weight = 1*3e5				--Scaling parameter for the IMU rotation term.

return options

