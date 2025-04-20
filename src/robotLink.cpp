#include "robotLink.hpp"
#include "pros/llemu.hpp"
#include "pros/rtos.h"
#include <cstring>

RobotLink::RobotLink(int32_t index, const char* name, pros::link_type_e_t type)
	: pros::Link(index, name, type, true) {
	rxTask = pros::c::task_create(rxMain, this, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Robot Link RX");
	txTask = pros::c::task_create(txMain, this, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Robot Link TX");
}
RobotLink::~RobotLink() {}

int32_t RobotLink::getNumPacketsRx() {
	return numPacketsRx;
}
int32_t RobotLink::getNumErrorsRx() {
	return numErrorsRx;
}
int32_t RobotLink::getNumTimeoutsRx() {
	return numTimeoutsRx;
}
int32_t RobotLink::getNumTotalRx() {
	return totalDataReceived;
}

int32_t RobotLink::getNumPacketsTx() {
	return numPacketsTx;
}
int32_t RobotLink::getNumErrorsTx() {
	return numErrorsTx;
}

void RobotLink::setRemoteLocation(lemlib::Pose pose) {
	txLock.take();
	posePacketTx = pose;
	txLock.give();
}

lemlib::Pose RobotLink::getLocalLocation() {
	txLock.take();
	lemlib::Pose ret = posePacketTx;
	txLock.give();
	return ret;
}
lemlib::Pose RobotLink::getRemoteLocation() {
	rxLock.take();
	lemlib::Pose ret = posePacketRx;
	rxLock.give();
	return ret;
}

void RobotLink::rxMain(void* params) {
	uint8_t buffer[128];
	int32_t bufferLength;

	if (params == NULL) return;

	RobotLink* link = static_cast<RobotLink*>(params);

    // while (!link->connected()) pros::delay(50);

	uint32_t time = pros::millis();
	while (true) {
		if (link->receive(&link->posePacketRx, sizeof(lemlib::Pose)) > 0) {
            link->numPacketsRx++;
        } else {
            link->numErrorsRx++;
        }
		link->clear_receive_buf();

		pros::Task::delay_until(&time, 25);
	}
}
void RobotLink::txMain(void* params) {
	if (params == NULL) return;

	RobotLink* link = static_cast<RobotLink*>(params);
	
	// Wait until connected
	while (!link->connected()) pros::delay(50);

	int loopFreq = 10; // In Hz - Calculate using number of bytes of each payload and the max bandwidth of the vex link (512 bytes/sec)

	uint32_t time = pros::millis();
	while (true) {
		link->txLock.take();

		if (link->transmit((uint8_t*) &link->posePacketTx, sizeof(lemlib::Pose)) > 0)
			link->numPacketsTx++;
		else
		 	link->numErrorsTx++;

		link->txLock.give();

		pros::Task::delay_until(&time, 1000/loopFreq);
	}
}