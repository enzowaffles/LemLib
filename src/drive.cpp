#include "main.h"

void setDrive(double left, double right) {
    drive_LF.move_voltage(left);
    drive_LM.move_voltage(left);
    drive_LB.move_voltage(left);

    drive_RF.move_voltage(right);
    drive_RM.move_voltage(right);
    drive_RB.move_voltage(right);
}

void setdrive(double left, double right) {
    drive_LF.move(left);
    drive_LM.move(left);
    drive_LB.move(left);

    drive_RF.move(right);
    drive_RM.move(right);
    drive_RB.move(right);
}

void Arcade() {
    const int deadband = 5;
    int x = abs(controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X));
    int y = abs(controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y));
    double LjoyY = y/10.0;
    double LjoyX = x/10.0;
    double power = 0;
    double turn = 0;
    double pTune = 7.5;
    double tTune = 0.1;

    //power = exp((abs(y) - 127) * pTune / 1000) * y;
    //turn = (exp(-1 * (tTune/10)) + exp((abs(x) - 127) / 10) * (1 - exp(-1 * (tTune / 10)))) * x;

    // if (x < deadband) {
    //     turn = 0;
    // }

    // if (y < deadband) {
    //     power = 0;
    // }

    if (x > deadband || y > deadband) {
        if (y < deadband) {
            power = 0;
        }
        else if (y > 85) {
            power = (12.7 / (1.0 + exp(-(3.0/4.0)*(LjoyY - 6.0)))) * 10.0 - 3.0;
        }
        else if (y > 55 && y <= 85) {
            power = (12.7 / (1.0 + exp(-(3.0/4.0)*(LjoyY - 6.0)))) * 10.0 - 10.0;
        }
        else {
            power = 5*pow((1.0/5.5)*(LjoyY), 3.0) * 12.7;
        }

        if (x < deadband) {
            turn = 0;
        }
        else if (x > 105) { //95
            turn = 0.9*(12.7 / (1.0 + exp(-(3.0/4.0)*(LjoyX - 6.0)))) * 10.0 - 3.0; //0.72
            // turn = 0.45*(12.7 / (1.0 + exp(-(3.0/4.0)*(LjoyX - 6.0)))) * 10.0 - 3.0;

        }

        else if (x > 85 && x < 105) {
            turn = 0.8*(12.7 / (1.0 + exp(-(3.0/4.0)*(LjoyX - 6.0)))) * 10.0 - 3.0;
        }

        else if (x > 55 && x <= 85) {
            turn = 0.6*(12.7 / (1.0 + exp(-(3.0/4.0)*(LjoyX - 6.0)))) * 10.0 - 3.0;
            // 0.37 initial value
            // turn = 0.4*(12.7 / (1.0 + exp(-(3.0/4.0)*(LjoyX - 6.0)))) * 10.0 - 10.0;
        }

        else if (x > 35 && x <= 55) {
            turn = 0.5*(12.7 / (1.0 + exp(-(3.0/4.0)*(LjoyX - 6.0)))) * 10.0 - 3.0;
        }

        else {
            turn = 0.25*(12.7 / (1.0 + exp(-(3.0/4.0)*(LjoyX - 6.0)))) * 10.0 - 3.0;
            // 0.32 initial value
            // turn = 0.45*5*pow((1.0/5.5)*(LjoyX), 3.0) * 12.7;
        }
    }

    // power = power * 12000 / 127;
    // turn = turn * 12000 / 127;
    power = power * 12000 / 127;
    turn = turn * 12000 / 127;

    if (controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) < 0) power = -power;

    if (controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X) < 0) turn = -turn;

    double leftPower = power + turn;
    double rightPower = power - turn;
    setDrive(leftPower, rightPower);
}

void Tank() {
    int x = (controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y));
    int y = (controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y));
    int rightdrive;
    int leftdrive;

    rightdrive = x*(6000/127);
    leftdrive = y*(6000/127);

    
    setDrive(leftdrive, rightdrive);
}

double leftPower;
double rightPower;

void ethandrive() {
        double x = abs(controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X));
        double y = abs(controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y));
        double power = y;
        double turn = x * (0.64 + y / 50 * 0.1);
        if (controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) < 0)   {
            power = -power;
        }
        if (controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X) < 0)  {
            turn = -turn;
        }
        leftPower = power + turn;
        rightPower = power - turn;
        setdrive(leftPower, rightPower);
}

void setDriveMotors() {
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)) // mogo tip
    {
        setDrive(12000, -12000);
        pros::delay(1000);
        clamp.set_value(true);
        pros::delay(250);
    }
    //Arcade();
    else
    {
        //ethandrive();
        Arcade();
    }
}    // Tank();
