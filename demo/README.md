# Simple ROS demo

This package can be used with or without ROS (as described in the [main Readme](../README.md)).
This demo contains a minimalistic usage example based on [roscpp_tutorials](https://github.com/ros/ros_tutorials/blob/noetic-devel/roscpp_tutorials) `add_two_ints_server` node.


## Run with Docker

Build and run the provided demo Docker image:

```bash
cd demo
docker compose -f docker-compose.ros.yaml build
docker compose -f docker-compose.ros.yaml run --rm util_caching_ros
```

There, you'll find a built catkin workspace and running roscore in `/home/blinky/catkin_ws`. 
To run the demo:

```bash
blinky@253f3b320ece:~/catkin_ws$ cd catkin_ws
blinky@253f3b320ece:~/catkin_ws$ source devel/setup.bash
blinky@253f3b320ece:~/catkin_ws$ rosrun roscpp_tutorials add_two_ints_server &
blinky@253f3b320ece:~/catkin_ws$ rosservice call /add_two_ints 3 5
[ INFO] [1707157423.856703322]: request: x=3, y=5
[ INFO] [1707157423.857378676]: not found in cache, computing...
[ INFO] [1707157423.857390462]:   sending back response: [8]
sum: 8
blinky@253f3b320ece:~/catkin_ws$ rosservice call /add_two_ints 3 5
[ INFO] [1707157427.556043566]: request: x=3, y=5
[ INFO] [1707157427.556130000]: found in cache, skipping computation
[ INFO] [1707157427.556155233]:   sending back response: [8]
sum: 8
blinky@253f3b320ece:~/catkin_ws$ rosservice call /add_two_ints 3 1
[ INFO] [1707157432.368645502]: request: x=3, y=1
[ INFO] [1707157432.368712881]: not found in cache, computing...
[ INFO] [1707157432.368757645]:   sending back response: [4]
sum: 4
```