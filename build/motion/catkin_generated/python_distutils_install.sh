#!/bin/sh

if [ -n "$DESTDIR" ] ; then
    case $DESTDIR in
        /*) # ok
            ;;
        *)
            /bin/echo "DESTDIR argument must be absolute... "
            /bin/echo "otherwise python's distutils will bork things."
            exit 1
    esac
fi

echo_and_run() { echo "+ $@" ; "$@" ; }

echo_and_run cd "/home/piyush/wheelchair/src/motion"

# ensure that Python install destination exists
echo_and_run mkdir -p "$DESTDIR/home/piyush/wheelchair/install/lib/python2.7/dist-packages"

# Note that PYTHONPATH is pulled from the environment to support installing
# into one location when some dependencies were installed in another
# location, #123.
echo_and_run /usr/bin/env \
    PYTHONPATH="/home/piyush/wheelchair/install/lib/python2.7/dist-packages:/home/piyush/wheelchair/build/lib/python2.7/dist-packages:$PYTHONPATH" \
    CATKIN_BINARY_DIR="/home/piyush/wheelchair/build" \
    "/usr/bin/python2" \
    "/home/piyush/wheelchair/src/motion/setup.py" \
     \
    build --build-base "/home/piyush/wheelchair/build/motion" \
    install \
    --root="${DESTDIR-/}" \
    --install-layout=deb --prefix="/home/piyush/wheelchair/install" --install-scripts="/home/piyush/wheelchair/install/bin"
