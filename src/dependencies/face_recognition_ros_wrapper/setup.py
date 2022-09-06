#!/usr/bin/env python

from distutils.core import setup
from catkin_pkg.python_setup import generate_distutils_setup

setup_args = generate_distutils_setup(
    install_requires=['opencv-contrib-python', 'dlib', 'face_recognition']
)

setup(**setup_args)
