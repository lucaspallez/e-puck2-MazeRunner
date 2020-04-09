
#include <Inc/proximity_control.h>
#include <sensors/proximity.h>
#include <ch.h>
#include <hal.h>

static THD_WORKING_AREA(wall_detection_thd_wa, 256);
static THD_FUNCTION(wall_detection_thd, arg)
{
	(void) arg;
    chRegSetThreadName(__FUNCTION__);
    systime_t time;

    while(1)
    {
    	time = chVTGetSystemTime();
		uint16_t prox_average = 0;

		prox_average =  get_calibrated_prox(IR_FRONT_LEFT) + get_calibrated_prox(IR_FRONT_RIGHT);	//Getting sensor data and taking the average
		prox_average /= NB_SENSOR;

		if(prox_average >= PROX_THRESHOLD)
		{
			palClearPad(GPIOD, GPIOD_LED_FRONT);
			palClearPad(GPIOB, GPIOB_LED_BODY);
		}
		else
		{
			palSetPad(GPIOD, GPIOD_LED_FRONT);
			palSetPad(GPIOB, GPIOB_LED_BODY);
		}
		chThdSleepUntilWindowed(time, time + MS2ST(10)); //reduced the sample rate to 100Hz
    }
}

void wall_detection_start(void)
{
	chThdCreateStatic(wall_detection_thd_wa, sizeof(wall_detection_thd_wa), NORMALPRIO, wall_detection_thd, NULL);
}
