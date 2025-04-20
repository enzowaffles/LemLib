#include "main.h"

void turn(double targetheading, double maxspeed, float kP, float kI, float kD);

void driveDistance(const double targetdistance, double maxspeed, double kP, double kI, double kD, double anglekP);

void swingleft(double radius, double angle, double maxspeed, float kP, float kI, float kD);

void swingright(double radius, double angle, double maxspeed, float kP, float kI, float kD);

void lbgetring(double kP, int maxspeed);