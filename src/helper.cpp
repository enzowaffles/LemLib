#include "main.h"

void scoreAllianceStake()
{
    arm.move_velocity(600);
    pros::delay(300);
    while (true)
    {
        if (armsensor.get_velocity() < 50)
        {
            arm.move_velocity(-600);
            break;
        }
    }
    pros::delay(300);
    while (true)
    {
         if (armsensor.get_velocity() > -50)
         {
            arm.move_velocity(0);
            break;
         }
    }
}

void scoreWallStake()
{
    arm.move(12000);
    pros::delay(800);
}