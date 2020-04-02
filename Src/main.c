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
#include <chprintf.h>
#include <arm_math.h>

#include <Inc/main.h>
#include <Inc/fft.h>
#include <Inc/audio_processing.h>
#include <Inc/communications.h>


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

	static complex_float temp_tab[FFT_SIZE];
	    //send_tab is used to save the state of the buffer to send (double buffering)
	    //to avoid modifications of the buffer while sending it
	    static float send_tab[FFT_SIZE];

	    //starts the microphones processing thread.
	    //it calls the callback given in parameter when samples are ready
	    mic_start(&processAudioData);

    /* Infinite loop. */
    while (1) {
    	//waits until a recd sult must be sent to the computer
		wait_send_to_computer();
		//we copy the buffer to avoid conflicts
		arm_copy_f32(get_audio_buffer_ptr(LEFT_OUTPUT), send_tab, FFT_SIZE);
		SendFloatToComputer((BaseSequentialStream *) &SD3, send_tab, FFT_SIZE);
    	//waits 1 second
        chThdSleepMilliseconds(1000);
    }
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}
