
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
		uint16_t prox_front_average = 0;
		uint16_t prox_side_average = 0;
		uint16_t prox_left_corrected = 0;
		uint16_t prox_right_corrected = 0;
		uint16_t score_temp = 0;

		prox_front_average =  get_calibrated_prox(IR_FRONT_LEFT) + get_calibrated_prox(IR_FRONT_RIGHT);	//Getting sensor data and taking the average
		prox_front_average /= NB_SENSOR;

		if(prox_front_average >= PROX_THRESHOLD) //If everything is alright
		{
			palClearPad(GPIOD, GPIOD_LED_FRONT);
			palClearPad(GPIOB, GPIOB_LED_BODY);
		}
		else //If something is wrong (wall very close)
		{
			palSetPad(GPIOD, GPIOD_LED_FRONT);
			palSetPad(GPIOB, GPIOB_LED_BODY);
		}

		prox_side_average =  get_calibrated_prox(IR_LEFT) + get_calibrated_prox(IR_RIGHT);	//Getting sensor data and taking the average
		prox_side_average /= NB_SENSOR;
		prox_left_corrected = get_calibrated_prox(IR_LEFT) - prox_side_average;

		chThdSleepUntilWindowed(time, time + MS2ST(10)); //reduced the sample rate to 100Hz
    }
}

static THD_WORKING_AREA(score_thd_wa, 256);
static THD_FUNCTION(score_thd, arg)
{
	(void) arg;
    chRegSetThreadName(__FUNCTION__);

    static uint32_t score;


    while(1)
    {

    }

}
void wall_detection_start(void)
{
	chThdCreateStatic(wall_detection_thd_wa, sizeof(wall_detection_thd_wa), NORMALPRIO, wall_detection_thd, NULL);
}

void score_calculation_start(void)
{
	chThdCreateStatic(score_thd_wa, sizeof(score_thd_wa), NORMALPRIO, score_thd, NULL);
}
