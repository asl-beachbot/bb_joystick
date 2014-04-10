#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <geometry_msgs/Twist.h>
#include <bb_state/TwistWithID.h>


class JoystickNode
{
public:
  double absolutespeed;
  double angularspeed;
  uint8_t rake;

  void joystickCallback(const sensor_msgs::Joy &joymsg)
  {
    absolutespeed = joymsg.axes[1];
    angularspeed = joymsg.axes[0];
    rake = 0;
    for(int i = 0; i < 7; i++) {
      rake |= joymsg.buttons[i] << i;
    }
  }
};



int main(int argc, char **argv){
  JoystickNode joyinstance;
  const double abs_vel_mult = 0.4;
  const double ang_vel_mult = 1;
  ROS_INFO("Joystick Starter");

  ros::init(argc, argv, "JoystickNode");
  
  ros::NodeHandle n;
  ros::Subscriber joysticksub = n.subscribe("joy", 1, &JoystickNode::joystickCallback, &joyinstance);
  ros::Publisher JoystickTwistPub = n.advertise<bb_state::TwistWithID>("move_io", 1);
  ros::Rate loop_rate(25);

  geometry_msgs::Twist velocitymsg;

  while (ros::ok())
  {

    velocitymsg.linear.x = abs_vel_mult * joyinstance.absolutespeed;
    velocitymsg.angular.z = ang_vel_mult * joyinstance.angularspeed;

    bb_state::TwistWithID twist_publish;
    twist_publish.twist = velocitymsg;
    twist_publish.rake = joyinstance.rake;
    twist_publish.id = 2;   //ID for joystick state is 2
    JoystickTwistPub.publish(twist_publish);

    ros::spinOnce();
    loop_rate.sleep();
  }
}
