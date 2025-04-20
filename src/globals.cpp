#include "main.h"
#include "pros/adi.hpp"
#include "pros/gps.h"
#include "pros/gps.hpp"
#include "pros/imu.hpp"
#include "pros/misc.h"
#include "pros/misc.hpp"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include "pros/optical.hpp"
#include "pros/vision.hpp"
#include "lemlib/api.hpp"
#include <cstddef>

int MOTOR_LB = -12; //-11
int MOTOR_LM = -10; //-13
int MOTOR_LF = -19; //-1

int MOTOR_RB = 18; //20
int MOTOR_RM = 8; //12
int MOTOR_RF = 5; //18

int VERT_TRACKING_PORT = -4;
int HOR_TRACKING_PORT = 16;

int INERTIAL_PORT = 17;

// Pneumatics
char CLAMP_PORT = 'A';
char HANG_PORT = 'B';
char CLAW_PORT = 'G';
char DOINKER_PORT = 'F';


int MOTOR_INTAKE_1= -7; // top intake
int ARM_PORT = 1;
int INTAKE2_PORT = -6; // bottom intake
int ARM_SENSOR = 11;


int COLOR_SENSOR_PORT = 20; // 

int DISTANCE_PORT = 13;
int DISTANCE_PORT2 = 3;
int DISTANCE_PORT3 = 9;

int GPS_PORT = 13;

int TRANSMITTER_PORT = 12;
int RECEIVER_PORT = 14;

std::string random_string = ""; // varying this length won't change data size
std::string* pointer_to_find_data_size = &random_string;
int DATA_SIZE = sizeof(*pointer_to_find_data_size) * sizeof(pointer_to_find_data_size); // needs adjusting

std::string TRANSMITTER_ID = "2055A Big T";
std::string RECEIVER_ID = "2055A Big R";

//Drivetrain
pros::Motor drive_LB(MOTOR_LB, pros::v5::MotorGears::blue, pros::v5::MotorUnits::degrees);
pros::Motor drive_LM(MOTOR_LM, pros::v5::MotorGears::blue, pros::v5::MotorUnits::degrees);
pros::Motor drive_LF(MOTOR_LF, pros::v5::MotorGears::blue, pros::v5::MotorUnits::degrees);

pros::Motor drive_RB(MOTOR_RB, pros::v5::MotorGears::blue, pros::v5::MotorUnits::degrees);
pros::Motor drive_RM(MOTOR_RM, pros::v5::MotorGears::blue, pros::v5::MotorUnits::degrees);
pros::Motor drive_RF(MOTOR_RF, pros::v5::MotorGears::blue, pros::v5::MotorUnits::degrees);

// Other motors
pros::Motor intake1(MOTOR_INTAKE_1, pros::v5::MotorGears::blue, pros::v5::MotorUnits::degrees);
pros::Motor intake2(INTAKE2_PORT, pros::v5::MotorGears::green, pros::v5::MotorUnits::degrees);
pros::Motor arm(ARM_PORT, pros::v5::MotorGears::blue, pros::v5::MotorUnits::degrees);

//MotorGroups
pros::MotorGroup Left_Drive({-12, -10, -19}, pros::v5::MotorGears::blue);
pros::MotorGroup Right_Drive({18, 8, 5}, pros::v5::MotorGears::blue);

//Controller
pros::Controller controller(pros::E_CONTROLLER_MASTER);

//Pneumatics
pros::adi::DigitalOut clamp('A', true);
pros::adi::DigitalOut hang_piston('B', false);
pros::adi::DigitalOut doinker('F', false);

//Sensors
pros::Rotation vert_encoder(VERT_TRACKING_PORT);
pros::Rotation hort_encoder(HOR_TRACKING_PORT);
pros::Rotation armsensor(ARM_SENSOR);
pros::IMU imu(INERTIAL_PORT);
pros::Optical colorsensor(COLOR_SENSOR_PORT);

pros::Distance distancesensor(DISTANCE_PORT);
pros::Distance distancesensor2(DISTANCE_PORT2);
pros::Distance distancesensor3(DISTANCE_PORT3);
pros::Gps gpssensor(GPS_PORT);

// Radio Link
// pros::Link transmitter(TRANSMITTER_PORT, TRANSMITTER_ID, pros::E_LINK_TRANSMITTER);
// pros::Link receiver(RECEIVER_PORT, TRANSMITTER_ID, pros::E_LINK_RECIEVER);
// RobotLink* transmitter = nullptr;
RobotLink* receiver = nullptr;

//Tracking Wheels
lemlib::TrackingWheel vert_tracking(&vert_encoder, 1.975, -0.75); // 0.75 inches left of the tracking center
lemlib::TrackingWheel hort_tracking(&hort_encoder, 1.95, 0.75); // 0.75 inches forward of tracking center

lemlib::Drivetrain drivetrain {
    &Left_Drive, // left motor group
    &Right_Drive, // right motor group
    11.5, // 10 inch track width
    2.5, // using new 4" omnis
    450, // drivetrain rpm is 360
    2 // horizontal drift is 2 (for now)
};

lemlib::ControllerSettings linear_controller {
    14, // proportional gain (kP) 13
    0, // integral gain (kI)
    60, // derivative gain (kD) 60
    3, // anti windup
    1, // small error range, in inches
    100, // small error range timeout, in milliseconds
    3, // large error range, in inches
    500, // large error range timeout, in milliseconds
    0 // maximum acceleration (slew)
};
 

// turning PID    // 1.5, 0, 7  <-- last values that worked
lemlib::ControllerSettings angular_controller {
    2.47,//, // proportional gain (kP) 2.47
    0, // integral gain (kI)
    16,//, // derivative gain (kD) 16
    3, // anti windup
    1, // small error range, in degrees
    100, // small error range timeout, in milliseconds
    3, // large error range, in degrees
    500, // large error range timeout, in milliseconds
    0 // maximum acceleration (slew)
};

/*
list:
0.5, 0, 0
*/

// odometry struct
lemlib::OdomSensors sensors {
    &vert_tracking, // vertical tracking wheel 1
    nullptr, // vertical tracking wheel 2
    &hort_tracking, // horizontal tracking wheel 1
    nullptr, // horizontal tracking wheel 2
    &imu // &Inertial // inertial sensor
};

// create the chassis
lemlib::Chassis chassis(drivetrain, 
                        linear_controller, 
                        angular_controller, 
                        sensors
);

