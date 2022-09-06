# Face Recognition ROS Wrapper

A modern ROS Wrapper for the python [face_recognition](https://github.com/ageitgey/face_recognition) library based on dlib. To make this code accessible as a ROS package on your system, simply clone this repo into the `src` folder of one of your catkin workspaces and `catkin_make`:

    cd /path/to/catkin_ws/src
    git clone https://github.com/RishabhMalviya/face_recognition_ros_wrapper.git
    cd ../
    catkin_make


## How it Works

The face_recognition algorithm recognizes faces by checking against a set of saved encodings of known faces (as a dictionary of `name`-`encoding` pairs). When a new image appears, the system detects and crops out faces from the new image, and compares them against the existing database using a pre-trained siamese neural network (for a more in-depth explanation, take a look at [this article](https://www.pyimagesearch.com/2018/06/18/face-recognition-with-opencv-python-and-deep-learning/)).


## Installng the Dependencies

First, you must make sure you have the relevant dependencies installed.

***TODO***: *Figure out how to use catkin to get the OS to install dependencies automatically during `catkin_make`*

### Python Package Dependencies

The following command should install everything in the correct directories for most cases:

    pip install dlib
    pip install face_recognition

The first command will install the `.so` files and the Python wrappers for [dlib](http://dlib.net/) (which is actually a C++ library for Deep Learning & Computer Vision).

The second will install the [face_recognition](https://github.com/ageitgey/face_recognition) package.

Note, that any Python package you want to use in ROS Python script/node needs to be installed into the  `site-packages` folder associated with the python executable mentioned at the top of the ROS Python script/node (the `#!/usr/bin/env python`). In case your packages are installed into a virtual environment, you will need to change the first line of the relevant ROS Python script/nodes to point to the  virtual environment's python executable:

    #!/path/to/virtual/environment/bin/python

All ROS Python scripts/nodes in this package are present in the folder `scripts`.

### ROS Package Dependencies

If you want to recognize faces in realtime from your webcam, you will require [this ROS package](https://github.com/ros-drivers/video_stream_opencv). Clone it into your catkin workspace and build:

    cd /path/to/catkin_ws/src
    git clone https://github.com/ros-drivers/video_stream_opencv.git
    cd ../
    catkin_make

Refer to the [wiki for this ROS package](http://wiki.ros.org/video_stream_opencv) for any further clarifications.


## Running the Code

There are three launch files given in the `launch` folder:

### Registering Faces

Run this launch file:

    roslaunch face_recognition_ros_wrapper register_faces.launch

This will start up two ROS Service servers:

1. `add_new_face_server`
2. `recognize_faces_server`

The first one can be used to save encodings for new faces. You must provide it with an image containing one face and the name of the person whose face it is:

    rosrun face_recognition_ros_wrapper add_new_face_client </path/to/image/file> <person_name>

By default, these encodings are saved in the file `resources/encodings.pickle`, but you can choose to save to a different file by editing the `config/params.yaml` (as of now, it has just one parameter, which is the location of the encodings file).

The second ROS Service server, `recognize_faces_server`, recognizes faces in a static image. It can be used to verify that the system is able to correctly identify faces that were registered:

    rosrun face_recognition_ros_wrapper recognize_faces_client </path/to/image/file>

Note that you may need to register multiple images of each person to get better accuracy.

### Live Face Recognition from Webcam

To run this launch file:

    roslaunch face_recognition_ros_wrapper live_face_recognition_from_webcam.launch

This will continuously monitor the webcam feed from your laptop (or more accurately, whatever video device is mounted at `/dev/video0` on your Linux machine), and output any recognized faces to the ROS topic `/recognized_faces`.

***TODO***: *Add timestamps and frame_id to the data being published on the `/recognized_faces` topic; basically, make it possible to correlate back to the original video stream and see which image from the stream gave rise to a certain face_recognition prediction.*

### Live Face Recognition from Video Stream from ROS Topic

To run this launch file:

    roslaunch face_recognition_ros_wrapper live_face_recognition_from_video_stream.launch video_stream_topic:=<video/stream/topic>

This will run nodes that expect a stream of messages of type `sensor_msgs/Image` to be published on the given ROS topic (argument `video_stream_topic`). The recognized_faces will be published on the ROS topic `/recognized_faces`.
