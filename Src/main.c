#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ch.h"
#include "hal.h"
#include "memory_protection.h"
#include <usbcfg.h>
#include <motors.h>
#include <audio/microphone.h>
#include <sensors/proximity.h>
#include <chprintf.h>
#include <arm_math.h>

#include <Inc/main.h>
#include <Inc/fft.h>
#include <Inc/audio_processing.h>
#include <Inc/motor_control.h>
#include <Inc/proximity_control.h>


messagebus_t bus;
MUTEX_DECL(bus_lock);
CONDVAR_DECL(bus_condvar);


static void serial_start(void)
{
	static SerialConfig ser_cfg = {
	    115200,
	    0,
	    0,
	    0,
	};

	sdStart(&SD3, &ser_cfg); // UART3.
}

int main(void)
{

    halInit();
    chSysInit();
    mpu_init();

    //starts the serial communication
	serial_start();
	//starts the USB communication
	usb_start();
	//inits the motors
	motors_init();

	//init IR sensors
	proximity_start();

	//init messagebus
	messagebus_init(&bus, &bus_lock, &bus_condvar);

	//start wall detection thread
	wall_detection_start();

	//starts the microphones processing thread.
	//it calls the callback given in parameter when samples are ready
	mic_start(&processAudioData);

	//start motor control thread
	motor_control_start();

    /* Infinite loop. */
    while (1)
    {

    }
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}
