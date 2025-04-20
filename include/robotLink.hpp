#pragma once

#include "lemlib/chassis/chassis.hpp"
#include "pros/link.hpp"
#include "pros/rtos.hpp"

class RobotLink : public pros::Link {
	public:
		RobotLink(int32_t index, const char* name, pros::link_type_e_t type);
		~RobotLink();

		int32_t getNumPacketsRx();
		int32_t getNumErrorsRx();
		int32_t getNumTimeoutsRx();
		int32_t getNumTotalRx();

		int32_t getNumPacketsTx();
		int32_t getNumErrorsTx();

		/* Send and Receive Information -- Currently only methods from the demo; customize later */
		void setRemoteLocation(lemlib::Pose pose);
		lemlib::Pose getLocalLocation();
		lemlib::Pose getRemoteLocation();

	private:
		// Storage for received and transmitted packets
		lemlib::Pose posePacketTx = {32, 32, 32};
		lemlib::Pose posePacketRx = {0, 0, 0};
		
		uint32_t numPacketsRx = 0;
		uint32_t numErrorsRx = 0;
		uint32_t numTimeoutsRx = 0;
		uint32_t totalDataReceived;

		uint32_t numPacketsTx = 0;
		uint32_t numErrorsTx = 0;

		pros::Mutex rxLock;
		pros::Mutex txLock;

		pros::task_t rxTask;
		pros::task_t txTask;
		static void rxMain(void* params);
		static void txMain(void* params);

		// Should have been inherited from vex::serial_link, but pros::Link did not include this -- Copied from Jetson Demo
		static uint32_t crc32Table[256];
		static uint32_t crc32(uint8_t* pData, uint32_t numBytes, uint32_t accumulator);
};