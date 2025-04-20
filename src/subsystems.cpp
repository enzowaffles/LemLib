#include "main.h"


//--------------------------------------------------------------------------------------------------
//Clamping Mobile Goal
// -------------------------------------------------------------------------------------------------

// bool clamp_status = true;

// void ClampTilt_Goal () {
//     if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)) {
//         clamp_status = !clamp_status;
//         // clamp_status = true;
//         clamp.set_value(clamp_status);
//         pros::delay(250);
//         tilt.set_value(clamp_status);
//         // clamp.set_value(false);
//     }
// }

bool clampState = false;
void driveClamp()
{
    // if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2))
    // {
        clamp.set_value(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2));
        //clampState = !clampState;
        //clamp.set_value(clampState);
    //}
}

// void setarm() {
//     if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B))
//     {
//         lbgetring(1, 5);
//     }
// }

// ideal position: 1064

int currentAngle;
int error = 1064;
const double kP = 0.06;
const int deadband = 350;
const int targetAngle = 900;
bool hold;

void setArmLoadNew()
{
    while (true)
    {
        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A))
        {
            arm.set_brake_mode(MOTOR_BRAKE_HOLD);
            currentAngle = armsensor.get_angle();
            if (30000 <= currentAngle && currentAngle <= 36000) 
            {
                currentAngle = 0 - (36000 - currentAngle);
            }
            error = targetAngle - currentAngle;
            while (abs(error) > deadband)
            {
                currentAngle = armsensor.get_angle();
                if (30000 <= currentAngle && currentAngle <= 36000) 
                {
                    currentAngle = 0 - (36000 - currentAngle);
                }
                error = targetAngle - currentAngle;
                arm.move_velocity(error * kP);
               //pros::lcd::set_text(3, "Target: " + std::to_string(target));
                pros::lcd::set_text(4, "Current Angle: " + std::to_string(currentAngle));
                pros::lcd::set_text(5, "Error: " + std::to_string(error));
                pros::delay(1);
            }
            arm.move_velocity(0);
            // arm.set_brake_mode(MOTOR_BRAKE_HOLD);
            // arm.move_velocity(0);
        }
        // else
        // {
        //     // hold arm in place if in loading position
        //     // if (hold)
        //     // {
        //     //     // 100 too fast, 40 too fast, 20 too fast, 10 too fast, 5 too fast, 2 too fast
        //     //     // this is the velocity at which the arm will move upwards to hold
        //     //     arm.move_velocity(0);
        //     // }
        //     driveArm();
        //     pros::delay(1);
        // }
    }
}

const int numstates = 2;
int states[numstates] = {0, 1000}; //1000
int currState = 0;


void setArmLoad1()
{
    while (true)
    {
        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B))
        {
            currState += 1;
            if (currState == 2) {
                currState = 0;
            }
            int target = states[currState];

            arm.set_brake_mode(MOTOR_BRAKE_HOLD);
            currentAngle = armsensor.get_angle();
            if (30000 <= currentAngle && currentAngle <= 36000) 
            {
                currentAngle = 0 - (36000 - currentAngle);
            }
            error = target - currentAngle;
            while (abs(error) > deadband)
            {
                currentAngle = armsensor.get_angle();
                if (30000 <= currentAngle && currentAngle <= 36000) 
                {
                    currentAngle = 0 - (36000 - currentAngle);
                }
                error = target - currentAngle;
                arm.move_velocity(error * kP);
                // pros::lcd::set_text(3, "Target: " + std::to_string(currState));
                // pros::lcd::set_text(4, "Current Angle: " + std::to_string(currentAngle));
                // pros::lcd::set_text(5, "Error: " + std::to_string(error));
                pros::delay(1);
            }
            arm.move_velocity(0);
            // arm.set_brake_mode(MOTOR_BRAKE_HOLD);
            // arm.move_velocity(0);
        }
        else
        {
            driveArm();
            pros::delay(1);
        }
    }
}

void setarm() {
    arm.set_brake_mode(MOTOR_BRAKE_HOLD);
        currentAngle = armsensor.get_angle();
        if (30000 <= currentAngle && currentAngle <= 36000) 
        {
            currentAngle = 0 - (36000 - currentAngle);
        }
        error = targetAngle - currentAngle;
        while (abs(error) > deadband)
        {
            currentAngle = armsensor.get_angle();
            if (30000 <= currentAngle && currentAngle <= 36000) 
            {
                currentAngle = 0 - (36000 - currentAngle);
            }
            error = targetAngle - currentAngle;
            arm.move_velocity(error * kP);
            pros::delay(1);
        }
        arm.move_velocity(0);
    }

bool checkForJam = false;

void setIntake(int power)
{
    intake1.move(power);
    intake2.move(power);
}

// this function is used for turning on the intake when driving
void driveIntake() 
{
    int power = 127 * (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1) - controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)); // power = 127 if L1 is being pressed, otherwise power = 0
    setIntake(power);
    // int power2 = 110 * (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1) - controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2));
    // intake2.move(power2);
}

bool checkforJam()
{
    if (abs(intake1.get_actual_velocity()) > 10)
    {
        return true;
    }
    else 
    {
        return false;
    }
}

void fixJam()
{
    setIntake(0);
    pros::delay(250);
    setIntake(127);
}

void antiJamTask()
{
    while (true)
    {
        if (checkForJam)
        {
            if (abs(intake1.get_actual_velocity()) < 10)
            {
                setIntake(0);
                pros::lcd::set_text(2, "JAM DETECTED: MOTOR STOPPED.");
                pros::delay(250);
                setIntake(127);
                pros::lcd::set_text(3, "WAIT COMPLETED: MOTOR SPINNING.");
            }
        }
    pros::delay(100);
    }
}

bool doink_status = false;
void doink() {
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) {
        doink_status = !doink_status;
        doinker.set_value(doink_status);
    }
}

bool hang_status = false;
void hang() {
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)) {
        hang_status = !hang_status;
        hang_piston.set_value(hang_status);
    }
}

// HOLDING L1 RAISES THE ARM, HOLDING L2 LOWERS THE ARM UNTIL YOU LET GO.
void driveArm()
{
    int arm_power = 600 * (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1));

    if (armsensor.get_angle() > 13500 && armsensor.get_angle() < 20800) {

        if (arm_power > 0)
        {
            arm_power = 0;
        }
    }

    arm.move_velocity(arm_power);

}
    
bool clawState = true;
void driveClaw()
{
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X))
    {
        clawState = !clawState;
        claw.set_value(clawState);
    }
}

double d1=distancesensor.get();
double v=distancesensor.get_object_velocity();
bool robot=false;
bool istherearobot() {
    // double d2=d1;
    // pros::delay(50);
    // d1=distancesensor.get();
    v=distancesensor.get_object_velocity();
    // if (double(d2) - double(d1)>=100) {
    //     bool robot=true;
    // }
    // else if (double(d2) - double(d1)<=-100) {
    //     bool robot=false;
    // }
    // double velocity = d1 - d2;
    if (v>0.1) {
        bool robot=true;
    }
    else if (v<=0.1) {
        bool robot=false;
    }
    //d1=distancesensor.get();
    //pros::delay(10);
    pros::lcd::set_text(2, std::to_string(d1));
    pros::lcd::set_text(1, std::to_string(robot));
    // pros::lcd::set_text(3, std::to_string(velocity));
    return(robot);

}

//return values using triangulation, converted to inches and in (x,y). rmr to add the size of the bot, using full width and half length. if values are unusually low (see if anlge is pointing at corner, that will have larger distance), then go to diff tracking system.
double idistance1 = distancesensor.get();
double fdistance = idistance1;
double distance2 = distancesensor2.get();
double distance3 = distancesensor3.get();
double botlength = 444.5; //-23; //real length - distance sensor length (BOTH ESTIMATED)
double botwidth = 345.6; //-23; //real length - distance sensor width (BOTH ESTIMATED)
double fieldmax = 5165.5; //5161.88
int fieldL = 3700; //3650

std::vector<double> epicPositionTest() {
    //if in quadrant x, turn to heading 90/180/270/0
    //back up until wall
    //test values
    double x = chassis.getPose().x;
    double y = chassis.getPose().y;
    bool correcty = true;
    bool correctx = true;
    std::vector<std::pair<double, double>> coords = {{0,0}}; //imma comment this out for uploading stuff ok
    int quadrant = 0;
    if (x>0) {
        if (y>0) {
            quadrant = 1;
        }
        else {
            quadrant = 4;
        }
    }
    else if (x<0) {
        if (y>0) {
            quadrant = 2;
        }
        else {
            quadrant = 3;
        }
    }
    if (quadrant==3 || quadrant==4) {
        chassis.turnToHeading(0,1000, {}, false);
    }
    else if (quadrant==1 || quadrant==2) {
        chassis.turnToHeading(180,1000, {}, false);
    }
    setDrive(-6000,-6000);
    pros::delay(4000);
    setDrive(0,0);
    //make a distance reading, 
    double distance1 = distancesensor.get();
    double distance2 = distancesensor2.get();
    double distance3 = distancesensor3.get();
    //test if distance1+robotlength is less than fieldL. if so, something is in the way.
    double yreading = distance1+botlength;
    //test if distance2+distance3+robotwidth is less than fieldL. if so, something is in the way.
    double xreading = distance2+distance3+botwidth;
    if (yreading<fieldL && yreading>3500) {
        //do some things
        correcty = true;
        yreading = yreading/25.4; //for inches
        xreading = xreading/25.4;
    }
    else {
        correcty = false;
        //if theres time or if i suddenly feel liek it, test distances and determine what is blocking (robot, ring, etc.)
        //^probably wont work 
    }

    if (correcty=true) {

    }
}
//issues:
/*
mogo in clamp
solution: make a way to test that (distance, optical, line) and then only perform this test when we dont have a goal
solution 2: if we have a mogo, we still test, but just taking into account the mogo length
^ not good method imo

There are probably going to be rings in the corners (neg corner might be fine if we put sensor more up bc we sweep in auton)
mogo with goal will be in positive
a possible solution WOULD be to always use the negative corners, but it may be inconvenient
solution: check how much time has passed since last check, if long enough, wait for robot to enter q1/q2 to do test against neg walls
new issue!: what if robot is doing smth
*/



// std::string receiveMessage() {
//     std::string received_data;
//     receiver.receive((void*)&received_data, DATA_SIZE);
//     return received_data;
// }

// void transmitMessage(std::string message) { // problem: this thing is somehow accessing out of bounds stuff and crashing the bot
//     int len = std::min((int)message.size(), DATA_SIZE);
//     if (transmitter.connected()) {
//         transmitter.transmit((void*)message.data(), len);
//     }
// }