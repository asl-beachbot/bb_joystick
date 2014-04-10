cmake_minimum_required(VERSION 2.8.3)
project(bb_joystick)

find_package(catkin REQUIRED COMPONENTS roscpp rospy std_msgs geometry_msgs genmsg tf bb_state)

catkin_package()

include_directories(${catkin_INCLUDE_DIRS})

add_executable(JoystickNode src/JoystickNode.cpp)
target_link_libraries(JoystickNode ${catkin_LIBRARIES})
add_dependencies(JoystickNode bb_state_gencpp)
