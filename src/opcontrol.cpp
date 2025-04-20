#include "main.h"
#include "api.h"
#include "pros/adi.hpp"
#include "pros/gps.h"
#include "pros/imu.hpp"
#include "pros/misc.h"
#include "pros/misc.hpp"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include "pros/optical.hpp"
#include "pros/vision.hpp"
#include "lemlib/api.hpp"
// #include "globals.cpp"

void my_opcontrol()
{
    
    // pros::Link transmitter(12, "ABCD", pros::E_LINK_TRANSMITTER);
    // pros::Link receiver(11, "ABCD", pros::E_LINK_RECIEVER);
    // transmitter.setRemoteLocation(lemlib::Pose(42, 42, 42));
    // RobotLink receiver(11, "Robot_VRC_2055", pros::E_LINK_RECIEVER);
    // transmitter->setRemoteLocation(lemlib::Pose(42, 42, 42));
    lemlib::Pose info = {23, 23, 23};
    // pros::rtos::Task my_task(my_task_fn);
    while (true)
    {
        // info = receiver->getRemoteLocation(); // current issue: unsure if receiver ever started receiving (and if transmitter transmitted in the first place)
        // pros::lcd::set_text(1, "RECEIVED " + std::to_string(info.x) + " " + std::to_string(info.y) + " " + std::to_string(info.theta));
        // pros::lcd::set_text(2, "DATA: " + std::to_string(receiver->getNumErrorsRx()));
        
        setDriveMotors(); // sets motors based on joystick inputs
        driveIntake(); // sets intake based on L1 input -- comment this when running colorsort task
        driveClamp();
        doink();
        hang();
        // istherearobot();
        // pros::lcd::set_text(5, "X: "  +  std::to_string(chassis.getPose().x)); // print the x position
        // pros::lcd::set_text(6, "Y: " + std::to_string(chassis.getPose().y)); // print the y position
        // pros::lcd::set_text(7, "Angle: " + std::to_string(chassis.getPose().theta)); // print the heading
        pros::lcd::set_text(5,"d: "+std::to_string(distancesensor.get()));
        // pros::lcd::set_text(6,"confidence: "+std::to_string(distancesensor.get_confidence()));
        // pros::lcd::set_text(7,"size: "+std::to_string(distancesensor.get_object_size()));
        pros::lcd::set_text(6,"velocity: "+std::to_string(distancesensor.get_object_velocity()));

        pros::lcd::set_text(1,"GPS x: "+std::to_string(gpssensor.get_position_x()*39.7));
        pros::lcd::set_text(2,"GPS y: "+std::to_string(gpssensor.get_position_y()*39.7));
        pros::lcd::set_text(3, "Error: " + std::to_string(gpssensor.get_error()*39.7));

        // if (istherearobot()==false) {
        //     pros::lcd::set_text(7,":)");
        // }
        // else if (istherearobot()==true) {
        //     pros::lcd::set_text(7,"smth");
        // }

        // if (distancesensor.get_object_velocity()<0.09) {
        //     pros::lcd::set_text(7,":)");
        // }
        // else if (distancesensor.get_object_velocity()>0.09) {
        //     pros::lcd::set_text(1,"......................................................................................");
        //     pros::lcd::set_text(2,"........................................................................................");
        //     // pros::lcd::set_text(3,"");
        //     // pros::lcd::set_text(4,"");
        //     pros::lcd::set_text(5,"........................................................................................");
        //     pros::lcd::set_text(6,"........................................................................................");
        //     pros::lcd::set_text(7,"........................................................................................");

        // }

        // pros::lcd::set_text(5,"x: "+std::to_string(gpssensor.get_position_x()));
        // pros::lcd::set_text(6,"y: "+std::to_string(gpssensor.get_position_y()));
        // pros::screen::print(TEXT_MEDIUM, 1, "X Position: %3f", gpssensor.get_position_x());
        // pros::screen::print(TEXT_MEDIUM, 2, "Y Position: %3f", gpssensor.get_position_y());

        pros::delay(1);
    }
}

