#include "main.h"

// -------------------------------------------------------------------------------------------------
//PID Functions
// -------------------------------------------------------------------------------------------------

void turn(double targetheading, double maxspeed, float kP, float kI, float kD) {
    double currentheadibng = imu.get_rotation();
    double error = targetheading - currentheadibng;
    double errortotal = 0;
    float lasterror;
    double dT = 50;

    while (abs(error)>0.1) {
        double currentheading = imu.get_rotation();
        double error = targetheading - currentheading;

        if (error < 5 || error !=0) {
            errortotal += error;
        }
        else {
            errortotal = 0;
        }

        if (errortotal > 50 / kI) {
            errortotal = 50 / kI;
        }

        double proportional = kP * error;
        double integral = kI * errortotal;
        double derivative = (error - lasterror) / dT * kD;

        double preverror = error;

        double output = proportional + integral + derivative;

        drive_LF.move(output * maxspeed);
        drive_LM.move(output * maxspeed);
        drive_LB.move(output * maxspeed);

        drive_RF.move(output * -maxspeed);
        drive_RM.move(output * -maxspeed);
        drive_RB.move(output * -maxspeed);

        lasterror = error;

        pros::delay(50);
    }
      drive_LF.move(0);
        drive_LM.move(0);
        drive_LB.move(0);

        drive_RF.move(0);
        drive_RM.move(0);
        drive_RB.move(0);
}

void driveDistance(const double targetdistance, double maxspeed, double kP, double kI, double kD, double anglekP) {
    const double pi = 3.14159;
    double tpr = 360;
    double ticks;
    double currentdistance = ((1.017*2) * pi) * (ticks / 36000);
    double error = targetdistance - currentdistance;
    double preverror;
    double errortotal = 0;
    int dT = 20;
    double proportional;
    double integral;
    double derivative;
    double output;
    double targetangle = 0;
    double currentangle = chassis.getPose().theta;
    double anglep;
    double angleerror;
    double prevangleerror;
    double horticks = hort_encoder.get_position();
    double hortcurrentdistance;
    

    while (abs(error) > 0.05) {
        ticks = (vert_encoder.get_position());
        currentdistance = ((1.017*2) * pi) * (ticks / 36000);
        hortcurrentdistance = ((1.017*2) * pi) * (ticks / 36000);
        error = targetdistance - currentdistance;
        pros::lcd::set_text(3, "error: " + std::to_string(error));
        pros::lcd::set_text(4, "current d1: " + std::to_string(currentdistance));

        currentangle = chassis.getPose().theta;
        angleerror = targetangle - currentangle;
        // pros::lcd::set_text(7, "ticks: " + std::to_string(ticks));

        if (error < 2 && error !=0) {
            errortotal += error;
        }
        else {
            errortotal = 0;
        }

        if (errortotal > (50/kI)) {
            errortotal = 50 / kI;
        }

        proportional = error * kP;
        integral = errortotal * kI;
        derivative = (error - preverror) / dT * kD;

        anglep = angleerror * anglekP;

        output = proportional + integral + derivative;
        
        preverror = error;
        prevangleerror = angleerror;

        if (-1 < angleerror < 1) 
        {
            drive_LF.move(abs((maxspeed * output)));
            drive_LM.move(abs((maxspeed * output)));
            drive_LB.move(abs((maxspeed * output)));

            drive_RF.move(abs((maxspeed * output)));
            drive_RM.move(abs((maxspeed * output)));
            drive_RB.move(abs((maxspeed * output)));
        }

        else 
        {
            drive_LF.move(abs((maxspeed * output) - anglep));
            drive_LM.move(abs((maxspeed * output) - anglep));
            drive_LB.move(abs((maxspeed * output) - anglep));

            drive_RF.move(abs((maxspeed * output) + anglep));
            drive_RM.move(abs((maxspeed * output) + anglep));
            drive_RB.move(abs((maxspeed * output) + anglep));
        }


        // else {
        //     drive_LF.move(abs((maxspeed * output)));
        //     drive_LM.move(abs((maxspeed * output)));
        //     drive_LB.move(abs((maxspeed * output)));

        //     drive_RF.move(abs((maxspeed * output)));
        //     drive_RM.move(abs((maxspeed * output)));
        //     drive_RB.move(abs((maxspeed * output)));
        // }

        pros::delay(20);
    
    }

    drive_LF.move(0);
    drive_LM.move(0);
    drive_LB.move(0);

    drive_RF.move(0);
    drive_RM.move(0);        
    drive_RB.move(0);

    pros::lcd::set_text(1, "DONE!");
    pros::delay(2000);
    ticks = (vert_encoder.get_position());
    currentdistance = ((1.017*2) * pi) * (ticks / 36000);
    error = targetdistance - currentdistance;
    pros::lcd::set_text(3, "error: " + std::to_string(error));
    pros::lcd::set_text(4, "horizangtal_error: " + std::to_string(hort_encoder.get_position()));
    pros::lcd::set_text(7, "angle: " + std::to_string(chassis.getPose().theta));

}

void swingleft(double radius, double angle, double maxspeed, double kP, double kI, double kD) {
   const double botwidth = 11.25;
   const double pi = 3.14159;
   const double dT = 50;
   double errortotal = 0;
   double preverror;

   double targetheadingleft = ((radius * 2 * pi ) * (angle / 360));
   double targetheadingright = (((radius + botwidth) * 2 * pi) * (angle / 360 ));

   double ticksleft = ((drive_LF.get_position() + drive_LB.get_position()) / 2);
   double ticksright = ((drive_RF.get_position() + drive_RB.get_position()) / 2);

   double currentheadingleft = (ticksleft / 360);
   double currentheadingright = (ticksright / 360);

   double errorleft = targetheadingleft - currentheadingleft;
   double errorright = targetheadingright - currentheadingright;

   while (errorleft > 2) {
    double targetheadingleft = ((radius * 2 * pi ) * (angle / 360));
    double ticksleft = ((drive_LF.get_position() + drive_LB.get_position()) / 2);
    double currentheadingleft = (ticksleft / 360);
    double errorleft = targetheadingleft - currentheadingleft;

    if (errorleft < 4 || errorleft!=0) {
        errortotal += errorleft;
    }
    else {
        errortotal = 0;
    }
    
    if (errortotal > (50 / kI) ) {
        errortotal = 50 / kI;
    }

    double porportional = errorleft * kP;
    double integral = errortotal * kI;
    double derivative = ((errorleft - preverror) / (dT * kD));

    preverror = errorleft;

    double output = porportional + integral + derivative;

    drive_LF.move(output * maxspeed);
    drive_LM.move(output * maxspeed);
    drive_LB.move(output * maxspeed);

    pros::delay(50);

   }

   while (errorright > 2) {
    double targetheadingright = (((radius+botwidth) * 2 * pi ) * (angle / 360));
    double ticksright = ((drive_RF.get_position() + drive_RB.get_position()) / 2);
    double currentheadingright = (ticksright / 360);
    double errorright = targetheadingright - currentheadingright;

    if (errorright < 4 || errorright!=0) {
        errortotal += errorright;
    }
    else {
        errortotal = 0;
    }
    
    if (errortotal > (50 / kI) ) {
        errortotal = 50 / kI;
    }

    double porportional = errorright * kP;
    double integral = errortotal * kI;
    double derivative = ((errorright - preverror) / (dT * kD));

    preverror = errorright;

    double output = porportional + integral + derivative;

    drive_RF.move(output * maxspeed);
    drive_RM.move(output * maxspeed);
    drive_RB.move(output * maxspeed);

    pros::delay(50);

   }

    drive_RF.move(0);
    drive_RM.move(0);
    drive_RB.move(0);

    drive_LF.move(0);
    drive_LM.move(0);
    drive_LB.move(0);
    
}

void swingright(double radius, double angle, double maxspeed, double kP, double kI, double kD) {
   const double botwidth = 10;
   const double pi = 3.14159;
   const double dT = 50;
   double errortotal = 0;
   double preverror;

   double targetheadingright = ((radius * 2 * pi ) * (angle / 360));
   double targetheadingleft = (((radius + botwidth) * 2 * pi) * (angle / 360 ));

   double ticksleft = ((drive_LF.get_position() + drive_LB.get_position()) / 2);
   double ticksright = ((drive_RF.get_position() + drive_RB.get_position()) / 2);

   double currentheadingleft = (ticksleft / 360);
   double currentheadingright = (ticksright / 360);

   double errorleft = targetheadingleft - currentheadingleft;
   double errorright = targetheadingright - currentheadingright;

   while (errorleft > 2) {
    double targetheadingleft = (((radius + botwidth) * 2 * pi ) * (angle / 360));
    double ticksleft = ((drive_LF.get_position() + drive_LB.get_position()) / 2);
    double currentheadingleft = (ticksleft / 360);
    double errorleft = targetheadingleft - currentheadingleft;

    if (errorleft < 4 || errorleft!=0) {
        errortotal += errorleft;
    }
    else {
        errortotal = 0;
    }
    
    if (errortotal > (50 / kI) ) {
        errortotal = 50 / kI;
    }

    double porportional = errorleft * kP;
    double integral = errortotal * kI;
    double derivative = ((errorleft - preverror) / (dT * kD));

    preverror = errorleft;

    double output = porportional + integral + derivative;

    drive_LF.move(output * maxspeed);
    drive_LM.move(output * maxspeed);
    drive_LB.move(output * maxspeed);

    pros::delay(50);

   }

   while (errorright > 2) {
    double targetheadingright = ((radius * 2 * pi ) * (angle / 360));
    double ticksright = ((drive_RF.get_position() + drive_RB.get_position()) / 2);
    double currentheadingright = (ticksright / 360);
    double errorright = targetheadingright - currentheadingright;

    if (errorright < 4 || errorright!=0) {
        errortotal += errorright;
    }
    else {
        errortotal = 0;
    }
    
    if (errortotal > (50 / kI) ) {
        errortotal = 50 / kI;
    }

    double porportional = errorright * kP;
    double integral = errortotal * kI;
    double derivative = ((errorright - preverror) / (dT * kD));

    preverror = errorright;

    double output = porportional + integral + derivative;

    drive_RF.move(output * maxspeed);
    drive_RM.move(output * maxspeed);
    drive_RB.move(output * maxspeed);

    pros::delay(50);

   }

    drive_RF.move(0);
    drive_RM.move(0);
    drive_RB.move(0);

    drive_LF.move(0);
    drive_LM.move(0);
    drive_LB.move(0);
    
}

void lbgetring(double kP, int maxspeed) {
    double currentrotation = armsensor.get_angle()/100;
    double targetheading = 45;
    double error = targetheading - currentrotation;
    double preverror;

    while (abs(error>5)) {
        double currentroation = armsensor.get_angle()/100;
        double targetheading = 45;
        double error = targetheading - currentrotation;

        double porportional = error * kP;

        arm.move(porportional);

        pros::delay(10);

    }

    arm.brake();
}