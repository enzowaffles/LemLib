#include "main.h"
#include "lemlib/chassis/chassis.hpp"


extern pros::Imu Inertial;

//drivetrain
extern pros::Motor drive_LF;
extern pros::Motor drive_LM;
extern pros::Motor drive_LB;

extern pros::Motor drive_RF;
extern pros::Motor drive_RM;
extern pros::Motor drive_RB;

//MotorGroups
extern pros::MotorGroup Left_Drive;
extern pros::MotorGroup Right_Drive;


//Controller
extern pros::Controller controller;

//Pneumatics
// extern pros::adi::DigitalOut tilt;
extern pros::adi::DigitalOut clamp;
extern pros::adi::DigitalOut doinker;
extern pros::adi::DigitalOut hang_piston;

extern lemlib::Drivetrain drivetrain;
extern lemlib::ControllerSettings linear_controller;
extern lemlib::ControllerSettings angular_controller;
extern lemlib::OdomSensors sensors;
extern lemlib::Chassis chassis;

extern pros::Rotation vert_encoder;
extern pros::Rotation hort_encoder;

extern pros::IMU imu;

extern pros::Motor intake1;
extern pros::Motor arm;
extern pros::Motor intake2;

extern pros::Optical colorsensor;
extern pros::Distance distancesensor;
extern pros::Distance distancesensor2;
extern pros::Distance distancesensor3;
extern pros::Gps gpssensor;


extern pros::Rotation armsensor;
extern pros::adi::DigitalOut claw;

// extern pros::Link transmitter;
// extern pros::Link receiver;

// extern RobotLink* transmitter;
extern RobotLink* receiver;

extern int DATA_SIZE;